#include <stdlib.h>

typedef struct Node
{
  int value;

  struct Node *next;
} Node;

typedef struct List
{
  Node *head;
} List;

void insert_node(List *list, Node *node)
{
  if (list->head == NULL || (list->head && list->head->value >= node->value))
  {
    node->next = list->head;
    list->head = node;

    return;
  }

  Node *previous = NULL;
  Node *current = list->head;

  while (current != NULL && current->value < node->value)
  {
    previous = current;
    current = current->next;
  }

  previous->next = node;
  node->next = current;
}

Node *find_element(List *list, int val)
{
  if (list->head == NULL)
    return NULL;

  Node *current = list->head;

  while (current != NULL)
  {
    if (current->value == val)
      return current;

    if (current->value > val)
      return NULL;

    current = current->next;
  }

  return NULL;
}

void delete_element(List *list, int val)
{
  if (list->head == NULL)
    return;

  Node *previous = NULL;
  Node *current = list->head;

  if (current->value == val)
  {
    list->head = current->next;
    free(current);

    return;
  }

  while (current != NULL)
  {
    if (current->value == val)
    {
      previous->next = current->next;

      free(current);
      return;
    }

    if (current->value > val)
      return;

    previous = current;
    current = current->next;
  }
}
