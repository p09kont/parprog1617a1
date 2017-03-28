// your C program without sse instructions

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
//#include <time.h>

// Compile with: gcc -O2 -Wall no-sse.c -o no-sse -DN=.. -DR=..

// use -DN=.. and -DR=... at compilation
//#define N 1000000
//#define R 10


#define K0 0.5
#define K1 0.5
#define K2 0.5
#define K3 0.5
#define K4 5.0
#define K5 0.5
#define K6 0.5
#define K7 0.5
#define K8 0.5



void get_walltime(double *wct) {
  struct timeval tp;
  gettimeofday(&tp,NULL);
  *wct = (double)(tp.tv_sec+tp.tv_usec/1000000.0);
}

int main() {
//srand((unsigned)time(NULL));

float *p[R],*newP[R];
int i,j;
double ts,te,mflops;

  // allocate test arrays
for(i=0;i<R;i++){
	p[i] = (float *)malloc(N*sizeof(float));
}
  
for(i=0;i<R;i++){
	newP[i] = (float *)malloc(N*sizeof(float));
}

  
  //initialize all arrays - cache warm-up
  for (i=0;i<N;i++) {
	for(j=0; j<R; j++){
		 p[i][j]=1.0;
	}
  }
 
printf("\n");

  for(i=0;i<N; i++){
	for(j=0; j<R; j++){
		newP[i][j]=0.0;	
	}
  }

  // get starting time (double, seconds) 
  get_walltime(&ts);
  
  // do triad artificial work
  for (j=1;j<R-1;j++) {
    for (i=1;i<N-1;i++) {
      newP[i][j]=p[i-1][j-1]*K0+p[i-1][j]*K1+p[i-1][j+1]*K2+p[i][j-1]*K3+p[i][j]*K4+p[i][j+1]*K5+p[i+1][j-1]*K6+p[i+1][j]*K7+p[i+1][j+1]*K8;
    }
  }
 
  // get ending time
  get_walltime(&te);
  
  // compute mflops/sec (2 operations per R*N passes)
  mflops = (R*N*2.0)/((te-ts)*1e6);
  
  printf("MFLOPS/sec = %f\n",mflops);
  
  // free arrays
for (i = 0; i < R; i++){
   free(p[i]);
}

for(i=0; i<R; i++){
 free(newP[i]);
}


  return 0;
