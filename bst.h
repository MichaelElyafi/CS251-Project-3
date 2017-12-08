
// typedef int ELEM_TYPE;

typedef struct bst BST;

extern BST * bst_create();

extern void bst_free(BST * t);

/** TODO:  modify for augmentations and size-balancing  **/
extern void bst_insert(BST * t, int x);

/** TODO:  modify for augmentations and size-balancing  **/
extern int bst_remove(BST * t, int x);

extern int bst_contains(BST * t, int x);

/** NOTE:  not part of the assignment, but you should be able
           to make bst_size O(1) trivially once everything else
           is in place.
*/
extern int bst_size(BST * t);

extern int bst_height(BST * t);

extern int bst_min(BST * t);

extern int bst_max(BST * t);

/************************************************************/


extern void bst_inorder(BST * t);

extern void bst_preorder(BST * t);

extern void bst_postorder(BST * t);

extern BST * bst_from_sorted_arr(int *a, int n);

/*** TODO ***/

extern int * bst_to_array(BST * t);

extern int bst_get_ith(BST * t, int i);

extern int bst_get_nearest(BST * t, int x);

extern int bst_num_geq(BST * t, int x);

extern int bst_num_leq(BST * t, int x);

extern int bst_num_range(BST * t, int min, int max);

extern int bst_sb_work(BST *t);


