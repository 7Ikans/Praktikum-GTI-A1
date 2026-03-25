#include <GL/glut.h>

void Display(void) {
    // Membersihkan buffer warna
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Menggambar Poligon (Segitiga dengan warna gradasi)
    glBegin(GL_POLYGON);
        glColor3f(0.0, 0.0, 0.0);    // Hitam
        glVertex3f(-0.5, -0.5, -3.0);
        
        glColor3f(1.0, 0.0, 0.0);    // Merah
        glVertex3f(0.5, -0.5, -3.0);
        
        glColor3f(0.0, 0.0, 1.0);    // Biru
        glVertex3f(0.5, 0.5, -3.0);
    glEnd();

    glFlush(); // Selesai rendering
}

void Reshape(int x, int y) {
    // Jika jendela diminimize, berhenti agar tidak error pembagian nol
    if (y == 0 || x == 0) return;

    // Mengatur matriks proyeksi baru
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // gluPerspective(fovy, aspect, zNear, zFar)
    // fovy: 40 derajat, zNear: 0.5, zFar: 20.0
    gluPerspective(40.0, (GLdouble)x / (GLdouble)y, 0.5, 20.0);

    // Kembali ke matriks modelview
    glMatrixMode(GL_MODELVIEW);
    
    // Gunakan seluruh area window untuk rendering
    glViewport(0, 0, x, y);
}

int main(int argc, char **argv) {
    // Inisialisasi GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    
    // Pengaturan jendela
    glutInitWindowSize(300, 300);
    glutCreateWindow("Intro");

    // Warna latar belakang (hitam)
    glClearColor(0.0, 0.0, 0.0, 0.0);

    // Registrasi fungsi callback
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);

    // Memulai loop utama
    glutMainLoop();
    
    return 0;
}
