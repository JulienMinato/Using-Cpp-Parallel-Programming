#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>


//Set the number of threads
#ifndef NUMT
#define NUMT		4
#endif


unsigned int seed = 0;


//"state" of the system
int	NowYear;		// 2020 - 2025
int	NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int	    NowNumDeer;		// number of deer in the current population

int  GrainMoods;  //Grain is in a good mood and grows fast


// Your basic time step will be one month
const float GRAIN_GROWS_PER_MONTH =		    16.0; //inches
const float ONE_DEER_EATS_PER_MONTH =		1.5;

const float AVG_PRECIP_PER_MONTH =		7.0;	// average
const float AMP_PRECIP_PER_MONTH =		6.0;	// plus or minus
const float RANDOM_PRECIP =			2.0;	    // plus or minus noise

const float AVG_TEMP =				60.0;	   // average  Units of temperature are degrees Fahrenheit
const float AMP_TEMP =				20.0;	   // plus or minus
const float RANDOM_TEMP =			10.0;	   // plus or minus noise

const float MIDTEMP =				40.0;
const float MIDPRECIP =				10.0;


float Ranf( unsigned int *seedp,  float low, float high );
int Ranf( unsigned int *seedp, int ilow, int ihigh );
float SQR( float x );


void GrainDeer();
void Watcher();
void Grain();
void GrainMood(); // MyAgent  Grain is in a good mood and grows fast, but in a bad mood and grows slowly,lol
 

int main( int argc, char **argv )
{

    //Verify OpenMP support
	#ifndef _OPENMP
		fprintf( stderr, "No OpenMP support!\n" );
		return 1;
	#endif

    //Random function
    float x = Ranf( &seed, -1.f, 1.f );

    // starting date and time:
    NowMonth =    0;
    NowYear  = 2020;

    // starting state (feel free to change this if you want):
    NowNumDeer = 1;
    NowHeight =  1.;

  
    // The temperature and precipitation are a function of the particular month
    float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

    float temp = AVG_TEMP - AMP_TEMP * cos( ang );
    NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
    NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );

    if( NowPrecip < 0. )
	        NowPrecip = 0.;



    omp_set_num_threads(NUMT);
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            Watcher();
            
        }
        #pragma omp section
        {
            Grain();
        }
        #pragma omp section
        {
            GrainDeer();
        }
        #pragma omp section
        {
            GrainMood();
        }
    }

    // implied barrier -- all functions must return in order
	// to allow any of them to get past here
    return 0;
}



//help function
float Ranf(unsigned int *seedp, float low, float high) {
    float r = (float) rand_r(seedp);
    return (low + r * (high - low) / (float)RAND_MAX);
}

int Ranf(unsigned int *seedp, int ilow, int ihigh) {
    float low = (float)ilow;
    float high = (float)ihigh + 0.9999f;
    return (int)(Ranf(seedp, low, high));
}

float SQR(float x) {
    return x * x;
}



void GrainDeer()
{

    while( NowYear < 2026 )
    {
	// compute a temporary next-value for this quantity
	// based on the current state of the simulation:
	float height = NowHeight;
    int deer = NowNumDeer;

    if (deer > height)
      deer--;
    else
        deer ++ ;

    // too mant deer
        //deer -= deer/1.1;
    
    if(deer < 0)
        deer =0;
    
	// DoneComputing barrier:
	#pragma omp barrier
	NowNumDeer = deer;

	// DoneAssigning barrier:
	#pragma omp barrier
	

	// DonePrinting barrier:
	#pragma omp barrier
	
    }



}

void Watcher()
{

    while( NowYear < 2026 )
    {
	// compute a temporary next-value for this quantity
	// based on the current state of the simulation:
	

	// DoneComputing barrier:
	#pragma omp barrier
	

	// DoneAssigning barrier:
	#pragma omp barrier
	//printf to csv
    //NowHeight *= 2.54; //inch ->to cm
    //NowTemp =(5./9.)*(NowTemp-32); //F -> oC
    printf("%d-%d\t%6.2f\t%6.2f\t%6.2f\t%d\t%d\n", NowYear,NowMonth+1, NowPrecip, (5./9.)*(NowTemp-32), NowHeight*2.54, NowNumDeer, GrainMoods);
  


    // next year or next month
    if (NowMonth == 11) {
      NowMonth = 0;
      NowYear++;
    }
    else
      NowMonth++;

    //The temperature and precipitation are a function of the particular month
    //copy from http://web.engr.oregonstate.edu/~mjb/cs575/Projects/proj03.html
    float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

    float temp = AVG_TEMP - AMP_TEMP * cos( ang );
    NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
    NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
    
    if( NowPrecip < 0. )
	        NowPrecip = 0.;

	// DonePrinting barrier:
	#pragma omp barrier
	
    }


}



void Grain()
{

    while( NowYear < 2026 )
    {
	// compute a temporary next-value for this quantity
	// based on the current state of the simulation:
	float height = NowHeight;
    //copy from http://web.engr.oregonstate.edu/~mjb/cs575/Projects/proj03.html
    float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );
    float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );

    //Be sure to clamp nextHeight against zero.
    height += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;


    
    //test if happy, trying
    if(GrainMoods >=6)
        height += 2;
    else if(GrainMoods >=3)
        height += 1;
    else
        height += 0;
    
    
    height -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
    

    if(height<0)
        height = 0;
    

	// DoneComputing barrier:
	#pragma omp barrier
	NowHeight = height;

	// DoneAssigning barrier:
	#pragma omp barrier
	

	// DonePrinting barrier:
	#pragma omp barrier
	
    }



}

 
// MyAgent  Grain is in a good mood and grows fast, but in a bad mood and grows slowly,lol

void GrainMood()
{
    
    while( NowYear < 2026 )
    {
	// compute a temporary next-value for this quantity
	// based on the current state of the simulation:
    int mood = GrainMoods;

    mood = 1;
    //Calculate mood index

    if(NowYear % 400 == 0 || (NowYear % 4 == 0 && NowYear % 100 != 0))
        mood += 1;
    else if(NowTemp >= 42.0 && NowTemp <=62.0 )
        mood +=3;
    else if(NowPrecip > 5)
        mood += 2;
    else if(NowMonth == 5 || NowMonth == 6)
        mood += 2;
    else if (NowMonth == 11 &&NowTemp <= 50 )
        mood -=1 ;
    else
        mood -= 2;
    

    if (mood <= 0)
         mood = 0;

	// DoneComputing barrier:
	#pragma omp barrier
	
    GrainMoods = mood;

	// DoneAssigning barrier:
	#pragma omp barrier
	

	// DonePrinting barrier:
	#pragma omp barrier
	

    }


  
}
