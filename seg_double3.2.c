#include "stdio.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <sys/time.h>
#include <string.h>

#define PI	3.1415926535897932384626
#define NOS	13			// number of strips
#define A	3.101			// exp fit aplitutde
#define B	1.82815			// exp fit sd
#define C	0.	//-0.00335916	// intercept - x
#define D	0.	//0.0235454	// intercept - y
#define Pitch	2.5			// distance between two different adjacent strips
#define Delay	2.0			// delay    between  "
#define Epsil	0.01			// time( or position) segmentation

//double SUM1[(NOS-1)*250][(NOS-1)*250];
long double max[5][(NOS-1)*250];


//########################## String categorize fth ###############################
//#                                                                              #

char* itoa(int value, char* result, int base) {
        // check that the base if valid
        if (base < 2 || base > 36) { *result = '\0'; return result; }
        char* ptr = result, *ptr1 = result, tmp_char;
        int tmp_value;

        do {
                tmp_value = value;
                value /= base;
                *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
        } while ( value );

                // Apply negative sign
        if (tmp_value < 0) *ptr++ = '-';
        *ptr-- = '\0';
        while(ptr1 < ptr) {
                tmp_char = *ptr;
                *ptr--= *ptr1;
                *ptr1++ = tmp_char;
        }
        return result;
	}

//#                                                                               #
//#################################################################################



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//		define the normal distribution function				//@

long double ND(long double x, long double u, long double s){

	return 1./(powl(2.*PI,0.5)*s)*expl(-powl((x-u)/s,2.));

}

//										//@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//              define the pulse height coefficient function by time            //@

long double corl(long double x, long double y){

	return A/(powl(2.*PI,0.5)*B)*expl(-(x-(y+C))*(x-(y+C))/(2.*B*B))+D;

}

//                                                                              //@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//				position evaluation				 //@

void CFD(double hp){

  double r;

  //r = Epsil*Delay*hb-Delay*1.5;



}

//                                                                             	 //@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void main(int argc, char *argv[]){


  FILE * fpc0;


	long double * mu = (long double *)malloc(NOS * sizeof(long double));
	long double * sig = (long double *)malloc(NOS * sizeof(long double));

  //double SUM[(NOS-1)*25][(NOS-1)*250],COR[(NOS-1)*25][(NOS-1)*250];
  //	double max[2][400];
	long double r, xi, mean, norm, target;
	long double xii;
  long double psum,ssum;
  int i,k,j,uu,tline,id;
  double t1, t2, position, dt;
	long double left_seg_max, right_seg_max;

  char *p;
  double num;
  long double distrib;
 // distrib = atof(argv[1]);
  errno = 0;
  long conv;

  char *file = ( char * ) malloc( 15 * sizeof( char ) );
  char *str1 = ( char * ) malloc( 5 * sizeof( char ) );
  char *str2 = ( char * ) malloc( 5 * sizeof( char ) );
  char *str3 = ( char * ) malloc( 5 * sizeof( char ) );

  if (argc > 1){

    /*@*///#####################	Specify the event coordination to visualiz	########################/*@*/
    /*@*/													/*@*/
    /*@*/	//conv = strtol(argv[1], &p, 10);				       				/*@*/
    /*@*/													/*@*/
    /*@*/	//if (errno != 0 || *p != '\0' || conv > INT_MAX){}						/*@*/
    /*@*/	//else num = conv;										/*@*/
    /*@*/	num = atof(argv[1]);			       							/*@*/
    /*@*/	target = (num+Pitch*(NOS-1.)/2.)/(Epsil*Pitch)*Delay*5./20.;	                		/*@*/
    /*@*/	tline = (int)target;										/*@*/
    /*@*/													/*@*/
    /*@*/	printf("Visualize coordinate: %.2lf mm (a %d-th time segment) heat event\n",num,tline);		/*@*/
    /*@*/													/*@*/
    /*@*///#####################################################################################################/*@*/

  }

  else {

    tline = (NOS-1)*20 -1;
    printf("Visualize the Center event\n");

  }

  sig[0] = sig[1] = sig[2] = sig[3] = sig[4] = sig[5] = sig[6] = sig[7] = 2.56;

	distrib = 1.;

