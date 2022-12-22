#include "treap.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#define MAX_PRIORITY 1000

int compare_ints(int a, int b)
{
    return a - b;
}

// Create treap structure
Treap_tree *treap_create()
{
    srand(time(NULL));
    Treap_tree *tree = malloc(sizeof(Treap_tree));
    tree->compar = compare_ints;
    if (tree == NULL)
    {
        return NULL;
    }
    tree->root = NULL;
    return tree;
}

void node_free(Node **node)
{
    if (!(*node))
    {
        return;
    }
    node_free(&(*node)->left);
    node_free(&(*node)->right);

    free(*node);
}

void treap_free(Treap_tree *tree)
{
    if (tree->root != NULL)
    {
        node_free(&tree->root);
    }
    free(tree);
}


Node *node_create(int value)
{
    Node *node = malloc(sizeof(Node));
    if (node == NULL)
    {
        return NULL;
    }

    node->data = value;

    // leaf have height 0
    node->priority = rand() % MAX_PRIORITY;
    node->left = NULL;
    node->right = NULL;

    return node;
}

int max(int a, int b)
{
    if (a > b)
    {
        return a;
    }
    return b;
}

int priority(Node *node)
{
    if (node == NULL)
    {
        return INT_MIN;
    }
    return node->priority;
}


void rotate_right(Node **node)
{
    Node *lson = (*node)->left;
    Node *lrson = (*node)->left->right;

    // rotate
    lson->right = *node;
    (*node)->left = lrson;

    // new root
    *node = lson;
}


void rotate_left(Node **node)
{
    Node *rson = (*node)->right;
    Node *rlson = (*node)->right->left;

    // rotate
    rson->left = *node;
    (*node)->right = rlson;

    // new root
    *node = rson;
}


void treap_insert(Node **node, int value, int (*compar)(int, int))
{
    if ((*node) == NULL)
    {
        *node = node_create(value);
        return;
    }

    if (compar(value, (*node)->data) < 0)
    {
        // go to left subtree
        treap_insert(&((*node)->left), value, compar);
        // check if heap property is correct
        if (priority((*node)->left) > priority(*node))
        {
            rotate_right(node);
        }
    }
    else
    {
        // go to right subtree
        treap_insert(&((*node)->right), value, compar);
        // check if heap property is correct
        if (priority((*node)->right) > priority(*node))
        {
            rotate_left(node);
        }
    }
}


void treap_delete(Node **node, int value, int (*compar)(int, int))
{
    // Check if the tree is empty
    if (*node == NULL)
    {
        return;
    }

    // Compare the value to be deleted with the current node's value
    int comp_result = compar(value, (*node)->data);

    // If the value to be deleted is smaller than the current node's value,
    // recursively call treap_delete on the left child
    if (comp_result < 0)
    {
        treap_delete(&(*node)->left, value, compar);
        // After deleting the node, check if the treap properties are still maintained
        // If not, perform a right rotation
        if ((*node)->left != NULL && (*node)->left->priority > (*node)->priority)
        {
            rotate_right(node);
        }
        return;
    }

    // If the value to be deleted is larger than the current node's value,
    // recursively call treap_delete on the right child
    if (comp_result > 0)
    {
        treap_delete(&(*node)->right, value, compar);
        // After deleting the node, check if the treap properties are still maintained
        // If not, perform a left rotation
        if ((*node)->right != NULL && (*node)->right->priority > (*node)->priority)
        {
            rotate_left(node);
        }
        return;
    }

    // If we reach this point, it means that the value to be deleted has been found
    Node *to_delete = *node;

    // If the current node has no children (i.e. it is a leaf node), simply delete it
    if ((*node)->left == NULL && (*node)->right == NULL)
    {
        *node = NULL;
        free(to_delete);
    }
    // If the current node has only one child, delete it and replace it with its child
    else if ((*node)->left == NULL)
    {
        *node = (*node)->right;
        free(to_delete);
    }
    else if ((*node)->right == NULL)
    {
        *node = (*node)->left;
        free(to_delete);
    }
    // If the current node has two children, find the in-order successor (i.e. the smallest
    // value in the right subtree) and delete it. Then, replace the current node with the
    // in-order successor
    else
    {
        Node *successor = (*node)->right;
        while (successor->left != NULL)
        {
            successor = successor->left;
        }
        (*node)->data = successor->data;
        treap_delete(&(*node)->right, successor->data, compar);
        // After deleting the node, check if the treap properties are still maintained
        // If not, perform a left rotation
        if ((*node)->right != NULL && (*node)->right->priority > (*node)->priority)
        {
            rotate_left(node);
        }
    }
}

void *get_key(Node *node, int value, int (*compar)(int, int))
{
    if (!node)
        return NULL;

    if (compar(value, node->data) == 0)
    {
        return node;
    }

    // go to left subtree
    if (compar(value, node->data) <= 0)
    {
        if (node->left != NULL)
        {
            return get_key(node->left, value, compar);
        }
        else
        {
            return NULL;
        }
        // go to right subtree
    }
    else
    {
        if (node->right != NULL)
        {
            return get_key(node->right, value, compar);
        }
        else
        {
            return NULL;
        }
    }
}

void printTreap(FILE *out, Node *root)
{
    if (root == NULL)
        return;

    fprintf(out, "%d ", root->data);

    printTreap(out, root->left);
    printTreap(out, root->right);
}
