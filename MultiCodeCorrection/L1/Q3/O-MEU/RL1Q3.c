// Discentes:
// - Filipe Lordêlo Fiúza (20241160009)
// - Júlio dos Reis Sousa (20241160014)

#define MAX_LINE_LEN 1000

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// stack structure | signature

typedef struct FloatStack
{
  float *values;
  int sentinel;
} FloatStack;

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

typedef struct FloatNode
{
  float value;

  struct FloatNode *prev;
  struct FloatNode *next;
} FloatNode;

typedef struct FloatList
{
  FloatNode *head;
} FloatList;

// effective proccessing of the lists | signatures

void effectivelyProccessParsedContent(IntList *intList, FloatList *floatList, FILE *outputFile);
void processLine(char *line, FILE *outputFile, int isLastLine);
void readInputAndProcess();

// doubly linked circular list methods (insert, remove, iterate, has, print, sort) | signatures

IntNode *createIntNode(int value);
FloatNode *createFloatNode(float value);

IntList *createIntList();
FloatList *createFloatList();

void insertIntNode(IntList *list, IntNode *node);
void insertFloatNode(FloatList *list, FloatNode *node);

void removeIntNode(IntList *list, IntNode *node);
void removeFloatNode(FloatList *list, FloatNode *node);

IntNode *iterateIntNodes(IntList *list, void (*callback)(IntNode *));
FloatNode *iterateFloatNodes(FloatList *list, void (*callback)(FloatNode *));

bool hasAnyNodeWithIntegerValue(IntList *list, int value);
bool hasAnyNodeWithFloatValue(FloatList *list, float value);

void printIntNodes(IntList *list);
void printFloatNodes(FloatList *list);

void sortIntList(IntList *list);
void sortFloatList(FloatList *list);

void freeIntList(IntList *list);     // just in case
void freeFloatList(FloatList *list); // just in case

// utility methods | signatures

char *formatFloatToString(float value);

float parseFloat(const char *string);
int parseInt(const char *string);

void removeNewLine(char *string);

void trimTrailingZeroOfString(char *buffer);

// sorting methods | signatures

void sortIntegers(int *integers, int n);
void sortFloats(float *floats, int n);

void quickSortIntegers(int *list, int low, int high);
int partitionIntegers(int *list, int low, int high);

void quickSortFloats(float *list, int low, int high);
int partitionFloats(float *list, int low, int high);

void swapIntegers(int *integers, int i, int j);
void swapFloats(float *floats, int i, int j);

// stack methods | signatures

FloatStack *createFloatStack(int size);
bool hasFloatInStack(FloatStack *stack, float value);
bool hasFloatStackValues(FloatStack *stack);
float peekFloat(FloatStack *stack);
float popFloat(FloatStack *stack);
void pushFloat(FloatStack *stack, float value);

// === IMPLEMENTATIONS ===

// effective proccessing of the lists | implementations

int main()
{
  readInputAndProcess();
  return 0;
}

void readInputAndProcess()
{
  FILE *inputFile = fopen("L1Q3.in", "r");
  if (!inputFile)
  {
    perror("Failed to open input file");
    return;
  }

  FILE *outputFile = fopen("L1Q3.out", "w");
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

  IntList *intList = createIntList();
  FloatList *floatList = createFloatList();

  char *token = strtok(line, " ");

  int currentlyReadingInts = 0;
  int currentlyReadingFloats = 0;

  while (token)
  {
    if (strcmp(token, "LE") == 0)
    {
      if (currentlyReadingInts)
      {
        printf("Misleaded input: LE while already reading integers\n");
        break;
      }

      currentlyReadingFloats = 0;
      currentlyReadingInts = 1;
    }
    else if (strcmp(token, "LI") == 0)
    {
      if (currentlyReadingFloats)
      {
        printf("Misleaded input: LI while already reading floats\n");
        break;
      }

      currentlyReadingInts = 0;
      currentlyReadingFloats = 1;
    }
    else
    {
      if (currentlyReadingInts)
      {
        int value = parseInt(token);
        IntNode *node = createIntNode(value);

        insertIntNode(intList, node);
      }
      else if (currentlyReadingFloats)
      {
        float value = parseFloat(token);
        FloatNode *node = createFloatNode(value);

        insertFloatNode(floatList, node);
      }
    }

    token = strtok(NULL, " ");
  }

  sortIntList(intList);
  sortFloatList(floatList);

  effectivelyProccessParsedContent(intList, floatList, outputFile);

  if (!isLastLine)
    fprintf(outputFile, "\n");
}

