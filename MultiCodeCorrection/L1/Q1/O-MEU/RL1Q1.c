// Discentes:
// - Filipe Lordêlo Fiúza (20241160009)
// - Júlio dos Reis Sousa (20241160014)

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_COLUMN_LEN 100
#define MAX_LINE_LEN 100

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

typedef struct RowPair
{
  int *row;
  int sum;
} RowPair;

void matrixToStdout(int matrix[MAX_COLUMN_LEN][MAX_LINE_LEN], int rowLengths[MAX_LINE_LEN])
{
  for (int i = 0; i < MAX_LINE_LEN; i++)
  {
    int cols = rowLengths[i];

    if (!cols)
      continue;

    for (int j = 0; j < cols; j++)
    {
      printf("%d ", matrix[i][j]);
    }

    printf("\n");
  }
}

void sortIntegers(int *integers, int n);
void quickSortIntegers(int *list, int low, int high);
int partitionIntegers(int *list, int low, int high);
void swapIntegers(int *integers, int i, int j);

void quickSortRowPairs(RowPair *list, int *rowLengths, int low, int high);
int partitionRowPairs(RowPair *list, int *rowLengths, int low, int high);
void swapRowPairs(RowPair *list, int *rowLengths, int i, int j);

void readInputAndProcess();
void processLine(char *line, FILE *outputFile, int isLastLine);

int parseInt(const char *str);

void removeNewLine(char *string);

int main()
{
  readInputAndProcess();
  return 0;
}

void readInputAndProcess()
{
  FILE *inputFile = fopen("L1Q1.in", "r");
  if (!inputFile)
  {
    perror("Failed to open input file");
    return;
  }

  FILE *outputFile = fopen("L1Q1.out", "w");
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

  int begun = 0;
  int matrix[MAX_LINE_LEN][MAX_COLUMN_LEN] = {0};
  int rowLengths[MAX_LINE_LEN] = {0};
  int lineIndex = 0, columnIndex = 0;

  char *token = strtok(line, " ");

  while (token)
  {
    if (strcmp(token, "start") == 0)
    {
      if (begun)
      {
        rowLengths[lineIndex++] = columnIndex;
        columnIndex = 0;
      }

      begun = 1;
    }
    else
    {
      int value = parseInt(token);

      matrix[lineIndex][columnIndex++] = value;
    }

    token = strtok(NULL, " ");
  }

  if (begun)
    rowLengths[lineIndex++] = columnIndex;

  for (int i = 0; i < lineIndex; i++)
  {
    int *row = matrix[i];
    int rowLength = rowLengths[i];

    sortIntegers(row, rowLength);
  }

  RowPair rowPairs[MAX_LINE_LEN] = {0};

  for (int i = 0; i < lineIndex; i++)
  {
    int *row = matrix[i];
    int rowLength = rowLengths[i];

    int sum = 0;
    for (int j = 0; j < rowLength; j++)
    {
      sum += row[j];
    }

    rowPairs[i].row = row;
    rowPairs[i].sum = sum;
  }

  quickSortRowPairs(rowPairs, rowLengths, 0, lineIndex - 1);

  IntList *intList = createIntList();

  for (int i = 0; i < lineIndex; i++)
  {
    int sum = rowPairs[i].sum;

    insertIntNode(intList, createIntNode(sum));
  }

  for (int i = 0; i < lineIndex; i++)
  {
    int isTheLastRow = i == lineIndex - 1;

    int *row = rowPairs[i].row;
    int sum = rowPairs[i].sum;

    int ocurrencesOfSum = countIntNodeOccurrences(intList, sum);

    removeIntValue(intList, sum);

    if (ocurrencesOfSum > 1)
      continue;

    int rowLength = rowLengths[i];

    fprintf(outputFile, "start ");

    for (int j = 0; j < rowLength; j++)
    {
      int isTheLastElement = j == rowLength - 1;

      fprintf(outputFile, "%d", row[j]);

      if (!(isTheLastElement && isTheLastRow))
      {
        fprintf(outputFile, " ");
      }
    }
  }

  if (!isLastLine)
    fprintf(outputFile, "\n");
}

void sortIntegers(int *integers, int n)
{
  quickSortIntegers(integers, 0, n - 1);
}

void quickSortIntegers(int *list, int low, int high)
{
  if (low < high)
  {
    int pivotIndex = partitionIntegers(list, low, high);
    quickSortIntegers(list, low, pivotIndex - 1);
    quickSortIntegers(list, pivotIndex + 1, high);
  }
}

int partitionIntegers(int *list, int low, int high)
{
  int pivot = list[high];
  int i = low - 1;

  for (int j = low; j < high; j++)
  {
    if (list[j] <= pivot)
    {
      i++;
      swapIntegers(list, i, j);
    }
  }

  swapIntegers(list, i + 1, high);
  return i + 1;
}

void swapIntegers(int *integers, int i, int j)
{
  int temp = integers[i];
  integers[i] = integers[j];
  integers[j] = temp;
}

void quickSortRowPairs(RowPair *list, int *rowLengths, int low, int high)
{
  if (low < high)
  {
    int pivotIndex = partitionRowPairs(list, rowLengths, low, high);
    quickSortRowPairs(list, rowLengths, low, pivotIndex - 1);
    quickSortRowPairs(list, rowLengths, pivotIndex + 1, high);
  }
}

int partitionRowPairs(RowPair *list, int *rowLengths, int low, int high)
{
  int pivot = list[high].sum;
  int i = low - 1;

  for (int j = low; j < high; j++)
  {
    if (list[j].sum <= pivot)
    {
      i++;
      swapRowPairs(list, rowLengths, i, j);
    }
  }

  swapRowPairs(list, rowLengths, i + 1, high);
  return i + 1;
}

void swapRowPairs(RowPair *list, int *rowLengths, int i, int j)
{
  RowPair temp = list[i];
  list[i] = list[j];
  list[j] = temp;

  int tempLength = rowLengths[i];
  rowLengths[i] = rowLengths[j];
  rowLengths[j] = tempLength;
}

int parseInt(const char *str)
{
  return atoi(str);
}

void removeNewLine(char *string)
{
  int copyIndex = 0;
  int size = strlen(string);

  char *copy = malloc(sizeof(char) * size);

  for (int index = 0; index < size; index++)
  {
    char value = string[index];

    if (value != '\n')
      copy[copyIndex++] = value;
  }

  strcpy(string, copy);
}

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