#include <GL/glut.h>
#include <stdlib.h>

static int shoulder = 0, elbow = 0, hand = 0;

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

// Fungsi untuk menggambar satu ruas jari berbentuk balok kawat
void gambarJari(float x, float y, float z) {
    glPushMatrix();
        glTranslatef(x, y, z);      
        glScalef(0.4, 0.1, 0.1);    
        glutWireCube(1.0);          
    glPopMatrix();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
        // --- BAHU (SHOULDER) ---
        glTranslatef(-1.0, 0.0, 0.0);
        glRotatef((GLfloat) shoulder, 0.0, 0.0, 1.0);
        glTranslatef(1.0, 0.0, 0.0);
        glPushMatrix();
            glScalef(2.0, 0.4, 1.0);
            glutWireCube(1.0);
        glPopMatrix();

        // --- SIKU (ELBOW) ---
        glTranslatef(1.0, 0.0, 0.0);
        glRotatef((GLfloat) elbow, 0.0, 0.0, 1.0);
        glTranslatef(1.0, 0.0, 0.0);
        glPushMatrix();
            glScalef(2.0, 0.4, 1.0);
            glutWireCube(1.0);
        glPopMatrix();

        // --- TELAPAK TANGAN ---
        glTranslatef(1.0, 0.0, 0.0);
        glRotatef((GLfloat) hand, 0.0, 0.0, 1.0);
        glTranslatef(0.25, 0.0, 0.0);             
        glPushMatrix();
            glScalef(0.5, 0.6, 1.0);              
            glutWireCube(1.0);
        glPopMatrix();

        float ujungTelapak = 0.25; 
        gambarJari(ujungTelapak + 0.2,  0.20, 0.0); // Jari 1
        gambarJari(ujungTelapak + 0.2,  0.10, 0.0); // Jari 2
        gambarJari(ujungTelapak + 0.2,  0.00, 0.0); // Jari 3
        gambarJari(ujungTelapak + 0.2, -0.10, 0.0); // Jari 4
        gambarJari(ujungTelapak + 0.2, -0.20, 0.0); // Jari 5

    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, (GLfloat) w / (GLfloat) h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 's': shoulder = (shoulder + 5) % 360; glutPostRedisplay(); break;
        case 'S': shoulder = (shoulder - 5) % 360; glutPostRedisplay(); break;
        case 'e': elbow = (elbow + 5) % 360; glutPostRedisplay(); break;
        case 'E': elbow = (elbow - 5) % 360; glutPostRedisplay(); break;
        case 'f': hand = (hand + 5) % 360; glutPostRedisplay(); break;
        case 'F': hand = (hand - 5) % 360; glutPostRedisplay(); break;
        case 27: exit(0); break;
        default: break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Lengan Robot dengan 5 Jari");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
