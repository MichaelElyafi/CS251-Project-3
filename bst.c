#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

/* Michael Elyafi project3
*/

/*
int max_sb_height(int n) {
	unsigned int big;

	if (n <= 0) return -1;
	if (n == 1) return 0;
	big = (n - 1) - (n / 3);  // makes as imbalanced as possible within rules
	//    of being size-balanced

	return 1 + max_sb_height(big);
}

*/
struct bst_node {
	int val;
	int size;
	int lNode;
	int rNode;
	int work;
	struct bst_node *left;
	struct bst_node *right;
};
typedef struct bst_node NODE;


struct bst {
	NODE *root;
};

/**
* function:  bst_create()
* desc:      allocates and initizlizes
*            an empty BST and returns it as
*            a BST pointer.
*/
BST * bst_create(){
	BST * t = malloc(sizeof(struct bst));
	t->root = NULL;
	return t;
}

/**
* function:  free_r()
* desc:      recursive helper function deallocating
*            the nodes of a tree rooted at NODE r
*/
static void free_r(NODE *r){
	if (r == NULL) return;
	free_r(r->left);
	free_r(r->right);
	free(r);
}

/**
* function:  bst_free()
* desc:      deallocates all storage associated with
*            BST given by t.
*/
void bst_free(BST * t){
	free_r(t->root);
	free(t);
}

NODE * bst_balance(NODE *r){
	BST * t2 = bst_create();
	t2->root = r;

	int * arr = bst_to_array(t2);
	t2 = bst_from_sorted_arr(arr, t2->root->size);
	return t2->root;

}

/**
* function:  insert()
* desc:      recursive helper function inserting x into
*            binary search tree rooted  at r.
*
* returns:   pointer to root of tree after insertion.
*
* notes:     if x is already in tree, no modifications are made.
*/
static NODE * insert(NODE *r, int x, int *check){
	NODE *leaf;
	if (r == NULL){
		leaf = malloc(sizeof(NODE));
		leaf->left = NULL;
		leaf->right = NULL;
		leaf->val = x;
		leaf->lNode = 0;
		leaf->rNode = 0;
		leaf->work = 0;
		leaf->size = 1;
		return leaf;
	}

	if (r->val == x){
		return r;
	}
	if (x < r->val){
		r->left = insert(r->left, x, check);
		r->size++;
		r->lNode++;
		if (r->lNode > 2 * r->rNode + 1){
			*check = 1;
		}
		if (*check){
			if (r->lNode <= 2 * r->rNode + 1){
				r->work += r->lNode;
				r->left = bst_balance(r->left);
				*check = 0;
			}

		}
		return r;
	}
	else {
		r->right = insert(r->right, x, check);
		r->size++;
		r->rNode++;
		if (r->rNode > 2 * r->lNode + 1){
			*check = 1;
		}
		if (*check){
			if (r->rNode <= 2 * r->lNode + 1){
				r->work += r->rNode;
				r->right = bst_balance(r->right);
				*check = 0;
			}
		}
		return r;
	}
}

// how about an iterative version?
static NODE *insert_i(NODE *r, int x){

	return NULL;

}

/**
* function:  bst_insert()
* desc:      inserts x into BST given by t.  Note that
*            a BST stores a SET -- no duplicates.  Thus,
*            if x is already in t when call made, no
*            modifications to tree result.
*
* note:      helper function does most of the work.
*
*/
void bst_insert(BST * t, int x){
	int check = 0;
	t->root = insert(t->root, x, &check);

	if (check){
		t->root->work += t->root->size;
		t->root = bst_balance(t->root);
		check = 0;
	}
}

/**
* function:  bst_contains()
* desc:      returns 0 or 1 depending on whether x is an
*            element of BST given by pointer t.
*
*/
int bst_contains(BST * t, int x){
	NODE *p = t->root;

	while (p != NULL){

		if (p->val == x)
			return 1;
		if (x < p->val){
			p = p->left;
		}
		else
			p = p->right;
	}
	return 0;
}

/**
* function:  min_h()
* desc:      helper function returning the minimum value
*            stored in tree rooted at r.
*
* notes:  this is a local helper function and it is
*         presumed that it will be called on a non-null
*         node pointer r.  (An empty tree has no meaningful
*         minimum value).
*/
static int min_h(NODE *r){
	if (r == NULL)
		return -1; // should never happen!
	while (r->left != NULL)
		r = r->left;
	return r->val;
}

