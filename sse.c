// your C program using sse instructions
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <emmintrin.h>

// Compile with: gcc -msse2 -O2 -Wall sse.c -o sse -DN=.. -DR=..


// use -DN=.. and -DR=... at compilation
//#define N 1000000
//#define R 10

void get_walltime(double *wct) {
  struct timeval tp;
  gettimeofday(&tp,NULL);
  *wct = (double)(tp.tv_sec+tp.tv_usec/1000000.0);
}

int main() {

float constants[]={0.5, 0.5, 0.5, 0.5, 5.0, 0.5, 0.5, 0.5, 0.5};

float *p, *newP, *c;
__m128 *vp,*vnewP, *vc;

int i,j;
double ts,te,mflops;

  // allocate test arrays - request alignment at 16 bytes
  i = posix_memalign((void **)&p,16,N*sizeof(float));
  if (i!=0) exit(1);
  i = posix_memalign((void **)&newP,16,N*sizeof(float));
  if (i!=0) { free(p); exit(1); }
  i = posix_memalign((void **)&c,16,9*sizeof(float));
  if (i!=0) { free(p); free(newP); exit(1); }
  
  
  //initialize all arrays - cache warm-up
  for (i=0;i<N;i++) {
    p[i]=0.0; newP[i]=1.0; 
  }
 
 
  // get starting time (double, seconds) 
  get_walltime(&ts);
  
  for(i=0; i<9; i++){
  	*(c+i)= constants[i];
  }
  
  // do triad artificial work
  for (j=0;j<R;j++) {
    // alias the sse pointers to arrays
    vp = (__m128 *)p; vnewP = (__m128 *)newP;
    vc = (__m128 *)c; 
	
    for (i=0;i<N;i+=4) {
      *vnewP = _mm_add_ps(_mm_mul_ps(*vc,*vp),_mm_mul_ps(*vc,*vp));
      vp++; vnewP++; vc++;
      
    }
  }
 
  // get ending time
  get_walltime(&te);
  
  // compute mflops/sec (2 operations per R*N passes)
  mflops = (R*N*2.0)/((te-ts)*1e6);
  
  printf("MFLOPS/sec = %f\n",mflops);
  
  // free arrays
  free(p); free(newP);
  
  return 0;
}
