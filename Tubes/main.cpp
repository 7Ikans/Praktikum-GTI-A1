#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

// --- Player Setup ---
float shipX = -10.0f;
float shipY = 0.0f; 
float speed = 0.5f;
int playerHP = 5; 
int score = 0;

// --- Dash Setup ---
bool isDashing = false;
int dashTimer = 0;
int dashCooldown = 0;
float dashDirX = 1.0f; 
float dashDirY = 0.0f;

// --- Auto-attack & Power-up Setup ---
int autoShootTimer = 0;
int currentShootDelay = 30; 
int currentBranches = 1;    

#define MAX_BULLETS 100 
struct Bullet {
    float x, y, vy;
    bool active; 
};
Bullet bullets[MAX_BULLETS]; 
float bulletSpeed = 0.6f;

#define MAX_POWERUPS 10
struct PowerUp {
    float x, y;
    int type; // 0 = Speed, 1 = Branch, 2 = Heal
    bool active;
};
PowerUp powerups[MAX_POWERUPS];

// --- Enemy Setup ---
#define MAX_ENEMIES 50
struct Enemy {
    float x, y, baseY;
    float speed;
    float time; 
    bool active;
};
Enemy enemies[MAX_ENEMIES];

// --- Wave Setup ---
int waveTimer = 0;
int waveInterval = 150; 
int currentWave = 1;    

// --- Parallax Background Setup ---
#define MAX_STARS 100
struct Star {
    float x, y, z, speed;
};
Star stars[MAX_STARS];

// Batas layar 
float limitX = 13.0f; 
float limitY = 9.0f;

// --- Utility: Fungsi Text ---
void drawText(float x, float y, const char* string) {
    glRasterPos2f(x, y);
    for (const char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

// --- Player & Enemies Drawing ---
void drawPlayerShip() {
    glPushMatrix();
    glTranslatef(shipX, shipY, 0.0f);
    
    glRotatef(15.0f, 1.0f, 0.0f, 0.0f); 
    glRotatef(15.0f, 0.0f, 1.0f, 0.0f);

    // Saat Dashing, ubah material body utama menjadi putih terang
    if (isDashing) glColor3f(1.0f, 1.0f, 1.0f);
    else glColor3f(0.0f, 0.5f, 0.8f);

    // Body Utama
    glPushMatrix();
    glScalef(2.5f, 0.6f, 0.8f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Moncong
    glColor3f(0.0f, 0.8f, 1.0f);
    glPushMatrix();
    glTranslatef(1.25f, 0.0f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glutSolidCone(0.4f, 1.0f, 16, 16);
    glPopMatrix();

    // Sayap
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(-0.2f, 0.0f, 0.0f);
    glScalef(1.0f, 0.1f, 2.5f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Kokpit
    glColor3f(0.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.2f, 0.3f, 0.0f);
    glScalef(1.2f, 0.8f, 0.8f);
    glutSolidSphere(0.4f, 16, 16);
    glPopMatrix();

    glPopMatrix();
}

void drawEnemyShip(float ex, float ey) {
    glPushMatrix();
    glTranslatef(ex, ey, 0.0f);
    glRotatef(25.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(glutGet(GLUT_ELAPSED_TIME) * 0.2f, 0.0f, 1.0f, 0.0f);

    // Cakram Utama
    glColor3f(0.6f, 0.6f, 0.6f); 
    glPushMatrix();
    glScalef(1.8f, 0.3f, 1.8f); 
    glutSolidSphere(0.5f, 20, 20);
    glPopMatrix();

    // Kubah
    glColor3f(0.2f, 0.9f, 0.2f); 
    glPushMatrix();
    glTranslatef(0.0f, 0.15f, 0.0f);
    glScalef(1.0f, 0.8f, 1.0f); 
    glutSolidSphere(0.35f, 16, 16);
    glPopMatrix();

    // Sabuk Torus
    glColor3f(1.0f, 0.2f, 0.2f); 
    glPushMatrix();
    glScalef(1.0f, 0.2f, 1.0f);
    glutSolidTorus(0.08f, 0.85f, 16, 32);
    glPopMatrix();

    glPopMatrix();
}

void drawBullets() {
    glColor3f(1.0f, 1.0f, 0.0f); 
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            glPushMatrix();
            glTranslatef(bullets[i].x, bullets[i].y, 0.0f);
            glutSolidSphere(0.15f, 10, 10);
            glPopMatrix();
        }
    }
}

void drawPowerUps() {
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (powerups[i].active) {
            glPushMatrix();
            glTranslatef(powerups[i].x, powerups[i].y, 0.0f);
            glRotatef(glutGet(GLUT_ELAPSED_TIME) * 0.1f, 1, 1, 0); 
            
            if (powerups[i].type == 0) glColor3f(0.0f, 1.0f, 0.0f); 
            else if (powerups[i].type == 1) glColor3f(1.0f, 0.0f, 1.0f); 
            else if (powerups[i].type == 2) glColor3f(1.0f, 0.0f, 0.0f); 

            glutSolidCube(0.7f); 
            glPopMatrix();
        }
    }
}

void drawParallaxBackground3D() {
    glColor3f(0.8f, 0.8f, 0.8f);
    for (int i = 0; i < MAX_STARS; i++) {
        glPushMatrix();
        glTranslatef(stars[i].x, stars[i].y, stars[i].z);
        glutSolidSphere(0.08f, 5, 5);
        glPopMatrix();
    }
}

// --- Fungsi Overlay UI 2D ---
void drawUI() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-13.0, 13.0, -10.0, 10.0); 
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // Score & Wave
    char scoreStr[64];
    sprintf(scoreStr, "SCORE: %d   WAVE: %d", score, currentWave);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(6.0f, 9.0f, scoreStr);

    // UI LIVES
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(-12.5f, 9.1f, "LIVES: ");
    for (int i = 0; i < playerHP; i++) {
        glPushMatrix();
        glTranslatef(-10.0f + (i * 0.8f), 9.3f, 0.0f); 
        glColor3f(1.0f, 0.0f, 0.0f); 
        glBegin(GL_POLYGON);
        glVertex2f(0.0f, -0.2f);
        glVertex2f(0.2f, 0.0f);
        glVertex2f(0.1f, 0.2f);
        glVertex2f(-0.1f, 0.2f);
        glVertex2f(-0.2f, 0.0f);
        glEnd();
        glPopMatrix();
    }

    // UI DASH COOLDOWN
    if (dashCooldown == 0) {
        glColor3f(0.0f, 1.0f, 1.0f);
        drawText(-12.5f, 8.3f, "DASH: READY (SPACE)");
    } else {
        glColor3f(0.5f, 0.5f, 0.5f);
        drawText(-12.5f, 8.3f, "DASH: COOLDOWN");
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0f, 2.0f, 25.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);   

    drawParallaxBackground3D();
    drawPlayerShip();
    
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) drawEnemyShip(enemies[i].x, enemies[i].y);
    }

    drawPowerUps();
    drawBullets(); 
    drawUI(); 

    glutSwapBuffers();
}

