#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

#include "header.c"
#include "tutil.c"




char *Desc= "get_nearest test 1";

int test(int h, int n) {
  BST *t = build_balanced(h);
  int success = 1;
  int i;
  int x;


  x = bst_get_nearest(t, -1);
  if(x != 1) success = 0;
  for(i=1; i<=n; i++) {
    x = bst_get_nearest(t, i);
    if(x != i)
      success = 0;
  }
  x = bst_get_nearest(t, 2*n);
  if(x != n) success = 0;
  bst_free(t);
  return success;
}



int main(int argc, char *argv[]) {
  int n = __N;
  int n2 = __N2;
  int height = __HEIGHT;
  int height2 = __HEIGHT2;
  int ntrials = __NTRIALS;

  clock_t budget;

  if(argc > 1)
    n = atoi(argv[1]);
  if(argc > 2)
    ntrials = atoi(argv[2]);

  set_ntrials(ntrials);

  cal_big_fudge();
  budget = calibrate_nlogn(n, ntrials);


  START("[get_nearest]: builds balanced tree + get_nearest scan");

  TEST_RET_MESSAGE(baseline(), "COMPILATION", 1, 2.0); 
  TEST_RET_MESSAGE(test(height, n), "CORRECTNESS-ONLY-TEST", 1, 3.0); 
  TIME2(test(height, n), test(height2, n2), Desc, 1, 2.5, 5.0);


  report();

  END;
}

