/*
 *  queue.h
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

#ifndef QUEUE_H
#define QUEUE_H

typedef struct node_t node_t;
struct node_t {
    char *data;
    node_t *next;
    node_t *prev;
};

typedef struct {
    node_t *front;
    node_t *back;
} queue_t;

queue_t *queue_create(void);
void queue_destroy(queue_t *queue);
int queue_push_front(queue_t *queue, char *data);
char *queue_pop_back(queue_t *queue);
int queue_empty(const queue_t *queue);

#endif
