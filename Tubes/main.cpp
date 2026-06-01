#include "game.h"

void keyboard(unsigned char key, int x, int y)
{
    if (currentState == MENU)
    {
        if (key == 13)
        {
            resetGame();
            currentState = PLAYING;
        }
    }
    else if (currentState == GAME_OVER || currentState == VICTORY)
    {
        if (key == 'r' || key == 'R')
        {
            resetGame();
            currentState = PLAYING;
        }
    }
    else if (currentState == PLAYING)
    {
        if (isDashing)
            return;
        switch (key)
        {
        case 'w':
        case 'W':
            dashDirX = 0.0f;
            dashDirY = 1.0f;
            shipY += speed * deltaTime;
            break;
        case 's':
        case 'S':
            dashDirX = 0.0f;
            dashDirY = -1.0f;
            shipY -= speed * deltaTime;
            break;
        case 'a':
        case 'A':
            dashDirX = -1.0f;
            dashDirY = 0.0f;
            shipX -= speed * deltaTime;
            break;
        case 'd':
        case 'D':
            dashDirX = 1.0f;
            dashDirY = 0.0f;
            shipX += speed * deltaTime;
            break;
        case ' ':
            if (dashCooldown == 0)
            {
                isDashing = true;
                dashTimer = 5;
                dashCooldown = 90;
            }
            break;
        }
    }
    if (key == 27)
        exit(0);
    glutPostRedisplay();
}

void reshape(int w, int h)
{
    if (h == 0)
        h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void init()
{
    glClearColor(0.02f, 0.02f, 0.05f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    GLfloat light_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light_position[] = {10.0f, 20.0f, 15.0f, 0.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat mat_shininess[] = {50.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    bgTexture = loadBMP_custom("background.bmp");
    meteorTex = loadBMP_custom("meteor.bmp");

    quadObj = gluNewQuadric();
    gluQuadricTexture(quadObj, GL_TRUE);
    gluQuadricNormals(quadObj, GLU_SMOOTH);

    lastFrameTime = glutGet(GLUT_ELAPSED_TIME);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("BEAVER");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, updateTimer, 0);

    glutMainLoop();
    return 0;
}