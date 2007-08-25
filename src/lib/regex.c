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

/**
 * @file    regex.c
 * @brief   Implements regular-expression related functions to ease the pain.
 */

#include "regex.h"
#include <malloc.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**
 * @brief   Perform a regular expression search and replace
 *
 * subj gets matched against the compiled regular expression regexp. It is
 * then replaced with the replace string, substituting any references with
 * the appropriate captured string.
 *
 * @see     regexec()
 * @see     regcomp()
 *
 * @param   regexp      A compiled regular expression using regcomp()
 * @param   subj        The string to search and replace
 * @param   replace     String to replace with. May contain references of the
 *                      form \\n where n is a number 0-9. Each reference
 *                      will be replaced by the text captured by the n'th
 *                      parenthesized pattern.
 * @param   eflags      Error flags which are to be passed to regexec()
 * @param   err         Error code returned by the function if something fails
 *
 * @return  Resultant string
 */
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

    /* Match the expression and check for errors */
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
            /* Concatenate the replace string */
            *p_newstr = *p_replace;
            p_newstr++;
        }

        p_replace++;
    }

    *p_newstr = '\0';

    return newstr;
}

queue_t *regex_parse_replace(queue_t *files, char *regex, char *replace, int cflags)
{
    filename_pair_t *file;
    regex_t regexp;
    queue_t *files_new = queue_create();

    if(!files_new) {
        return NULL;
    }

    if(regcomp(&regexp, regex, cflags) != 0) {
        return NULL;
    }

    while((file = queue_pop_back(files)) != NULL) {
        file->new = regex_replace(&regexp, file->old, replace, cflags, NULL);
        queue_push_front(files_new, file);
    }

    queue_destroy(files);

    return files_new;
}
