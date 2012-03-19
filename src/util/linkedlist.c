/*
 * Copyright (c) Members of the EGEE Collaboration. 2006-2010.
 * See http://www.eu-egee.org/partners/ for details on the copyright holders.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <stdio.h>

#include "linkedlist.h"
#include "log.h"

/**
 * List node
 */
struct linkedlist_node {
    void * element;
    struct linkedlist_node * next;
};

/**
 * Linked list type
 */
struct linkedlist {
    size_t length;
    struct linkedlist_node * head;
    struct linkedlist_node * tail;
};

linkedlist_t * llist_create( void ) {
	linkedlist_t * list= calloc(1,sizeof(linkedlist_t));
	if (list == NULL) {
		log_error("llist_create: can't allocate linkedlist_t.");
		return NULL;
	}
	list->head= NULL;
	list->tail= list->head;
	list->length= 0;
	return list;
}

size_t llist_length(const linkedlist_t * list) {
	if (list == NULL) {
		log_error("llist_length: NULL pointer list.");
		return 0;
	}
	return list->length;
}

int llist_add(linkedlist_t * list, void * element) {
    struct linkedlist_node * node;
	if (list == NULL) {
		log_error("llist_add: NULL pointer list.");
		return LLIST_ERROR;
	}
    node= calloc(1,sizeof(struct linkedlist_node));
	if (node == NULL) {
		log_error("llist_add: can't allocate linkedlist node.");
		return LLIST_ERROR;
	}
    node->element= element;
    node->next= NULL;
    if (list->head == NULL) {
        /* empty list */
        list->head= node;
    }
    else {
        list->tail->next= node;
    }
    list->tail= node;
    list->length++;
    return LLIST_OK;
}

void * llist_get(linkedlist_t * list, int i) {
	int j;
    struct linkedlist_node * current;
	if (list == NULL) {
		log_error("llist_get: NULL pointer list.");
		return NULL;
	}
	if (i < 0 || i >= list->length) {
		log_error("llist_get: index %d out of range.", i);
		return NULL;
	}
	current= list->head;
    j= 0;
	while( j < i ) {
		if (current == NULL) {
			log_error("llist_get: element at %d is NULL.",j);
			return NULL;
		}
		current= current->next;
		j++;
	}
	return current->element;
}

/**
 * Removes the element at index i [0..n-1] from the linked list.
 * Returns the removed element or NULL.
 */
void * llist_remove(linkedlist_t * list, int i) {
    int j;
    struct linkedlist_node * current, * previous;
    void * element;
	if (list == NULL) {
		log_error("llist_remove: NULL pointer list.");
		return NULL;
	}
	if (i < 0 || i >= list->length) {
		log_error("llist_remove: index %d out of range.", i);
		return NULL; /* empty list case included */
	}
	j= 0;
	current= list->head;
	previous= NULL;
	while( j++ < i ) {
		if (current == NULL) {
			log_error("llist_remove: index %d not found, NULL at %d.", i, j);
			return NULL;
		}
		previous= current;
		current= current->next;
	}
	if (current == list->head) {
		/* first element */
		list->head= current->next;
	}
	else {
		/* not the first element */
		previous->next= current->next;
	}
	if (current == list->tail) {
		/* last element */
		list->tail= previous;
	}

	element= current->element;
	free(current);
	list->length--;
	return element;
}

int llist_delete_elements(linkedlist_t * list, delete_element_func deletef) {
    size_t list_l;
    void ** unique_elts;
    void * elt, * unique_elt;
    int unique_elts_l, i, j, duplicated;
	if (list == NULL) {
		log_error("llist_delete_elements: NULL pointer list.");
		return LLIST_ERROR;
	}
	/* WARN: the list can contains many times the same element (same memory address) */
	list_l= llist_length(list);
	unique_elts= calloc(list_l,sizeof(void *));
	unique_elts_l= 0, i= 0, j= 0;
	for (i=0; i<list_l; i++) {
		elt= llist_get(list,i);
		/* add elt to elts list if not already included */
		duplicated= 0;
		for(j= 0; j<unique_elts_l; j++) {
			if (elt == unique_elts[j]) {
				duplicated= 1;
			}
		}
		if (!duplicated) {
			/* add at end */
			unique_elts[unique_elts_l++]= elt;
		}
	}
	/* apply delete func on unique element */
	for(i= 0; i<unique_elts_l; i++) {
		unique_elt= unique_elts[i];
		if (deletef) {
			deletef(unique_elt);
		}
	}
	free(unique_elts);
	return LLIST_OK;
}

int llist_delete(linkedlist_t * list) {
    struct linkedlist_node * current, * next;
	if (list == NULL) {
		log_error("llist_delete: NULL pointer list.");
		return LLIST_ERROR;
	}
	current= list->head;
    while( current != NULL ) {
        next= current->next;
        free(current);
        current= next;
    }
    free(list);
    list= NULL;
    return LLIST_OK;
}