/**
* function:  max_h()
* desc:      helper function returning the maximmum value
*            stored in tree rooted at r.
*
* notes:  this is a local helper function and it is
*         presumed that it will be called on a non-null
*         node pointer r.  (An empty tree has no meaningful
*         maximum value).
*/
static int max_h(NODE *r){
	if (r == NULL)
		return -1; // should never happen!
	while (r->right != NULL)
		r = r->right;
	return r->val;
}

/**
* function:  remove_r()
* desc:      recursive function implementing the
*            "deletion by copying" approach to BST deletion.
*
* params:  r - root of tree for deletion
*          x - value to delete (if it is in tree of course)
*          success - used to communicate if a deletion actually
*            occurred.  *success is set to 0 on failure; 1 on success.
*
* return:  returns root of resulting tree after deletion.
*/
static NODE *remove_r(NODE *r, int x, int *success, int *check){
	NODE   *tmp;
	int sanity;

	if (r == NULL){
		*success = 0;
		return r;
	}
	if (r->val == x){
		*success = 1;

		if (r->left == NULL){
			tmp = r->right;
			free(r);
			return tmp;
		}
		if (r->right == NULL){
			tmp = r->left;
			free(r);
			return tmp;
		}
		// if we get here, r has two children
		r->val = min_h(r->right);
		r->right = remove_r(r->right, r->val, &sanity, check);
		if (!sanity){
			printf("ERROR:  remove() failed to delete promoted value?\n");
		}
		return r;
	}
	if (x < r->val){
		r->left = remove_r(r->left, x, success, check);
		if (*success){
			r->size--;
			r->lNode--;
			if (r->rNode > 2 * r->lNode + 1){
				*check = 1;
				if (*check){
					if (r->rNode <= 2 * r->lNode + 1){
						r->work += r->rNode;
						r->right = bst_balance(r->right);
						*check = 0;
					}
				}
			}
		}
		return r;
	}
	else {
		r->right = remove_r(r->right, x, success, check);
		if (*success){
			r->size--;
			r->rNode--;
			if (r->lNode > 2 * r->rNode + 1){
				*check = 1;
				if (*check){
					if (r->lNode <= 2 * r->rNode + 1){
						r->work += r->rNode;
						r->left = bst_balance(r->left);
						*check = 0;
					}
				}
			}
		}
		return r;
	}

}

/**
* function:  bst_remove()
* desc:      if x is an element of BST given by pointer t,
*            it is removed, and 1 (for success) is returned.
*            Otherwise (x not an element of tree), the tree is
*            unchanged and 0 (for failure is returned).
*
* note:      helper function remove_r does most of the work.
*
*/
int bst_remove(BST * t, int x){
	int check = 0;
	int success = 0;
	t->root = remove_r(t->root, x, &success, &check);
	if (t == NULL) return 0;
	if (check){
		t->root = bst_balance(t->root);
		t->root->work += t->root->size;
		check = 0;
	}

	return success;
}

/**
* function:  bst_size()
* desc:      returns number of nodes/elements in given
*              BST.
*/
int bst_size(BST * t){
	NODE *r = t->root;
	if (r == NULL){
		return 0;
	}
	else{
		return t->root->size;
	}
}

/**
* function:  height()
* desc:      returns the height of the tree given
*            by NODE pointer r.
*
* method:  recursive traversal of tree computing subtree
*            heights from results of recursive calls.
*
* notes:  this is a recursive helper function operating
*         at the "node level"; called by bst_height.
*/
static int height(NODE *r){
	int l_h, r_h;

	if (r == NULL) return -1;
	l_h = height(r->left);
	r_h = height(r->right);
	return 1 + (l_h > r_h ? l_h : r_h);

}

/**
* function:  bst_height()
* desc:      returns the height of the tree given
*            by BST pointer t.
*
* note:  helper function height() does most of the work.
*/
int bst_height(BST * t){
	return height(t->root);

}

/**
* function:  bst_min()
* desc:      returns the minimum element stored in BST
*            given by pointer t.
*
* exception: if tree is empty, an arbitrary value is returned.
*            Caller should not rely on such a value having anyu
*            real meaning (and should just invoke this function
*            on only non-empty trees).
*
* note:  helper function min_h() does most of the work.
*/
int bst_min(BST * t){
	return min_h(t->root);
}

/**
* function:  bst_max()
* desc:      returns the maximum element stored in BST
*            given by pointer t.
*
* exception: if tree is empty, an arbitrary value is returned.
*            Caller should not rely on such a value having anyu
*            real meaning (and should just invoke this function
*            on only non-empty trees).
*
* note:  helper function max_h() does most of the work.
*/
int bst_max(BST * t){
	return max_h(t->root);
}

