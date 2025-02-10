// Discentes:
// - Filipe Lordêlo Fiúza (20241160009)
// - Júlio dos Reis Sousa (20241160014)

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_COLUMN_LEN 1000
#define MAX_LINE_LEN 1000

enum Operation
{
  INSERTION,
  REMOVAL,
};

// bst structures | signatures

typedef struct Node
{
  int value;

  struct Node *left;
  struct Node *right;

  struct Node *parent;
} Node;

// bst methods | signatures

Node *createNode(int value);
Node *insertNode(Node *node, int value);
void removeNode(Node **root, int value);
bool hasValue(Node *root, int value);
void inorderTraversal(Node *root, Node **result, int *index);
int heightFromRoot(Node *root, Node *target);
Node *minNode(Node *node);

void readInputAndProcess();
void processLine(char *line, FILE *outputFile, int isLastLine);

void removeNewLine(char *string);

int main()
{
  readInputAndProcess();
  return 0;
}

void readInputAndProcess()
{
  FILE *inputFile = fopen("L2Q3.in", "r");
  if (!inputFile)
  {
    perror("Failed to open input file");
    return;
  }

  FILE *outputFile = fopen("L2Q3.out", "w");
  if (!outputFile)
  {
    perror("Failed to open output file");
    fclose(inputFile);
    return;
  }

  int totalLines = 0, lineCounter = 0;
  char line[MAX_LINE_LEN];
  while (fgets(line, sizeof(line), inputFile))
  {
    totalLines++;
  }

  rewind(inputFile);

  while (fgets(line, sizeof(line), inputFile))
  {
    lineCounter++;

    processLine(line, outputFile, lineCounter == totalLines);
  }

  fclose(inputFile);
  fclose(outputFile);
}

void processLine(char *line, FILE *outputFile, int isLastLine)
{
  removeNewLine(line);

  Node *treeRoot = NULL;
  char *token = strtok(line, " ");

  int operation = -1;

  while (token != NULL)
  {
    bool isTokenOperation = token[0] == 'a' || token[0] == 'r';

    if (isTokenOperation)
    {
      operation = token[0] == 'a' ? INSERTION : REMOVAL;
    }
    else if (operation == INSERTION)
    {
      int value = atoi(token);

      if (treeRoot == NULL)
      {
        treeRoot = insertNode(treeRoot, value);
      }
      else
      {
        insertNode(treeRoot, value);
      }
    }
    else if (operation == REMOVAL)
    {
      int value = atoi(token);

      if (hasValue(treeRoot, value))
      {
        removeNode(&treeRoot, value);
      }
      else
      {
        if (treeRoot == NULL)
        {
          treeRoot = insertNode(treeRoot, value);
        }
        else
        {
          insertNode(treeRoot, value);
        }
      }
    }

    token = strtok(NULL, " ");
  }

  int index = 0;

  Node *inOrderNodes[MAX_COLUMN_LEN];
  inorderTraversal(treeRoot, inOrderNodes, &index);

  for (int i = 0; i < index; i++)
  {
    Node *node = inOrderNodes[i];
    int nodeHeight = heightFromRoot(treeRoot, node);

    fprintf(outputFile, "%d (%d)", node->value, nodeHeight);

    if (i < index - 1)
    {
      fprintf(outputFile, " ");
    }
  }

  if (!isLastLine)
  {
    fprintf(outputFile, "\n");
  }
}

void removeNewLine(char *string)
{
  int copyIndex = 0;
  int size = strlen(string);

  char *copy = malloc(sizeof(char) * size);

  for (int index = 0; index < size; index++)
  {
    char value = string[index];

    if (value != '\n' && value != '\r')
      copy[copyIndex++] = value;
  }

  strcpy(string, copy);
}

// bst methods | implementations

Node *createNode(int value)
{
  Node *node = (Node *)malloc(sizeof(Node));

  node->value = value;

  node->left = NULL;
  node->right = NULL;

  node->parent = NULL;
  return node;
}

Node *insertNode(Node *root, int value)
{
  Node *newNode = createNode(value);

  if (root == NULL)
    return newNode;

  Node *current = root;
  Node *parent = NULL;

  while (current != NULL)
  {
    parent = current;

    if (value < current->value)
      current = current->left;
    else
      current = current->right;
  }

  if (value < parent->value)
    parent->left = newNode;
  else
    parent->right = newNode;

  return newNode;
}

void removeNode(Node **root, int value)
{
  Node *current = *root;
  Node *parent = NULL;

  while (current != NULL && current->value != value)
  {
    parent = current;

    if (value < current->value)

      current = current->left;

    else

      current = current->right;
  }

  if (current == NULL)

    return;

  // node has no left child
  if (current->left == NULL)
  {
    if (parent == NULL)
      *root = current->right;
    else if (parent->left == current)
      parent->left = current->right;
    else
      parent->right = current->right;

    if (current->right != NULL)
      current->right->parent = parent;
  }

  // node has no right child
  else if (current->right == NULL)
  {
    if (parent == NULL)
      *root = current->left;
    else if (parent->left == current)
      parent->left = current->left;
    else
      parent->right = current->left;

    if (current->left != NULL)
      current->left->parent = parent;
  }

  // node has two children
  else
  {
    Node *successor = minNode(current->right);

    current->value = successor->value;

    parent = current;
    current = current->right;

    while (current != NULL && current->value != successor->value)
    {
      parent = current;
      current = current->left;
    }

    if (current->right != NULL)
    {
      if (parent->left == current)
        parent->left = current->right;
      else
        parent->right = current->right;

      current->right->parent = parent;
    }
    else
    {
      if (parent->left == current)
        parent->left = NULL;
      else
        parent->right = NULL;
    }
  }

  free(current);
}

bool hasValue(Node *root, int value)
{
  if (!root)
    return false;

  if (root->value == value)
    return true;

  return value < root->value ? hasValue(root->left, value) : hasValue(root->right, value);
}

void inorderTraversal(Node *root, Node **result, int *index)
{
  if (root == NULL)
    return;

  inorderTraversal(root->left, result, index);

  result[*index] = root;
  (*index)++;

  inorderTraversal(root->right, result, index);
}

int heightFromRoot(Node *root, Node *target)
{
  if (root == NULL)
    return -1;

  if (root == target)
    return 0;

  int left = heightFromRoot(root->left, target);
  if (left != -1)
    return left + 1;

  int right = heightFromRoot(root->right, target);
  if (right != -1)
    return right + 1;

  return -1;
}

Node *minNode(Node *node)
{
  while (node && node->left)
    node = node->left;

  return node;
}