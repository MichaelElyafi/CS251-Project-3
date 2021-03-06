#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

#include "header.c"
#include "tutil.c"





char *Desc= "bst_get_ith test2";

int test(int h, int n) {
  BST *t = build_balanced_rem_evens(h);
  int success = 1;
  int i, tgt;
  int x;

  int nodd = (n+1)/2;

  if(nodd != bst_size(t))
    printf("ERRORRRRRRR\n");

  for(i=1, tgt=1; i<=nodd; i++, tgt+=2) {
    x = bst_get_ith(t, i);
    if(x != tgt) 
      success = 0;
  }
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


  START("[get_ith] balanced+rem_evens+get_ith");

  TEST_RET_MESSAGE(baseline(), "COMPILATION", 1, 2.0); 
  TEST_RET_MESSAGE(test(height, n), "CORRECTNESS-ONLY-TEST", 1, 3.0); 
  TIME2(test(height, n), test(height2, n2), Desc, 1, 2.5, 5.0);


  report();

  END;
}

