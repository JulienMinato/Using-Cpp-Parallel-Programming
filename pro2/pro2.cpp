#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

//Set the number of threads
#ifndef NUMT
#define NUMT		1
#endif

//Set the number of nodes
#ifndef NUMNODES 	
#define NUMNODES	4
#endif

//Set the power indices for superquadratic	
#ifndef N 	
#define N	4
#endif


// how many tries to discover the maximum performance:
#ifndef NUMTRIES
#define NUMTRIES 16
#endif



#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.


//Function Prototype
float Height( int, int );



float Height( int iu, int iv )	// iu,iv = 0 .. NUMNODES-1
{

	float x = -1.  +  2.*(float)iu /(float)(NUMNODES-1);	// -1. to +1.
	float y = -1.  +  2.*(float)iv /(float)(NUMNODES-1);	// -1. to +1.

	float xn = pow( fabs(x), (double)N );
	float yn = pow( fabs(y), (double)N );
	float r = 1. - xn - yn;
	if( r < 0. )
	        return 0.;
	float height = pow( 1. - xn - yn, 1./(float)N );
	return height;
}




int main( int argc, char **argv )
{
    
    
    //Verify OpenMP support
	#ifndef _OPENMP
		fprintf( stderr, "No OpenMP support!\n" );
		return 1;
	#endif


    //Set the number of threads for the for loop
	omp_set_num_threads(NUMT);

    float volume = 0.;

    // the area of a single full-sized tile:

	float fullTileArea = (  ( ( XMAX - XMIN )/(float)(NUMNODES-1) )  *
				( ( YMAX - YMIN )/(float)(NUMNODES-1) )  );

	// sum up the weighted heights into the variable "volume"
	// using an OpenMP for loop and a reduction:

    float maxPerformance = 0.;  

    for (int t = 0; t < NUMTRIES; t++)
	{

		double time0 = omp_get_wtime();

	// sum up the weighted heights into the variable "volume"
	// using an OpenMP for loop and a reduction:


    //#pragma omp parallel for default(none) . . .
	    #pragma omp parallel for default(none) reduction(+:volume)
	    for( int i = 0; i < NUMNODES*NUMNODES; i++ )
	    {
		    int iu = i % NUMNODES;
		    int iv = i / NUMNODES;
		    float z = Height( iu, iv );
		    if((iu == 0 && iv == 0) || (iu == 0 && iv == NUMNODES-1) || 
            (iu == NUMNODES-1 && iv == 0) ||  (iu == NUMNODES-1 && iv == NUMNODES-1)) 
            {
			    volume += z*0.25;
		    }
		    else if(iu == 0 || iv == 0 || iu == NUMNODES-1 || iv == NUMNODES-1)
            {
			    volume += z*0.5;
		    }
		    else 
            {
			volume += z;
            }

            
            //volume *= 2*fullTileArea;
        }
        
	    
	    

        double time1 = omp_get_wtime();
		double megaTrialsPerSecond = (double)NUMNODES*NUMNODES / (time1 - time0) / 1000000.;
		if (megaTrialsPerSecond > maxPerformance)
    		maxPerformance = megaTrialsPerSecond;

    }
        volume *= 2*fullTileArea/NUMTRIES;

        //printf("Volume: %lf\n", volume);
        //printf("The peak performance is %f \n" ,maxPerformance);
        //Number of Numnodes
        printf("%d,%d,%1.7lf,%8.2lf\n",NUMT,NUMNODES,volume,maxPerformance);


    return 0;


}




