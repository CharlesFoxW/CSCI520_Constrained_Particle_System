#include "chain.h"

int windowWidth, windowHeight;
Chain mainChain;
double h = 1.0 / (double) N;

void myinit() {
    /*
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0,1.0,0.01,1000.0);

    // set background color
    glClearColor(0.1, 0.1, 0.1, 0.0);

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    */
    if (N > 1)
        mainChain = Chain();
    else {
        printf("Error::N has to be greater than 2.");
        exit(0);
    }

    Chain testChain = Chain();

    return;
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    double radius = 0.5;
    glBegin(GL_LINE_LOOP);
    glColor3f(0.3, 0.3, 0.3);
    for (int i = 0; i < 360; i++) {
        double degInRad = i * PI / 180.0;
        glVertex2f((float) (cos(degInRad) * radius), (float) (sin(degInRad) * radius - 0.5));
    }
    glEnd();


    mainChain.alterChainByGravity();

    glBegin(GL_POINTS);
    glColor3f(0.8, 0.3, 0.3);
    for (int i = 0; i < N + 1; i++) {
        //printf("%d, %.2f, %.2f \n", i, mainChain.stateQ[2*i], mainChain.stateQ[2*i+1]);
        glVertex2f((float) mainChain.stateQ[2*i], (float) mainChain.stateQ[2*i+1]);
    }

    glEnd();



    glutSwapBuffers();

    glutPostRedisplay();
}

void reshape(int w, int h) {
    // Prevent a divide by zero, when h is zero.
    // You can't make a window of zero height.
    if(h == 0)
        h = 1;

    glViewport(0, 0, w, h);
    windowWidth = w;
    windowHeight = h;

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc,argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    windowWidth = 500;
    windowHeight = 500;

    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(0, 0);
    glutCreateWindow ("Constraint Partical System Chain");
    glutCreateMenu(NULL);


    /* tells glut to use a particular display function to redraw */
    glutDisplayFunc(display);

    /* replace with any animate code */
    //glutIdleFunc(doIdle);

    /* callback for mouse drags */
    //glutMotionFunc(mouseMotionDrag);

    /* callback for window size changes */
    glutReshapeFunc(reshape);

    /* callback for mouse movement */
    //glutPassiveMotionFunc(mouseMotion);

    /* callback for mouse button changes */
    //glutMouseFunc(mouseButton);

    /* register for keyboard events */
    //glutKeyboardFunc(keyboardFunc);

    /* do initialization */
    myinit();

    /* forever sink in the black hole */
    glutMainLoop();




    return 0;
}