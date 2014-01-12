#ifndef __QUEUE_H__
#define __QUEUE_H__

struct list {
    struct list *next;
    void *data;
};

struct queue {
    struct list *front;
    struct list *rear;
};

void init_queue(struct queue *queue);
int enqueue(struct queue *queue, void *data);
void *dequeue(struct queue *queue);

int is_empty(struct queue *queue);
#endif
