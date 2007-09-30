/*
 *  main.c
 * 
 *  Copyright (c) 2007  Sebastian Nowicki
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, 
 *  USA.
 */

#include <getopt.h>
#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <malloc.h>

#include "main.h"
#include "../lib/regex.h"
#include "../lib/queue.h"
#include "../lib/rename.h"

/**
 * @brief   Contains configuration options
 */
typedef struct {
    unsigned short help:1;      /**< @brief Whether to show usage */
    unsigned short version:1;   /**< @brief Whether to show version */
    unsigned short verbose:1;   /**< @brief Whether to be verbose */
    unsigned short overwrite:1; /**< @brief Whether to force renaming */
    unsigned short regex_flags; /**< @brief Flags for regcomp */
} config_t;

/**
 * @brief   Regex-replace pair, like in s/regex/replace/
 */
typedef struct {
    char *regex;                /**< @brief Plaintext regular expression */
    char *replace;              /**< @brief Text to replace with */
} expression_t;

config_t config;

static int parse_args(int argc, char *argv[], queue_t *expr_queue);
static int move(const char *src, const char *dst);
static void usage();
static void version();
static int expr_split(const char *str, expression_t *expr);

int main(int argc, char *argv[])
{
    queue_t *expr_queue = queue_create();
    queue_t *file_queue = NULL;
    filename_pair_t *file;
    expression_t *expr;
    struct stat fstat;
    int index;

    config.regex_flags = 0;

    if(parse_args(argc, argv, expr_queue) > 0) {
        usage(PROGNAME);
        queue_destroy(expr_queue);
        return 1;
    }

    /* TODO: Check that we have enough operands */
    if(config.help) {
        usage();
        queue_destroy(expr_queue);
        return 0;
    } else if(config.version) {
        version();
        queue_destroy(expr_queue);
        return 0;
    }

    stat(argv[argc-1], &fstat);
    if(S_ISDIR(fstat.st_mode)) {
        for(index = optind; index < argc - 1; index++) {
            move(argv[index], argv[argc - 1]);
        }
    } else if(argc - optind == 2) {
        rename(argv[optind], argv[argc-1]);
    } else if(!queue_is_empty(expr_queue)) {
        file_queue = queue_create();

        /* Get all files and push onto queue */
        for(; optind < argc; optind++) {
            file = malloc(sizeof(filename_pair_t));
            if(!file) {
                return 1;
            }
            file->old = malloc((strlen(argv[optind])+1) * sizeof(char));
            if(!file->old) {
                return 1;
            }
            file->old = strcpy(file->old, argv[optind]);
            file->new = NULL;
            queue_push_front(file_queue, file);
        }

        while((expr = queue_pop_back(expr_queue))) {
            file_queue = regex_parse_replace(file_queue, expr->regex, expr->replace, config.regex_flags);
        }

        rename_files(file_queue, config.overwrite);
    } else {
        queue_destroy(expr_queue);
        return 0;
    }

    queue_destroy(expr_queue);

    return 0;
}

/**
 * @brief   Parse arguments passed from the command line
 *
 * @param   argc        Amount of arguments
 * @param   argv        Arguments passed from the command line
 * @param   expr_queue  Queue to put expressions into
 *
 * @return  0 on success, 1 if OoM
 */
static int parse_args(int argc, char *argv[], queue_t *expr_queue)
{
    int opt;
    int opt_index = 0;
    expression_t *expr;
    struct option opts[] =
    {
        {"help",            no_argument,        0, 'h'},
        {"version",         no_argument,        0, 'V'},
        {"verbose",         no_argument,        0, 'v'},
        {"expression",      no_argument,        0, 'e'},
        {"extended-regexp", no_argument,        0, 'E'},
        {"ignore-case",     no_argument,        0, 'i'},
        {"force",           no_argument,        0, 'f'},
        {0,                 0,                  0,  0 }
    };

    while((opt = getopt_long(argc, argv, "hVve:Eif", opts, &opt_index)) && opt >= 0)
    {
        switch(opt)
        {
            case 'h': config.help = 1; break;
            case 'v': config.version = 1; break;
            case 'V': config.verbose = 1; break;
            case 'E': config.regex_flags |= REG_EXTENDED; break;
            case 'i': config.regex_flags |= REG_ICASE; break;
            case 'f': config.overwrite = 1; break;
            case 'e':
                expr = malloc(sizeof(expression_t));
                if(!expr) {
                    return 1;
                }
                if(expr_split(optarg, expr) != 0) {
                    return 1;
                }
                queue_push_front(expr_queue, expr);
                break;
        }
    }

    return 0;
}