/**
* function:  indent()
* desc:      utility function simply printing
*            m dashes ('-') without a new line.
*
* notes:  used for printing trees where level of indentation
*         indicates distance from root.
*/
static void indent(int m){
	int i;
	for (i = 0; i<m; i++)
		printf("-");
}

/**
* function:  inorder()
* desc:      prints the elements of tree rooted at r "in-order".
* notes;     this is a helper function operating at the "node level"
*/
static void inorder(NODE *r){
	if (r == NULL) return;
	inorder(r->left);
	printf("[%d]\n", r->val);
	inorder(r->right);

}

/**
* function:  bst_inorder()
* desc:      prints the elements of BST given by pointer t "in-order"
* notes:  recursive helper function inorder does most of the work
*/
void bst_inorder(BST * t){

	printf("========BEGIN INORDER============\n");
	inorder(t->root);
	printf("=========END INORDER============\n");

}

/**
* function:  preorder()
* desc:      prints the elements of tree rooted at r according to
*            standard "pre-order" traversal according to the description
*            of bst_preorder
*/
static void preorder(NODE *r, int margin){
	if (r == NULL) {
		indent(margin);
		printf("NULL \n");
	}
	else {
		indent(margin);
		printf("%d\n", r->val);
		preorder(r->left, margin + 3);
		preorder(r->right, margin + 3);
	}
}

/**
* function:  bst_preorder()
* desc:      prints the elements of BST given by pointer t according to
*            standard "pre-order" traversal.
*
*            Elements are printed one-per-line and are indented to
*            indicate the tree structure:
*                    root has indentation zero
*                    child nodes are indented "one level" past the
*                       indentation of their parents.
*            Thus, all elements at the same distance from the root
*            are indented exaclty the same.
*/
void bst_preorder(BST * t){

	printf("========BEGIN PREORDER============\n");
	preorder(t->root, 0);
	printf("=========END PREORDER============\n");

}

/*
* func:  postorder
* desc:  placeholder for unimplemented recursive postorder printing
*          function at the "node level"
*
*        indentation should be proportional to depth of node being printed
*        depth is #hops from root.
*
* Complete the (recursive) helper function for the post-order traversal.
* Remember: the indentation needs to be proportional to the height of the node!
*/
static void postorder(NODE *r, int margin){
	if (r == NULL) {
	}
	else {
		indent(margin);
		postorder(r->left, margin + 3);
		postorder(r->right, margin + 3);
		printf("[%d]\n", r->val);
	}
}

/*
* func:  bst_postorder
* desc:  prints elements of BST given by pointer t in "post-order".
*        Indentation is proportional to depth of node being printed.
*        Depth is #hops from root.
*
* Notes: depends on helper function postorder which is currently
*        unimplemented.
*/
// indentation is proportional to depth of node being printed
//   depth is #hops from root.
void bst_postorder(BST * t){

	printf("========BEGIN POSTORDER============\n");
	postorder(t->root, 0);
	printf("=========END POSTORDER============\n");

}

/*
* func:  from_arr
* desc:  recursive helper function from_arr which constructs a perfectly
*        balanced tree (at the "node level") from the values stored in
*        array a.
*
* requirement:  given array a is assumed to be in sorted order.
*
* returns:  returns pointer to root of resulting balanced tree.
*
* Write the (recursive) helper function from_arr, used by
* bst_from_sorted_arr(...). The function must return a sub-tree that is
* perfectly balanced, given a sorted array of elements a.
*/

static NODE * from_arr(int *a, int n){
	int m;
	NODE *root;

	if (n <= 0) return NULL;
	m = n / 2;
	root = malloc(sizeof(NODE));
	root->val = a[m];
	root->size = n;
	root->left = from_arr(a, m);
	root->right = from_arr(&(a[m + 1]), n - (m + 1));
	root->lNode = (n / 2);
	root->rNode = n - 1 - (n / 2);
	root->work += n;
	return root;

}

/*
* func:  bst_from_sorted_arr
* desc:  builds a BST which is as balanced as possible from sorted array a.
*
* precondition:  given array a is assumed to be sorted and have no duplicates.
*                Assumed to be the caller's responsiblity to ensure these
*                properties.
*
* notes:  helper function from_arr does most of the work.
*/
BST * bst_from_sorted_arr(int *a, int n){
	BST * t = bst_create();
	t->root = from_arr(a, n);
	return t;
}

