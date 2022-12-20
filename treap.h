#include <stdio.h>
#ifndef __TREAP_H_
#define __TREAP_H_

#define MAX_PRIORITY 1000

typedef struct Node
{
    int data;
    int priority;

    struct Node *left, *right;
} Node;
typedef struct
{
    Node *root;
    int (*compar)(int, int);
} Treap_tree;

// Creeaza structura Treap-ului
Treap_tree *treap_create();

void node_free(Node **node);

void treap_free(Treap_tree *tree);

/* Creeaza un nod
 * @param1: Valoarea ce trebuie pusa in nod.
 */
Node *node_create(int value);

// Nodurile NULL au prioritatea -1 pentru a pastra proprietatea de max-heap.
int priority(Node *node);

/* Rotire dreapta
 * Pentru a nu fi nevoie sa mentinem pointer catre nodul parinte,
 * se vor folosi pointeri la noduri
 *
 *      node  			    lson
 *     /    \			   /    \
 *   lson    y    --->    x    node
 *   /  \     		           /   \
 *  x   lrson                lrson  y
 */
void rotate_right(Node **node);
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
void rotate_left(Node **node);

/* Inserare in Treap
 *
 * @param1: Nodul radacina al subarborelui din parcurgerea recursiva.
 * @param2: Valoare de adaugat in Treap.
 * @param3: Numarul de octeti pe care se scrie valoarea.
 * @param4: Functia de comparare pentru datele din Treap.
 */
void treap_insert(Node **node, int value, int (*compar)(int, int));

/* Stergere din Treap
 *
 * @param1: Nodul radacina al subarborelui din parcurgerea recursiva.
 * @param2: Valoare de adaugat in Treap.
 * @param3: Numarul de octeti pe care se scrie valoarea.
 * @param4: Functia de comparare pentru datele din Treap.
 */
void treap_delete(Node **node, int value, int (*compar)(int, int));

void *get_key(Node *node, int value, int (*compar)(int, int));

/* Verifica daca un arbore respecta proprietatile unui treap
 *
 * @param1: Nodul curent in parcurgerea recursiva.
 * @param2: Functia de comparare a datelor din fiecare nod.
 * @return: Daca arborele e Treap, vom returna numarul de noduri al
 * arborelui, altfel, vom returna o valoare negativa.
 */
int check_treap(Node *node, int (*compar)(int, int));
void printTreap(FILE *out, Node *root);
Node *treap_pop(Node **node, int (*compar)(int, int));
int compare_ints(int a, int b);
#endif
