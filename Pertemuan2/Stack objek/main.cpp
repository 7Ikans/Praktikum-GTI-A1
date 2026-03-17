/*
 * FreeGLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone 
 * and torus shapes in FreeGLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */

#include <GL/glut.h>

#include <stdlib.h>

#include <gl/gl.h>
#include <math.h>

#define PI 3.1415926535898

void RenderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Objek 1: Garis
    glPushMatrix();
        glLineWidth(2.0f);
        glBegin(GL_LINES);
            glColor3f(1.0f, 1.0f, 1.0f);
            glVertex3f(0.00, 0.20, 0.0);
            glVertex3f(0.00, -0.20, 0.0);
        glEnd();
    glPopMatrix();

    // Objek 2: Titik
    glPushMatrix();
        glPointSize(5.0f);
        glTranslatef(0.35, 0.35, 0.0);
        glBegin(GL_POINTS);
            glColor3f(1.0f, 1.0f, 1.0f);
            glVertex3f(0.25, 0.25, 0.0);
        glEnd();
    glPopMatrix();

    // Objek 3: Segitiga (Triangle Strip)
    glPushMatrix();
        glTranslatef(0.50, 0.50, 0.00);
        glBegin(GL_TRIANGLE_STRIP);
            glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-0.05, -0.05, 0.00);
            glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(0.15, -0.05, 0.00);
            glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(-0.05, 0.05, 0.00);
            glColor3f(1.0f, 1.0f, 0.0f); glVertex3f(0.15, 0.05, 0.00);
        glEnd();
    glPopMatrix();

    // Objek 4: Lingkaran
    glPushMatrix();
        glBegin(GL_LINE_LOOP);
            GLint circle_points = 100;
            for (int i = 0; i < circle_points; i++) {
                float angle = 2 * PI * i / circle_points;
                glVertex2f(cos(angle), sin(angle));
            }
        glEnd();
    glPopMatrix();

    glFlush();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutCreateWindow("Simple Stack");
    glutDisplayFunc(RenderScene);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glutMainLoop();
    return 0;
}
