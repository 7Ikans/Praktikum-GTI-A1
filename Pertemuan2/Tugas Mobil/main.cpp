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

#include <math.h>

#define PI 3.1415926535898

// Fungsi untuk menggambar roda (lingkaran)
void gambarRoda(float cx, float cy, float r) {
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glColor3f(0.1f, 0.1f, 0.1f); // Warna Hitam Ban
    glBegin(GL_POLYGON);
    for (int i = 0; i < 30; i++) {
        float theta = 2.0f * PI * float(i) / 30.0f;
        glVertex2f(r * cosf(theta), r * sinf(theta));
    }
    glEnd();
    
    // Detail velg kecil di tengah
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 20; i++) {
        float theta = 2.0f * PI * float(i) / 20.0f;
        glVertex2f((r/3) * cosf(theta), (r/3) * sinf(theta));
    }
    glEnd();
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // --- BADAN MOBIL UTAMA ---
    glColor3f(0.2f, 0.4f, 0.8f); // Warna Biru Mobil
    
    // Bagian Belakang (Kotak Utama)
    glRectf(-0.2f, -0.1f, 0.6f, 0.4f); 
    
    // Bagian Kap Depan (Bentuk Poligon agar miring sesuai sketsa)
    glBegin(GL_POLYGON);
        glVertex2f(-0.2f, -0.1f);  // Bawah belakang kap
        glVertex2f(-0.6f, -0.1f);  // Bawah depan
        glVertex2f(-0.6f, 0.15f);  // Depan atas (moncong)
        glVertex2f(-0.3f, 0.15f);  // Atas tengah
        glVertex2f(-0.2f, 0.4f);   // Nyambung ke kaca depan
    glEnd();

    // --- JENDELA ---
    glColor3f(1.0f, 1.0f, 1.0f); // Warna Putih Jendela
    
    // Jendela Depan (Miring mengikuti kap)
    glBegin(GL_POLYGON);
        glVertex2f(-0.15f, 0.2f);
        glVertex2f(0.15f, 0.2f);
        glVertex2f(0.15f, 0.35f);
        glVertex2f(-0.1f, 0.35f);
    glEnd();
    
    // Jendela Belakang
    glRectf(0.25f, 0.2f, 0.5f, 0.35f);

    // --- RODA (Menggunakan Stack) ---
    gambarRoda(-0.4f, -0.15f, 0.15f); // Roda Depan
    gambarRoda(0.35f, -0.15f, 0.15f); // Roda Belakang

    glFlush();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)w / (float)h;
    if (w >= h) {
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Background Putih
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutCreateWindow("Tugas Mobil 2D - Revan");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
