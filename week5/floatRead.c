#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int comparedouble( const void* left, const void* right){
	double diff = *(double*)left - *(double*)right;
	if(diff < 0){
		return -1;
	} else if(diff == 0){
		return 0;
	} else {
		return 1;
	}
}


int main(int argc, char* argv[]){
	double* doubleArray;
	FILE* input, *output;
	int max = 1<<20;
	doubleArray = malloc(max*sizeof(double));
	input = fopen("realdata.bin", "r");
	fread( doubleArray, sizeof(double), max, input);
	qsort( doubleArray, max, sizeof(double), comparedouble);
//	input = fopen("realdata.lis", "r");
//	while(1==1){
//		if( (fscanf(input, "%lf", &doubleArray[numFloats])) != 1){
//			break;
//		}
//		numFloats++;
//		if(numFloats > max){
//			max = max * 2;
//			double* tempFloatArray = malloc(max*sizeof(double));
//			for(int i = 0; i < numFloats+1; i++){
//				tempFloatArray[i] = doubleArray[i];
//			}
//			free(doubleArray);
//			doubleArray = tempFloatArray;
//		}
//	}
	fclose(input);

	for(int i = 0; i < max; i++){
		printf("%f\n", doubleArray[i]);
	}

	output = fopen("realdata.bin", "w");
	fwrite( doubleArray, sizeof(double), max, output);

	fclose(output);
	return 0;
}
