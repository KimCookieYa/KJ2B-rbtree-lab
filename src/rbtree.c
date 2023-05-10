#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nil_node = (node_t *)malloc(sizeof(node_t));
  nil_node->color = RBTREE_BLACK;
  p->root = nil_node;
  p->nil = nil_node;
  return p;
}

void print_node(rbtree *t, node_t *x) {
  if (x == t->nil)
    return;
  if (x->color == RBTREE_BLACK)
    printf("B");
  else
    printf("R");
  printf("%d ", x->key);
  print_node(t, x->left);
  print_node(t, x->right);
}

void print_rbtree(rbtree *t) {
  printf("print_rbtree: ");
  print_node(t, t->root);
  printf("\n");
}

void left_rotate(rbtree *t, node_t *x) {
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil)
    y->left->parent = x;
  y->parent = x->parent;

  if (x->parent == t->nil)
    t->root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else {
    x->parent->right = y;
  }

  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x) {
  node_t *y = x->left;
  x->left = y->right;
  if (y->right != t->nil)
    y->right->parent = x;
  y->parent = x->parent;

  if (x->parent == t->nil)
    t->root = y;
  else if (x == x->parent->right)
    x->parent->right = y;
  else {
    x->parent->left = y;
  }
  
  y->right = x;
  x->parent = y;
}

void delete_node(rbtree *t, node_t *x) {
  if (x == t->nil)
    return;
  
  delete_node(t, x->left);
  delete_node(t, x->right);
  free(x);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_node(t, t->root);
  free(t->nil);
  free(t);
}

void rbtree_insert_fixup(rbtree *t, node_t *z) {
  printf("fixup\n");
  while (z->parent->color == RBTREE_RED) {
    if (z->parent == z->parent->parent->left) {
      node_t *y = z->parent->parent->right;
      if (y->color == RBTREE_RED) {
        // case 1
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
        if (z->parent == t->nil && z->color == RBTREE_RED) {
          z->color = RBTREE_BLACK;
        }
        print_rbtree(t);
      }
      else if (z == z->parent->right) {
        // case 2
        z = z->parent;
        left_rotate(t, z);
      }
      else {
        // case 3
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        
        z = z->parent->parent;
        right_rotate(t, z);
      print_rbtree(t);
      }
      
    }
    else {
      node_t *y = z->parent->parent->left;
      if (y->color == RBTREE_RED) {
        // case 1
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
        if (z->parent == t->nil && z->color == RBTREE_RED) {
          z->color = RBTREE_BLACK;
        }
        print_rbtree(t);
      }
      else if (z == z->parent->left) {
        // case 2
        z = z->parent;
        right_rotate(t, z);
      }
      else {
        // case 3
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
        left_rotate(t, z);
      print_rbtree(t);
      }
      
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  printf("insert %d\n", key);
  node_t *new_node = (node_t *)malloc(sizeof(node_t));
  new_node->color = RBTREE_RED;
  new_node->key = key;
  new_node->left = t->nil;
  new_node->right = t->nil;

  if (t->root == t->nil) {
    new_node->parent = t->nil;
    new_node->color = RBTREE_BLACK;
    t->root = new_node;
    return t->root;
  }
  
  node_t *temp = t->nil;
  node_t *parent = t->root;
  while (parent != t->nil) {
    temp = parent;
    if (parent->key <= new_node->key)
      parent = parent->right;
    else
      parent = parent->left;
  }
  new_node->parent = temp;

  if (new_node->key < temp->key)
    temp->left = new_node;
  else
    temp->right = new_node;

  rbtree_insert_fixup(t, new_node);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *p = t->root;
  while (p != t->nil) {
    if (p->key == key)
      return p;
    else if (p->key > key)
      p = p->left;
    else
      p = p->right;
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *pp = t->nil;
  node_t *p = t->root;
  while (p != t->nil) {
    pp = p;
    p = p->left;
  }
  return pp;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *pp = t->nil;
  node_t *p = t->root;
  while (p != t->nil) {
    pp = p;
    p = p->right;
  }
  return pp;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

void inorder_rbtree(const rbtree *t, const node_t *x, key_t *arr, const size_t n, int *idx) {
  if (x == t->nil)
    return;
  
  inorder_rbtree(t, x->left, arr, n, idx);
  arr[*idx] = x->key;
  *idx += 1;
  inorder_rbtree(t, x->right, arr, n, idx);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  int *idx = (int *)calloc(1, sizeof(int));
  inorder_rbtree(t, t->root, arr, n, idx);
  printf("ans: ");
  for (int i = 0; i < n; i++)
    printf("%d ", arr[i]);
  printf("\n");
  return 0;
}
