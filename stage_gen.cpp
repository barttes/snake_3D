#include "stage_gen.h"
#include <stdio.h>

void gen(int n, float* ver, float* color){

   float square[] = {1, 0, 1,   0,0,0,   -1,0,1,
   -1,0,1,    0,0,0,   -1,0,-1,
   0,0,0,  -1,0,-1,    1,0,-1,
   1,0,-1,   0,0,0,  1, 0, 1};

   int i, j;
   for (i= 0; i<n; i++){
        for (int k =0; k<n;k++){
            for (j=0; j<12; j++){

                ver[k*36*n+i*36+j*3+0]=square[j*3+0]+2*i-n;
                ver[k*36*n+i*36+j*3+1]=0;
                ver[k*36*n+i*36+j*3+2]=square[j*3+2]+2*k-n;
                color[k*36*n+i*36+j*3+0]=0.3*((i+k)%2)+0.1;
                color[k*36*n+i*36+j*3+1]=0.3*((i+k)%2)+0.1;;
                color[k*36*n+i*36+j*3+2]=0.3*((i+k)%2)+0.1;;
            }
        }
   }

}
