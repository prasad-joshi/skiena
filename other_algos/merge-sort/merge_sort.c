#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

unsigned long long merge_inversion(int min, int max, int *array)
{
	int ae;		/* #elements in array */
	int le;		/* #elements in left sub-array */
	int re;		/* #elements in right sub-array */
	int mid;

	/* left and right sub-array */
	int *la;
	int *ra;

	/* indexes for each array */
	int ai;
	int li;
	int ri;

	unsigned long long inversion;

	assert(min < max);

	ae  = max - min + 1;
	le  = ae / 2;
	re  = ae - le;
	mid = min + le;

	la = malloc(sizeof(*la) * le);
	assert(la);

	ra = malloc(sizeof(*ra) * re);
	assert(ra);

	/* copy sorted left sub-array */
	for (li = 0, ai = min; li < le; li++, ai++)
		la[li] = array[ai];

	/* copy sorted right sub-array */
	for (ri = 0, ai = mid; ri < re; ri++, ai++)
		ra[ri] = array[ai];

	inversion = 0;
	li        = 0;
	ri        = 0;
	for (ai = min; ai <= max; ai++) {
		if (li >= le) {
			array[ai] = ra[ri];
			ri++;
		} else if (ri >= re) {
			array[ai] = la[li];
			li++;
		} else {
			if (la[li] > ra[ri]) {
				array[ai] = ra[ri];
				ri++;

				inversion += (le - li);
			} else {
				array[ai] = la[li];
				li++;
			}
		}
	}
	return inversion;
}

unsigned long long  mergesort_inversion(int min, int max, int *array)
{
	int mid;

	/* inversions from left, right sub-arrays */
	unsigned long long li;
	unsigned long long ri;

	/* inversions due to merge of sub-arrays */
	unsigned long long mi;

	if (min >= max)
		return 0;

	mid = min + ((max - min + 1) / 2);

	li = mergesort_inversion(min, mid - 1, array);
	ri = mergesort_inversion(mid, max, array);
	mi = merge_inversion(min, max, array);

	return li + ri + mi;
}

void usage(const char *p)
{
	fprintf(stderr, "Usage: %s <no elements> <file>\n", p);
}

int main(int argc, char **argv)
{
	int no_elements;
	char *file_name;
	int *array;

	FILE *file;
	int i;
	int rc;
	int no;

	unsigned long long inversions;

	if (!argv[1] || !argv[2]) {
		usage(argv[0]);
		return EINVAL;
	}

	no_elements = atoi(argv[1]);
	file_name   = argv[2];

	array = malloc(sizeof(*array) * no_elements);
	assert(array);

	file = fopen(file_name, "r");
	if (!file) {
		perror("fopen");
		return EINVAL;
	}

	i = 0;
	while (!feof(file)) {
		rc = fscanf(file, "%d\n", &no);
		if (rc == EOF) {
			assert(errno == 0);
			break;
		}
		array[i] = no;
		i++;
	}

	if (i != no_elements) {
		fprintf(stderr, "Number of elements does no match.\n");
		fprintf(stderr, "Please check you input.\n");
		usage(argv[0]);
		return EINVAL;
	}

	inversions = mergesort_inversion(0, no_elements - 1, array);

#if 0
	printf("Sorted Array: \n");
	for (i = 0; i < no_elements; i++) {
		printf("%d ", array[i]);
	}
#endif

	printf("\nInversions required to sort array: %llu\n", inversions);
	return 0;
}
