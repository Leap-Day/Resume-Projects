#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "contiguous.h"

struct contiguous {
  struct cnode *first;
  void *upper_limit;
};

struct cnode {
  size_t nsize;
  struct cnode *prev;
  struct cnode *next;
  struct contiguous *block;
};

const int SIZEOF_CONTIGUOUS = sizeof(struct contiguous);
const int SIZEOF_CNODE = sizeof(struct cnode);



static const char STAR_STR[] = "*";
static const char NULL_STR[] = "NULL";

// maybe_null(void *p) return a pointer to "NULL" or "*",
//   indicating if p is NULL or not.
static const char *maybe_null(void *p) {
  return p ? STAR_STR : NULL_STR;
}

// gapsize(n0, n1) determine the size (in bytes) of the gap between n0 and n1.
static size_t gapsize(struct cnode *n0, struct cnode *n1) {
  assert(n0);
  assert(n1);
  void *v0 = n0;
  void *v1 = n1;
  return (v1 - v0) - n0->nsize - sizeof(struct cnode);
}

// print_gapsize(n0, n1) print the size of the gap between n0 and n1,
//     if it's non-zero.
static void print_gapsize(struct cnode *n0, struct cnode *n1) {
  assert(n0);
  assert(n1);
  size_t gap = gapsize(n0, n1);
  
  if (gap != 0) { 
    printf("%zd byte gap\n", gap);
  }
}


// pretty_print_block(chs, size) Print size bytes, starting at chs,
//    in a human-readable format: printable characters other than backslash
//    are printed directly; other characters are escaped as \xXX
static void pretty_print_block(unsigned char *chs, int size) {
  assert(chs);
  for (int i = 0; i < size; i++) {
    printf(0x20 <= chs[i] && chs[i] < 0x80 && chs[i] != '\\'
           ? "%c" : "\\x%02X", chs[i]);
  }
  printf("\n");
}

// print_node(node) Print the contents of node and all nodes that
//    follow it.  Return a pointer to the last node.
static struct cnode *print_node(struct cnode *node) {
  while (node != NULL) {
    void *raw = node + 1;     // point at raw data that follows.
    printf("struct cnode\n");
    printf("    nsize: %ld\n", node->nsize);
    printf("    prev: %s\n", maybe_null(node->prev));
    printf("    next: %s\n",  maybe_null(node->next));

    printf("%zd byte chunk: ", node->nsize);
    
    pretty_print_block(raw, node->nsize);
    
    if (node->next == NULL) {
      return node;
    } else {
      print_gapsize(node, node->next);
      node = node->next;
    }
  }
  return NULL;
}



static void print_hr(void) {
    printf("----------------------------------------------------------------\n");
}

// print_debug(block) print a long message showing the content of block.
void print_debug(struct contiguous *block) {
  assert(block);
  void *raw = block;

  print_hr();
  printf("struct contiguous\n");
  printf("    first: %s\n", maybe_null(block->first));

  if (block->first == NULL) {
    size_t gap = block->upper_limit - raw - sizeof(struct contiguous);
    printf("%zd byte gap\n", gap);           
  } else {
    void *block_first = block->first;
    size_t gap = block_first - raw - sizeof(struct contiguous);
    if (gap) {
      printf("%zd byte gap\n", gap);
    }
  }
 
  struct cnode *lastnode = print_node(block->first);
  
  if (lastnode != NULL) {
    print_gapsize(lastnode, block->upper_limit);
  }

  print_hr();
}



struct contiguous *make_contiguous(size_t size) {
  // ...
  assert(size >= sizeof(struct contiguous));

  // Create the void pointer
  void *p = malloc(size * sizeof(size_t));

  // Treat the beginning of the void pointer to be a struct contiguous
  struct contiguous *cp = p;
  cp->first = NULL;
  cp->upper_limit = p + size;

  // Chance unused space to char to assign '$'
  char *unused = p + sizeof(struct contiguous);
  size_t size_to_fill = size - sizeof(struct contiguous);
  for (int i = 0; i < size_to_fill; i++) {
    unused[i] = '$';
  }