/**
 * @brief   Print usage
 */
static void usage()
{
    printf("Usage:  %s [OPTION]... SOURCE DEST\n", PROGNAME);
    printf("   or:  %s [OPTION]... SOURCE... DIRECTORY\n", PROGNAME);
    printf("   or:  %s [OPTION]... -e SRC_PATTERN:DST_PATTERN... FILE...\n\n", PROGNAME);
    printf("Rename SOURCE to DEST, or move SOURCE(s) to DIRECTORY.\n\n");
    printf(" -h, --help             Show this text\n");
    printf(" -v, --version          Show program version\n");
    printf(" -V, --verbose          Be verbose\n");
    printf(" -i, --ignore-case      Ignore case distinctions\n");
    printf(" -E, --extended-regexp  Use extended regular expressions\n");
    printf(" -e, --expression       Use regular expression and substitution for renaming.\n");
    printf(" -f, --force            Force renaming (overwrite).\n");
}

/**
 * @brief   Print version
 */
static void version()
{
    printf("%s-%s\n", PROGNAME, VERSION_STR);
    printf("Copyright (C) 2007 Sebastian Nowicki\n\n");
    printf("This program may be freely redistributed under ");
    printf("the GNU General Public License\nv2.0 ");
    printf("<http://www.gnu.org/licenses/gpl-2.0.html>.\n");
    printf("There is NO WARRANTY, to the extent permitted by law.\n");
}

/**
 * @brief   Split an expression to regex and replace string
 *
 * @param   str     Expression to be split
 * @param   expr    Container for the split expression
 *
 * @return  0 on success, 1 on OoM
 */
static int expr_split(const char *str, expression_t *expr)
{
    size_t colon_len;
    char *colon;
    unsigned int colon_index;

    /* Find the seperator between the regex and replace strings */
    colon = strstr(str, "\\:");
    colon_index = colon - str;
    colon_len = strlen(colon);

    /* Check if it's a valid string */
    /* TODO: Elaborate */
    if(*(colon + 1) == '\0') {
        return 2;
    }

    /*
    expr = malloc(sizeof(expression_t));
    if(!expr) {
        return 1;
    }
    */

    expr->regex = malloc(colon_index * sizeof(char));
    if(!expr->regex) {
        return 1;
    }
    strncpy(expr->regex, str, colon_index);

    colon += 2;

    expr->replace = malloc(colon_len * sizeof(char));
    if(!expr->replace) {
        return 1;
    }
    strncpy(expr->replace, colon, colon_len);

    return 0;
}

/**
 * @brief   Rename a file or move into a directory
 *
 * @param   src     Filename of file to be moved/renamed
 * @param   dst     New filename or directory to move in to
 *
 * @return  0 on success, 1 if OoM, 2 if file already exists
 */
static int move(const char *src, const char *dst)
{
    struct stat fstat;
    char *buf;

    /* Check if file exists */
    if(stat(dst, &fstat) == 0) {
        /* Check if dst is a directory */
        if(S_ISDIR(fstat.st_mode)) {
            buf = malloc((strlen(src) + strlen(dst) + 1) * sizeof(char));
            if(buf == NULL) {
                return 1;
            }

            strcpy(buf, dst);
            strcat(buf, "/");
            strcat(buf, src);
            rename(src, buf);
            free(buf);
        } else {
            /* TODO: File already exists, for now just return an error */
            return 2;
        }
    } else {
        if(rename(src, dst) != 0) {
            return 1;
        }
    }

    return 0;
}
