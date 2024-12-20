// Discentes:
// - Filipe Lordêlo Fiúza (20241160009)
// - Júlio dos Reis Sousa (20241160014)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_COLUMN_LEN 100
#define MAX_LINE_LEN 100

void matrixToStdout(int matrix[MAX_COLUMN_LEN][MAX_LINE_LEN], int rowLengths[MAX_LINE_LEN]) {
    // Loop through each row
    for (int i = 0; i < MAX_LINE_LEN; i++) {
        // Get the number of columns for this row from row_lengths
        int cols = rowLengths[i];
        
        if(!cols) continue;

        // Loop through the actual number of columns in the current row
        for (int j = 0; j < cols; j++) {
            // Print each element followed by a space
            printf("%d ", matrix[i][j]);
        }
        
        // Print a new line after each row
        printf("\n");
    }

}

void sortIntegers(int *integers, int n);
void quickSortIntegers(int *list, int low, int high);
int partitionIntegers(int *list, int low, int high);

void readInputAndProcess();
void processLine(char *line, FILE *outputFile, int isLastLine);
void writeOutputToFile(const char *output);

int parseInt(const char *str);
void swapIntegers(int *integers, int i, int j);

void removeNewLine(char* string);

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
    printf("NextLine\n");
  }

  fclose(inputFile);
  fclose(outputFile);
}

void processLine(char *line, FILE *outputFile, int isLastLine)
{  
  int begun = 0;
  int matrix[MAX_LINE_LEN][MAX_COLUMN_LEN], lineIndex = 0, columnIndex = 0, rowLengths[MAX_LINE_LEN];

  char *token = strtok(line, " ");
    
  while (token) {
    if (strcmp(token, "start") == 0) {
      if(begun) {
        printf("\nBeggining Line %d, Row Length %d\n", lineIndex, columnIndex);

      rowLengths[lineIndex] = columnIndex;
      columnIndex = 0;

      lineIndex++;

      printf("New Line Index: %d | ", lineIndex);
      }

      begun = 1;
    }
    else {
      printf("DefininG %dx%d to %d | ", lineIndex, columnIndex + 1, parseInt(token));
      matrix[lineIndex][columnIndex++] = parseInt(token);
    }

    token = strtok(NULL, " ");
  }

  int arrsz = sizeof(rowLengths) / sizeof(int);

  for(int i = 0; i < arrsz; i++) {}
    // printf("\n i = %d | %d\n", i, rowLengths[i]);

  matrixToStdout(matrix, rowLengths);
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

int parseInt(const char *str)
{
  return atoi(str);
}

void removeNewLine(char* string) {
  int copyIndex = 0;
  int size = strlen(string);

  char* copy = malloc(sizeof(char) * size);

  for (int index = 0; index < size; index++) {
    char value = string[index];

    if (value != '\n') 
      copy[copyIndex++] = value;
  }

  strcpy(string, copy);
}