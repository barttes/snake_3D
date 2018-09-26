#ifndef MYCUBE_H
#define MYCUBE_H
#include <vector>
unsigned int n = 10;
unsigned int mySquareVertexCount=12*n*n;

float* mySquareVertices= new float[36*n*n];

float* mySquareColors=new float[36*n*n];

float* mySquareNormals=new float[36*n*n];

float* mySquareTextures=new float[24*n*n];

unsigned int mySquareTexHight;

unsigned int mySquareTexWidth;

std::vector<unsigned char> mySquareTexImage;

#endif

