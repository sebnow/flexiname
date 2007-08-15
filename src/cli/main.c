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

#include "main.h"

typedef struct {
    unsigned short help;
    unsigned short version;
    unsigned short verbose;
} config_t;

config_t config;

int main(int argc, char *argv[])
{
    if(parse_args(argc, argv) > 0) {
        usage(PROGNAME);
        return 1;
    }

    if(config.help) {
        usage(PROGNAME);
        return 0;
    } else if(config.version) {
        printf("%s %s\n", PROGNAME, VERSION_STR);
    }

    return 0;
}

static int parse_args(int argc, char *argv[])
{
    int opt;
    int opt_index = 0;
    static struct option opts[] =
    {
        {"help",        no_argument,        0, 'h'},
        {"version",     no_argument,        0, 'V'},
        {"verbose",     no_argument,        0, 'v'},
        {0,             0,                  0,  0 }
    };

    while((opt = getopt_long(argc, argv, "hVv", opts, &opt_index)) && opt >= 0)
    {
        switch(opt)
        {
            case 'h': config.help = 1; break;
            case 'V': config.version = 1; break;
            case 'v': config.verbose = 1; break;
        }
    }

    /* while(optind < argc) add files to list */

    return 0;
}

static void usage()
{
    printf("Usage:  %s [OPTION]... SOURCE DEST\n", PROGNAME);
    printf("   or:  %s [OPTION]... SOURCE... DIRECTORY\n", PROGNAME);
    printf("   or:  %s [OPTION]... -t DIRECTORY SOURCE...\n", PROGNAME);
    printf("Rename SOURCE to DEST, or move SOURCE(s) to DIRECTORY.\n");
}
