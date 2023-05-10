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

void print_node(const rbtree *t, node_t *x) {
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

void print_rbtree(const rbtree *t) {
  printf("print_rbtree: ");
  print_node(t, t->root);
  printf("\n\n");
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
  return new_node;
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

void rbtree_transplant(rbtree * t, node_t *u, node_t *v) {
  if (u->parent == t->nil)
    t->root = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  
  v->parent = u->parent;
}

node_t *rbtree_succesor(const rbtree *t, node_t *p) {
  rbtree *tt = (rbtree *)malloc(sizeof(rbtree));
  tt->nil = t->nil;

  node_t *succesor = t->nil;
  if (p->right == t->nil) {
    tt->root = p->left;
    succesor = rbtree_max(tt);
  }
  else {
    tt->root = p->right;
    succesor = rbtree_min(tt);
  }
  
  free(tt);
  return succesor;
}

void rbtree_erase_fixup(rbtree *t, node_t *x) {
  while (x != t->root && x->color == RBTREE_BLACK) {
    if (x == x->parent->left) {
      node_t *w = x->parent->right;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }

      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else if (w->right->color == RBTREE_BLACK) {
        w->left->color = RBTREE_BLACK;
        w->color = RBTREE_RED;
        right_rotate(t, w);
        w = x->parent->right;
      }
      else {
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }
    else if (x == x->parent->right) {
      node_t *w = x->parent->left;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }

      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else if (w->left->color == RBTREE_BLACK) {
        w->right->color = RBTREE_BLACK;
        w->color = RBTREE_RED;
        left_rotate(t, w);
        w = x->parent->left;
      }
      else {
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }

  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  key_t res = p->key;
  
  node_t *x = t->nil;
  node_t *y = p;
  color_t y_original_color = y->color;

  if (p->left == t->nil) {
    x = p->right;
    rbtree_transplant(t, p, p->right);
  }
  else if (p->right == t->nil) {
    x = p->left;
    rbtree_transplant(t, p, p->left);
  }
  else {
    y = rbtree_succesor(t, p);
    y_original_color = y->color;
    x = y->right;
    if (y->parent == p)
      x->parent = y;
    else {
      rbtree_transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }

    rbtree_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }

  if (y_original_color == RBTREE_BLACK)
    rbtree_erase_fixup(t, x);
  
  return res;
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
  return 0;
}
