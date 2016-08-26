#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

static void usage(const char *name)
{
	fprintf(stderr, "Usage: \n");
	fprintf(stderr, "\t %s <series of 0s and 1s>\n", name);
}

int main(int argc, char *argv[])
{
	int nelements;
	int *array;
	int i;
	int j;
	int low;
	int high;
	int t;

	if (argv[1] == NULL) {
		usage(argv[0]);
		return EINVAL;
	}

	nelements = argc - 1;

	array = malloc(sizeof(*array) * nelements);
	if (array == NULL) {
		return ENOMEM;
	}

	for (i = 1; i < argc; i++) {
		if ((argv[i][0] == '0' || argv[i][0] == '1') &&
				argv[i][1] == 0) {
			array[j] = argv[i][0] - '0';
			j++;
		} else {
			free(array);
			usage(argv[0]);
			return EINVAL;
		}
	}

	/*
	 * idea is to maintain 3 sections
	 *
	 * a[0]    .. a[low]       - contain 0s
	 * a[low]  .. a[high]      - unknow section
	 * a[high] .. a[nelements] - contains 1s
	 * 
	 * Idea is to shrink unknown section on each iteration
	 */

	low  = 0;
	high = nelements - 1;
	while (low <= high) {
		switch (array[low]) {
		default:
			assert(0);
			break;
		case 0:
			low++;
			break;
		case 1:
			/* SWAP */
			t           = array[low];
			array[low]  = array[high];
			array[high] = t;

			high--;
			break;
		}
	}

	for (i = 0; i < nelements; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");
	free(array);
	return 0;
}
