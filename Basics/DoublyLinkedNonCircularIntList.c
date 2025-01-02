#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// doubly linked non-circular list node structures | signatures

typedef struct IntNode
{
  int value;

  struct IntNode *prev;
  struct IntNode *next;
} IntNode;

typedef struct IntList
{
  IntNode *head;
} IntList;

// doubly linked non-circular list methods (create, insert, remove, has, print, free, count ocurrences) | signatures

IntNode *createIntNode(int value);
IntList *createIntList();
void insertIntNode(IntList *list, IntNode *node);
void removeIntNode(IntList *list, IntNode *node);
void removeIntValue(IntList *list, int value);
bool hasAnyNodeWithIntegerValue(IntList *list, int value);
void printIntNodes(IntList *list);
void freeIntList(IntList *list); // just in case
int countIntNodeOccurrences(IntList *list, int value);

// doubly linked non-circular list methods (insert, remove, iterate, print) | implementations

IntNode *createIntNode(int value)
{
  IntNode *node = malloc(sizeof(IntNode));
  node->value = value;
  node->prev = NULL;
  node->next = NULL;

  return node;
}

IntList *createIntList()
{
  IntList *list = malloc(sizeof(IntList));
  list->head = NULL;

  return list;
}

void insertIntNode(IntList *list, IntNode *node)
{
  if (!list->head)
  {
    list->head = node;
    return;
  }

  IntNode *current = list->head;

  while (current->next)
  {
    current = current->next;
  }

  current->next = node;
  node->prev = current;
}

void removeIntNode(IntList *list, IntNode *node)
{
  if (!list->head)
  {
    return;
  }

  IntNode *current = list->head;

  while (current)
  {
    if (current == node)
    {
      if (current->prev)
      {
        current->prev->next = current->next;
      }

      if (current->next)
      {
        current->next->prev = current->prev;
      }

      free(current);
      break;
    }

    current = current->next;
  }
}

void removeIntValue(IntList *list, int value)
{
  if (!list->head)
  {
    return;
  }

  IntNode *current = list->head;

  while (current)
  {
    if (current->value == value)
    {
      if (current->prev)
      {
        current->prev->next = current->next;
      }

      if (current->next)
      {
        current->next->prev = current->prev;
      }

      free(current);
      break;
    }

    current = current->next;
  }
}

bool hasAnyNodeWithIntegerValue(IntList *list, int value)
{
  IntNode *current = list->head;

  while (current)
  {
    if (current->value == value)
    {
      return true;
    }

    current = current->next;
  }

  return false;
}

void printIntNodes(IntList *list)
{
  IntNode *current = list->head;

  if (!current)
  {
    printf("Empty list\n");
    return;
  }

  while (current)
  {
    printf("%d ", current->value);
    current = current->next;
  }

  printf("\n");
}

void freeIntList(IntList *list)
{
  IntNode *current = list->head;

  while (current)
  {
    IntNode *next = current->next;
    free(current);
    current = next;
  }

  free(list);
}

int countIntNodeOccurrences(IntList *list, int value)
{
  IntNode *current = list->head;
  int count = 0;

  while (current)
  {
    if (current->value == value)
    {
      count++;
    }

    current = current->next;
  }

  return count;
}