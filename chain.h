//
// Created by Clarence Wang on 4/5/17.
//

#ifndef CSCI520PROJ3_CHAIN_H
#define CSCI520PROJ3_CHAIN_H
#ifdef WIN32
  #include <windows.h>
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "openGL-headers.h"
#include <gsl/gsl_linalg.h>

#define PI  3.141592653589793238462643383279
#define N   12
#define dt  0.001

struct Point {
    double x;
    double y;
};

class Chain {
public:
    Chain();
    ~Chain();
    void alterChainByGravity();

    double edgeLength;
    double numOfB;
    double constraintVector[N+2];
    double stateQ[2*N+2];
    double stateQV[2*N+2];
};


#endif //CSCI520PROJ3_CHAIN_H
