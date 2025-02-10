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
int heightFromRoot(Node *root, Node *target);
Node *predecessor(Node *root, int value);

void readInputAndProcess();
void processLine(char *line, FILE *outputFile, int isLastLine);

char *itoa(int value);

void removeNewLine(char *string);

int main()
{
  readInputAndProcess();
  return 0;
}

void readInputAndProcess()
{
  FILE *inputFile = fopen("L2Q1.in", "r");
  if (!inputFile)
  {
    perror("Failed to open input file");
    return;
  }

  FILE *outputFile = fopen("L2Q1.out", "w");
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

  bool isMaxInitialized = false;
  int currentMaxValue = 0, currentMaxHeight = 0;
  Node *maxValuePredecessor = NULL;

  while (token != NULL)
  {
    int currentValue = atoi(token);
    Node *newNode = insertNode(treeRoot, currentValue);

    if (treeRoot == NULL)
    {
      treeRoot = newNode;
    }

    int nodeHeight = heightFromRoot(treeRoot, newNode);
    fprintf(outputFile, "%d ", nodeHeight);

    if (!isMaxInitialized || currentValue > currentMaxValue)
    {
      currentMaxValue = currentValue;
      currentMaxHeight = nodeHeight;
      maxValuePredecessor = predecessor(treeRoot, currentValue);
      isMaxInitialized = true;
    }

    token = strtok(NULL, " ");
  }

  fprintf(outputFile, "max %d alt %d pred %s", currentMaxValue, currentMaxHeight,
          maxValuePredecessor ? itoa(maxValuePredecessor->value) : "NaN");

  if (!isLastLine)
  {
    fprintf(outputFile, "\n");
  }
}

char *itoa(int value)
{
  int size = floor(log10(abs(value))) + 1;
  char *result = malloc(sizeof(char) * size);

  sprintf(result, "%d", value);

  return result;
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

Node *predecessor(Node *root, int value)
{
  Node *predecessor = NULL;

  while (root)
  {
    if (value > root->value)
    {
      predecessor = root;
      root = root->right;
    }
    else
      root = root->left;
  }

  return predecessor;
}