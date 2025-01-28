#include <stdlib.h>

typedef struct Node
{
  int val;

  struct Node *prev;
  struct Node *nxt;
} Node;

typedef struct List
{
  Node *head;
} List;

void insert_node_into_list(List *l, Node *n)
{
  if (l->head == NULL)
  {
    l->head = n;

    n->nxt = n;
    n->prev = n;
    return;
  }

  Node *current = l->head;

  do
  {
    if (current->val >= n->val)
    {
      // Insert before the current node
      Node *prev = current->prev;

      n->nxt = current;
      n->prev = prev;

      prev->nxt = n;
      current->prev = n;

      // If inserting before the head, update the head pointer
      if (current == l->head)
      {
        l->head = n;
      }

      return;
    }
    current = current->nxt;
  } while (current != l->head);

  // If we reached here, `n` is the largest element and should be inserted at the end
  Node *tail = l->head->prev;

  n->nxt = l->head;
  n->prev = tail;

  tail->nxt = n;
  l->head->prev = n;
}

Node *find_element(List *l, int value)
{
  if (l->head == NULL)
    return NULL;

  Node *cur = l->head;

  do
  {
    if (cur->val == value)
      return cur;

    cur = cur->nxt;
  } while (cur != l->head);

  return NULL;
}