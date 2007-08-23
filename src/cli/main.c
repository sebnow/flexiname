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
#include <sys/stat.h>
#include <malloc.h>
#include <regex.h>
#include <string.h>

#include "main.h"
#include "../lib/queue.h"

typedef struct {
    unsigned short help:1;
    unsigned short version:1;
    unsigned short verbose:1;
    unsigned short regex_flags;
} config_t;

config_t config;

static int parse_args(int argc, char *argv[], queue_t *expr_queue);
static void usage();
static void version();
static int move(const char *src, const char *dst);

int main(int argc, char *argv[])
{
    queue_t *expr_queue = queue_create();
    struct stat fstat;
    int index;

    config.regex_flags = 0;

    if(parse_args(argc, argv, expr_queue) > 0) {
        usage(PROGNAME);
        queue_destroy(expr_queue);
        return 1;
    }

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
        printf("Regular expression are not yet implemented.\n");
    } else {
        queue_destroy(expr_queue);
        return 0;
    }

    queue_destroy(expr_queue);

    return 0;
}

static int parse_args(int argc, char *argv[], queue_t *expr_queue)
{
    int opt;
    int opt_index = 0;
    struct option opts[] =
    {
        {"help",            no_argument,        0, 'h'},
        {"version",         no_argument,        0, 'V'},
        {"verbose",         no_argument,        0, 'v'},
        {"expression",      no_argument,        0, 'e'},
        {"extended-regexp", no_argument,        0, 'E'},
        {"ignore-case",     no_argument,        0, 'i'},
        {0,             0,                  0,  0 }
    };

    while((opt = getopt_long(argc, argv, "hVve:Ei", opts, &opt_index)) && opt >= 0)
    {
        switch(opt)
        {
            case 'h': config.help = 1; break;
            case 'v': config.version = 1; break;
            case 'V': config.verbose = 1; break;
            case 'e': queue_push_front(expr_queue, optarg); break;
            case 'E': config.regex_flags |= REG_EXTENDED; break;
            case 'i': config.regex_flags |= REG_ICASE; break;
        }
    }

    return 0;
}

static void usage()
{
    printf("Usage:  %s [OPTION]... SOURCE DEST\n", PROGNAME);
    printf("   or:  %s [OPTION]... SOURCE... DIRECTORY\n", PROGNAME);
    printf("   or:  %s [OPTION]... -e SRC_PATTERN:DST_PATTERN... FILE...\n\n", PROGNAME);
    printf("Rename SOURCE to DEST, or move SOURCE(s) to DIRECTORY.\n\n");
    printf(" -h, --help             Show this text\n");
    printf(" -v, --version          Show program version\n");
    printf(" -V, --verbose          Be verbose\n");
    printf(" -E, --extended-regexp  Use extended regular expressions\n");
    printf(" -e, --expression=SRC_PATTERN:DEST_PATTERN\n");
    printf("                    \n");
    printf(" -h, --help         Show this text\n");
}

static void version()
{
    printf("%s-%s\n", PROGNAME, VERSION_STR);
    printf("Copyright (C) 2007 Sebastian Nowicki\n\n");
    printf("This program may be freely redistributed under ");
    printf("the GNU General Public License\nv2.0 ");
    printf("<http://www.gnu.org/licenses/gpl-2.0.html>.\n");
    printf("There is NO WARRANTY, to the extent permitted by law.\n");
}

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

