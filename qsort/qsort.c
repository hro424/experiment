#include <stdio.h>
#include <stdlib.h>


typedef struct list_t {
	int start;
	int end;
} List_t;

static int wp;
static int rp;
static int list_length;
static List_t *tasklist;

static int
init_tasklist(size_t length)
{
	wp = 0;
	rp = 0;
	list_length = length;
	tasklist = calloc(sizeof(List_t), list_length);
	if (tasklist == NULL) {
		return -1;
	}
	return 0;
}

static void
push_task(int start, int end)
{
	tasklist[wp].start = start;
	tasklist[wp].end = end;
	wp++;
	if (wp == list_length) {
		wp = 0;
	}
}

static List_t *
pop_task(void)
{
	if (rp == wp) {
		return NULL;
	}

	List_t *ptr = tasklist + rp;
	rp++;
	if (rp == list_length) {
		rp = 0;
	}
	return ptr;
}

static int
choose_pivot(List_t *list)
{
	int index = rand() % (list->end - list->start);
	return index + list->start;
}

static void
swap(int *v, int i, int j)
{
	if (i >= 0 && j >= 0 && i != j) {
		int tmp = v[i];
		v[i] = v[j];
		v[j] = tmp;
	}
}

static void
dump(int *vector, size_t length)
{
	printf("D: ");
	for (size_t i = 0; i < length; i++) {
		printf("%d ", vector[i]);
	}
	putchar('\n');
}

static int
quick_sort(int *vector, size_t length)
{
	init_tasklist(length);
	push_task(0, length);

	for (;;) {
		List_t *list = pop_task();
		if (list == NULL) {
			break;
		}

		if (list->start + 1 == list->end) {
			continue;
		}

		int pivot = vector[choose_pivot(list)];
		int swap_ptr = list->start;

		dump(vector, length);
		printf("start: %d end: %d pivot: %d\n",
		       list->start, list->end, pivot);

		for (int i = list->start; i < list->end; i++) {
			if (vector[i] < pivot) {
				swap(vector, i, swap_ptr);
				swap_ptr++;
			}
		}

		if (swap_ptr - list->start > 2) {
			push_task(list->start, swap_ptr);
		}

		if (list->end - swap_ptr > 2) {
			push_task(swap_ptr, list->end);
		}
	}

	return 0;
}

#define N	10

int
main(int argc, char *argv[])
{
	int test_vector[N];
	int length = N;

	for (int i = 0; i < length; i++) {
		test_vector[i] = rand();
	}

	dump(test_vector, length);
	quick_sort(test_vector, length);
	dump(test_vector, length);

	return 0;
}

