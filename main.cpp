#include "chain.h"

time_t timer, startTime;
int windowWidth, windowHeight;
Chain mainChain;
bool g_released, saveScreenToFile;
int sprite;

int g_vMousePos[2];
int g_iRightMouseButton;

// camera parameters
double Theta = 1.24904577;
double Phi = PI / 2;
double R = 1.58113883;

void myinit() {

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

    g_released = false;
    saveScreenToFile = false;
    sprite = 0;

    if (N > 1)
        mainChain = Chain();
    else {
        printf("Error::N has to be greater than 2.");
        exit(0);
    }
    printf("Program Starts.\n");
    time(&startTime);
    printf("t = %s\n", ctime(&startTime));
    return;
}

/* Write a screenshot, in the PPM format, to the specified filename, in PPM format */
void saveScreenshot(int windowWidth, int windowHeight, char *filename)
{
    if (filename == NULL)
        return;

    // Allocate a picture buffer
    Pic * in = pic_alloc(windowWidth, windowHeight, 3, NULL);

    printf("File to save to: %s\n", filename);

    for (int i=windowHeight-1; i>=0; i--)
    {
        glReadPixels(0, windowHeight-i-1, windowWidth, 1, GL_RGB, GL_UNSIGNED_BYTE,
                     &in->pix[i*in->nx*in->bpp]);
    }

    if (ppm_write(filename, in))
        printf("File saved Successfully\n");
    else
        printf("Error in Saving\n");

    pic_free(in);
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(R * cos(Phi) * cos (Theta), R * sin(Phi) * cos (Theta), R * sin (Theta),
              0.0, -0.5, 0.0, 0.0, 1.0, 0.0);

    // draw the ground:
    glFrontFace(GL_CW);
    glBegin(GL_TRIANGLES);
    glColor4f(0.3,0.3,0.3,0);
    glVertex3f((float) 0, (float) -1.0, (float) 0);
    glVertex3f((float) -1.5, (float) -1.0, (float) 1.5);
    glVertex3f((float) -1.5, (float) -1.0, (float) -1.5);

    glColor4f(0.2,0.2,0.2,0);
    glVertex3f((float) 0, (float) -1.0, (float) 0);
    glVertex3f((float) -1.5, (float) -1.0, (float) -1.5);
    glVertex3f((float) 1.5, (float) -1.0, (float) -1.5);
    glEnd();

    glFrontFace(GL_CCW);
    glBegin(GL_TRIANGLES);
    glColor4f(0.3,0.3,0.3,0);

    glVertex3f((float) 0, (float) -1.0, (float) 0);
    glVertex3f((float) 1.5, (float) -1.0, (float) 1.5);
    glVertex3f((float) 1.5, (float) -1.0, (float) -1.5);

    glColor4f(0.2,0.2,0.2,0);
    glVertex3f((float) 0, (float) -1.0, (float) 0);
    glVertex3f((float) -1.5, (float) -1.0, (float) 1.5);
    glVertex3f((float) 1.5, (float) -1.0, (float) 1.5);
    glEnd();


    // draw the circle:
    double radius = 0.5;
    glLineWidth(5);
    glBegin(GL_LINE_LOOP);
    glColor3f(0.2, 0.7, 0.7);
    for (int i = 0; i < 360; i++) {
        double degInRad = i * PI / 180.0;
        glVertex2f((float) (cos(degInRad) * radius), (float) (sin(degInRad) * radius - 0.5));
    }
    glEnd();

    // draw the stand:
    glLineWidth(1);
    glColor3f(0.2, 0.5, 0.8);
    glBegin(GL_LINES);
    glVertex3f(0.5f, -0.5f, 0);
    glVertex3f(0.7f, -0.5f, 0);
    glVertex3f(0.7f, -0.5f, 0);
    glVertex3f(0.7f, -1.0f, 0);
    glVertex3f(-0.5f, -0.5f, 0);
    glVertex3f(-0.7f, -0.5f, 0);
    glVertex3f(-0.7f, -0.5f, 0);
    glVertex3f(-0.7f, -1.0f, 0);
    glVertex3f(-1.5f, -1.0f, 0);
    glVertex3f(1.5f, -1.0f, 0);
    glEnd();

    mainChain.alterChainByGravity(None);

    //glPointSize(6);
    //glBegin(GL_POINTS);
    glColor3f(0.7, 0.6, 0.3);

    time(&timer);
    printf("At time: %s", ctime(&timer));
    printf("q = \n");
    for (int i = 0; i < N+1; i++) {
        //printf("%d, %.2f, %.2f \n", i, mainChain.stateQ[2*i], mainChain.stateQ[2*i+1]);
        //glVertex2f((float) mainChain.stateQ[2*i], (float) mainChain.stateQ[2*i+1]);
        //glTranslatef(0, 0, 0);
        glTranslatef((float) mainChain.stateQ[2*i], (float) mainChain.stateQ[2*i+1], 0);
        glutSolidSphere(0.0075,50,50);
        glTranslatef((float) (-1.0 * mainChain.stateQ[2*i]), (float) (-1.0 * mainChain.stateQ[2*i+1]), 0);
        printf("%4.5f, %4.5f\n", mainChain.stateQ[2*i], mainChain.stateQ[2*i+1]);
    }
    //glEnd();

    glLineWidth(1);
    glBegin(GL_LINES);
    glColor3f(0.8, 0.3, 0.2);
    for (int i = 1; i < N+1; i++) {
        glVertex2f((float) mainChain.stateQ[2*i], (float) mainChain.stateQ[2*i+1]);
        glVertex2f((float) mainChain.stateQ[2*i-2], (float) mainChain.stateQ[2*i-1]);
    }
    glEnd();

    GLfloat mat_specular[] = {1.0, 1.0,1.0,1.0};
    GLfloat mat_shininess[] = {50.0};
    GLfloat light_position[] = {1.0,1.0,1.0,0.0};

    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
    glLightfv(GL_LIGHT0,GL_POSITION,light_position);    // This has to be light not material
    //glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_DEPTH_TEST);
    //glTranslatef(0.5,0,0);
    //glutSolidSphere(0.1,100,100);
    //glTranslatef(-1.0f,0,0);
    //glutSolidSphere(0.1,100,100);
    //glPopMatrix();
    //glFlush();



    // save screen to file
    char s[20]="picxxxx.ppm";
    s[3] = 48 + (sprite / 1000);
    s[4] = 48 + (sprite % 1000) / 100;
    s[5] = 48 + (sprite % 100 ) / 10;
    s[6] = 48 + sprite % 10;
    if (saveScreenToFile)
    {
        saveScreenshot(windowWidth, windowHeight, s);
        //saveScreenToFile=0; // save only once, change this if you want continuos image generation (i.e. animation)
        sprite++;
    }

    if (sprite >= 300) // allow only 300 snapshots
    {
        exit(0);
    }


    glutSwapBuffers();

    glutPostRedisplay();
}

