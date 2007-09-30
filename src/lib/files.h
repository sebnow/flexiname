/*
 *  files.h
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
 * @file    files.h
 * @brief   Defines functions and structs dealing with files
 */

#ifndef FILES_H
#define FILES_H

/**
 * @brief   Old filename and new filename pair
 */
typedef struct {
    char *old;      /**< @brief Old (current) filename */
    char *new;      /**< @brief New filename */
} filename_pair_t;

#endif
