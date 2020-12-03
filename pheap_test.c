#include <stdio.h>
#include <stdlib.h>

#include "pheap.h"

struct Sample {
	int value;
	PHeap pheap;
};

struct Sample * heap = NULL;

int comparator(PHeap * left, PHeap * right) {
	struct Sample * l = PHEAP_value(struct Sample, left);
	struct Sample * r = PHEAP_value(struct Sample, right);

	return l->value < r->value;
}

void print_values(PHeap * value) {
	struct Sample * e = PHEAP_value(struct Sample, value);

	fprintf(stderr, "%p = %d\n", e, e->value);
}

int main(int argc, char * argv[]) {

	int heap_max = atoi(argv[1]);
	int heap_size = 0;

	for (int i = 2; i < argc; ++i) {
		int value = atoi(argv[i]);
		struct Sample * elem;
		if (heap_size != heap_max) {
			elem = malloc(sizeof(struct Sample));
			heap_size++;
			fprintf(stderr, "Heap will now contain %d elems\n", heap_size);
		} else if (heap && value > heap->value) {
			fprintf(stderr, "%d > %d so will remove the latter\n", value, heap->value);
			elem = heap;
			heap = PHEAP_delete_min(heap, comparator);
			fprintf(stderr, "which makes the heap head now at %p = %d\n", heap, heap->value);
		} else {
			fprintf(stderr, "Heap is full and %d does not qualify (min heap = %d)\n", value, heap->value);
			continue;
		}
		elem->value = value;
		heap = PHEAP_insert(heap, elem, comparator);
		fprintf(stderr, "Adding %d to heap\n", elem->value);
		fprintf(stderr, "Heap head is now %d (%p)\n", heap->value, heap);
		PHEAP_visit(heap, print_values);
	}

	fprintf(stderr, "Elements in heap:\n");
	PHEAP_visit(heap, print_values);

	fprintf(stderr, "Sorting and drestryoing:\n");
	while (heap) {
		struct Sample * min = heap;
		heap = PHEAP_delete_min(heap, comparator);
		printf("%d\n", min->value);
		free(min);
		fprintf(stderr, "heap = %p\n", heap);
	}

	return 0;
}
