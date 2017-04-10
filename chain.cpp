//
// Created by Clarence Wang on 4/6/17.
//
#include <gsl/gsl_matrix.h>
#include "chain.h"


Chain::Chain() {
    numOfB = 50;
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
    for (int i = 0; i < 2*N+2; i++) {
        stateQV[i] = 0;
    }

    //assign constraint vector:
    constraintVector[0] = stateQ[0];
    constraintVector[1] = stateQ[1];
    //constraintVector[2] = pow(stateQ[2], 2.0) + pow(stateQ[3], 2.0) - pow(edgeLength, 2.0);
    for (int i = 2; i < N+2; i++) {
        constraintVector[i] = pow(stateQ[2*i-2] - stateQ[2*i-4], 2.0)
                              + pow(stateQ[2*i-1] - stateQ[2*i-3], 2.0) - pow(edgeLength, 2.0);
    }
    if (!g_released) {
        constraintVector[N + 2] = pow(stateQ[2 * N], 2.0) + pow(stateQ[2 * N + 1] + 0.5, 2.0) - 0.25;
    }
}

Chain::~Chain() {


}

void Chain::alterChainByGravity(ForceType force) {
    double M[2*N+2][2*N+2] = {0};
    double ft[2*N+2] = {0};
    double delCQ[N+3][2*N+2] = {0};
    double delCQT[2*N+2][N+3] = {0};
    double delCQDelTime[N+3][2*N+2] = {0};
    double stateQDelTime[2*N+2] = {0};
    double secondB[N+3] = {0};
    double acceleration[2*N+2] = {0};

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
    delCQ[0][0] = 1.0;
    delCQ[1][1] = 1.0;
    //delCQ[2][2] = 2 * stateQ[2];
    //delCQ[2][3] = 2 * stateQ[3];
    for (int i = 2; i < N+2; i++) {
        delCQ[i][2*i-4] = 2.0 * (stateQ[2*i-4] - stateQ[2*i-2]);
        delCQ[i][2*i-3] = 2.0 * (stateQ[2*i-3] - stateQ[2*i-1]);
        delCQ[i][2*i-2] = 2.0 * (stateQ[2*i-2] - stateQ[2*i-4]);
        delCQ[i][2*i-1] = 2.0 * (stateQ[2*i-1] - stateQ[2*i-3]);
    }
    if (!g_released) {
        delCQ[N+2][2*N] = 2.0 * stateQ[2*N];
        delCQ[N+2][2*N+1] = 2.0 * stateQ[2*N+1] + 0.99;
    }


    // get dC/dQ transpose:
    for (int i = 0; i < 2*N+2; i++) {
        for (int j = 0; j < N+3; j++) {
            delCQT[i][j] = delCQ[j][i];
        }
    }

    // get dC'/dQ:
    delCQDelTime[0][0] = stateQV[0];
    delCQDelTime[1][1] = stateQV[1];
    //delCQDelTime[2][2] = 2 * stateQV[2];
    //delCQDelTime[2][3] = 2 * stateQV[3];
    for (int i = 2; i < N+2; i++) {
        delCQDelTime[i][2*i-4] = 2.0 * (stateQV[2*i-4] - stateQV[2*i-2]);
        delCQDelTime[i][2*i-3] = 2.0 * (stateQV[2*i-3] - stateQV[2*i-1]);
        delCQDelTime[i][2*i-2] = 2.0 * (stateQV[2*i-2] - stateQV[2*i-4]);
        delCQDelTime[i][2*i-1] = 2.0 * (stateQV[2*i-1] - stateQV[2*i-3]);
    }
    if (!g_released) {
        delCQDelTime[N+2][2*N] = 2.0 * stateQV[2*N];
        delCQDelTime[N+2][2*N+1] = 2.0 * stateQV[2*N+1] + 0.99;
    }


    for (int i = 0; i < 2*N+2; i++) {
        stateQDelTime[i] = stateQV[i];
    }

    for (int i = 0; i < N+3; i++) {
        double factor1 = 0, factor2 = 0;
        for (int j = 0; j < 2*N+2; j++) {
            factor1 += delCQDelTime[i][j] * stateQDelTime[j];
        }
        for (int j = 0; j < 2*N+2; j++) {
            factor2 += delCQ[i][j] * stateQDelTime[j];
        }
        //secondB[i] = -1.0 * factor1 - 0.1 * factor2 - 10 * constraintVector[i];
        secondB[i] = -1.0 * factor1 - 2.0 * numOfB * factor2 - pow(numOfB, 2.0) * constraintVector[i];
        //secondB[i] = -1.0 * factor1;
    }


    // construct the linear system:
    int dimension = N+3 + 2*N+2;

    double a_data[(N+3 + 2*N+2) * (N+3 + 2*N+2)] = {0};
    double b_data[N+3 + 2*N+2] = {0};

    for (int i = 0; i < 2*N+2; i++) {
        for (int j = 0; j < 2*N+2; j++) {
            a_data[i*dimension+j] = M[i][j];
        }
        for (int j = 0; j < N+3; j++) {
            a_data[i*dimension+j+2*N+2] = delCQT[i][j];
        }
    }
    for (int i = 2*N+2; i < dimension; i++) {
        for (int j = 0; j < 2*N+2; j++) {
            a_data[i*dimension+j] = delCQ[i-(2*N+2)][j];
        }
        for (int j = 0; j < N+3; j++) {
            a_data[i*dimension+j+2*N+2] = 0;
        }
    }
    /*
    int size = 0;
    while (size < dimension * dimension) {
        printf("%4.2f, ", a_data[size]);
        if ((size+1) % dimension == 0)
            printf("\n");
        size++;
    }
    printf("\n\n");
    */

    for (int i = 0; i < 2*N+2; i++) {
        b_data[i] = ft[i] - 0.5 * stateQV[i];
    }

    int endPoint = N;
    if (!g_released)
        endPoint++;

    if (force == Up) {
        for (int i = 2; i < endPoint; i++) {
            b_data[2*i+1] += 10.0;
        }
    } else if (force == Down) {
        for (int i = 2; i < endPoint; i++) {
            b_data[2*i+1] += -10.0;
        }
    } else if (force == Left) {
        for (int i = 2; i < endPoint; i++) {
            b_data[2*i] += -10.0;
        }
    } else if (force == Right) {
        for (int i = 2; i < endPoint; i++) {
            b_data[2*i] += 10.0;
        }
    }


    for (int i = 2*N+2; i < dimension; i++) {
        b_data[i] = secondB[i-(2*N+2)];
    }

    gsl_matrix_view m = gsl_matrix_view_array (a_data, (size_t) dimension, (size_t) dimension);
    gsl_vector_view b = gsl_vector_view_array (b_data, (size_t) dimension);
    gsl_vector *x = gsl_vector_alloc ((size_t) dimension);

    //int signum;
    gsl_matrix *V = gsl_matrix_alloc((size_t) dimension, (size_t) dimension);
    gsl_vector *S = gsl_vector_alloc((size_t) dimension);
    gsl_vector *work = gsl_vector_alloc((size_t) dimension);

    //gsl_permutation * p = gsl_permutation_alloc ((size_t) dimension);

    //gsl_linalg_LU_decomp (&m.matrix, p, &signum);
    //gsl_linalg_LU_solve (&m.matrix, p, &b.vector, x);
    /*
    gsl_matrix *mm = gsl_matrix_alloc((size_t) dimension, (size_t) dimension);
    gsl_matrix_memcpy(mm, &m.matrix);
    gsl_matrix_transpose(mm);
    int z = gsl_matrix_equal(mm, &m.matrix);
    printf("z = %d\n", z);
    */

    gsl_linalg_SV_decomp(&m.matrix, V, S, work);

    double eps = 0.000001;
    double maxS = 0;
    for (int i = 0; i < dimension; i++) {
        if (gsl_vector_get(S, (size_t) i) > maxS)
            maxS = gsl_vector_get(S, (size_t) i);
    }
    for (int i = 0; i < dimension; i++) {
        if (gsl_vector_get(S, (size_t) i) < eps * maxS)
            gsl_vector_set(S, (size_t) i, 0);
    }

    //printf ("S = \n");
    //gsl_vector_fprintf (stdout, S, "%4.20f");
    gsl_linalg_SV_solve(&m.matrix, V, S, &b.vector, x);


    //printf ("x = \n");
    //gsl_vector_fprintf (stdout, x, "%4.20f");

    for (int i = 0; i < 2*N+2; i++) {
        acceleration[i] = gsl_vector_get(x, (size_t) i);
    }

    // free the memory:
    //gsl_permutation_free(p);
    gsl_vector_free(x);
    gsl_matrix_free(V);
    gsl_vector_free(S);
    gsl_vector_free(work);

    //stateQ[0] = 0;
    //stateQ[1] = 0;
    for (int i = 0; i < 2*N+2; i++) {
        stateQ[i] += stateQV[i] * dt;
        stateQV[i] += acceleration[i] * dt;
        //printf("Q [%d] = %4.4f\n", i, stateQ[i]);
    }

}


void Chain::addAccUp() {
    alterChainByGravity(Up);
}
void Chain::addAccDown() {
    alterChainByGravity(Down);
}
void Chain::addAccLeft() {
    alterChainByGravity(Left);
}
void Chain::addAccRight() {
    alterChainByGravity(Right);
}








