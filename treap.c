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

// Creeaza structura Treap-ului
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
/* Creeaza un nod
 * @param1: Valoarea ce trebuie pusa in nod.
 * @param2: Numarul de octeti pe care scrie valoarea.
 */
Node *node_create(int value)
{
    Node *node = malloc(sizeof(Node));
    if (node == NULL)
    {
        return NULL;
    }

    // Copiere octet cu octet din value, in nodul curent.
    node->data = value;

    // Nodurile frunze au inaltimea 0.
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

// Nodurile NULL au prioritatea -1 pentru a pastra proprietatea de max-heap.
int priority(Node *node)
{
    if (node == NULL)
    {
        return INT_MIN;
    }
    return node->priority;
}
/* Rotire dreapta
 * Pentru a nu fi nevoie sa mentinem pointer catre nodul parinte,
 * se vor folosi pointeri la noduri
 *
 *      node  			    lson
 *     /    \			   /    \
 *   lson    y ---> x    node
 *   /  \     		      /   \
 *  x   lrson         lrson  y
 */
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
/* Rotire stanga
 * Pentru a nu fi nevoie sa mentinem pointer catre nodul parinte,
 * se vor folosi pointeri la noduri
 *
 *     node  			    rson
 *    /    \			   /    \
 *   x     rson    --->  node    y
 *         /   \		 /   \
 *       rlson  y       x   rlson
 */
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

/* Inserare in Treap
 *
 * @param1: Nodul radacina al subarborelui din parcurgerea recursiva.
 * @param2: Valoare de adaugat in Treap.
 * @param3: Numarul de octeti pe care se scrie valoarea.
 * @param4: Functia de comparare pentru datele din Treap.
 */
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

/* Stergere din Treap
 *
 * @param1: Nodul radacina al subarborelui din parcurgerea recursiva.
 * @param2: Valoare de adaugat in Treap.
 * @param3: Functia de comparare pentru datele din Treap.
 */
void treap_delete(Node **node, int value, int (*compar)(int, int))
{
    if (*node == NULL || node == NULL)
    {
        return;
    }

    if (compar(value, (*node)->data) < 0)
    { // go to left subtree
        treap_delete(&((*node)->left), value, compar);
    }
    else if (compar(value, (*node)->data) > 0)
    { // go to right subtree
        treap_delete(&((*node)->right), value, compar);
        // If the node with "value" has been found
    }
    else if ((*node)->left == NULL &&
             (*node)->right == NULL)
    { // if it is a leaf
        node_free(node);
        *node = NULL;
        // compare priorities between two children
    }
    else if (priority((*node)->left) > priority((*node)->right))
    {
        rotate_right(node);
        treap_delete(&((*node)->right), value, compar);
    }
    else
    {
        rotate_left(node);
        treap_delete(&((*node)->left), value, compar);
    }
}
void *get_key(Node *node, int value, int (*compar)(int, int))
{
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

/* get the max element from the treap */
Node *treap_peek(Node *node)
{
    if (node == NULL)
    {
        printf("The treap is empty");
        return NULL;
    }

    if (node->right == NULL)
    {
        return node;
    }
    Node *max_node = treap_peek(node->right);
    return max_node;
}

Node *treap_pop(Node **node, int (*compar)(int, int))
{
    Node *removed_node = malloc(sizeof(Node));
    removed_node->data = (treap_peek(*node))->data;
    treap_delete(node, removed_node->data, compar);
    return removed_node;
}
