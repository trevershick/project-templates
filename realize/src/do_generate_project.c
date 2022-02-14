// needed for ftw
#define _XOPEN_SOURCE 500
// needed for getline
#define _POSIX_C_SOURCE 200809L

#include "ezfs.h"
#include "ezstring.h"
#include "realize_defs.h"
#include "realize_io.h"
#include "smatch.h"
#include <assert.h>
#include <ftw.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

// this is disappointing
static realize_options_t *g_opts = 0;
static smatch_node_t g_tree;

static int visit_path(const char *path, const struct stat *s, int typeflag);
static int make_dir(realize_options_t *, const char *s, const char *d,
                    bool dexists);
static int make_file(realize_options_t *, const char *s, const char *d,
                     bool dexists);

int on_smatch_match(const match_t *m, void *d)
{
    FILE *fp = d;
    if (m->t == MATCH)
    {
        fputs(g_opts->project_name, fp);
    }
    else
    {
        fwrite(m->buffer, m->len, 1, fp);
    }
    return 0;
}

int do_generate_project(realize_options_t *opts)
{
    assert(!g_opts);
    g_opts = opts;

    initialize_tree(&g_tree);
    add_to_tree(&g_tree, TOKEN, true);

    int ret = 0;
    ret = ftw(opts->proj_tmpl_path, visit_path, FTW_MOUNT | FTW_PHYS);
    if (ret == -1)
    {
        return -1;
    }
    g_opts = 0;
    return 0;
}

static int visit_path(const char *path, const struct stat *s, int typeflag)
{
    // work out the destination path
    char dpath[PATH_MAX];
    memset(dpath, 0, sizeof(dpath));
    memcpy(dpath, path, strlen(path));
    // replace any tokens in the path and change it's relative location
    str_replace(dpath, sizeof(dpath), TOKEN, g_opts->project_name);
    str_replace(dpath, sizeof(dpath), g_opts->proj_tmpl_path, ".");

    // print out what we're doing
    const bool dexists = ez_exists(dpath);
    int ret = 0;
    char type = ' ';
    char action = ' ';
    switch (typeflag)
    {
    case FTW_D:
        type = 'D';
        if (!dexists)
            action = '+';
        ret = make_dir(g_opts, path, dpath, dexists);
        break;
    case FTW_F:
        type = 'F';
        if (g_opts->force || !dexists)
            action = '+';
        ret = make_file(g_opts, path, dpath, dexists);
        break;
    default:
        type = '?';
        action = ' ';
    }

    // screen geometry calc
    int sz = (g_opts->window_size.ws_col - 10) / 2;

    // work out a human readable source path
    char b[PATH_MAX];
    memset(b, 0, sizeof(b));
    memcpy(b, path, strlen(path));
    str_replace(b, sizeof(b), g_opts->proj_tmpls_root_path, "<root>");

    rprintf(g_opts, LOG_LEVEL_DEBUG, "%c %-*.*s -> %-*.*s  %c\n", type, sz, sz,
            b, sz, sz, dpath, action);
    rprintf(g_opts, LOG_LEVEL_TRACE, "make_file returning %d\n", ret);
    return ret;
}

static int make_dir(realize_options_t *opts, const char *src, const char *dst,
                    bool exists)
{
    int ret = 0;
    opts->stats.template_dirs++;
    if (exists)
        return 0;

    if (!opts->dryrun)
        ret = mkdir(dst, DEFAULT_MODE);

    opts->stats.dirs_added++;

    if (ret)
        perror(dst);

    return ret;
}

static int make_file(realize_options_t *opts, const char *src, const char *dst,
                     bool exists)
{
    opts->stats.template_files++;

    if (exists && !opts->force)
    {
        return 0;
    }

    off_t offset = 0;
    char c;
    smatch_ctx_t ctx;
    FILE *fr = NULL;
    FILE *fw = NULL;
    int ret = 0;

    for (; !opts->dryrun && !fr && !fw && !ret;)
    {
        fr = fopen(src, "r");
        fw = fopen(dst, "w");
        if (fr == NULL || fw == NULL)
        {
            perror(src);
            ret = -1;
            break;
        }
        init_smatch_ctx(&ctx, &g_tree, on_smatch_match, fw);
        do
        {
            smatch_process(&ctx, offset, c = fgetc(fr));
        } while (c != EOF);
    }
    if (fr && fclose(fr))
    {
        perror("fclose");
        ret = -1;
    }
    if (fw && fclose(fw))
    {
        perror("fclose");
        ret = -1;
    }

    if (exists)
        opts->stats.files_replaced++;
    else
        opts->stats.files_added++;

    return ret;
}
