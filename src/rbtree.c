#include "rbtree.h"

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
    y->left = x;
    x->parent = y;
  }
}

void right_rotate(rbtree *t, node_t *x) {
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
    y->left = x;
    x->parent = y;
  }
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  node_t *p = t->root;
  node_t *left = t->nil;
  node_t *right = t->nil;
  while (p != t->nil) {
    left = p->left;
    right = p->right;
  }
  free(t);
}

void rbtree_insert_fixup(rbtree *t, node_t *z) {
  while (z->parent->color == RBTREE_RED) {
    if (z->parent == z->parent->parent->left) {
      node_t *y = z->parent->parent->right;
      if (y->color == RBTREE_RED) {
        // case 1
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else if (z == z->parent->right) {
        // case 2
        z = z->parent;
        left_rotate(t, z);
        // case 3
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }
    else if (z->parent == z->parent->parent->right) {
      node_t *y = z->parent->parent->left;
      if (y->color == RBTREE_RED) {
        // case 1
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else if (z == z->parent->left) {
        // case 2
        z = z->parent;
        right_rotate(t, z);
        // case 3
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
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
    parent = temp;
    if (parent->key <= new_node->key)
      parent = parent->right;
    else
      parent = parent->left;
  }
  new_node->parent = temp;

  if (temp == t->nil)
    t->root = new_node;
  else if (new_node->key < temp->key)
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

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
