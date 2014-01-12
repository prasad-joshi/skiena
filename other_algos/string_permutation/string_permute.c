#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void usage(char *name)
{
	fprintf(stderr, "\nUsage: \n\t %s <string>\n", name);
}

void print(char *a, int elements)
{
	int i;
#if 0
	for (i = 0; i < elements; i++) {
		printf("%c", a[i]);
	}
	printf("\n");
#else
	printf("%s\n", a);
#endif
}

void swap(char *a, int i, int j)
{
	char t = a[i];
	a[i] = a[j];
	a[j] = t;
}

void permute(char *a, int start, int elements)
{
	int i;

	if (start == elements) {
		print(a, elements);
		return;
	}

	for (i = start; i < elements; i++) {
		swap(a, i, start);
		permute(a, start + 1, elements);
		swap(a, i, start);
	}
}

int main(int argc, char *argv[])
{
	//char a[] = {'a', 'b', 'c', 'd', 0};

	char *a = argv[1];
	char *b;

	if (a == NULL) {
		usage(argv[0]);
		return (1);
	}

	b = strdup(a);
	assert(b != NULL);

	permute(b, 0, strlen(b));
	return (0);
}