// --- Logic & Update ---
void spawnEnemiesWave() {
    int enemiesToSpawn = 1 + (currentWave / 2); 
    if (enemiesToSpawn > 10) enemiesToSpawn = 10; 

    for (int i = 0; i < enemiesToSpawn; i++) {
        for (int j = 0; j < MAX_ENEMIES; j++) {
            if (!enemies[j].active) {
                enemies[j].active = true;
                enemies[j].x = 14.0f + (rand() % 5); 
                enemies[j].baseY = -7.0f + (rand() % 14); 
                enemies[j].y = enemies[j].baseY;
                enemies[j].speed = 0.05f + (rand() % 3) * 0.01f + (currentWave * 0.005f);
                if (enemies[j].speed > 0.15f) enemies[j].speed = 0.15f; 
                enemies[j].time = rand() % 10; 
                break; 
            }
        }
    }
    currentWave++; 
}

void fireAutoBullets() {
    int spawned = 0;
    for (int i = 0; i < MAX_BULLETS && spawned < currentBranches; i++) {
        if (!bullets[i].active) {
            bullets[i].active = true;
            bullets[i].x = shipX + 2.0f; 
            bullets[i].y = shipY;
            
            if (currentBranches == 1) {
                bullets[i].vy = 0.0f;
            } else if (currentBranches == 2) {
                bullets[i].vy = (spawned == 0) ? 0.05f : -0.05f;
            } else if (currentBranches == 3) {
                bullets[i].vy = (spawned == 0) ? 0.0f : ((spawned == 1) ? 0.08f : -0.08f);
            } else if (currentBranches == 4) {
                if (spawned == 0) bullets[i].vy = 0.1f;
                else if (spawned == 1) bullets[i].vy = 0.04f;
                else if (spawned == 2) bullets[i].vy = -0.04f;
                else if (spawned == 3) bullets[i].vy = -0.1f;
            }
            spawned++;
        }
    }
}

