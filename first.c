#include<stdlib.h>
#include<stdio.h>

char** allocate_array(int, int);
void printBoard(char**);
int* checkSubGrid(char**, int, int, int*);
int* checkRow(char**, int, int*);
int* checkCol(char**, int, int*);
char** solveSudoku(char**);
int getValueOfChar(char x);
char getCharOfValue(int x);
int validSudoku(char**);
void printArray(int*);

char** allocate_array(int rows, int cols){

	char** arr = malloc(rows * sizeof(char*));
	for(int i = 0; i<rows; i++){
		arr[i] = malloc(cols * sizeof(char));
	}
	return arr;
}

int validSudoku(char** board){
		
	for(int i = 0; i<16; i++){
		int* validRow = malloc(16*sizeof(int)); //automatically sets all values to 0
		int* validCol = malloc(16*sizeof(int));
		
		validRow = checkRow(board, i, validRow);
		//printf("Index: %d\n", i);
		//printArray(validRow);
		validCol = checkCol(board, i, validCol);
		//printArray(validCol);
		
		for(int j = 0; j<16; j++){
			if(validRow[j] > 1 || validCol[j] > 1){
				free(validRow);
				free(validCol);
				return -1;
			}
		}
		
		free(validRow);
		free(validCol);
	}
	
	for(int r = 0; r<16; r+=4){
		for(int c = 0; c<16; c+=4){
			int* validSub = malloc(16*sizeof(int));
			//printf("Index: %d\n", r+(c/4));
			validSub = checkSubGrid(board, r, c, validSub);
			//printArray(validSub);
			
			for(int j = 0; j<16; j++){
				if(validSub[j] > 1){
					free(validSub);
					return -1;
				}
			}
			
			free(validSub);
		}
	}
	
	
	return 0;
}

char** solveSudoku(char** board){
	char** temp = board;
	
	int x = validSudoku(board);
	int run = 0;
	int runLimit = 1;
	
	if(x == -1){
		//printf("Not valid sudoku\n");
		return NULL;
	}
	
	do{
		run++;
		for(int r = 0; r<16; r++){
			for(int c = 0; c<16; c++){
				
				if(temp[r][c] == '-'){	
					int* present = malloc(16*sizeof(int));
					
					for(int i = 0; i<16; i++){
						present[i] = 0;
					}			
					present = checkRow(board, r, present);
					present = checkCol(board, c, present);
					present = checkSubGrid(board, r, c, present);
		/*	Prints present array			
					for(int i = 0; i<16; i++){
						printf("%d  ", present[i]);
					}
					
					printf("\n");
		*/
					
					//checking for multiple options
					int options = 0;
					for(int i = 0; i<16; i++){
						if(present[i] == 0){
							options++;
						}
					}
					
					if(options > 1){
						runLimit++;
						break;
					}
					

					int index = 0;
					while(present[index] != 0){
						index++;
						if(index == 16){ //this means that there are no options to put into the board, present is all 0s
							printf("r: %d, c: %d\n", r, c);
							printf("No options\n");
							return NULL;
						}
					}
					
					char fillIn = getCharOfValue(index);
					temp[r][c] = fillIn;
					free(present);
				}
				/*
					find which numbers are not present,
						declare present array and fill with 0s int* present = malloc(16*sizeof(int));
						 FOR CHECKS [10 = a, 11 = b, 12 = c, 13 = d, 14 = e, 15 = f]
						check row ------ present = checkRow(char** board, int* present, int row);
						check col ------ present = checkCol(char** board, int* present, int col);
						check subgrid ------ present = checkSubGrid(char** board, int row, int col);
					int fillNumber = findNumber(present); (returns -1 if there if there is a dependency, or if no possible values)
					if(fillNumber > 0){
						temp[r][c] = some value, must convert to character
					}
						temp[r][c] = 'X';
					
				*/

			}
		}
	}while(run < runLimit);
		
	return temp;
}

int main(int argc, char** argv){
	if(argc != 2){
		printf("insufficient arguments\n");
		exit(0);
	}
	FILE* fp = fopen(argv[1], "r");
	if(fp == NULL){
		printf("error\n");
	}
	char num;
	char** board = allocate_array(16, 16);
	
	for(int r = 0; r<16; r++){
		for(int c = 0; c<16; c++){
			if(fscanf(fp, "%c\t", &num) >= 0){
				board[r][c] = num;
			}
		}	
	}
	board = solveSudoku(board);
	printBoard(board);
	return 0;
}

int* checkRow(char** board, int row, int* present){
	
	int* temp = present;
	//only iterating through columns as the row is constant
	for(int c = 0; c<16; c++){
		if(board[row][c] != '-'){
			int value = getValueOfChar(board[row][c]);
			temp[value]++;
		}		
	}
	return temp;
}

int* checkCol(char** board, int col, int* present){
	
	int* temp = present;
	//only iterating through columns as the row is constant
	for(int r = 0; r<16; r++){
		if(board[r][col] != '-'){
			int value = getValueOfChar(board[r][col]);
			temp[value]++;
		}		
	}
	return temp;
}

char getCharOfValue(int x){
	if(x == 10){
		return 'A';
	} else if(x == 11){
		return 'B';
	} else if(x == 12){
		return 'C';
	} else if(x == 13){
		return 'D';
	} else if(x == 14){
		return 'E';
	} else if(x == 15){
		return 'F';
	} else {
		return (x+'0');
	}
}

int getValueOfChar(char x){
	if(x == 'A'){
		return 10;
	} else if(x == 'B'){
		return 11;
	} else if(x == 'C'){
		return 12;
	} else if(x == 'D'){
		return 13;
	} else if(x == 'E'){
		return 14;
	} else if(x == 'F'){
		return 15;
	} else {
		return (x-'0');
	}
}

int* checkSubGrid(char** board, int row, int col, int* present){

	int* temp = present;
	int x = row%4;
	int y = col%4;

	for(int i = row-x; i<(row+4-x); i++){
		for(int j = col-y; j<(col+4-y); j++){
			//printf("%c\t", board[i][j]);    --------- FOR PRINTING
			if(board[i][j] != '-'){
				int value = getValueOfChar(board[i][j]);
				temp[value]++;
			}
		}
		//printf("\n");  ----- FOR PRINTING
	}
	return temp;
}

void printArray(int* arr){

	for(int i = 0; i<16; i++){
		printf("%d ", arr[i]);
	}
	
	printf("\n");
}

void printBoard(char** arr){
	if(arr == NULL){
		printf("no-solution\n");
		return;
	}
	
	for(int i = 0; i<16; i++){
		for(int j = 0; j<16; j++){
			printf("%c\t", arr[i][j]);
		}
		printf("\n");
	}
}