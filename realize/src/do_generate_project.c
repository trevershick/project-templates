// needed for ftw
#define _XOPEN_SOURCE 500
// needed for getline
#define _POSIX_C_SOURCE 200809L

#include <assert.h>
#include <ftw.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include <smatch.h>

#include "ezfs.h"
#include "ezstring.h"
#include "realize_defs.h"
#include "realize_io.h"

char line_buffer[4096];

// this is disappointing
static realize_options_t *g_opts = 0;

static int visit_path(const char *path, const struct stat *s, int typeflag);
static int make_dir(realize_options_t *, const char *s, const char *d,
                    bool dexists);
static int make_file(realize_options_t *, const char *s, const char *d,
                     bool dexists);

typedef struct
{
    FILE *fp;
    realize_options_t *opts;
} smatch_data;

int on_smatch_match(const match_t *m, void *d)
{
    smatch_data *data = d;
    if (m->t == MATCH)
    {
        fputs(data->opts->project_name, data->fp);
    }
    else
    {
        fwrite(m->buffer, m->len, 1, data->fp);
    }
    return 0;
}

int do_generate_project(realize_options_t *opts)
{
    assert(!g_opts);
    g_opts = opts;
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
    char destination_path[PATH_MAX];
    memset(destination_path, 0, sizeof(destination_path));
    memcpy(destination_path, path, strlen(path));
    // replace any tokens in the path and change it's relative location
    str_replace(destination_path, sizeof(destination_path), TOKEN,
                g_opts->project_name);
    str_replace(destination_path, sizeof(destination_path),
                g_opts->proj_tmpl_path, ".");

    // work out a human readable source path
    char b[PATH_MAX];
    memset(b, 0, sizeof(b));
    memcpy(b, path, strlen(path));
    str_replace(b, sizeof(b), g_opts->proj_tmpls_root_path, "<root>");

    // print out what we're doing
    const bool destination_exists = ez_exists(destination_path);
    int ret = 0;
    char type = ' ';
    char action = ' ';
    switch (typeflag)
    {
    case FTW_D:
        type = 'D';
        if (!destination_exists)
            action = '+';
        ret = make_dir(g_opts, path, destination_path, destination_exists);
        break;
    case FTW_F:
        type = 'F';
        if (g_opts->force || !destination_exists)
            action = '+';
        ret = make_file(g_opts, path, destination_path, destination_exists);
        break;
    default:
        type = '?';
        action = ' ';
    }
    // screen geometry calc
    int sz = (g_opts->window_size.ws_col - 10) / 2;
    rprintf(g_opts, LOG_LEVEL_DEBUG, "%c %-*.*s -> %-*.*s  %c\n", type, sz, sz,
            b, sz, sz, destination_path, action);
    rprintf(g_opts, LOG_LEVEL_TRACE, "make_file returning %d\n", ret);
    return ret;
}

static int make_dir(realize_options_t *opts, const char *src, const char *dst,
                    bool exists)
{
    int ret = 0;
    opts->stats.template_dirs++;
    if (!exists)
    {
        if (!opts->dryrun)
            ret = mkdir(dst, DEFAULT_MODE);
        opts->stats.dirs_added++;
        if (ret)
            perror(dst);
    }
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

    int ret = 0;
    if (!opts->dryrun)
    {
        // create the file and filter its contents
        FILE *fw = fopen(dst, "w");
        if (fw == NULL)
        {
            perror(dst);
            return -1;
        }

        FILE *fr = fopen(src, "r");
        if (fr == NULL)
        {
            perror(src);
            return -1;
        }

        smatch_node_t tree;
        initialize_tree(&tree);
        add_to_tree(&tree, TOKEN, true);
        smatch_ctx_t ctx;
        smatch_data d;
        d.opts = opts;
        d.fp = fw;
        init_smatch_ctx(&ctx, &tree, on_smatch_match, &d);
        off_t offset = 0;

        char c;
        do {
            smatch_process(&ctx, offset, c = fgetc(fr));
        } while (c != EOF);

        if (fclose(fw))
        {
            perror("fclose");
            return -1;
        }

        if (fclose(fr))
        {
            perror("fclose");
            return -1;
        }
    }

    if (exists)
        opts->stats.files_replaced++;
    else
        opts->stats.files_added++;

    return ret;
}
