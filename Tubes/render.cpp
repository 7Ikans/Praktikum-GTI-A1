#include "game.h"

GLuint loadBMP_custom(const char *imagepath)
{
    FILE *file = fopen(imagepath, "rb");
    if (!file)
        return 0;
    unsigned char header[54];
    if (fread(header, 1, 54, file) != 54 || header[0] != 'B' || header[1] != 'M')
        return 0;
    unsigned int dataPos = *(int *)&(header[0x0A]);
    unsigned int imageSize = *(int *)&(header[0x22]);
    unsigned int width = *(int *)&(header[0x12]);
    unsigned int height = *(int *)&(header[0x16]);
    if (imageSize == 0)
        imageSize = width * height * 3;
    if (dataPos == 0)
        dataPos = 54;
    unsigned char *data = new unsigned char[imageSize];
    fread(data, 1, imageSize, file);
    fclose(file);
    for (unsigned int i = 0; i < imageSize; i += 3)
    {
        unsigned char tmp = data[i];
        data[i] = data[i + 2];
        data[i + 2] = tmp;
    }
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    delete[] data;
    return textureID;
}

void drawText(float x, float y, const char *string)
{
    glRasterPos2f(x, y);
    for (const char *c = string; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

void drawTextCentered(float y, const char *string)
{
    int textWidth = 0;
    for (const char *c = string; *c != '\0'; c++)
        textWidth += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, *c);
    float x = -((float)textWidth / glutGet(GLUT_WINDOW_WIDTH)) * 13.0f;
    glRasterPos2f(x, y);
    for (const char *c = string; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

void drawDropShadow(float x, float y, float radius)
{
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
    glPushMatrix();
    glTranslatef(x, -9.5f, 0.0f);
    glScalef(1.0f, 0.01f, 0.5f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10)
    {
        float rad = i * 3.14159265f / 180.0f;
        glVertex3f(cos(rad) * radius, sin(rad) * radius, 0.0f);
    }
    glEnd();
    glPopMatrix();
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

void drawPlayerShip()
{
    drawDropShadow(shipX, shipY, 3.0f);

    glPushMatrix();
    glTranslatef(shipX, shipY, 0.0f);
    glRotatef(15.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(15.0f, 0.0f, 1.0f, 0.0f);

    if (isDashing)
        glColor3f(1.0f, 1.0f, 1.0f);
    else
        glColor3f(0.0f, 0.5f, 0.8f);

    glPushMatrix();
    glScalef(2.5f, 0.6f, 0.8f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glColor3f(0.0f, 0.8f, 1.0f);
    glPushMatrix();
    glTranslatef(1.25f, 0.0f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glutSolidCone(0.4f, 1.0f, 16, 16);
    glPopMatrix();

    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(-0.2f, 0.0f, 0.0f);
    glScalef(1.0f, 0.1f, 2.5f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glColor3f(0.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.2f, 0.3f, 0.0f);
    glScalef(1.2f, 0.8f, 0.8f);
    glutSolidSphere(0.4f, 16, 16);
    glPopMatrix();

    glPopMatrix();
}

void drawEnemyShip(float ex, float ey)
{
    drawDropShadow(ex, ey, 2.0f);

    glPushMatrix();
    glTranslatef(ex, ey, 0.0f);
    glRotatef(25.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(glutGet(GLUT_ELAPSED_TIME) * 0.2f, 0.0f, 1.0f, 0.0f);

    glColor3f(0.6f, 0.6f, 0.6f);
    glPushMatrix();
    glScalef(1.8f, 0.3f, 1.8f);
    glutSolidSphere(0.5f, 20, 20);
    glPopMatrix();

    glColor3f(0.2f, 0.9f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, 0.15f, 0.0f);
    glScalef(1.0f, 0.8f, 1.0f);
    glutSolidSphere(0.35f, 16, 16);
    glPopMatrix();

    glColor3f(1.0f, 0.2f, 0.2f);
    glPushMatrix();
    glScalef(1.0f, 0.2f, 1.0f);
    glutSolidTorus(0.08f, 0.85f, 16, 32);
    glPopMatrix();

    glPopMatrix();
}

void drawBoss()
{
    if (!currentBoss.active)
        return;
    drawDropShadow(currentBoss.x, currentBoss.y, 6.0f);

    glPushMatrix();
    glTranslatef(currentBoss.x, currentBoss.y, 0.0f);
    if (currentBoss.isDying)
        glRotatef(currentBoss.rot, 0.0f, 0.0f, 1.0f);
    else
        glRotatef(glutGet(GLUT_ELAPSED_TIME) * 0.1f, 0.0f, 1.0f, 0.0f);

    if (currentBoss.type == 1)
    {
        glColor3f(0.7f, 0.7f, 0.8f);
        glPushMatrix();
        glScalef(4.0f, 1.0f, 4.0f);
        glutSolidSphere(1.0f, 32, 32);
        glPopMatrix();

        glColor3f(0.0f, 1.0f, 1.0f);
        glutSolidTorus(0.5f, 4.0f, 16, 32);
    }
    else if (currentBoss.type == 2)
    {
        glColor3f(0.2f, 0.8f, 0.2f);
        glutSolidSphere(2.5f, 32, 32);

        glColor3f(0.8f, 0.1f, 0.1f);
        for (int i = 0; i < 360; i += 45)
        {
            glPushMatrix();
            glRotatef(i, 0.0f, 0.0f, 1.0f);
            glTranslatef(0.0f, 2.5f, 0.0f);
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
            glutSolidCone(0.5f, 2.0f, 16, 16);
            glPopMatrix();
        }
    }
    else if (currentBoss.type == 3)
    {
        if (currentBoss.isInvulnerable && !currentBoss.isDying)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(1.0f, 1.0f, 0.0f, 0.4f);
            glutSolidSphere(4.5f, 32, 32);
            glDisable(GL_BLEND);
        }

        glColor3f(0.1f, 0.1f, 0.1f);
        glutSolidSphere(3.0f, 32, 32);

        glColor3f(1.0f, 0.0f, 0.0f);
        for (int i = 0; i < 360; i += 30)
        {
            for (int j = -45; j <= 45; j += 45)
            {
                glPushMatrix();
                glRotatef(i, 0.0f, 1.0f, 0.0f);
                glRotatef(j, 1.0f, 0.0f, 0.0f);
                glTranslatef(0.0f, 0.0f, 3.0f);
                glutSolidCone(0.8f, 3.5f, 8, 8);
                glPopMatrix();
            }
        }
    }
    glPopMatrix();
}

void drawMeteor(float mx, float my, float rot)
{
    drawDropShadow(mx, my, 2.5f);
    glPushMatrix();
    glTranslatef(mx, my, 0.0f);
    glRotatef(rot, 1.0f, 1.0f, 1.0f);
    glScalef(1.8f, 1.4f, 1.6f);
    glColor3f(1.0f, 1.0f, 1.0f);

    if (meteorTex)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, meteorTex);
    }

    if (quadObj)
    {
        gluSphere(quadObj, 1.5f, 8, 8);
    }
    else
    {
        glutSolidSphere(1.5f, 8, 8);
    }

    if (meteorTex)
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void drawBullets()
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].active)
        {
            float angle = atan2(bullets[i].vy, bulletSpeed) * 180.0f / 3.14159265f;
            glPushMatrix();
            glTranslatef(bullets[i].x, bullets[i].y, 0.0f);
            glRotatef(angle, 0.0f, 0.0f, 1.0f);

            glColor3f(0.8f, 1.0f, 1.0f);
            glPushMatrix();
            glScalef(2.5f, 0.5f, 0.5f);
            glutSolidSphere(0.25f, 10, 10);
            glPopMatrix();

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glDisable(GL_LIGHTING);
            glColor4f(0.0f, 0.8f, 1.0f, 0.6f);
            glPushMatrix();
            glScalef(4.0f, 1.0f, 1.0f);
            glutSolidSphere(0.25f, 10, 10);
            glPopMatrix();
            glEnable(GL_LIGHTING);
            glDisable(GL_BLEND);

            glPopMatrix();
        }
    }
}

void drawEnemyBullets()
{
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++)
    {
        if (enemyBullets[i].active)
        {
            float angle = atan2(enemyBullets[i].vy, enemyBullets[i].vx) * 180.0f / 3.14159265f;
            glPushMatrix();
            glTranslatef(enemyBullets[i].x, enemyBullets[i].y, 0.0f);
            glRotatef(angle, 0.0f, 0.0f, 1.0f);

            glColor3f(1.0f, 0.8f, 1.0f);
            glPushMatrix();
            glScalef(2.5f, 0.5f, 0.5f);
            glutSolidSphere(0.25f, 10, 10);
            glPopMatrix();

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glDisable(GL_LIGHTING);
            glColor4f(1.0f, 0.0f, 0.8f, 0.6f);
            glPushMatrix();
            glScalef(4.0f, 1.0f, 1.0f);
            glutSolidSphere(0.25f, 10, 10);
            glPopMatrix();
            glEnable(GL_LIGHTING);
            glDisable(GL_BLEND);

            glPopMatrix();
        }
    }
}

void drawExplosions()
{
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (explosions[i].active)
        {
            glColor4f(1.0f, 0.5f, 0.0f, explosions[i].life);
            glPushMatrix();
            glTranslatef(explosions[i].x, explosions[i].y, 0.0f);
            glutSolidCube(0.4f * explosions[i].life);
            glPopMatrix();
        }
    }

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

void drawPowerUps()
{
    for (int i = 0; i < MAX_POWERUPS; i++)
    {
        if (powerups[i].active)
        {
            glPushMatrix();
            glTranslatef(powerups[i].x, powerups[i].y, 0.0f);
            glRotatef(glutGet(GLUT_ELAPSED_TIME) * 0.1f, 1, 1, 0);

            if (powerups[i].type == 0)
                glColor3f(0.0f, 1.0f, 0.0f);
            else if (powerups[i].type == 1)
                glColor3f(1.0f, 0.0f, 1.0f);
            else if (powerups[i].type == 2)
                glColor3f(1.0f, 0.0f, 0.0f);

            glutSolidCube(0.7f);
            glPopMatrix();
        }
    }
}

void drawParallaxBackground3D()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    if (bgTexture)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, bgTexture);
        glColor3f(1.0f, 1.0f, 1.0f);

        float offsetU = camX * 0.003f;
        float offsetV = camY * 0.003f;

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f + offsetU, 0.0f + offsetV);
        glVertex2f(0.0f, 0.0f);
        glTexCoord2f(1.0f + offsetU, 0.0f + offsetV);
        glVertex2f(1.0f, 0.0f);
        glTexCoord2f(1.0f + offsetU, 1.0f + offsetV);
        glVertex2f(1.0f, 1.0f);
        glTexCoord2f(0.0f + offsetU, 1.0f + offsetV);
        glVertex2f(0.0f, 1.0f);
        glEnd();

        glDisable(GL_TEXTURE_2D);
    }

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void drawMenuScreen()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-13.0, 13.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(-13.0f, -10.0f);
    glVertex2f(13.0f, -10.0f);
    glVertex2f(13.0f, 10.0f);
    glVertex2f(-13.0f, 10.0f);
    glEnd();
    glDisable(GL_BLEND);

    glColor3f(0.0f, 1.0f, 1.0f);
    drawTextCentered(3.0f, "B E A V E R");

    glColor3f(0.6f, 0.9f, 1.0f);
    drawTextCentered(0.0f, "DO YOU WANT TO START ?");

    glColor3f(1.0f, 1.0f, 1.0f);
    drawTextCentered(-3.0f, "> PRESS [ENTER] FOR YES      [ESC] FOR NO");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void drawGameOverScreen()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-13.0, 13.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(-13.0f, -10.0f);
    glVertex2f(13.0f, -10.0f);
    glVertex2f(13.0f, 10.0f);
    glVertex2f(-13.0f, 10.0f);
    glEnd();
    glDisable(GL_BLEND);

    glColor3f(0.2f, 0.8f, 1.0f);
    drawTextCentered(2.0f, "G A M E   O V E R");

    char scoreStr[64];
    sprintf(scoreStr, "FINAL SCORE: %d", score);
    drawTextCentered(-1.0f, scoreStr);

    glColor3f(0.6f, 0.9f, 1.0f);
    drawTextCentered(-4.0f, "DO YOU WANT TO CONTINUE ?");

    glColor3f(1.0f, 1.0f, 1.0f);
    drawTextCentered(-6.0f, "> PRESS [R] FOR YES      [ESC] FOR NO");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void drawVictoryScreen()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-13.0, 13.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(-13.0f, -10.0f);
    glVertex2f(13.0f, -10.0f);
    glVertex2f(13.0f, 10.0f);
    glVertex2f(-13.0f, 10.0f);
    glEnd();
    glDisable(GL_BLEND);

    glColor3f(0.0f, 1.0f, 0.0f);
    drawTextCentered(2.0f, "VICTORY! MARS SECURED!");
    char scoreStr[64];
    sprintf(scoreStr, "Final Score: %d", score);
    glColor3f(1.0f, 1.0f, 0.0f);
    drawTextCentered(0.0f, scoreStr);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawTextCentered(-2.0f, "Press [R] to Return to Base");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void drawUI()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-13.0, 13.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    char scoreStr[64];
    sprintf(scoreStr, "SCORE: %d   WAVE: %d", score, currentWave);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(6.0f, 9.0f, scoreStr);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(-12.5f, 9.1f, "LIVES: ");
    for (int i = 0; i < playerHP; i++)
    {
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

    if (dashCooldown == 0)
    {
        glColor3f(0.0f, 1.0f, 1.0f);
        drawText(-12.5f, 8.3f, "DASH: READY (SPACE)");
    }
    else
    {
        glColor3f(0.5f, 0.5f, 0.5f);
        drawText(-12.5f, 8.3f, "DASH: COOLDOWN");
    }

    if (currentBoss.active && !currentBoss.isDying)
    {
        if (currentBoss.isInvulnerable)
            glColor3f(1.0f, 1.0f, 0.0f);
        else
            glColor3f(1.0f, 0.0f, 0.0f);

        glBegin(GL_QUADS);
        glVertex2f(-5.0f, 8.5f);
        glVertex2f(-5.0f + (10.0f * (currentBoss.hp / currentBoss.maxHp)), 8.5f);
        glVertex2f(-5.0f + (10.0f * (currentBoss.hp / currentBoss.maxHp)), 8.8f);
        glVertex2f(-5.0f, 8.8f);
        glEnd();
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(-5.0f, 8.5f);
        glVertex2f(5.0f, 8.5f);
        glVertex2f(5.0f, 8.8f);
        glVertex2f(-5.0f, 8.8f);
        glEnd();
    }

    if (waveMessageTimer > 0 && !currentBoss.isDying)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
        glBegin(GL_QUADS);
        glVertex2f(-13.0f, -1.5f);
        glVertex2f(13.0f, -1.5f);
        glVertex2f(13.0f, 1.5f);
        glVertex2f(-13.0f, 1.5f);
        glEnd();

        glColor4f(0.0f, 1.0f, 1.0f, 0.8f);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        glVertex2f(-13.0f, 1.5f);
        glVertex2f(13.0f, 1.5f);
        glVertex2f(-13.0f, -1.5f);
        glVertex2f(13.0f, -1.5f);
        glEnd();
        glLineWidth(1.0f);
        glDisable(GL_BLEND);

        char waveMsg[64];
        if (currentWave % 5 == 0 && currentWave <= 15)
        {
            glColor3f(1.0f, 0.0f, 0.0f);
            sprintf(waveMsg, "W A R N I N G :  B O S S   A P P R O A C H I N G");
            drawTextCentered(0.2f, waveMsg);
            if ((waveMessageTimer / 10) % 2 == 0)
            {
                glColor3f(1.0f, 1.0f, 0.0f);
                drawTextCentered(-0.8f, "PREPARE FOR BATTLE");
            }
        }
        else
        {
            glColor3f(0.0f, 1.0f, 1.0f);
            sprintf(waveMsg, "W A V E   %d", currentWave);
            drawTextCentered(0.2f, waveMsg);
            glColor3f(1.0f, 1.0f, 1.0f);
            drawTextCentered(-0.8f, "GET READY");
        }
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float finalCamX = camX;
    float finalCamY = camY;
    if (shakeTimer > 0 && currentState == PLAYING)
    {
        finalCamX += ((rand() % 100) / 100.0f - 0.5f) * 1.5f;
        finalCamY += ((rand() % 100) / 100.0f - 0.5f) * 1.5f;
    }

    gluLookAt(finalCamX, finalCamY, camZ, targetX, targetY, targetZ, 0.0f, 1.0f, 0.0f);
    drawParallaxBackground3D();

    if (currentState == MENU)
        drawMenuScreen();
    else if (currentState == GAME_OVER)
        drawGameOverScreen();
    else if (currentState == VICTORY)
        drawVictoryScreen();
    else if (currentState == PLAYING)
    {
        drawExplosions();
        drawPlayerShip();
        drawBoss();
        for (int i = 0; i < MAX_METEORS; i++)
            if (meteors[i].active)
                drawMeteor(meteors[i].x, meteors[i].y, meteors[i].rot);
        for (int i = 0; i < MAX_ENEMIES; i++)
            if (enemies[i].active)
                drawEnemyShip(enemies[i].x, enemies[i].y);
        drawPowerUps();
        drawBullets();
        drawEnemyBullets();
        drawUI();
    }

    glutSwapBuffers();
}