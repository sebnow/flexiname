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

/**
 * @file    queue.h
 * @brief   Defines functions for a doubly-linked queue.
 */

#ifndef QUEUE_H
#define QUEUE_H

/**
 * @struct  node_t
 * @brief   A node within a queue.
 *
 * node_t->next and node_t->prev point to the next and previous nodes in the
 * queue, respectively.
 */
typedef struct node_t node_t;
struct node_t {
    void *data;     /**< Data to be stored */
    node_t *next;   /**< Pointer to the next node_t */
    node_t *prev;   /**< Pointer to previous node_t */
};

/**
 * @struct  queue_t
 * @brief   Metadata about queue
 */
typedef struct {
    node_t *front;  /**< Points to the node_t at the front/head of the queue */
    node_t *back;   /**< Points to the node_t at the back/tail of the queue */
} queue_t;


queue_t *queue_create(void);
void queue_destroy(queue_t *queue);
int queue_push_front(queue_t *queue, void *data);
void *queue_pop_back(queue_t *queue);
int queue_is_empty(const queue_t *queue);

#endif
