#include<stdlib.h>
#include<stdio.h>

double** allocate_array(int, int);
double** getTranspose(double**, int, int);
void printArray(double**, int, int);
double** multiply(double**, double**, int, int, int, int);
double** findInverse(double** product, int row, int col);
double* multiplyWithSingle(double**, double*, int, int);


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

double** multiply(double** xt, double** x, int rowsOne, int colsOne, int rowsTwo, int colsTwo){

    double** product = allocate_array(rowsOne, colsTwo);
    //int count = 0;
    
    for(int i=0; i<rowsOne; i++){
        for(int j=0; j<colsTwo; j++){
        double sum = 0;
            for(int k=0; k<colsOne; k++){
                //printf("x: %lf\t xt: %lf\n", x[i][k], xt[k][j]);
                sum = sum + (x[k][j] * xt[i][k]);
    
            }
            product[i][j] = sum;
        }
    }
    
    //printf("%d\n", count);
    
    return product;
}

double* multiplyWithSingle(double** xt, double* y, int row, int col){
    //row = 4, col = 10
    //the col is the number of rows in y, the number of cols in y is always 1
    //result shoule be 4 by 1
    
    double* product = malloc(row*sizeof(double));
    
    for(int i = 0; i<row; i++){
        for(int j = 0; j<1; j++){
        double sum = 0;
            for(int k = 0; k<col; k++){
                sum = sum + (xt[i][k] * y[k]);
            
            }
        product[i] = sum;
        }
    }
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
    
    printf("Augmented Matrix\n");
    
    printArray(augX, row, col*2);
    
    for(int ri = 0; ri<row; ri++){
        double d = augX[ri][ri];
        
            //dividing the row by the leading entry
            for(int c = ri; c<(col*2); c++){
                augX[ri][c] /= d;
            }

            //rb --> Rows Below
            for(int rj = 1; rj<(row - (ri)); rj++){
                double constant = augX[ri+rj][ri];
                for(int k = ri; k<(col*2); k++){
                    augX[ri+rj][k] = (-constant)*augX[ri][k] + augX[ri+rj][k];
                }
            }
    }
    
    printf("Augmented Matrix in rref\n");
    printArray(augX, row, col*2);
    
    //this loop is for the leading entry
    for(int current = row-1; current>0; current--){
        printf("pivot: %lf ", augX[current][current]);
        
        for(int rowsAbove = 1; rowsAbove<=current; rowsAbove++){
            double constant = augX[current-rowsAbove][current];
            printf("Number above: %lf ", constant);
            for(int c = 0; c < (col*2); c++){
                augX[current-rowsAbove][c] += (-constant)*augX[current][c];
            }
        }
        
        printf("\n");
    }
    
    printf("\n");
            
//THIS IS THE FIRST COLUMN
    for(int r = 0; r<row; r++){
        printf("%lf\n", augX[r][0]);
    }
    
    double** inverse = allocate_array(row, row);
    
    for(int r = 0; r<row; r++){
        for(int c = 0; c<row; c++){
            inverse[r][c] = augX[r][c+col];
        }
    }

printf("THIS IS THE OG matrix put in rref\n");
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
    double** data = allocate_array(row, col+1);
    
    for(int r = 0; r<row; r++){
        data[r][0] = 1;
    }
    
    //creating array for housePrice
    double* y = malloc(row * sizeof(double));
    double num;
    for(int r = 0; r<row; r++){
        for(int c = 1; c<col+2; c++){
            if(fscanf(fp, "%lf,", &num) > 0){
                if(c != col+1){
                    data[r][c] = num;
                } else {
                    y[r] = num;
                }
            }
        }
    }
    
    fclose(fp);

printf("DATA w/ 1s appended");
    printArray(data, row, col+1);
    printf("\n");
    
    //we are changing col bc of appending the 1s column
    col = col +1;
    
printf("TRANSPOSE\n");
    double** transpose = getTranspose(data, row, col);
    printArray(transpose, col, row);
    printf("\n");
    
//multiply(transpose, data, trasposerow, transposecol, datarow, datacol)
    double** product = multiply(transpose, data, col, row, row, col);
printf("PRODUCT OF XT and X\n");
    printArray(product, col, col);
    
// THE DIMENSIONS OF PRODUCT ARE ROW BY ROW, IT IS A SQUARE MATRIX AT THIS POINT BC A MATRIX TIMES ITS TRANSPOSE IS A SQUARE
    
    double** inverse = findInverse(product, col, col);
    
    printf("THIS IS THE INVERSE\n");
    printArray(inverse, col, col);
    
printf("PLEASE BE IDENITY MATRIX\n");
    double** Identity = multiply(product, inverse, col, col, col, col);
    printArray(Identity, col, col);
    
    double* xty = multiplyWithSingle(transpose, y, col, row);
printf("X transpose times y\n");
    for(int i = 0; i<col; i++){
        printf("%lf ", xty[i]);
    }
    printf("\n");
    
printf("W!!!!!\n");
    double* w = multiplyWithSingle(inverse, xty, col, col);
    for(int i = 0; i<col; i++){
        printf("%lf ", w[i]);
    }
    printf("\n");
    
//GETTING THE TEST
    
    fp = fopen(argv[2], "r");
    fscanf(fp, "%d", &row);
    //reduce the number of
    col = col - 1;
    
    double** test = allocate_array(row, col);
    
    for(int i = 0; i < row; i++){
        double num;
        for(int j = 0; j < col; j++){
            if(fscanf(fp, "%lf,", &num) > 0){
                test[i][j] = num;
            }
        }
    }
printf("TEST ARRAY\n");
    printArray(test, row, col);
    
    double* weightsFromOne = malloc()
    
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

