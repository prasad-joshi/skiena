#ifndef __SLL_H__
#define __SLL_H__

#define offsetof(type, member) \
	((size_t)(&(((type *) 0)->member)))

#define container_of(ptr, type, member) \
	((type *) (((char *) ptr) - offsetof(type, member)))

typedef struct sll {
	struct sll *next;
} sll_t;

void  sll_init(sll_t *headp);
int   sll_is_empty(sll_t *headp);
void  sll_insert_head(sll_t *headp, sll_t *nodep);
void  sll_delete_head(sll_t *headp, sll_t **nodepp);
sll_t *sll_next(sll_t *nodep);
void  sll_get_mid(sll_t *headp, sll_t **nodepp);
void  sll_reverse(sll_t *headp);
void  sll_reverse_recursive(sll_t *headp);

#endif
