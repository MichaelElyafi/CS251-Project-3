#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

#include "header.c"
#include "tutil.c"




char *Desc= "sb_work test";
/**
 * func: test
 * desc: tests sb_work function
 *
 *
 */
int test(int n) {
  int success1 = 1;
  int success2 = 1;
  int i;
  static int first = 1;
  

  BST *t;

  build_1_N(n, &t);

  if(!sb_height_ok(t))
    success1 = 0;

  for(i=1; i<=n; i++) {
    if(!bst_contains(t, i))
      success2 = 0;
  }

  if(first) {
    if(!success1)
      printf("   FAILED HEIGHT TEST\n");
    if(!success2)
      printf("   FAILED bst_contains TEST after build\n");
  }

  int ok_wrk = 5*bst_height(t)*n;

  int sb_wrk = bst_sb_work(t);

  if(first) {
    printf(" REPORTED    sb_work:  %i\n", sb_wrk);
    printf(" MAX ALLOWED sb_work:  %i\n", ok_wrk);
  }
  first = 0;

  bst_free(t);

  return success1 && success2 && (sb_wrk <= ok_wrk);
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

  cal_huge_fudge();
  budget = calibrate_nlogn(n, ntrials);


  START("[sb_work] ");

  TEST_RET_MESSAGE(baseline(), "COMPILATION", 1, 3.0); 
  TEST_RET_MESSAGE(test(n), "CORRECTNESS-ONLY-TEST", 1, 4.5); 
  TIME2(test(n), test(n2), Desc, 1, 2.5, 7.5);


  report();

  END;
}

