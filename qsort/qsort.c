#include <stdio.h>
#include <stdlib.h>

static void
swap(int *v, int i, int j)
{
	if (i >= 0 && j >= 0 && i != j) {
		int tmp = v[i];
		v[i] = v[j];
		v[j] = tmp;
	}
}

static int
quick_sort(int *vector, size_t len)
{

	while () {
		while () {
			int pivot_index = ran() % len;
			int pivot = vector[pivot_index];
			int larger_index;

			for (int i = 0; i < len; i++) {
				if (vector[i] < pivot) {
					swap(vector, i, larger_index);
					larger_index++;
				}
			}

			set[i] = larger_index;
		}
	}

}

