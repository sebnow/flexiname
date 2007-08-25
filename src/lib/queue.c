/*
 *  queue.c
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

#include "queue.h"

#include <stdio.h>
#include <malloc.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

/**
 * @brief   Create a new node_t and set all members to NULL
 *
 * @return  node_t with allocated memory
 */
static node_t *node_create(void)
{
    node_t *new_node = (node_t *) malloc (sizeof (node_t));
    if(new_node != NULL) {
        new_node->data = NULL;
        new_node->next = NULL;
        new_node->prev = NULL;
    }

    return new_node;
}

/**
 * @brief   Allocate memory to a queue_t and return address
 *
 * @return  queue_t with allocated memory
 */
queue_t *queue_create(void)
{
    queue_t *queue = malloc(sizeof(queue_t));
    if(queue != NULL) {
        queue->front = NULL;
        queue->back = NULL;
    }

    return queue;
}

/**
 * @brief Free all nodes and queue
 *
 * @param queue
 */
void queue_destroy(queue_t *queue)
{
    node_t *next, *current = queue->front;

    while(current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    free(queue);
}

/**
 * @brief   Push data onto the front of a queue
 *
 * @param   queue
 * @param   data
 *
 * @return  0 on success, 1 if memory is exhausted
 */
int queue_push_front(queue_t *queue, void *data)
{
    node_t *new_node = node_create();

    /* Allocate memory for the node. Return false if memory is exhausted */
    if(new_node == NULL) {
        return 1;
    }

    new_node->data = data;

    new_node->next = queue->front;

    /* If the front is empty just add the node, otherwise link it in */
    if(queue->front == NULL) {
        queue->back = new_node;
    } else {
        queue->front->prev = new_node;
    }

    queue->front = new_node;

    return 0;
}

/**
 * @brief   Pop data from the back of the stack
 *
 * @param   queue
 *
 * @return  Data
 */
void *queue_pop_back(queue_t *queue)
{
    void *data = NULL;
    node_t *old_node = queue->back;

    /* Check that we have something to pop */
    if(old_node != NULL) {
        /* Relink the queue */
        if(old_node == queue->front) {
            queue->front = queue->back = NULL;
        } else {
            old_node->prev->next = NULL;
            queue->back = old_node->prev;
        }

        /* Get the data and remove the node */
        data = old_node->data;
        old_node->next = NULL;
        old_node->prev = NULL;
        free(old_node);
    }

    return data;
}

#if 0
int queue_push_back(queue_t *queue, char *data)
{
    node_t new_node;

    new_node.data = *data;

    /* Set the last node if queue is empty */
    if(queue->front == 0) {
        queue->front = &new_node;
        new_node.next = 0;
    }

    /* If the back is empty just add the node, otherwise link it in */
    if(queue->back == 0) {
        queue->back = &new_node;
    } else {
        new_node->next = queue->back;
        queue->back = &new_node;
    }
}

int queue_pop_front(queue_t *queue, char *data)
{
    node_t *old_node;

    /* Check that we have something to pop */
    if(queue->front == 0)
    {
        return 0;
    }

    /* Get the data and remove the node */
    old_node = queue->front;
    data = old_node->data;
    queue->front = old_node->next;

    old_node->next = 0;
    old_node = 0;

    return 1;
}
#endif

/**
 * @brief   Check if the queue is empty
 *
 * @param   queue
 *
 * @return  1 if queue is empty, 0 if not
 */
int queue_is_empty(const queue_t *queue)
{
    return queue->front == 0 && queue->back == 0;
}
