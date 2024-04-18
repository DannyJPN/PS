// ***********************************************************************
//
// Demo program for education in subject
// Computer Architectures and Paralel Systems
// Petr Olivka, dep. of Computer Science, FEI, VSB-TU Ostrava
// email:petr.olivka@vsb.cz
//
// Example of CUDA Technology Usage
// Multiplication of elements in float array
//
// ***********************************************************************

#include <stdio.h>

// Function prototype from .cu file
void run_mult( float *pole, int L, float mult );

#define N 200

int main()
{
	// Array initialization 
	float * prvky;
	prvky = new float[N];
	//OCfloat prvky[ N ];
	for ( int i = 0; i < N; i++ )
		prvky[ i ] = ( float ) i;

	// Function calling 
	run_mult( prvky, N, (float) 3.14 );

	// Print result
	for ( int i = 0; i < N; i++ )
		printf( "%8.2f\n", prvky[ i ] );
	printf( "\n" );
	return 0;
}

