#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>

#include "sll.h"

struct tst_basic {
	sll_t    list;
	uint32_t number;
};

struct tst_basic *tst_basic_new(uint32_t no)
{
	struct tst_basic *tip;

	tip = malloc(sizeof(*tip));
	if (tip) {
		tip->number = no;
		sll_init(&tip->list);
	}
	return tip;
}

static void basic_test(void)
{
	const uint32_t    NODES = 50000;
	sll_t             head;
	int               rc;
	struct tst_basic *np;
	sll_t             *l;
	sll_t             *midp;
	uint32_t          n;
	uint32_t          i;
	uint32_t          j;

	sll_init(&head);

	rc = sll_is_empty(&head);
	assert(rc != 0);

	for (i = 0; i < NODES; i++) {
		np = tst_basic_new(i);
		assert(np != NULL);

		sll_insert_head(&head, &np->list);

		rc = sll_is_empty(&head);
		assert(rc == 0);

		midp = NULL;
		sll_get_mid(&head, &midp);
		assert(midp != NULL);
		np = container_of(midp, struct tst_basic, list);
		assert(np->number == (i >> 1));
	}

	for (j = 0; j < 10; j++) {
		i = NODES - 1;
		l = sll_next(&head);
		while (l != NULL) {
			np = container_of(l, struct tst_basic, list);
			assert(np != NULL);
			assert(np->number == i);
			i--;
			l = sll_next(l);
		}

		sll_reverse(&head);
		i = 0;
		l = sll_next(&head);
		while (l != NULL) {
			np = container_of(l, struct tst_basic, list);
			assert(np != NULL);
			assert(np->number == i);
			i++;
			l = sll_next(l);
		}

		sll_reverse_recursive(&head);
	}

	for (j = 0; j < 10; j++) {
		i = NODES - 1;
		l = sll_next(&head);
		while (l != NULL) {
			np = container_of(l, struct tst_basic, list);
			assert(np != NULL);
			assert(np->number == i);
			i--;
			l = sll_next(l);
		}

		sll_reverse_recursive(&head);
		i = 0;
		l = sll_next(&head);
		while (l != NULL) {
			np = container_of(l, struct tst_basic, list);
			assert(np != NULL);
			assert(np->number == i);
			i++;
			l = sll_next(l);
		}

		sll_reverse(&head);
	}

	while (sll_is_empty(&head) == 0) {
		l  = NULL;
		sll_delete_head(&head, &l);
		assert(l != NULL);

		np = container_of(l, struct tst_basic, list);
		free(np);

	}
}

int main(void)
{
	basic_test();
	return 0;
}
