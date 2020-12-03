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
#include <stddef.h>
#include <stdint.h>

#ifndef PHEAP_H
#define PHEAP_H

typedef struct PHeap PHeap;
struct PHeap {
	PHeap * subheaps; /* If this has a heap, it has a list of children */
	PHeap * next;  /* If this heap has a sibling to the right, this is it */
};

typedef int (*pheap_comparator_func)(PHeap *, PHeap *);

PHeap * pheap_insert(PHeap * heap, PHeap * elem, pheap_comparator_func comparator);
PHeap * pheap_delete_min(PHeap * heap, pheap_comparator_func comparator);
void pheap_visit_heap(PHeap * heap, void (*func)(PHeap *));
void * pheap_or(void * a, uintptr_t b);

#ifndef container_of
#define container_of(ptr, type, member) ((type *)((char *)(ptr) - offsetof(type, member)))
#endif

#define PHEAP_insert(H, E, C) container_of( \
	pheap_insert((H) ? &(H)->pheap : NULL, &(E)->pheap, (C)), \
	typeof(*H), pheap)

#define PHEAP_visit(H, F) (pheap_visit_heap(&(H)->pheap, (F)))
#define PHEAP_value(type, H) (container_of(H, type, pheap))
#define PHEAP_delete_min(H, C) container_of( \
	pheap_or(pheap_delete_min(&(H)->pheap, (C)), offsetof(typeof(*H), pheap)), \
	typeof(*H), pheap)

#endif /* PHEAP_H */
