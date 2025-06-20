#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "contiguous.h"
#include <stdlib.h>

void public_test(void) {
  struct contiguous *c = make_contiguous(150);
  print_debug(c);

  char *p0 = cmalloc(c, 12 * sizeof(char));
  strcpy(p0, "Hello World");
  print_debug(c);
  // Test: is it offset by exactly enough space for the two
  //     structures?
  assert(p0 == SIZEOF_CONTIGUOUS + SIZEOF_CNODE + (void *) c);
  // Test: we copied the date in correctly.
  assert(!strcmp(p0, "Hello World"));  
  
  char *p1 = cmalloc(c, 20);
  p1[0] = '!';
  print_debug(c);
  // Test: is it offset by exactly enough space for the chunk and the
  //     new node?
  assert(p1 == 12 * sizeof(char) + SIZEOF_CNODE + (void *) p0);
  // Test: data is set properly.
  assert(p1[0] == '!');
  for (int i=1; i < 20; i++) {
    assert(p1[i] == '$');
  }
  
  cfree(p0);
  p0 = NULL;
  print_debug(c);

  int *p2 = cmalloc(c, sizeof(int));
  *p2 = 0x00EEFFC0;
  print_debug(c);

  // Test: is it in the first slot?
  assert(p2 == SIZEOF_CONTIGUOUS + SIZEOF_CNODE + (void *) c);
  // Test: data is set properly.
  assert(*p2 == 0x00EEFFC0);
  

  void *p3 = cmalloc(c, 0);
  print_debug(c);

  // Test: immediately after slot p1).
  assert(p3 == 20 * sizeof(char) + SIZEOF_CNODE + (void *) p1); 

  void *p4 = cmalloc(c, 0);
  print_debug(c);

  // Test: returned NULL when there is not enough space.
  assert(p4 == NULL);
  
  cfree(p2);
  p2 = NULL;
  cfree(p1);
  p1 = NULL;
  print_debug(c);

  cfree(p3);
  p3 = NULL;
  print_debug(c);

  printf("Now we call destroy_contiguous. "
    "It calls free.\n");
  
  destroy_contiguous(c);
}

void test_make_contiguous(void) {
  struct contiguous *c = make_contiguous(50);
  print_debug(c);
  char *b = cmalloc(c, 2);
  assert(b[1] == '$');
  free(c);
  printf("test_make_contiguous passed\n");
}

void test_destroy_contiguous_warn(void) {
  struct contiguous *c = make_contiguous(150);
  char *s = cmalloc(c, 5);
  print_debug(c);
  s[0] = 'T';
  print_debug(c);
  char *s2 = cmalloc(c, 5);
  s2[4] = 'T';
  print_debug(c);
  destroy_contiguous(c);
  printf("test_destroy_contiguous passed\n");
}

void test_cmalloc_0_byte_chunk(void) {
  printf("/////////////////////////////////////////////////////////\n");
  printf("Start test_cmalloc_0_byte_chunk\n");
  struct contiguous *c = make_contiguous(150);
  print_debug(c);
  int *num = cmalloc(c, 0);
  print_debug(c);
  assert(num == (void *)c + SIZEOF_CONTIGUOUS + SIZEOF_CNODE);
  int *num2 = cmalloc(c, sizeof(int));
  *num2 = 88;
  assert(*num2 == 88);
  printf("num is %d\n", *num2);
  print_debug(c);
  destroy_contiguous(c);
  printf("test_cmalloc_0_byte_chunk passed\n");
}

