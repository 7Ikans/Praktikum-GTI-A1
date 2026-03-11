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

#include <gl/glut.h> 

void Segitiga(void)

{ 
glClear (GL_COLOR_BUFFER_BIT);
glBegin (GL_TRIANGLES);
glColor3f (1.0f, 0.0f, 0.0f);
glVertex3f(-0.10,-0.10, 0.00);
glColor3f (0.0f, 1.0f, 0.0f);
glVertex3f (0.10,-0.10, 0.00);
glColor3f (0.0f, 0.0f, 1.0f);
glVertex3f ( 0.00, 0.10, 0.00);
glEnd();
glFlush();
}

int main(int argc, char* argv[])
{ 
glutInit(&argc, argv);
glutInitWindowSize (640,480);
glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA);
glutCreateWindow("Segitiga");
glutDisplayFunc (Segitiga);
glClearColor (0.0f, 0.0f, 1.0f, 1.0f);
glutMainLoop(); 
return 0;
}