for(id = 0;  id < 2/*10*/; id++){


  j = 0;
  uu = 0;

	itoa(id,str2,10);
	str1 = "ft";
	str3 = ".txt";
	snprintf( file,sizeof( file ), "%s%s%s", str1, str2, str3 );


        fpc0 = fopen(file,"w");


	while(j < (NOS-1)*250){

		r = Epsil*(long double)(j+1)*Delay/Pitch - 0.3125*(long double)(NOS-1.)*Epsil+0.028;                // time

		for( k = 0; k < NOS; k++) mu[k] = Delay*(double)k;

    //if(mu[0]>=0.) break;

			i = 0;
			mean = 0.;
			norm = 0.;
			uu=(int)j/16;
			max[0][j] = 0.;
			max[2][j] = 0.;

			while(i < (NOS-1)*600){

				xi = Epsil*(long double)i-(long double)(NOS-1.)*Epsil*200.;                                  // position
				psum = 0.;
				ssum = 0.;

				for(k = 0; k < NOS; k++) {

					//if(k>6) psum += 0.;
					/*else*/if(k<=6) psum += corl(mu[k],r) * ND(xi, mu[k], sig[2]);
					if(k>=6) ssum += corl(mu[k],r) * ND(xi, mu[k], sig[2]);
					//else ssum += 0.;

				}

				if(max[0][j] < psum){

					max[0][j] = psum;
					max[1][j] = xi;

				}

				if(max[2][j] < ssum){

					max[2][j] = ssum;
					max[3][j] = xi;

				}


      //mean += sum*xi;
      //norm += sum;
      //      SUM1[j][i] = sum;
      //			if(j%16==0 ) {
      //COR[uu][i] = corl(mu[1],r);
      //				SUM[uu][i] = sum;//SUM[(j&13)*500+i]=sum;
      //			if(i==0)dkljfor( k = 0; k < NOS; k++) printf("%d -> %lf\n",k,mu[k]-r);
      //			}

				i++;

      //if(!(sum > 0.0) || !(sum < 0.0) ){ printf("singularity error has occuerd\n"); break; }

				}

			left_seg_max = (long double)max[1][j]*Pitch/Delay-(long double)(NOS-1.)*Pitch/2.;
			right_seg_max = (long double)max[3][j]*Pitch/Delay-(long double)(NOS-1.)*Pitch/2.;


		//	if(xii == max[1][j] && id == 1) printf("descrete error has apeared at j = %d\n",j);
		//	xii = max[1][j];  diagnosis  of discrete err

			fprintf(fpc0, "%.8Lf %.8Lf %.8Lf\n"/*%.8Lf %.8L/\n"*/,-Pitch*(long double)(NOS-1.)/2.*distrib+j*(Epsil*Pitch)/Delay*4./5.*distrib,left_seg_max*distrib,right_seg_max*distrib/*,max[1][j],max[3][j]*/);

    //fprintf(ff,"%lf %lf\n",,);
    // fprintf(fr2, "%lf %lf\n",mean/norm,r);
    //for( k = 0; k < NOS; k++) printf("%d -> %lf\n",k,mu[k]);

			j++;

		}

		printf("%d-th simulation\n",id);

		distrib = 1 + id*0.25;

		if(distrib > 3.3){
			id = 15;
     			break;
		}

		fclose(fpc0);

 } // distribution - loop





 
/*	for(j = 0; j<250; j++) {
//
for(i = 0; i<13; i++) fprintf(fr, "%lf ", SUM[i][j]);
fprintf(fr, "%lf\n",0.12*j-6.);
//
}*/


     free(mu);
     free(sig);
     free(file);


     FILE *fit = fopen("gnuft0","w");
  

     fprintf(fit,"set term qt 1\n\n set xrange [-15.85:-0.9]\n f(x) = a*x**9+b*x**8+c*x**7+d*x**6+e*x**5+g*x**4+h*x**3+i*x**2+j*x+k\n");
     fprintf(fit,"fit [-17:-0.9] f(x) 'ft1.txt' u 2:1 via a, b, c, d, e, g, h, i, j, k\n plot 'ft1.txt' u 2:1, f(x) \n\n");
     fprintf(fit,"set print \"ft1.txta\" \n print a, b, c, d, e, g, h, i, j, k\n set print\n\n");
     fprintf(fit,"set term qt 2\n\n set xrange [-1:-0.2]\n f(x) = a*x**9+b*x**8+c*x**7+d*x**6+e*x**5+g*x**4+h*x**3+i*x**2+j*x+k\n");
     fprintf(fit,"fit [-1:-0.2] f(x) 'ft1.txt' u 2:1 via a, b, c, d, e, g, h, i, j, k\n plot 'ft1.txt' u 2:1, f(x)\n\n");
     fprintf(fit,"set print \"ft1.txtb\"\n print a, b, c, d, e, g, h, i, j, k\n set print\n\n");
     fprintf(fit,"set term qt 3\n\n set xrange [0.9:15.85]\n f(x) = a*x**9+b*x**8+c*x**7+d*x**6+e*x**5+g*x**4+h*x**3+i*x**2+j*x+k\n");
     fprintf(fit,"fit [0.9:17] f(x) 'ft1.txt' u 3:1 via a, b, c, d, e, g, h, i, j, k\n plot 'ft1.txt' u 3:1, f(x)\n\n");
     fprintf(fit,"set print \"ft1.txtc\"\n print a, b, c, d, e, g, h, i, j, k\n set print\n\n");
     fprintf(fit,"set term qt 4\n\n set xrange [0.2:1]\n f(x) = a*x**9+b*x**8+c*x**7+d*x**6+e*x**5+g*x**4+h*x**3+i*x**2+j*x+k\n");
     fprintf(fit,"fit [0.2:1] f(x) 'ft1.txt' u 3:1 via a, b, c, d, e, g, h, i, j, k\n plot 'ft1.txt' u 3:1, f(x)\n\n");
     fprintf(fit,"set print \"ft1.txtd\"\n print a, b, c, d, e, g, h, i, j, k\n set print\n\n");
     fprintf(fit,"pause -1 \"Hit any key to continue\"\n");
     fclose(fit);


     system("gnuplot gnuft0");
     
}
