#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// stack structure | signatures

typedef struct IntStack
{
  int *values;
  int sentinel;
} IntStack;

// stack methods | signatures

IntStack *createIntStack(int size);
bool hasIntInStack(IntStack *stack, int value);
bool hasIntStackValues(IntStack *stack);
int peekInt(IntStack *stack);
int popInt(IntStack *stack);
void pushInt(IntStack *stack, int value);

// stack methods | implementations
IntStack *createIntStack(int size)
{
  IntStack *stack = malloc(sizeof(IntStack));

  stack->values = malloc(sizeof(int) * size);
  stack->sentinel = 0;

  return stack;
}

bool hasIntInStack(IntStack *stack, int value)
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

bool hasIntStackValues(IntStack *stack)
{
  return stack->sentinel > 0;
}

int peekInt(IntStack *stack)
{
  if (!hasIntStackValues(stack))
  {
    return -1;
  }

  return stack->values[stack->sentinel - 1];
}

int popInt(IntStack *stack)
{
  if (!hasIntStackValues(stack))
  {
    return -1;
  }

  return stack->values[--stack->sentinel];
}

void pushInt(IntStack *stack, int value)
{
  stack->values[stack->sentinel++] = value;
}