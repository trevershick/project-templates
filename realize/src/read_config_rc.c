#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "realize_defs.h"
#include "realize_io.h"

static int resolve_config_rc(realize_options_t *opts, char *config_pathbuf,
                             int sz)
{
    // if xdg config home is set use that
    const char *xdg_config_home = getenv("XDG_CONFIG_HOME");
    struct stat st;
    if (xdg_config_home && stat(xdg_config_home, &st) == 0 &&
        S_ISDIR(st.st_mode))
    {
        strncpy(config_pathbuf, xdg_config_home, sz - 1);
    }
    else
    {
        const char *home = getenv("HOME");
        char tmp[PATH_MAX];
        strncpy(tmp, home, sizeof(tmp) - 1);
        strcat(tmp, "/.config");
        memcpy(config_pathbuf, tmp, strlen(tmp));
    }

    strcat(config_pathbuf, "/realize");
    if (stat(config_pathbuf, &st) == -1)
    {
        // any error (except non-existence) fails this function
        if (errno != ENOENT)
        {
            perror(config_pathbuf);
            return -1;
        }
        // we need to create it
        if (mkdir(config_pathbuf, DEFAULT_MODE))
        {
            perror("mkdir");
            return -1;
        }
    }
    else if (!S_ISDIR(st.st_mode))
    {
        rprintf(opts, LOG_LEVEL_ERROR, "%s is not a directory", config_pathbuf);
        return -1;
    }
    // so now we know we have a directory of .config/realize
    // we need to work out the file now
    strcat(config_pathbuf, "/realizerc");

    // set the config file location in the opts
    strncpy(opts->configuration_file, config_pathbuf,
            sizeof(opts->configuration_file) - 1);

    // let's make sure the path is a file
    if (stat(config_pathbuf, &st) == 0)
    {
        // the file exists and is regular
        if (S_ISREG(st.st_mode))
            return 0;

        // the file exists but is not a file
        rprintf(opts, LOG_LEVEL_ERROR, "%s is not a file", config_pathbuf);
        return -1;
    }

    if (errno != ENOENT)
    {
        perror(config_pathbuf);
        return -1;
    }

    FILE *f = fopen(config_pathbuf, "w");
    if (!f)
    {
        // couldn't create the file
        perror(config_pathbuf);
        return -1;
    }
    fclose(f);
    return 0;
}

int read_config_rc(realize_options_t *opts)
{
    char config_pbuf[PATH_MAX];
    memset(config_pbuf, 0, sizeof(config_pbuf));
    int ret = 0;
    if ((ret = resolve_config_rc(opts, config_pbuf, sizeof(config_pbuf))))
    {
        return ret;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    FILE *fp = fopen(config_pbuf, "r");
    if (!fp)
    {
        perror(config_pbuf);
        return -1;
    }

    char value[256];
    while ((read = getline(&line, &len, fp)) != -1)
    {
        memset(value, 0, sizeof(value));

        if (sscanf(line, "%[#]", value) == 1)
            continue;
        if (sscanf(line, " templates-root = %s", value) == 1)
        {
            strcpy(opts->proj_tmpls_root_path, value);
        }
    }
    fclose(fp);
    return ret;
}
