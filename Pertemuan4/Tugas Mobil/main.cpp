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
#include <math.h>

// Variabel untuk mengontrol rotasi
float angle = 0.0f;

void init() {
    // Mengaktifkan Depth Testing agar objek yang di depan menutupi yang di belakang
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Mengaktifkan sistem Pencahayaan (Lighting)
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); 
    glEnable(GL_COLOR_MATERIAL); // Memungkinkan warna dari glColor3f tetap terlihat

    // Pengaturan sumber cahaya
    GLfloat light_pos[] = { 10.0f, 10.0f, 10.0f, 1.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    glClearColor(0.0, 0.0, 0.0, 1.0); // Background hitam pekat
}

// Fungsi utama untuk membuat kotak (rangka)
// Parameter p, l, t menentukan skala (panjang, lebar, tinggi)
void buatRangka(float p, float l, float t) {
    glBegin(GL_QUADS);
    // Depan
    glNormal3f(0, 0, 1);
    glVertex3f(-p, -l, t); glVertex3f(p, -l, t); glVertex3f(p, l, t); glVertex3f(-p, l, t);
    // Belakang
    glNormal3f(0, 0, -1);
    glVertex3f(-p, -l, -t); glVertex3f(-p, l, -t); glVertex3f(p, l, -t); glVertex3f(p, -l, -t);
    // Atas
    glNormal3f(0, 1, 0);
    glVertex3f(-p, l, -t); glVertex3f(-p, l, t); glVertex3f(p, l, t); glVertex3f(p, l, -t);
    // Bawah
    glNormal3f(0, -1, 0);
    glVertex3f(-p, -l, -t); glVertex3f(p, -l, -t); glVertex3f(p, -l, t); glVertex3f(-p, -l, t);
    // Kiri
    glNormal3f(-1, 0, 0);
    glVertex3f(-p, -l, -t); glVertex3f(-p, -l, t); glVertex3f(-p, l, t); glVertex3f(-p, l, -t);
    // Kanan
    glNormal3f(1, 0, 0);
    glVertex3f(p, -l, -t); glVertex3f(p, l, -t); glVertex3f(p, l, t); glVertex3f(p, -l, t);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Mengatur posisi kamera
    gluLookAt(8.0, 5.0, 12.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
    // Melakukan rotasi otomatis pada seluruh objek mobil
    glRotatef(angle, 0, 1, 0);

    // --- BODY UTAMA MOBIL ---
    glColor3f(0.8, 0.0, 0.0); // Warna merah gelap
    glPushMatrix();
    buatRangka(3.0, 0.8, 1.5);
    glPopMatrix();

    // --- ATAP / KABIN ---
    glColor3f(0.6, 0.0, 0.0); 
    glPushMatrix();
    glTranslatef(-0.3, 1.3, 0); // Geser ke atas body
    buatRangka(1.6, 0.5, 1.2);
    glPopMatrix();

    // --- RODA (NILAI TAMBAH) ---
    // Menggunakan kotak kecil sebagai pengganti roda sesuai konsep rangka
    glColor3f(0.2, 0.2, 0.2); // Abu-abu gelap (ban)
    
    // Roda Depan Kiri & Kanan
    glPushMatrix(); glTranslatef(1.8, -0.7, 1.4); buatRangka(0.6, 0.6, 0.3); glPopMatrix();
    glPushMatrix(); glTranslatef(1.8, -0.7, -1.4); buatRangka(0.6, 0.6, 0.3); glPopMatrix();
    
    // Roda Belakang Kiri & Kanan
    glPushMatrix(); glTranslatef(-1.8, -0.7, 1.4); buatRangka(0.6, 0.6, 0.3); glPopMatrix();
    glPushMatrix(); glTranslatef(-1.8, -0.7, -1.4); buatRangka(0.6, 0.6, 0.3); glPopMatrix();

    glutSwapBuffers();
}

void idle() {
    // Kecepatan rotasi diperlambat menjadi 0.05
    angle += 0.05f; 
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Tugas Mobil 3D - Lighting & Depth");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