void reshape(int w, int h) {
    // Prevent a divide by zero, when h is zero.
    // You can't make a window of zero height.
    if(h == 0)
        h = 1;

    glViewport(0, 0, w, h);

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set the perspective
    double aspectRatio = 1.0 * w / h;
    gluPerspective(60.0f, aspectRatio, 0.01f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    windowWidth = w;
    windowHeight = h;

    glutPostRedisplay();
}

void mouseMotion (int x, int y)
{
    g_vMousePos[0] = x;
    g_vMousePos[1] = y;
}

void mouseButton(int button, int state, int x, int y)
{
    switch (button)
    {
        case GLUT_RIGHT_BUTTON:
            g_iRightMouseButton = (state==GLUT_DOWN);
            break;
    }

    g_vMousePos[0] = x;
    g_vMousePos[1] = y;
}

/* converts mouse drags into information about rotation/translation/scaling */
void mouseMotionDrag(int x, int y)
{
    int vMouseDelta[2] = {x-g_vMousePos[0], y-g_vMousePos[1]};
    double length;

    if (g_iRightMouseButton) // handle camera rotations
    {
        Phi += vMouseDelta[0] * 0.01;
        Theta += vMouseDelta[1] * 0.01;

        if (Phi>2*PI)
            Phi -= 2*PI;

        if (Phi<0)
            Phi += 2*PI;

        if (Theta>PI / 2 - 0.01) // dont let the point enter the north pole
            Theta = PI / 2 - 0.01;

        if (Theta<- PI / 2 + 0.01)
            Theta = -PI / 2 + 0.01;

        g_vMousePos[0] = x;
        g_vMousePos[1] = y;
    }

}

// gets called whenever a key is pressed
void keyboardFunc (unsigned char key, int x, int y) {
    switch (key) {
        case 27: // esc
            exit(0);
            break;
        case 'r':
            if (!g_released)
                g_released = true;
            break;
        case 's':
            saveScreenToFile = true;
        default:
            break;
    }
}

void SpecialInput(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            // add acceleration to +y direction
            mainChain.addAccUp();
            break;
        case GLUT_KEY_DOWN:
            // add acceleration to +y direction
            mainChain.addAccDown();
            break;
        case GLUT_KEY_LEFT:
            // add acceleration to +y direction
            mainChain.addAccLeft();
            break;
        case GLUT_KEY_RIGHT:
            // add acceleration to +y direction
            mainChain.addAccRight();
            break;
        default:
            break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc,argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    windowWidth = 800;
    windowHeight = 800;

    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(200, 0);
    glutCreateWindow ("Constraint Partical System Chain");
    glutCreateMenu(NULL);


    /* tells glut to use a particular display function to redraw */
    glutDisplayFunc(display);

    /* replace with any animate code */
    //glutIdleFunc(doIdle);

    /* callback for mouse drags */
    glutMotionFunc(mouseMotionDrag);

    /* callback for window size changes */
    glutReshapeFunc(reshape);

    /* callback for mouse movement */
    glutPassiveMotionFunc(mouseMotion);

    /* callback for mouse button changes */
    glutMouseFunc(mouseButton);

    /* register for keyboard events */
    glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(SpecialInput);

    /* do initialization */
    myinit();

    /* forever sink in the black hole */
    glutMainLoop();




    return 0;
}
