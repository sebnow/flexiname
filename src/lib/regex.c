/*
 *  regex.c
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

#include "regex.h"
#include <malloc.h>
#include <stdio.h>
#include <ctype.h>

char *regex_replace(regex_t *regexp, const char *subj, const char *replace, int eflags, int *err)
{
    unsigned short nmatch = regexp->re_nsub + 1;
    regmatch_t match[nmatch];
    size_t newstrlen = 0;
    char *needle;
    char *p_replace;
    char *p_newstr;
    char *newstr;
    char *substr;
    int index;
    int error;
    int nreplace;

    error = regexec(regexp, subj, nmatch, match, eflags);
    if(0 != error ) {
        *err = error;
        return NULL;
    }

    /* Get length of resulting string */
    for(needle = (char *) replace; *needle; newstrlen++, needle++);
    /* Exclude the '\1', '\2', etc */
    newstrlen -= (regexp->re_nsub * 2);
    /* Add the difference in byte offsets of all substrings */
    for(index = 1; index < nmatch; index++) {
        newstrlen += match[index].rm_eo - match[index].rm_so;
    }
    /* Don't forget the '\0'! */
    newstrlen++;

    newstr = malloc(newstrlen * sizeof(char));
    /* Check for memory exhaustion */
    if(NULL == newstr) {
        *err = 1;
        return NULL;
    }

    p_replace = (char *) replace;
    p_newstr = newstr;

    /* Traverse replace */
    while(*p_replace) {
        /* Check if we have something to substitute */
        if(*p_replace == '\\' && *(p_replace + 1) && isdigit(*(p_replace + 1))) {
            p_replace++;
            nreplace = (int) (*p_replace - '0');
            substr = (char *) subj + match[nreplace].rm_so;

            /* Concatenate newstr and substitution */
            while(*substr && substr != (subj + match[nreplace].rm_eo)) {
                *p_newstr = *substr;
                p_newstr++;
                substr++;
            }
        } else {
            *p_newstr = *p_replace;
            p_newstr++;
        }

        p_replace++;
    }

    *p_newstr = '\0';

    return newstr;
}
