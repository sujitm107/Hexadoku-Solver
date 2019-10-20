#include<stdlib.h>
#include<stdio.h>

double** allocate_array(int, int);
double** getTranspose(double**, int, int);
void printArray(double**, int, int);
double** multiply(double**, double**, int, int);
double** findInverse(double** product, int row, int col);
double** rowOperation(double** matrix, int c, int ri, int rj);

double** allocate_array(int row, int col){
	double** arr = malloc(row * sizeof(double*)); 
	for(int i = 0; i<row; i++){
		arr[i] = malloc(col*sizeof(double));
	}
	return arr;
}

double** getTranspose(double** x, int row, int col){
//TRANSPOSE DIMENSIONS ARE COL X ROW
	double** transpose = allocate_array(col, row);
	
	for(int r = 0; r<col; r++){
		for(int c = 0; c<row; c++){
			transpose[r][c] = x[c][r];
		}
	}

	return transpose;
}

double** multiply(double** x, double** xt, int row, int col){
//row and col are the dimensions of x

	double** product = allocate_array(row, row);
	//int count = 0;
	
	for(int i=0; i<row; i++){
		for(int j=0; j<row; j++){
		double sum = 0;
			for(int k=0; k<col; k++){
				//printf("x: %lf\t xt: %lf\n", x[i][k], xt[k][j]);
				sum = sum + (xt[k][j] * x[i][k]);
	
			}
			product[i][j] = sum;
		}
	}
	
	//printf("%d\n", count);
	
	return product;
}

double** findInverse(double** product, int row, int col){
	
	double** augX = allocate_array(row, col*2);
	//copying product into augmented matrix
	for(int r = 0; r<row; r++){
		for(int c = 0; c<col; c++){
			augX[r][c] = product[r][c];
		}
	}
	//adding ones to augmented part of matrix
	for(int i = 0; i<row; i++){
		augX[i][col+i] = 1;
	}
	
	for(int ri = 0; ri<row; ri++){
		double d = augX[ri][ri];
		
			//dividing the row by the leading entry
			for(int c = ri; c<(col*2); c++){
				augX[ri][c] /= d;
			}
/*			
			if(ri != row-1){
				d = augX[ri+1][ri+1];
				
				for(int c = ri+1; c<(col*2); c++){
					augX[ri+1][c] /= d;
				}
				
			}
*/

			//rb --> Rows Below
			for(int rj = 1; rj<(row - (ri)); rj++){ 
				double constant = augX[ri+rj][ri];
				for(int k = ri; k<(col*2); k++){
					augX[ri+rj][k] = (-constant)*augX[ri][k] + augX[ri+rj][k];
				}
			}
	}
		
/*	
	for(int r = 0; r<row; r++){
		for(int c = 0; c<(col*2); c++){
			if(augX[r][c] == 0){
				augX[r][c] = 0;
			}
		}
	}
*/
			
//THIS IS THE FIRST COLUMN	
	for(int r = 0; r<row; r++){
		printf("%lf\n", augX[r][0]);
	}
	
	double** inverse = allocate_array(row, row);
	
	for(int r = 0; r<row; r++){
		for(int c = 0; c<row; c++){
			inverse[r][c] = augX[r][c+10];
		}
	}
	
	printArray(augX, row, row);
	printf("\n");
	
	return inverse;
}

double** rowOperation(double** matrix, int c, int ri, int rj){
//FOLLOW Cri + rj ----> rj
	
	double** temp = matrix;
	
	return temp;
	
}

int main(int argc, char** argv){
	FILE* fp = fopen(argv[1], "r");
	
	int row, col;
	fscanf(fp, "%d\n", &col);
	fscanf(fp, "%d", &row);
	
	printf("row: %d col: %d\n", row, col);
	
	//creating 2D array
	double** data = allocate_array(row, col);
	
	//creating array for housePrice
	double* y = malloc(row * sizeof(double));
	double num;
	for(int r = 0; r<row; r++){
		for(int c = 0; c<col+1; c++){
			if(fscanf(fp, "%lf,", &num) > 0){
				if(c != col){
					data[r][c] = num;
				} else {
					y[r] = num;
				}		
			}
		}
	}
	
	//printArray(data, row, col);
	//printf("\n");
	
	
	double** transpose = getTranspose(data, row, col);
	//printArray(transpose, col, row);
	//printf("\n");
	
	double** product = multiply(data, transpose, row, col);
	//printArray(product, row, row);
	
// THE DIMENSIONS OF PRODUCT ARE ROW BY ROW, IT IS A SQUARE MATRIX AT THIS POINT BC A MATRIX TIMES ITS TRANSPOSE IS A SQUARE
	
	double** inverse = findInverse(product, row, row);
	
	//printf("THIS IS THE INVERSE\n");
	printArray(inverse, row, row);
	
/*TO PRINT X AND Y
	for(int i = 0; i<row; i++){
		for(int j = 0; j<col; j++){
			printf("%lf\t", data[i][j]);
		}
		printf("%lf\n", y[i]);
	}
*/
	
	fclose(fp);
	return 0;
}

void printArray(double** arr, int rows, int cols){
	for(int i = 0; i<rows; i++){
		for(int j = 0; j<cols; j++){
			printf("%lf\t\t", arr[i][j]);
		}
		printf("\n");
	}
}