void updateTimer(int value) {
    if (playerHP <= 0) {
        glutPostRedisplay();
        glutTimerFunc(16, updateTimer, 0);
        return; 
    }

    // Update Dash Logic
    if (isDashing) {
        // REVISI: Kecepatan dash tetap, namun durasinya (dashTimer) dipangkas di fungsi keyboard
        shipX += dashDirX * (speed * 1.5f);
        shipY += dashDirY * (speed * 1.5f);

        // Batasan area saat dash
        if (shipX > limitX) shipX = limitX;
        if (shipX < -limitX) shipX = -limitX;
        if (shipY > limitY) shipY = limitY;
        if (shipY < -limitY) shipY = -limitY;

        dashTimer--;
        if (dashTimer <= 0) isDashing = false; // Dash selesai
    }
    if (dashCooldown > 0) dashCooldown--;

    // Update Parallax
    for (int i = 0; i < MAX_STARS; i++) {
        stars[i].x -= stars[i].speed;
        if (stars[i].x < -15.0f) {
            stars[i].x = 15.0f;
            stars[i].y = -10.0f + (rand() % 20);
        }
    }

    // Update Wave
    waveTimer++;
    if (waveTimer >= waveInterval) {
        spawnEnemiesWave();
        waveTimer = 0;
    }

    // Update Auto Attack
    autoShootTimer++;
    if (autoShootTimer >= currentShootDelay) {
        fireAutoBullets();
        autoShootTimer = 0;
    }

    // Update Bullets
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].x += bulletSpeed; 
            bullets[i].y += bullets[i].vy;
            if (bullets[i].x > 15.0f || bullets[i].y > 10.0f || bullets[i].y < -10.0f) {
                bullets[i].active = false; 
            }
        }
    }

    // Update Enemies
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            enemies[i].x -= enemies[i].speed;
            enemies[i].time += 0.05f;
            enemies[i].y = enemies[i].baseY + 3.0f * sin(enemies[i].time);

            if (enemies[i].x < -15.0f) enemies[i].active = false;

            // Hit by Player Body (Pemain kebal jika isDashing == true)
            if (!isDashing && fabs(shipX - enemies[i].x) < 2.5f && fabs(shipY - enemies[i].y) < 2.0f) {
                enemies[i].active = false;
                playerHP -= 1; 
            }

            // Hit by Bullets 
            for (int b = 0; b < MAX_BULLETS; b++) {
                if (bullets[b].active && fabs(bullets[b].x - enemies[i].x) < 2.0f && fabs(bullets[b].y - enemies[i].y) < 1.5f) {
                    enemies[i].active = false;
                    bullets[b].active = false;
                    score += 10;

                    int dropRoll = rand() % 100;
                    if (dropRoll < 10) { 
                        for (int p = 0; p < MAX_POWERUPS; p++) {
                            if (!powerups[p].active) {
                                powerups[p].active = true;
                                powerups[p].x = enemies[i].x;
                                powerups[p].y = enemies[i].y;
                                if (dropRoll < 5) powerups[p].type = 2; 
                                else powerups[p].type = rand() % 2;     
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    // Update PowerUps
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (powerups[i].active) {
            powerups[i].x -= 0.08f; 
            if (powerups[i].x < -15.0f) powerups[i].active = false;

            if (fabs(shipX - powerups[i].x) < 2.5f && fabs(shipY - powerups[i].y) < 2.0f) {
                powerups[i].active = false;
                if (powerups[i].type == 0) { if (currentShootDelay > 10) currentShootDelay -= 2; } 
                else if (powerups[i].type == 1) { if (currentBranches < 4) currentBranches++; }
                else if (powerups[i].type == 2) { if (playerHP < 5) playerHP++; }
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, updateTimer, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (isDashing) return; // Kunci kontrol input manual saat sedang melesat (Dash)

    switch (key) {
        case 'w': case 'W': if (shipY < limitY) shipY += speed; dashDirX = 0.0f; dashDirY = 1.0f; break;
        case 's': case 'S': if (shipY > -limitY) shipY -= speed; dashDirX = 0.0f; dashDirY = -1.0f; break;
        case 'a': case 'A': if (shipX > -limitX) shipX -= speed; dashDirX = -1.0f; dashDirY = 0.0f; break;
        case 'd': case 'D': if (shipX < limitX) shipX += speed; dashDirX = 1.0f; dashDirY = 0.0f; break;
        case ' ': // Eksekusi Dash
            if (dashCooldown == 0) {
                isDashing = true;
                dashTimer = 5;       // REVISI: Sebelumnya 10, sekarang 5 agar jaraknya separuh lebih pendek
                dashCooldown = 90;   // Cooldown tetap sama
            }
            break;
        case 27: exit(0); break;
    }
    glutPostRedisplay();
}

void reshape(int w, int h) {
    if (h == 0) h = 1; 
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f); 
    glEnable(GL_DEPTH_TEST); 
    
    glEnable(GL_LIGHTING);   
    glEnable(GL_LIGHT0);     
    glEnable(GL_COLOR_MATERIAL); 
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_NORMALIZE); 

    GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_position[] = { 5.0f, 10.0f, 15.0f, 1.0f }; 

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    for (int i = 0; i < MAX_STARS; i++) {
        stars[i].x = -15.0f + (rand() % 30);
        stars[i].y = -10.0f + (rand() % 20);
        stars[i].z = -5.0f - (rand() % 20); 
        stars[i].speed = 0.05f + (rand() % 10) * 0.01f;
    }

    for (int i = 0; i < MAX_BULLETS; i++) bullets[i].active = false;
    for (int i = 0; i < MAX_ENEMIES; i++) enemies[i].active = false;
    for (int i = 0; i < MAX_POWERUPS; i++) powerups[i].active = false;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600); 
    glutCreateWindow("Progres Game Platypus - Revisi Jarak Dash");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape); 
    glutKeyboardFunc(keyboard);
    
    glutTimerFunc(16, updateTimer, 0); 

    glutMainLoop();
    return 0;
}
