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

#include <GL/glut.h>

void drawSquare(float x, float y, float size) {
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + size, y);
        glVertex2f(x + size, y + size);
        glVertex2f(x, y + size);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    float size = 0.2f;      // Ukuran satu kotak
    float spacing = 0.02f;   // Jarak antar kotak
    float startX = -0.55f;   // Posisi awal X (disesuaikan agar di tengah)
    float startY = -0.6f;    // Posisi awal Y (baris paling bawah)

    // Loop untuk tinggi tumpukan (5 tingkat)
    for (int i = 0; i < 5; i++) {
        int squaresInRow = 5 - i; // Jumlah kotak berkurang setiap naik satu tingkat
        
        // Offset X agar setiap baris baru bergeser ke tengah (mengerucut)
        float rowOffset = i * (size + spacing) / 2.0f;

        for (int j = 0; j < squaresInRow; j++) {
            // Memberikan warna gradasi agar terlihat menarik
            glColor3f(0.2f, 0.5f + (i * 0.1f), 0.8f); 
            
            float xPos = startX + rowOffset + (j * (size + spacing));
            float yPos = startY + (i * (size + spacing));
            
            drawSquare(xPos, yPos, size);
        }
    }

    glFlush();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // Background putih
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Tumpukan Persegi Mengerucut - Revan");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

