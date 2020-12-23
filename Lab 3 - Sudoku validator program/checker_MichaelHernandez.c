#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define num_threads 27

int valid[num_threads] = { 0 };
typedef struct
{
int row;
int column;
} parameters;

int sudoku[9][9] = { 
 
  {6, 2, 4, 5, 3, 9, 1, 8, 7}, 
  {5, 1, 9, 7, 2, 8, 6, 3, 4}, 
  {8, 3, 7, 6, 1, 4, 2, 5, 5}, 
  {1, 4, 3, 8, 6, 5, 7, 2, 9}, 
  {9, 5, 8, 2, 4, 7, 3, 6, 1}, 
  {7, 6, 2, 3, 9, 1, 4, 5, 8}, 
  {3, 7, 1, 9, 5, 6, 8, 4, 2}, 
  {4, 9, 6, 1, 8, 2, 5, 7, 3}, 
  {2, 8, 5, 4, 7, 3, 9, 1, 6} 
 
};

void *column_worker(void *param)
{  int digit_check[10]={0};
   parameters * params = (parameters *) param;
   int row = params->row;
   int col = params->column;
   if (row != 0 || col > 8)
    	{
	fprintf (stderr,"Invalid Sodoku Puzzle. check row=%d, col=%d\n", row, col);
     	pthread_exit (NULL);
	}
  int checkArray[9] = { 0 };
  int i;
  for (i = 0; i < 9; i++)
    {
	int num = sudoku[i][col];
	if (num < 1 || num > 9 || checkArray[num - 1] == 1)
	{
	pthread_exit (NULL);
	}
	else
	{
	checkArray[num - 1] = 1;
	}
     }
  valid[18 + col] = 1;
  pthread_exit (NULL);
}

void *row_worker (void *param)
{
   parameters * params = (parameters *) param;
   int row = params->row;
   int col = params->column;
   if (col != 0 || row > 8)
  	{
	fprintf (stderr, "Sudoku puzzle is invalid. Check row=%d, col=%d\n",row, col);
	pthread_exit (NULL);
	}
   int arrayCheck[9] = { 0 };
   int i;
   for (i = 0; i < 9; i++)
    {
	int num = sudoku[row][i];   
	if (num < 1 || num > 9 || arrayCheck[num - 1] == 1)
	{
	pthread_exit (NULL);
	}
      else
	{
	arrayCheck[num - 1] = 1;
     	}
    }
   valid[9 + row] = 1;
   pthread_exit (NULL);
}

void *subfield_worker (void *param)
{
  parameters * params = (parameters *) param;
  int row = params->row;
  int col = params->column;
  if (row > 6 || row % 3 != 0 || col > 6 || col % 3 != 0)
	{
        fprintf (stderr, "Invalid Sodoku puzzle. Check row=%d, col=%d\n", row, col);
	pthread_exit (NULL);
	}
  int checkArray[9] = { 0 };
  int i, j;
  for (i = row; i < row + 3; i++)
    {
	for (j = col; j < col + 3; j++)
	{
	int num = sudoku[i][j];
	if (num < 1 || num > 9 || checkArray[num - 1] == 1)
	    {
	    pthread_exit (NULL);
	    }
	else
	    {
	    checkArray[num - 1] = 1;
	    }
	}
    }
  valid[row + col / 3] = 1;
  pthread_exit (NULL);
}


 
int main ()
{
pthread_t threads[num_threads];
int threadIndex = 0;
int i, j;

for (i = 0; i < 9; i++)
    {
     for (j = 0; j < 9; j++)
	{
	if (i % 3 == 0 && j % 3 == 0)
	    {
	    parameters * data = (parameters *) malloc (sizeof (parameters));
	    data->row = i;
	    data->column = j;
	    pthread_create (&threads[threadIndex++], NULL, subfield_worker, data);
	    }
	if (i == 0)
	    {
	    parameters * columnData = (parameters *) malloc (sizeof (parameters));
	    columnData->row = i;
	    columnData->column = j;
	    pthread_create (&threads[threadIndex++], NULL, column_worker, columnData);
	    }
	if (j == 0)
	    {
	    parameters * rowData = (parameters *) malloc (sizeof (parameters));
	    rowData->row = i;
	    rowData->column = j;
	    pthread_create (&threads[threadIndex++], NULL, row_worker, rowData);    
	    }
	}
    }
    for (i = 0; i < num_threads; i++)
    	{
   	 pthread_join (threads[i], NULL);
    	}
    for (i = 0; i < num_threads; i++)
    	{
	if (valid[i] == 0)
		{
		printf ("Sudoku puzzle is invalid.\n");
		return EXIT_SUCCESS;
		}
	}
printf ("Sudoku puzzle is valid.\n");
return EXIT_SUCCESS;

 
}
