//#include <stdio.h>
//
//int readInt() { int aux; scanf("%d", &aux); return aux; } 
//void printInt(int aux) { printf("%d", aux); }
//
//char* readStr() { char aux[100]; scanf("%s", aux); return aux; } 
//int printStr(char *str) { printf(str); }


void matmul(int A[3][3], int B[3][3], int C[3][3]) {
	int i=0, j=0, k=0;

	while (i < 3) {
		j = 0;

		while (j < 3) {
			k = 0;
			C[i][j] = 0;

			while (k < 3) {
				C[i][j] += A[i][k] * B[k][j];
				k++;
			}

			j++;
		}

		i++;
	}

}

int main() {
	int A[3][3], B[3][3], C[3][3];
	int i=0, j=0, N=3;

	// read matrix A
	while (i < N) {
		j = 0;
		while (j < N) {
			A[i][j] = readInt();
			j++;
		}

		i++;
	}

	// read matrix B
	i = 0;
	while (i < N) {
		j = 0;
		while (j < N) {
			B[i][j] = readInt();
			j++;
		}

		i++;
	}

	matmul(A, B, C);

	// print matrix C
	i = 0;
	while (i < N) {
		j = 0;
		while (j < N) {
			printInt(C[i][j]);
			printStr(" ");
			j++;
		}
		printStr("\n");

		i++;
	}


	return 0;
}
