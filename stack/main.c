#include <stdio.h>

void func(int i)
{
	char array[i];

	for (int j = 0; j < i; j++) {
		array[j] = 0;
	}
}

int main()
{
	func(4);
	return 0;
}
