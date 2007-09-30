/*
 *  rename.c
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

#include "rename.h"

/**
 * @brief   Rename all files in queue file_queue
 *
 * @param   file_queue  A queue of filename_pair_t structs
 * @param   overwrite   Whether to force renaming or not
 *
 * @return  0 on success
 */
int rename_files(queue_t *file_queue, unsigned short overwrite)
{
    filename_pair_t *filename;
    struct stat fstat;

    /* Traverse the file queue and rename each file */
    while((filename = queue_pop_back(file_queue))) {
        /* Only overwrite if the file does not exist or we want to overwrite */
        if(stat(filename->new, &fstat) != 0 || overwrite) {
            if(rename(filename->old, filename->new) != 0) {
                fprintf(stderr, "WARNING: Could not rename '%s' to '%s'\n",
                        filename->old, filename->new);
            }
        }
    }

    return 0;
}
