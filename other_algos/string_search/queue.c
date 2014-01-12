#include <stdlib.h>
#include <assert.h>
#include "queue.h"

#define LIST_POISON	(1ULL)

static struct list *get_new(void)
{
	struct list *l;

	l = malloc(sizeof(*l));
	if (l == NULL) {
		return (NULL);
	}

	l->next = NULL;
	l->data = NULL;
	return (l);
}

static int add_tail(struct list **head, struct list **tail, void *data)
{
	struct list *h;
	struct list *t;
	struct list *n;

	if (head == NULL || tail == NULL) {
		return (-1);
	}

	h = *head;
	t = *tail;
	n = get_new();
	if (n == NULL) {
		return (-1);
	}

	n->data = data;

	if (h == NULL) {
		t = h = n;
	} else {
		t->next = n;
		t = n;
	}

	*head = h;
	*tail = t;
	return (0);
}

static struct list *remove_head(struct list **head, struct list **tail)
{
	struct list *h;
	struct list *t;
	struct list *n;

	if (head == NULL || tail == NULL) {
		return (NULL);
	}

	h = *head;
	t = *tail;

	if (h == NULL) {
		assert(t == NULL);
		return (NULL);
	}

	n = h->next;
	if (n == NULL) {
		t = NULL;
	}

	*head = n;
	*tail = t;
	return h;
}

int is_empty(struct queue *queue)
{
	if (queue->front == NULL) {
		assert(queue->rear == NULL);
		return (1);
	}

	assert(queue->rear != NULL);
	return (0);
}

void init_queue(struct queue *queue)
{
	queue->front = queue->rear = NULL;
}

int enqueue(struct queue *queue, void *data)
{
	return (add_tail(&queue->front, &queue->rear, data));
}

void *dequeue(struct queue *queue)
{
	struct list *l = remove_head(&queue->front, &queue->rear);
	void *data = NULL;

	if (l == NULL) {
		return (NULL);
	}
	data = l->data;
	free(l);
	return (data);
}
