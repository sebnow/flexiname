/*
 *  rename.h
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
 * @file    rename.h
 * @brief   Defines functions for renaming files.
 */

#ifndef RENAME_H
#define RENAME_H

#include <sys/stat.h>
#include <stdio.h>

#include "queue.h"
#include "files.h"

int rename_files(queue_t *file_queue, unsigned short overwrite);

#endif
