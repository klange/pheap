/**
 * Simple intrusive pairing heap implementation.
 *
 * Based on pseudo-code description from the article "Pairing Heap"
 * on Wikipedia.
 *
 * Copyright 2020 K. Lange <klange@toaruos.org> All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "pheap.h"

/**
 * meld - Combine two heaps.
 *        Melding is an O(1) operation.
 */
static PHeap * pheap_meld(PHeap * left, PHeap * right, pheap_comparator_func comparator) {
	/*
	 * If either of the heaps is "empty" (represented by NULL),
	 * then simply return the other one.
	 */
	if (!left) {
		return right;
	}
	if (!right) {
		return left;
	}

	/*
	 * Otherwise, pull the 'smaller' of the two up and add the 'larger'
	 * to the front of the subheap list of the smaller one. We use
	 * intrusive lists within our Heap struct, so each Heap is also
	 * a List node (with a `next` pointer).
	 */
	if (comparator(left, right)) {
		/* Turns `left` into Heap(left→value, right :: left→subheaps) */
		if (left->subheaps) {
			right->next = left->subheaps;
		}
		left->subheaps = right;
		return left;
	} else {
		/* Turns `right` into Heap(right→value, left :: right→subheaps) */
		if (right->subheaps) {
			left->next = right->subheaps;
		}
		right->subheaps = left;
		return right;
	}
}

/**
 * insert - add a new element to an existing heap (which may be empty)
 *          Since inserting is just a meld, it is O(1)
 */
PHeap * pheap_insert(PHeap * heap, PHeap * elem, pheap_comparator_func comparator) {
	/*
	 * Inserting is a simple matter of making a new Heap(elem, []) and
	 * then melding it with the existing heap. We store Heap objects
	 * on the, uh, heap (system heap, that is).
	 */
	elem->subheaps = NULL;
	elem->next = NULL;
	return pheap_meld(elem, heap, comparator);
}

/**
 * merge_pairs - the core of the pairing heap, performs a left-to-right and
 *               then right-to-left merge of a list of subheaps.
 *               This is O(log n) as we must recursively meld to
 *               perform the two-stage merge process.
 */
static PHeap * pheap_merge_pairs(PHeap * list, pheap_comparator_func comparator) {
	if (!list) {
		/* An empty list is represented by NULL, and yields an empty Heap,
		 * which is also represented by NULL... */
		return NULL;
	} else if (list->next == NULL) {
		/* If a list entry doesn't have a next, it has a size of one,
		 * and we can just return this heap directly. */
		return list;
	} else {
		/* Otherwise we meld the first two, then meld them with the result of
		 * recursively melding the rest, which performs our left-right /
		 * right-left two-stage merge. */
		PHeap * next  = list->next;
		list->next = NULL;
		PHeap * rest = next->next;
		next->next = NULL;
		return pheap_meld(pheap_meld(list, next, comparator), pheap_merge_pairs(rest, comparator), comparator);
	}
}

/**
 * delete_min - Remove the 'smallest' value from the heap.
 *              As a fundamental property of a heap, this should always
 *              be the "root" node, and should never be part of a list
 *              of subheaps, so we just need to rebalance its own children.
 *              As deleting consists of a call to merge_pairs, it is O(log n)
 */
PHeap * pheap_delete_min(PHeap * heap, pheap_comparator_func comparator) {
	PHeap * subs = heap->subheaps;
	return pheap_merge_pairs(subs, comparator);
}

/**
 * visit_heap - Call a user function for ever node in the heap.
 */
void pheap_visit_heap(PHeap * heap, void (*func)(PHeap *)) {
	if (!heap) return;
	func(heap);
	pheap_visit_heap(heap->subheaps, func);
	pheap_visit_heap(heap->next, func);
}

void * pheap_or(void * a, uintptr_t b) {
	return (a == NULL) ? (void *)b : a;
}
