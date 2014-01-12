#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "queue.h"

#define M	5
#define N	10

#define P	15
#define Q	(N)	

#define IS_VALID_INDEX(nr, nc, r, c) \
	(((r) >= 0 && (r) < (nr) && (c) >= 0 && (c) < (nc)) ? 1 : 0)

typedef enum {
	START,
	RIGHT,
	DIAG,
	DOWN,
	INVAL
} DIRECTION;

char arr[M][N] = {
	{ 'P','P','P','P','P','P','P','P','A','B' },
	{ 'P','P','P','P','A','A','B','A','P','C' },
	{ 'P','P','P','P','B','P','P','B','C','P' },
	{ 'P','P','P','P','C','C','P','C','P','P' },
	{ 'P','P','P','P','P','P','P','P','P','P' },
};

#if 1
char arr1[P][N] = {
	{ 'P','P','S','P','P','P','P','P','P','P' },
	{ 'P','P','A','C','H','P','P','P','P','P' },
	{ 'P','P','P','H','I','P','P','P','P','P' },
	{ 'P','P','P','P','N','P','P','P','P','P' },
	{ 'P','P','P','P','P','P','P','P','P','P' },
	{ 'P','P','P','P','P','P','P','P','P','P' },
	{ 'P','P','P','P','P','P','P','P','P','P' },
	{ 'P','P','S','A','C','H','I','N','P','P' },
	{ 'P','P','A','A','P','H','I','P','P','P' },
	{ 'P','P','C','C','C','P','P','P','P','P' },
	{ 'P','P','H','H','P','H','P','P','P','P' },
	{ 'P','P','I','I','P','P','I','P','P','P' },
	{ 'P','P','N','P','P','P','P','N','P','P' },
	{ 'P','P','P','P','P','P','P','P','P','P' },
	{ 'P','P','P','P','P','P','P','P','P','P' },
};
#else
char arr1[P][N] = {
	{ 'P','P','S','P','P','P','P','P','P','P' },
	{ 'P','P','A','C','H','P','P','P','P','P' },
	{ 'P','P','P','H','I','P','P','P','P','P' },
	{ 'P','P','P','P','N','P','P','P','P','P' },
	{ 'P','P','P','P','P','P','P','P','P','P' },
	{ 'P','P','P','P','P','P','P','P','P','P' },
	{ 'P','P','P','P','P','P','P','P','P','P' },
	{ 'P','P','S','Q','Q','Q','Q','Q','P','P' },
	{ 'P','P','A','Q','P','Q','Q','P','P','P' },
	{ 'P','P','C','Q','Q','P','P','P','P','P' },
	{ 'P','P','H','Q','P','Q','P','P','P','P' },
	{ 'P','P','I','Q','P','P','Q','P','P','P' },
	{ 'P','P','N','P','P','P','P','Q','P','P' },
	{ 'P','P','P','P','P','P','P','P','P','P' },
	{ 'P','P','P','P','P','P','P','P','P','P' },
};
#endif

int count = 0;

DIRECTION next_direction(DIRECTION d)
{
	return ((d == INVAL) ? INVAL : ++d);
}

void next_index(DIRECTION d, int *row, int *col)
{
	switch (d) {
	case RIGHT:
		*col += 1;
		break;
	case DIAG:
		*col += 1;
		*row += 1;
		break;
	case DOWN:
		*row += 1;
		break;
	default:
		assert(0);
	}
}

void prev_index(DIRECTION d, int *row, int *col)
{
	switch (d) {
	case RIGHT:
		*col -= 1;
		break;
	case DIAG:
		*col -= 1;
		*row -= 1;
		break;
	case DOWN:
		*row -= 1;
		break;
	default:
		assert(0);
	}
}

int __rec_string_search(char arr[][N], int nrow, int ncol, DIRECTION dir, int r,
		int c, char *s)
{
	int row = r;
	int col = c;
	if (*s == 0) {
		count++;
		return (0);
	}

	while ((dir = next_direction(dir)) != INVAL) {
		r = row;
		c = col;
		next_index(dir, &r, &c);

		if (IS_VALID_INDEX(nrow, ncol, r, c) == 0) {
			continue;
		}

		if (arr[r][c] != *s) {
			continue;
		}

		__rec_string_search(arr, nrow, ncol, START, r, c, s + 1);
	}
}

int rec_string_search(char arr[][N], int nrow, int ncol, char *str)
{
	char s;
	int i;
	int j;

	s = *str;
	for (i = 0; i < nrow; i++) {
		for (j = 0; j < ncol; j++) {
			if (arr[i][j] == s) {
				__rec_string_search(arr, nrow, ncol, START, i,
						j, str + 1);
			}
		}	
	}
}

void print(char arr[][N], int nrow, int ncol)
{
	int i;
	int j;

	for (i = 0; i < nrow; i++) {
		for (j = 0; j < ncol; j++) {
			if (arr[i][j] == 'P') {
			printf("  %c  ", arr[i][j]);
			} else {
			printf(" *%c* ", arr[i][j]);
			}
		}
		printf("\n");
	}
}

struct q_data {
	char *s;
	int r;
	int c;
};

struct q_data *get_new()
{
	struct q_data *qd;

	qd = malloc(sizeof(*qd));
	assert(qd != NULL);
	return (qd);
}

void __iter_string_search(char arr[][N], int nrow, int ncol, DIRECTION dir,
		int r, int c, char *s)
{
	struct queue q;
	struct q_data *qd;
	struct q_data *t;

	char *str;
	int row;
	int col;

	init_queue(&q);

	qd = get_new();
	assert(qd != NULL);
	qd->s = s;
	qd->r = r;
	qd->c = c;

	assert(arr[r][c] == *s);
	enqueue(&q, qd);

	while ((t = dequeue(&q)) != NULL) {
		dir = START;
		if (*(t->s + 1) == 0) {
			count++;
			free(t);
			continue;
		}

		while ((dir = next_direction(dir)) != INVAL) {
			str = t->s;
			row = t->r;
			col = t->c;

			next_index(dir, &row, &col);
			if (IS_VALID_INDEX(nrow, ncol, row, col) == 0) {
				continue;
			}

			if (arr[row][col] != *(str + 1)) {
				continue;
			}

			qd = get_new();
			qd->s = str + 1;
			qd->r = row;
			qd->c = col;
			enqueue(&q, qd);
		}
		free(t);
	}
}

void iter_string_search(char arr[][N], int nrow, int ncol, char *str)
{
	char s;
	int i;
	int j;

	s = *str;
	for (i = 0; i < nrow; i++) {
		for (j = 0; j < ncol; j++) {
			if (arr[i][j] == s) {
				__iter_string_search(arr, nrow, ncol, START, i, j, str);
			}
		}
	}
}

int main()
{
	char *s = "ABC";
	char *s1 = "SACHIN";

	print(arr, M, N);
	count = 0;
	rec_string_search(arr, M, N, s);
	printf("%s recursive count = %d\n", s, count);

	count = 0;
	iter_string_search(arr, M, N, s);
	printf("%s iterative count = %d\n", s, count);

	printf("\n\n");

	print(arr1, P, Q);
	count = 0;
	rec_string_search(arr1, P, Q, s1);
	printf("%s recursive count = %d\n", s1, count);

	count = 0;
	iter_string_search(arr1, P, Q, s1);
	printf("%s iterative count = %d\n", s1, count);

	return (0);
}
