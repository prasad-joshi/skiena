/* Simple Binary Search Tree implementation */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int item_type;
void print_item(item_type item)
{
	printf("Item = %d\n", item);
}

typedef struct tree {
	item_type	item;
	struct tree	*parent;
	struct tree	*left;
	struct tree	*right;
} tree;

tree *search_tree(tree *t, item_type x)
{
	if (!t)
		return NULL;

	if (t->item == x)
		return t;

	if (t->item < x)
		return search_tree(t->right, x);
	else
		return search_tree(t->left, x);
}

tree *find_minimum(tree *t)
{
	if (!t)
		return NULL;

	while (t->left)
		t = t->left;
	return t;
}

tree *find_maximum(tree *t)
{
	if (!t)
		return NULL;

	while (t->right)
		t = t->right;
	return t;
}

void inorder_traverse(tree *t)
{
	if (!t)
		return;

	inorder_traverse(t->left);
	// print_item(t->item);
	printf("%d ",t->item);
	inorder_traverse(t->right);
}

void preorder_traverse(tree *t)
{
	if (!t)
		return;

	printf("%d ", t->item);
	preorder_traverse(t->left);
	preorder_traverse(t->right);
}

void postorder_traverse(tree *t)
{
	if (!t)
		return;

	postorder_traverse(t->left);
	postorder_traverse(t->right);
	printf("%d ", t->item);
}

tree *get_node(void)
{
	tree *n;

	n = malloc(sizeof(*n));
	if (!n)
		return NULL;
	n->parent = NULL;
	n->left   = NULL;
	n->right  = NULL;
	n->item   = 0;
	return n;
}

int insert_tree(tree **r, item_type x, tree *parent)
{
	tree *n;

	if (!*r) {
		n = get_node();
		if (!n)
			return -1;

		n->item   = x;
		n->parent = parent;
		*r        = n;
		return 0;
	}

	if ((*r)->item < x)
		return insert_tree(&(*r)->right, x, *r);
	else
		return insert_tree(&(*r)->left, x, *r);
}

int delete_tree(tree **r, item_type x)
{
	tree *d = *r;
	tree *p, *s, *l, *m;

	if (!d)
		return -1;

	if (d->item < x)
		return delete_tree(&(d->right), x);
	else if (d->item > x)
		return delete_tree(&(d->left), x);

	assert(d->item == x);

	if (!d->left && !d->right) {
		/* no child */
		p = d->parent;
		if (!p) {
			/* tree has only one element and we are deleting it. */
			*r = NULL;
			free(d);
			return 0;
		}

		if (p->left == d)
			p->left = NULL;
		else {
			assert (p->right == d);
			p->right = NULL;
		}

		free(d);
		return 0;
	} else if (!d->left || !d->right) {
		/* only one child */

		p = d->parent;
		if (!p) {
			/* tree has only 2 elements, root is being deleted */
			if (d->left) {
				*r = d->left;
			} else {
				assert(d->right);
				*r = d->right;
			}
			(*r)->parent = NULL;
			free(d);
			return 0;
		}

		if (!d->left) {
			s = d->right;
		} else {
			assert (!d->right);
			s = d->left;
		}

		if (p->left == d) {
			p->left = s;
		} else {
			assert(p->right == d);
			p->right = s;
		}
		s->parent = p;
		free(d);
		return 0;
	} else {
		assert (d->left && d->right);
		l = d->left;
		m = find_maximum(l);
		d->item = m->item;
		m->item = x;
		return delete_tree(&m, x);
	}
}

void free_tree(tree **r)
{
	if (!*r)
		return;

	free_tree(&(*r)->left);
	free_tree(&(*r)->right);
	free(*r);
	*r = NULL;
}

tree *root = NULL;

int main(void)
{
	const int	MAX = 20;
	int		i;
	int		no;
	int		array[MAX];

	for (i = 0; i < MAX; i++) {
		no = rand() % 100;
		array[i] = no;

		printf("Inserting: %d\n", no);

		if (insert_tree(&root, no, root) < 0)
			printf("Inserting %d failed.\n", no);
	}

	printf("Inorder: \n");
	inorder_traverse(root);
	printf("\nPreorder: \n");
	preorder_traverse(root);
	printf("\nPostorder: \n");
	postorder_traverse(root);
	printf("\n");

	printf("Verifying delete tree item is working: ");
	for (i = 0; i < MAX; i++) {
		delete_tree(&root, array[i]);
	}
	assert(root == NULL);
	printf("DONE\n");

	printf("Verifying free tree is working: ");
	for (i = 0; i < MAX; i++) {
		no = rand() % 100;
		if (insert_tree(&root, no, root) < 0)
			printf("Inserting %d failed.\n", no);
	}
	free_tree(&root);
	assert(root == NULL);
	printf("DONE\n");
	return 0;
}