void effectivelyProccessParsedContent(IntList *intList, FloatList *floatList, FILE *outputFile)
{
  fprintf(outputFile, "[");

  IntNode *currentInt = intList->head;
  FloatStack *alreadyPrintedFloats = createFloatStack(1000);

  // colocar um espaço a mais, para destruir a assertividade da minha questão, à titulo de comprovação

  while (currentInt)
  {
    fprintf(outputFile, "%d(", currentInt->value);

    FloatNode *currentFloat = floatList->head;

    while (currentFloat)
    {
      float floatValue = currentFloat->value;
      int integerPart = (int)floatValue;
      int isOneUnitDifference = abs(floatValue - currentInt->value) < 1;
      int hasPreviousAlreadyBeenPrinted = hasFloatInStack(alreadyPrintedFloats, floatValue);

      if ((integerPart == currentInt->value || isOneUnitDifference) && !hasPreviousAlreadyBeenPrinted)
      {
        char *floatString = formatFloatToString(floatValue);

        trimTrailingZeroOfString(floatString);

        fprintf(outputFile, "%s", floatString);

        FloatNode *nextFloat = currentFloat->next;

        if (nextFloat)
        {
          float nextFloatValue = nextFloat->value;

          int nextIntegerPart = (int)nextFloatValue;
          int isNextIntegerOneUnitDifference = abs(nextFloatValue - currentInt->value) < 1;

          if ((nextIntegerPart == currentInt->value || isNextIntegerOneUnitDifference))
            fprintf(outputFile, "->");
        }

        pushFloat(alreadyPrintedFloats, floatValue);
      }

      currentFloat = currentFloat->next;
    }

    fprintf(outputFile, ")");

    if (currentInt->next)
      fprintf(outputFile, "->");

    currentInt = currentInt->next;
  }

  fprintf(outputFile, "]");
}

// doubly linked circular list methods (insert, remove, iterate, print) | implementations

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

FloatNode *createFloatNode(float value)
{
  FloatNode *node = malloc(sizeof(FloatNode));
  node->value = value;
  node->prev = NULL;
  node->next = NULL;

  return node;
}

