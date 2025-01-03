// Discentes:
// - Filipe Lordêlo Fiúza (20241160009)
// - Júlio dos Reis Sousa (20241160014)

#define MAX_LINE_LEN 1000
#define MAX_STACK_SIZE 100

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#endif

enum STRING_COMPARISON
{
  LEFT = -1,
  EQUAL = 0,
  RIGHT = 1
};

// string stack | signatures

typedef struct Stack
{
  char *data[MAX_STACK_SIZE];
  int top;
} Stack;

typedef struct PopResult
{
  int oldTop;
  char *value;
} PopResult;

// effective proccessing | signatures

void processLine(char *line, FILE *outputFile, int isLastLine);
void readInputAndProcess();

// string stack | signatures

void initStack(Stack *stack);
int isEmpty(Stack *stack);
int isFull(Stack *stack);
void push(Stack *stack, const char *value);
PopResult pop(Stack *stack);
const char *peek(Stack *stack);

// utility methods | signatures

int compareStrings(const char *left, const char *str2);

// === IMPLEMENTATIONS ===

// effective proccessing of the lists | implementations

int main()
{

#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
#elif __linux__
  setlocale(LC_ALL, "Portuguese");
#endif

  readInputAndProcess();
  return 0;
}

void readInputAndProcess()
{
  FILE *inputFile = fopen("L1Q2.in", "r");
  if (!inputFile)
  {
    perror("Failed to open input file");
    return;
  }

  FILE *outputFile = fopen("L1Q2.out", "w");
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
  Stack stack, sentinel;

  initStack(&sentinel);
  initStack(&stack);

  char *names[MAX_LINE_LEN];
  int count = 0;
  char *token = strtok(line, " \n");

  while (token)
  {
    names[count++] = token;

    token = strtok(NULL, " \n");
  }

  for (int i = 0; i < count; i++)
  {
    if (isEmpty(&stack))
    {
      push(&stack, names[i]);
      fprintf(outputFile, "push-%s", names[i]);
    }
    else
    {
      const char *peeked = peek(&stack);
      int comparison = compareStrings(names[i], peeked);

      if (comparison == EQUAL || comparison == RIGHT)
      {
        push(&stack, names[i]);
        fprintf(outputFile, " push-%s", names[i]);
        continue;
      }

      int pops = 0;

      while (stack.top > -1 && comparison == LEFT)
      {
        pops++;
        push(&sentinel, pop(&stack).value);

        if (stack.top > -1)
        {
          peeked = peek(&stack);
          comparison = compareStrings(names[i], peeked);
        }
      }

      fprintf(outputFile, " %dx-pop", pops);
      push(&stack, names[i]);
      fprintf(outputFile, " push-%s", names[i]);

      while (!isEmpty(&sentinel))
      {
        char *restoredValue = pop(&sentinel).value;
        push(&stack, restoredValue);
        fprintf(outputFile, " push-%s", restoredValue);
      }
    }
  }

  if (!isLastLine)
    fprintf(outputFile, "\n");
}

// string stack | implementations

void initStack(Stack *stack)
{
  stack->top = -1;
}

int isEmpty(Stack *stack)
{
  return stack->top == -1;
}

int isFull(Stack *stack)
{
  return stack->top == MAX_STACK_SIZE - 1;
}

void push(Stack *stack, const char *value)
{
  if (!isFull(stack))
  {
    stack->data[++stack->top] = strdup(value);
  }
}

PopResult pop(Stack *stack)
{
  PopResult result;
  result.oldTop = stack->top;

  if (!isEmpty(stack))
  {
    result.value = stack->data[stack->top--];
  }

  return result;
}

const char *peek(Stack *stack)
{
  if (!isEmpty(stack))
  {
    return stack->data[stack->top];
  }
  return NULL;
}

// utility methods | implementations

int compareStrings(const char *left, const char *right)
{
  int comparison = strcmp(left, right);

  if (comparison < 0)
    return LEFT;
  else if (comparison > 0)
    return RIGHT;
  else
    return EQUAL;
}