/**
* function:  bst_to_array
* description:
*			allocates an integer array, populates it with the
*			elements of t (in-order) and returns the array as an
*			int pointer
*/
void bst_to_array_recursion(NODE *r, int a[], int *i){
	if (r == NULL)
		return;
	bst_to_array_recursion(r->left, a, i);
	a[*i] = r->val;
	(*i)++;
	bst_to_array_recursion(r->right, a, i);
}

int * bst_to_array(BST * t) {
	int *arr = malloc(sizeof(int)* t->root->size);
	int i = 0;
	bst_to_array_recursion(t->root, arr, &i);
	return arr;
}

/**
* function:  bst_get_ith
* description:
*			returns the ith smallest element in t.  i ranges
*			from 1..n where n is the number of elements in
*			the tree.
*
*			If i is outside this range, an error message is printed
*			to stderr and the return value is arbitrary (you may return
*			whatever you like, but it has no meaning.
*
*			Runtime:  O(h) where h is the tree height
*/
int bst_get_ith_recursion(NODE *r, int i){
	if (r == NULL){
		return -1;
	}
	if (r->lNode >= i){
		return bst_get_ith_recursion(r->left, i);
	}
	if (r->lNode + 1 == i){
		return r->val;
	}
	if (r->lNode <= i){

		return bst_get_ith_recursion(r->right, i - r->lNode - 1);
	}
}

int bst_get_ith(BST * t, int i) {
	int j = bst_get_ith_recursion(t->root, i);
	if (j == -1){
		fprintf(stderr, "bst_ith_recursion has determined that i is out of range\n");
		return j;
	}
	return j;
}

/**
* function:  bst_num_nearest
* description:
*			returns the value in the tree closest to x -- in other
*			words, some y in the tree where |x-y| is minimum.
*
*			If the tree is empty, a message is sent to stderr and
*			the return value is of your choosing.
*
*			Runtime:  O(h) where h is the tree height.
*/
int bst_get_nearest_recursion(NODE *r, int x, int *num, int *height){
	if (r == NULL){
		return 0;
	}
	if (r->val == x){
		return *num = x;
	}
	if (*height > abs(x - r->val)){
		*height = abs(r->val - x);
		*num = r->val;
	}
	if (x < r->val){
		bst_get_nearest_recursion(r->left, x, num, height);
	}
	if (x > r->val){
		bst_get_nearest_recursion(r->right, x, num, height);
	}
}
int bst_get_nearest(BST * t, int x){

	int num = -1;
	int height = 999999;
	bst_get_nearest_recursion(t->root, x, &num, &height);
	if (num == 0){
		fprintf(stderr, "bst_get_nearest has determined that the tree is empty\n");
	}
	return num;
}

/**
* function:  bst_num_geq
* description:
*			returns the number of elements in t which are greater
*			than or equal to x.
*
*			Runtime:  O(h) where h is the tree height
*
*/
int bst_num_geq_recursion(NODE *r, int x){
	if (r == NULL){
		return 0;
	}
	if (r->val == x){
		return (1 + r->rNode);
	}
	else if (r->val > x){
		return (1 + r->rNode + bst_num_geq_recursion(r->left, x));
	}
	else {
		return bst_num_geq_recursion(r->right, x);
	}
}

int bst_num_geq(BST * t, int x){
	return bst_num_geq_recursion(t->root, x);
}

/**
* function:  bst_num_leq
* description:
*			returns the number of elements in t which are less
*			than or equal to x.
*
*			Runtime:  O(h) where h is the tree height
*
*/
int bst_num_leq_recursion(NODE *r, int x){
	if (r == NULL){
		return 0;
	}
	if (r->val == x){
		return (1 + r->lNode);
	}
	else if (r->val < x){
		return (1 + r->lNode + bst_num_leq_recursion(r->right, x));
	}
	else {
		return bst_num_leq_recursion(r->left, x);
	}
}

int bst_num_leq(BST * t, int x){
	return bst_num_leq_recursion(t->root, x);
}


/**
* function:  bst_num_range
* description:
*			returns the number of elements in t which are between min
*			and max (inclusive).
*
*			Runtime:  O(h) where h is the tree height
*
*/
int bst_num_range(BST * t, int min, int max){
	int i = bst_num_geq(t, min) + bst_num_leq(t, max) - t->root->size;
	if (i <= -1){
		abs(i);
	}
	return i;
}

/**
* function:  bst_sb_work
* description:
*			returns the total amount of work performed by re-balancing
*			since the creation of the tree.
*			Every time a rebalance  operation happens, the work
*			is equal to the size of the subtree that was rebalanced.
*			The total work is simply taken over all re-balancing ops.
*/
int bst_sb_work(BST *t){
	return t->root->work;
}