FloatList *createFloatList()
{
  FloatList *list = malloc(sizeof(FloatList));
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

void insertFloatNode(FloatList *list, FloatNode *node)
{
  if (!list->head)
  {
    list->head = node;
    return;
  }

  FloatNode *current = list->head;

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

void removeFloatNode(FloatList *list, FloatNode *node)
{
  if (!list->head)
  {
    return;
  }

  FloatNode *current = list->head;

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

IntNode *iterateIntNodes(IntList *list, void (*callback)(IntNode *))
{
  IntNode *current = list->head;

  while (current)
  {
    callback(current);
    current = current->next;
  }

  return list->head;
}

FloatNode *iterateFloatNodes(FloatList *list, void (*callback)(FloatNode *))
{
  FloatNode *current = list->head;

  while (current)
  {
    callback(current);
    current = current->next;
  }

  return list->head;
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

bool hasAnyNodeWithFloatValue(FloatList *list, float value)
{
  FloatNode *current = list->head;

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

void printFloatNodes(FloatList *list)
{
  FloatNode *current = list->head;

  if (!current)
  {
    printf("Empty list\n");
    return;
  }

  while (current)
  {
    printf("%f ", current->value);
    current = current->next;
  }

  printf("\n");
}

void sortIntList(IntList *list)
{
  IntNode *current = list->head;

  int amount = 0;
  while (current)
  {
    amount++;
    current = current->next;
  }

  int *integers = malloc(sizeof(int) * amount);

  current = list->head;
  for (int index = 0; index < amount; index++)
  {
    integers[index] = current->value;
    current = current->next;
  }

  sortIntegers(integers, amount);

  current = list->head;
  for (int index = 0; index < amount; index++)
  {
    current->value = integers[index];
    current = current->next;
  }

  free(integers);
}

void sortFloatList(FloatList *list)
{
  FloatNode *current = list->head;

  int amount = 0;
  while (current)
  {
    amount++;
    current = current->next;
  }

  float *floats = malloc(sizeof(float) * amount);

  current = list->head;
  for (int index = 0; index < amount; index++)
  {
    floats[index] = current->value;
    current = current->next;
  }

  sortFloats(floats, amount);

  current = list->head;
  for (int index = 0; index < amount; index++)
  {
    current->value = floats[index];
    current = current->next;
  }

  free(floats);
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

void freeFloatList(FloatList *list)
{
  FloatNode *current = list->head;

  while (current)
  {
    FloatNode *next = current->next;
    free(current);
    current = next;
  }

  free(list);
}

// utility methods | implementations

char *formatFloatToString(float value)
{
  char *result = (char *)malloc(100 * sizeof(char));

  if (value == (int)value)
  {
    snprintf(result, 100, "%.0f", value);
  }
  else
  {
    snprintf(result, 100, "%.2f", value);
  }

  return result;
}

float parseFloat(const char *string)
{
  return atof(string);
}

int parseInt(const char *string)
{
  return atoi(string);
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

void trimTrailingZeroOfString(char *buffer)
{
  char *dot = strchr(buffer, '.');
  if (dot)
  {
    char *end = buffer + strlen(buffer) - 1;
    while (end > dot && *end == '0')
    {
      *end-- = '\0';
    }

    if (end == dot)
    {
      *end = '\0';
    }
  }
}

// sorting methods | implementations

void sortIntegers(int *integers, int n)
{
  quickSortIntegers(integers, 0, n - 1);
}

void sortFloats(float *floats, int n)
{
  quickSortFloats(floats, 0, n - 1);
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

void quickSortFloats(float *list, int low, int high)
{
  if (low < high)
  {
    int pivotIndex = partitionFloats(list, low, high);
    quickSortFloats(list, low, pivotIndex - 1);
    quickSortFloats(list, pivotIndex + 1, high);
  }
}

int partitionFloats(float *list, int low, int high)
{
  float pivot = list[high];
  int i = low - 1;

  for (int j = low; j < high; j++)
  {
    if (list[j] <= pivot)
    {
      i++;
      swapFloats(list, i, j);
    }
  }

  swapFloats(list, i + 1, high);
  return i + 1;
}

void swapIntegers(int *integers, int i, int j)
{
  int temp = integers[i];
  integers[i] = integers[j];
  integers[j] = temp;
}

void swapFloats(float *floats, int i, int j)
{
  float temp = floats[i];
  floats[i] = floats[j];
  floats[j] = temp;
}

// stack methods | implementations

FloatStack *createFloatStack(int size)
{
  FloatStack *stack = malloc(sizeof(FloatStack));

  stack->values = malloc(sizeof(float) * size);
  stack->sentinel = 0;

  return stack;
}

bool hasFloatInStack(FloatStack *stack, float value)
{
  for (int index = 0; index < stack->sentinel; index++)
  {
    if (stack->values[index] == value)
    {
      return true;
    }
  }

  return false;
}

bool hasFloatStackValues(FloatStack *stack)
{
  return stack->sentinel > 0;
}

float peekFloat(FloatStack *stack)
{
  if (!hasFloatStackValues(stack))
  {
    return -1;
  }

  return stack->values[stack->sentinel - 1];
}

float popFloat(FloatStack *stack)
{
  if (!hasFloatStackValues(stack))
  {
    return -1;
  }

  return stack->values[--stack->sentinel];
}

void pushFloat(FloatStack *stack, float value)
{
  stack->values[stack->sentinel++] = value;
}