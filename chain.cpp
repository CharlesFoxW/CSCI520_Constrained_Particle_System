//
// Created by Clarence Wang on 4/6/17.
//
#include "chain.h"


Chain::Chain() {
    numOfB = 5;
    //assign edge length between each particle
    edgeLength = 1.0 / (double) N;

    for (int i = 0; i < N/2; i++) {
        stateQ[2*i] = 0;
        stateQ[2*i+1] = -1.0 * (double)i * edgeLength;
    }
    for (int i = N/2; i < N+1; i++) {
        stateQ[2*i] = -1.0 * (double)(i - N/2) * edgeLength;
        stateQ[2*i+1] = -0.5;
    }

    //assign constraint vector:
    constraintVector[0] = stateQ[0];
    constraintVector[1] = stateQ[1];
    constraintVector[2] = pow(stateQ[2], 2.0) + pow(stateQ[3], 2.0) - pow(edgeLength, 2.0);
    for (int i = 3; i < N+2; i++) {
        constraintVector[i] = pow(stateQ[2*i-2] - stateQ[2*i-4], 2.0)
                              + pow(stateQ[2*i-1] - stateQ[2*i-3], 2.0) - pow(edgeLength, 2.0);
    }

}

Chain::~Chain() {
}

void Chain::alterChainByGravity() {
    double M[2*N+2][2*N+2] = {0};
    double ft[2*N+2] = {0};
    double delCQ[N+2][2*N+2] = {0};
    double delCQT[2*N+2][N+2] = {0};
    double delCQDelTime[N+2][2*N+2] = {0};
    double stateQDelTime[2*N+2] = {0};
    // assign mass matrix:
    for (int i = 0; i < 2*N+2; i++) {
        M[i][i] = 1.0 / (double)(N+1);
    }

    // gravity force:
    for (int i = 0; i < 2*N+2; i+=2) {
        ft[i] = 0;
        ft[i+1] = -1.0;
    }

    // calculated dC/dQ:
    delCQ[2][2] = 2 * stateQ[2];
    delCQ[2][3] = 2 * stateQ[3];
    for (int i = 3; i < N+2; i++) {
        delCQ[i][2*i-4] = 2.0 * (stateQ[2*i-4] - stateQ[2*i-2]);
        delCQ[i][2*i-3] = 2.0 * (stateQ[2*i-3] - stateQ[2*i-1]);
        delCQ[i][2*i-2] = 2.0 * (stateQ[2*i-2] - stateQ[2*i-4]);
        delCQ[i][2*i-1] = 2.0 * (stateQ[2*i-1] - stateQ[2*i-3]);
    }
    // get dC/dQ transpose:
    for (int i = 0; i < 2*N+2; i++) {
        for (int j = 0; j < N+2; j++) {
            delCQT[i][j] = delCQ[j][i];
        }
    }

    // get dC'/dQ:
    for (int i = 0; i < N+2; i++) {
        for (int j = 0; j < 2*N+2; j++) {
            delCQDelTime[i][j] = delCQ[i][j] / dt;
        }
    }

    for (int i = 0; i < 2*N+2; i++) {
        stateQDelTime[i] = stateQ[i] / dt;
    }



}










