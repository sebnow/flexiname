/*
 *  regex.h
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
 * @file    regex.h
 * @brief   Implements regular-expression related functions to ease the pain.
 */

#ifndef REGEX_H
#define REGEX_H

#include <regex.h>

#include "queue.h"

typedef struct {
    char *old;
    char *new;
} filename_pair_t;

char *regex_replace(regex_t *regexp, const char *subj, const char *replace, int eflags, int *err);
queue_t *regex_parse_replace(queue_t *files, char *regex, char *replace, int cflags);

#endif
