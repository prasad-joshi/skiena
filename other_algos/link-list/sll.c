#include <stdio.h>
#include <assert.h>

#include "sll.h"

void sll_init(sll_t *headp)
{
	assert(headp);
	headp->next = NULL;
}

int sll_is_empty(sll_t *headp)
{
	assert(headp);
	return headp->next == NULL;
}

void sll_insert_head(sll_t *headp, sll_t *nodep)
{
	assert(headp && nodep);

	nodep->next = headp->next;
	headp->next = nodep;
}

void sll_delete_head(sll_t *headp, sll_t **nodepp)
{
	sll_t *np;

	assert(headp && nodepp);

	if (headp->next == NULL) {
		*nodepp = NULL;
		return;
	}

	np          = headp->next;
	headp->next = np->next;
	*nodepp     = np;
}

sll_t *sll_next(sll_t *nodep)
{
	assert(nodep);
	return nodep->next;
}

void sll_get_mid(sll_t *headp, sll_t **midpp)
{
	sll_t *s;
	sll_t *f;

	assert(headp && midpp);

	s = headp->next;
	f = s;
	while (f != NULL) {
		f = f->next;
		if (f == NULL) {
			break;
		}
		f = f->next;
		s = s->next;
	}
	*midpp = s;
}

void sll_reverse(sll_t *headp)
{
	sll_t *p;
	sll_t *q;
	sll_t *r;

	assert(headp);

	p = NULL;
	q = headp->next;
	while (q != NULL) {
		r       = q->next;
		q->next = p;
		p       = q;
		q       = r;
	}
	headp->next = q;
}

static void _sll_reverse_recursive(sll_t *headp, sll_t *prevp, sll_t *nodep)
{
	assert(headp && nodep);

	if (nodep->next == NULL) {
		headp->next = nodep;
		nodep->next = prevp;
		return;
	}

	_sll_reverse_recursive(headp, nodep, nodep->next);
	nodep->next = prevp;
}

void sll_reverse_recursive(sll_t *headp)
{
	assert(headp);
	if (sll_is_empty(headp)) {
		return;
	}
	_sll_reverse_recursive(headp, NULL, headp->next);
}
