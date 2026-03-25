#include <GL/glut.h>

// Variabel untuk menyimpan derajat rotasi pada tiap sumbu
GLfloat xRotated, yRotated, zRotated;

void Display(void) {
    // Membersihkan buffer warna
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // 1. Translasi: Menjauhkan objek dari kamera agar terlihat (sumbu Z negatif)
    glTranslatef(0.0, 0.0, -4.0);

    // 2. Rotasi: Memutar objek berdasarkan nilai yang terus bertambah di fungsi Idle
    glRotatef(xRotated, 1.0, 0.0, 0.0);
    glRotatef(yRotated, 0.0, 1.0, 0.0);
    glRotatef(zRotated, 0.0, 0.0, 1.0);

    // 3. Skala: Mengubah perbandingan ukuran (memperpanjang sumbu X)
    glScalef(2.0, 1.0, 1.0);

    // Menggambar kubus kawat (wireframe)
    glutWireCube(1.0);

    glFlush();
    // Menukar buffer (karena menggunakan GLUT_DOUBLE untuk animasi halus)
    glutSwapBuffers();
}

void Reshape(int x, int y) {
    if (y == 0 || x == 0) return;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Menggunakan proyeksi perspektif agar objek terlihat 3D
    gluPerspective(40.0, (GLdouble)x / (GLdouble)y, 0.5, 20.0);
    
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, x, y);
}

void Idle(void) {
    // Menambah nilai rotasi setiap kali CPU sedang idle/bebas
    xRotated += 0.3;
    yRotated += 0.1;
    zRotated += -0.4;

    // Memanggil ulang fungsi display untuk menggambar posisi baru
    Display();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    
    // Menggunakan GLUT_DOUBLE (Double Buffering) untuk menghindari flickering pada animasi
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    
    glutInitWindowSize(300, 300);
    glutCreateWindow("Cube Example - Animasi Rotasi");

    // Mengatur agar poligon hanya digambar garisnya saja (wireframe)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Inisialisasi nilai rotasi
    xRotated = yRotated = zRotated = 0.0;

    glClearColor(0.0, 0.0, 0.0, 0.0);

    // Registrasi fungsi callback
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    
    // Fungsi Idle dipanggil terus-menerus untuk menjalankan animasi
    glutIdleFunc(Idle);

    glutMainLoop();
    return 0;
}