  return cp;
}


void destroy_contiguous(struct contiguous *block) {
  // ...
  assert(block);

  if (block->first != NULL) {
    printf("Destroying non-empty block!\n");
  }
  free(block);
}


void cfree(void *p) {
  // ...
  if (p == NULL) {
    return;
  }

  struct cnode *node = p - sizeof(struct cnode);
  struct contiguous *cp = node->block;

  // Associated node is at the front of the list
  if (node->prev == NULL) {
    cp->first = node->next;
    if (node->next != NULL) {
      node->next->prev = NULL;
    }
   // Associated node is at the end of the list   
  } else if (node->next == NULL) {
    struct cnode *prev_node = node->prev;
    prev_node->next = NULL;
  
   // Associated node is in the middle of the list somewhere
  } else {
    struct cnode *prev_node = node->prev;
    struct cnode *next_node = node->next;
    prev_node->next = next_node;
    next_node->prev = prev_node;
  }
}


void *cmalloc(struct contiguous *block, int size) {
  // ...
  assert(block);
  assert(size >= 0);
  
  size_t gap = 0;
  size_t total_size = size + sizeof(struct cnode);

  // List is empty
  if (block->first == NULL) {
    void *node_start = block + 1;
    gap = block->upper_limit - node_start;

    if (total_size <= gap) {
      // Get location of new node
      void *temp = block + 1;
      struct cnode *node = temp;

      // Initialize new node
      node->nsize = size;
      node->prev = NULL;
      node->next = NULL;
      node->block = block;

      // Link first to new node 
      block->first = node;

      // Get location of chunk and return it
      void *chunk = temp + sizeof(struct cnode);
      return chunk;
    } else {
      return NULL;
    }
  }
  
  // List is non-empty
  struct cnode *node = block->first;

  // Check for possible gap between struct contig and first node
  if (node->prev == NULL) {
    void *block_end = block + 1;
    void *cur = node;
    gap = cur - block_end;

     
    if (total_size <= gap) {
      // Get location of new node
      void *temp = block + 1;
      struct cnode *new_node = temp;

      // Initialize new node
      new_node->nsize = size;
      new_node->prev = NULL;
      new_node->next = node;
      new_node->block = block;        
      
      // Link new node
      block->first = new_node;
      node->prev = new_node;

      // Get location of chunk and return it
      void *chunk = new_node + 1;
      return chunk;
    }
  }
  

  while (node != NULL) {
    // Current Node is not only node on the list, but is last
    if (node->next == NULL) {
      void *cur = node;
      gap = (block->upper_limit - cur) - node->nsize - sizeof(struct cnode);
        
      if (total_size <= gap) {
        // Get location of new node
        void *temp = node + 1;
        temp += node->nsize;
          
        // Initialize new node
        struct cnode *new_node = temp;
        new_node->nsize = size;
        new_node->prev = node;
        new_node->next = NULL;
        new_node->block = block;
          
        // Link new node
        node->next = new_node;
          
        // Get location of chunk and return it
        void *chunk = temp + sizeof(struct cnode);
        return chunk;
      } else {
        // No Space at end of list
        return NULL;
      }

      // Current node is in middle of the list
    } else {
      void *cur = node;
      void *next = node->next;
      gap = (next - cur) - node->nsize - sizeof(struct cnode);


      if (total_size <= gap) {
        // Get location of new node
        void *temp = node + 1;
        temp += node->nsize;

        // Initialize new node
        struct cnode *new_node = temp;
        struct cnode *next_node = node->next;

        new_node->nsize = size;
        new_node->prev = node;
        new_node->next = next_node;
        new_node->block = block;

        // Link new node
        node->next = new_node;
        next_node->prev = new_node;

        // Get location of chunk and return it
        void *chunk = temp + sizeof(struct cnode);
        return chunk;
      } 

      // Check next nodes for suitable gaps
      node = node->next;
    }
  }
  return NULL;
}


