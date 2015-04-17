#include <stdio.h>

void printInt(int i) {
	printf("%x\n", i);
}

void printHexa(int i) {
	printf("0x%x\n", i);
}

int readInt() {
	int aux;
	scanf("%d", &aux);
	return aux;
}

void printLn() { printf("\n"); }
