#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

static void usage(const char *name)
{
	fprintf(stderr, "Usage: \n");
	fprintf(stderr, "\t %s <series of 0s, 1s, and 2s>\n", name);
}

static void swap(int *no1, int *no2)
{
	int t;
	t    = *no1;
	*no1 = *no2;
	*no2 = t;
}

int main(int argc, char *argv[])
{
	int  nelements;
	int  *array;
	char c;
	int  i;
	int  j;
	int  low;
	int  high;
	int  mid;

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
		c = argv[i][0];
		if ((c == '0' || c == '1' || c == '2') && argv[i][1] == 0) {
			array[j] = c - '0';
			j++;
		} else {
			free(array);
			usage(argv[0]);
			return EINVAL;
		}
	}

	/*
	 * idea is to maintain 4 sections
	 *
	 * a[0]      .. a[low-1]     - contain 0s
	 * a[low]    .. a[mid-1]     - contains 1s
	 * a[mid]    .. a[high]      - unknown
	 * a[high+1] .. a[nelements] - contains 2s
	 * 
	 * Idea is to shrink unknown section on each iteration
	 */

	low  = 0;
	mid  = 0;
	high = nelements - 1;
	while (mid <= high) {
		switch (array[mid]) {
		default:
			assert(0);
			break;
		case 0:
			/*
			 *   0 0 0 1 1 1 1 1 ? ? ? ? 2 2 2 2
			 *         ^         ^     ^
			 *         |         |     |
			 *        low       mid   high
			 *
			 *   if (array[mid] == 0) {
			 *       swap array[mid] and array[low]
			 *       increment both low and mid
			 *   }
			 */
			swap(&array[low], &array[mid]);
			low++;
			mid++;
			break;
		case 1:
			/*
			 *   0 0 0 1 1 1 1 1 ? ? ? ? 2 2 2 2
			 *         ^         ^     ^
			 *         |         |     |
			 *        low       mid   high
			 *
			 *   if (array[mid] == 1) {
			 *       increment mid;
			 *   }
			 */
			mid++;
			break;
		case 2:
			/*
			 *   0 0 0 1 1 1 1 1 ? ? ? ? 2 2 2 2
			 *         ^         ^     ^
			 *         |         |     |
			 *        low       mid   high
			 *
			 *   if (array[mid] == 2) {
			 *       swap array[mid] and array[high]
			 *       decrement high - mid remains as it is
			 *   }
			 */
			swap(&array[mid], &array[high]);
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
