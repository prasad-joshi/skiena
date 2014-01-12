#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define IS_VALID_INDEX(nr, nc, r, c) \
	        (((r) >= 0 && (r) < (nr) && (c) >= 0 && (c) < (nc)) ? 1 : 0)

int count = 0;
int nrows = 0;
int ncols = 0;

typedef enum {
	START,

	ULD,	/* Up-left-down :) */
	ULU,
	URU,
	URD,

	DLU,
	DLD,
	DRD,
	DRU,

	INVAL
} DIRECTION;

void usage(char *s)
{
	fprintf(stderr, "\nUsage:\n\t %s <#rows> <#cols>\n", s);
}

unsigned long **allocate_board(int nrows, int ncols)
{
	unsigned long **b;
	unsigned long *t;
	int i;

	b = malloc(sizeof(*b) * nrows);
	if (b == NULL) {
		return (NULL);
	}

	t = malloc(sizeof(*t) * (nrows * ncols));
	if (t == NULL) {
		free(b);
		return (NULL);
	}
	memset(t, 0, sizeof(*t) * (nrows * ncols));

	for (i = 0; i < nrows; i++) {
		b[i] = t + (i * ncols);
	}

	return (b);
}

void free_board(unsigned long **board, int nrows, int ncols)
{
	free(board[0]);
	free(board);
}

void print_board_ulong(unsigned long **b)
{
	int i;
	int j;

	printf("START\n");
	for (i = 0; i < nrows; i++) {
		for (j = 0; j < ncols; j++) {
			printf(" %lu ", b[i][j]);
		}
		printf("\n");
	}
	printf("END\n");
}

void print_board(unsigned long **board)
{
	int i;
	int j;

	for (i = 0; i < ncols * 3 + 4; i++) {
		printf("=");
	}
	printf("\n");

	for (i = 0; i < nrows; i++) {
		for (j = 0; j < ncols; j++) {
			if (j == 0) {
				printf("| ");
			}

			if (board[i][j] != 0) {
				printf(" %c ", (int) board[i][j]);
			} else {
				printf(" %lu ", board[i][j]);
			}

			if (j == ncols - 1) {
				printf(" |");
			}
		}
		printf("\n");
	}
	for (i = 0; i < ncols * 3 + 4; i++) {
		printf("=");
	}
	printf("\n");
}

DIRECTION next_direction(DIRECTION d)
{
	return ((d == INVAL) ? INVAL : ++d);
}

void next_index(DIRECTION d, int *row, int *col)
{
	assert(d != INVAL);

	switch (d) {
	case ULD:
		*row += 1;
		*col -= 2;
		break;

	case ULU:
		*row += 2;
		*col -= 1;
		break;

	case URU:
		*row += 2;
		*col += 1;
		break;

	case URD:
		*row += 1;
		*col += 2;
		break;

	case DLU:
		*row -= 1;
		*col -= 2;
		break;

	case DLD:
		*row -= 2;
		*col -= 1;
		break;

	case DRD:
		*row -= 2;
		*col += 1;
		break;

	case DRU:
		*row -= 1;
		*col += 2;
		break;

	default:
		assert(0);
	}
}

int traverse_done(unsigned long **board)
{
	int i;
	int j;

	for (i = 0; i < nrows; i++) {
		for (j = 0; j < ncols; j++) {
			if (board[i][j] == 0) {
				return (0);
			}
		}
	}
	return (1);
}

void set_board_position(unsigned long **board, int row, int col, int nr, int nc)
{
	board[row][col] = (nr << 16 | nc << 8 | 1);
}

void get_board_position(unsigned long **board, int row, int col, int *nr, int *nc)
{
	*nr = board[row][col] >> 16;
	*nc = (board[row][col] >> 8) & 0xff;
}

int traverse_board(unsigned long **board, int row, int col, DIRECTION dir)
{
	int r;
	int c;

	board[row][col] = 1;
	if (traverse_done(board) == 1) {
		count++;
		return (1);
	}
	board[row][col] = 0;

	while ((dir = next_direction(dir)) != INVAL) {
		r = row;
		c = col;

		next_index(dir, &r, &c);

		if (IS_VALID_INDEX(nrows, ncols, r, c) == 0) {
			continue;
		}

		if (board[r][c] != 0) {
			continue;
		}

		set_board_position(board, row, col, r, c);
		if (traverse_board(board, r, c, START) == 1) {
			return (1);
		}
		board[row][col] = 0;
	}
	return (0);
}

void knight_tour(unsigned long **board, int r, int c)
{
	unsigned long **b;
	int n;
	int nr;
	int nc;

	b = allocate_board(nrows, ncols);
	assert(b != NULL);

	b[r][c] = 'K';
	print_board(b);
	for (n = 1; n < nrows * ncols; n++) {
		get_board_position(board, r, c, &nr, &nc);
		b[nr][nc] = 'K';
		print_board(b);
		r = nr;
		c = nc;
	}

	free_board(b, nrows, ncols);
}

int main(int argc, char *argv[])
{
	char *s_nrows;
	char *s_ncols;

	unsigned long **board;
	unsigned long *t;

	if (argc != 3) {
		usage(argv[0]);
		return (EINVAL);
	}

	s_nrows = argv[1];
	s_ncols = argv[2];

	assert(s_ncols != NULL && s_nrows != NULL);

	sscanf(s_nrows, "%d", &nrows);
	sscanf(s_ncols, "%d", &ncols);

	if (nrows == 0 || ncols == 0) {
		usage(argv[0]);
		return (EINVAL);
	}

	board = allocate_board(nrows, ncols);
	traverse_board(board, 0, 0, START);

	if (count != 0) {
		knight_tour(board, 0, 0);
	}
	free_board(board, nrows, ncols);
	return (0);
}
