#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <xmmintrin.h>





#ifndef ARRAYSIZE
#define ARRAYSIZE 10000
#endif

#ifndef NUMTRIES
#define NUMTRIES 100
#endif

#ifndef SSE_WIDTH
#define SSE_WIDTH 4
#endif




float Ranf( unsigned int *seedp,  float low, float high );
float SimdMulSum( float *a, float *b, int len );
float NonSimdMulSum( float *a, float *b, int len);

float a_array[ARRAYSIZE];
float b_array[ARRAYSIZE];


int main(int argc, char **argv)
{

//Verify OpenMP support
#ifndef _OPENMP
	fprintf( stderr, "No OpenMP support!\n" );
	return 1;
#endif



	//Inialize the arrays:
	unsigned int seed = omp_get_wtime( );
	for(int i=0;i<ARRAYSIZE;i++)
	{
		a_array[i] = Ranf(&seed, 0.f, 1000.f );
		b_array[i] = Ranf(&seed, 0.f, 1000.f );
	}

	double maxMegaMultsSimd = 0.;
	double maxMegaMultsNonSimd = 0.;

	//Computing SimdMulSum performance
	for( int t = 0; t < NUMTRIES; t++ )
	{
		double time0 = omp_get_wtime( );
		SimdMulSum(a_array, b_array, ARRAYSIZE);
 		double time1 = omp_get_wtime( );
        double megaMults = (double)ARRAYSIZE/(time1-time0)/1000000.;
        if( megaMults > maxMegaMultsSimd )
            maxMegaMultsSimd = megaMults;



	}




	//Computing NonSimdMulSum performance
	for( int t = 0; t < NUMTRIES; t++ )
	{
		double time0 = omp_get_wtime( );
		NonSimdMulSum(a_array, b_array, ARRAYSIZE);
 		double time1 = omp_get_wtime( );
        double megaMultsNonSimd = (double)ARRAYSIZE/(time1-time0)/1000000.;
        if( megaMultsNonSimd > maxMegaMultsNonSimd )
            maxMegaMultsNonSimd = megaMultsNonSimd;



	}

	//speedup
	//printf( "Peak Performance with SIMD    = %8.2lf MegaMults/Sec\n", maxMegaMultsSimd );
	//printf( "Peak Performance without SIMD   = %8.2lf MegaMults/Sec\n", maxMegaMultsNonSimd );

	double speedup = maxMegaMultsSimd/maxMegaMultsNonSimd;

	//printf("SpeedUp = %8.2lf\n", speedup);

	printf("%d\t%8.2lf\t%8.2lf\t%8.2lf\t\n",ARRAYSIZE,maxMegaMultsSimd,maxMegaMultsNonSimd,speedup);

}






float Ranf(unsigned int *seedp, float low, float high) {
    float r = (float) rand_r(seedp);
    return (low + r * (high - low) / (float)RAND_MAX);
}




float SimdMulSum( float *a, float *b, int len )
{
	float sum[4] = { 0., 0., 0., 0. };
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;
	register float *pa = a;
	register float *pb = b;

	__m128 ss = _mm_loadu_ps( &sum[0] );
	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		ss = _mm_add_ps( ss, _mm_mul_ps( _mm_loadu_ps( pa ), _mm_loadu_ps( pb ) ) );
		pa += SSE_WIDTH;
		pb += SSE_WIDTH;
	}
	_mm_storeu_ps( &sum[0], ss );

	for( int i = limit; i < len; i++ )
	{
		sum[0] += a[i] * b[i];
	}

	return sum[0] + sum[1] + sum[2] + sum[3];
}



float NonSimdMulSum( float *a, float *b, int len)
{
	float sum = 0.;
	for(int i =0; i< len; i++)
	{
		sum += a[i] * b[i];
	}
	
	return sum;
}