void test_cmalloc_no_space(void) {
  printf("/////////////////////////////////////////////////////////\n");
  printf("Start test_cmalloc_no_space\n");
  struct contiguous *c = make_contiguous((2 * SIZEOF_CNODE) + SIZEOF_CONTIGUOUS);
  print_debug(c);
  int *num = cmalloc(c, 0);
  print_debug(c);
  assert(num == (void *)c + SIZEOF_CONTIGUOUS + SIZEOF_CNODE);
  int *num2 = cmalloc(c, 0);
  print_debug(c);
  assert(num2 == (void *)c + SIZEOF_CONTIGUOUS + (2 * SIZEOF_CNODE));
  int *num3 = cmalloc(c, 0);
  assert(num3 == NULL);
  print_debug(c);
  cfree(num);
  print_debug(c);
  num3 = cmalloc(c,0);
  assert(num3 == (void *)c + SIZEOF_CONTIGUOUS + SIZEOF_CNODE);
  print_debug(c);
  cfree(num2);
  cfree(num3);
  destroy_contiguous(c);
  printf("test_cmalloc_no_space passed\n");
}

void test_cfree_null(void) {
  void *p = NULL;
  cfree(p);
  assert(p == NULL);
  printf("test_cfree_null passed\n");
}

void test_1_gap(void) {
  printf("/////////////////////////////////////////////////////////\n");
  printf("test_1_gap\n");
  struct contiguous *c = make_contiguous((2 * SIZEOF_CNODE) + SIZEOF_CONTIGUOUS + 1);
  print_debug(c);
  int *num = cmalloc(c, 1);
  print_debug(c);
  assert(num == (void *)c + SIZEOF_CONTIGUOUS + SIZEOF_CNODE);
  int *num2 = cmalloc(c, 0);
  print_debug(c);
  assert(num2 == (void *)c + SIZEOF_CONTIGUOUS + (2 * SIZEOF_CNODE) + 1);
  cfree(num);
  num = NULL;
  print_debug(c);
  num = cmalloc(c, 2);
  assert(num == NULL);
  num = cmalloc(c, 0);
  print_debug(c);
  assert(num == (void *)c + SIZEOF_CONTIGUOUS + SIZEOF_CNODE);
  destroy_contiguous(c);
  printf("test_1_gap passed\n");
}

void public_test2(void) {
  struct contiguous *c = make_contiguous(200);
  char *p0 = cmalloc(c, 2 * sizeof(char)); strcpy(p0, "A");
  char *p1 = cmalloc(c, 2 * sizeof(char)); strcpy(p1, "B");
  char *p2 = cmalloc(c, 2 * sizeof(char)); strcpy(p2, "C");

  cfree(p0);

  char *p3 = cmalloc(c, 2 * sizeof(char)); strcpy(p3, "D");
  print_debug(c);

  cfree(p1);
  cfree(p2);
  cfree(p3);
  destroy_contiguous(c);
}

void public_test3(void) {
  for (int tryfree=0; tryfree < 2; tryfree++) {
    struct contiguous *c = make_contiguous(200);
  
    char *p0 = cmalloc(c, 2 * sizeof(char)); strcpy(p0, "A");
    char *p1 = cmalloc(c, 2 * sizeof(char)); strcpy(p1, "B");
    char *p2 = cmalloc(c, 2 * sizeof(char)); strcpy(p2, "C");
    char *p3 = cmalloc(c, 2 * sizeof(char)); strcpy(p3, "D");
    
    if (tryfree) {
      cfree(p2);
      printf("Free p2\n");
      print_debug(c);
      p2 = cmalloc(c, 2 * sizeof(char)); strcpy(p2, "E");
    }
    print_debug(c);
    
    cfree(p0);
    cfree(p1);
    cfree(p2);
    cfree(p3);
    destroy_contiguous(c);
  }
}


int main(void) {
  public_test();
  //public_test2();
  //public_test3();
  
  /*
  printf("\n");
  printf("/////////////////////////////////////////////////////////\n");
  printf("Start of my tests\n");
  test_make_contiguous();
  test_destroy_contiguous_warn();
  test_cmalloc_0_byte_chunk();
  test_cfree_null();
  test_cmalloc_no_space();
  test_1_gap();
  printf("All Tests Passed!\n");
  */
}
