#include "game.h"

void spawnEnemiesWave()
{
    int enemiesToSpawn = 2 + (currentWave / 2) + (bossesDefeated * 2);
    if (enemiesToSpawn > 15)
        enemiesToSpawn = 15;

    for (int i = 0; i < enemiesToSpawn; i++)
    {
        for (int j = 0; j < MAX_ENEMIES; j++)
        {
            if (!enemies[j].active)
            {
                enemies[j].active = true;
                enemies[j].x = 14.0f + (rand() % 5);
                enemies[j].baseY = -7.0f + (rand() % 14);
                enemies[j].y = enemies[j].baseY;
                enemies[j].speed = 2.0f + (rand() % 3) + (bossesDefeated * 1.5f);
                enemies[j].time = rand() % 10;
                break;
            }
        }
    }
}

void spawnBoss()
{
    currentBoss.active = true;
    currentBoss.isDying = false;
    currentBoss.x = 15.0f;
    currentBoss.y = 0.0f;
    currentBoss.time = 0.0f;
    currentBoss.rot = 0.0f;
    currentBoss.healTimer = 0.0f;
    currentBoss.invulnTimer = 0.0f;
    currentBoss.shootTimer = 0.0f;
    currentBoss.isInvulnerable = false;

    if (currentWave == 5)
    {
        currentBoss.type = 1;
        currentBoss.maxHp = 40;
        currentBoss.speed = 1.5f;
    }
    else if (currentWave == 10)
    {
        currentBoss.type = 2;
        currentBoss.maxHp = 90;
        currentBoss.speed = 2.5f;
    }
    else if (currentWave == 15)
    {
        currentBoss.type = 3;
        currentBoss.maxHp = 150;
        currentBoss.speed = 3.5f;
    }
    currentBoss.hp = currentBoss.maxHp;
}

void spawnMeteor()
{
    for (int i = 0; i < MAX_METEORS; i++)
    {
        if (!meteors[i].active)
        {
            meteors[i].active = true;
            int side = rand() % 2;
            if (side == 0)
            {
                meteors[i].x = -15.0f - (rand() % 5);
                meteors[i].speedX = 15.0f + (rand() % 10);
            }
            else
            {
                meteors[i].x = 15.0f + (rand() % 5);
                meteors[i].speedX = -(15.0f + (rand() % 10));
            }
            meteors[i].y = 12.0f + (rand() % 5);
            meteors[i].speedY = -(10.0f + (rand() % 10));
            meteors[i].rot = 0.0f;
            break;
        }
    }
}

void spawnExplosion(float x, float y, int count, float speedMod, float r, float g, float b)
{
    int spawned = 0;
    for (int i = 0; i < MAX_EXPLOSIONS && spawned < count; i++)
    {
        if (!explosions[i].active)
        {
            explosions[i].active = true;
            explosions[i].x = x;
            explosions[i].y = y;
            float angle = (rand() % 360) * 3.14159f / 180.0f;
            float spd = (1.0f + (rand() % 10) * 0.5f) * speedMod;
            explosions[i].vx = cos(angle) * spd;
            explosions[i].vy = sin(angle) * spd;
            explosions[i].life = 1.0f;
            spawned++;
        }
    }
}

void fireAutoBullets()
{
    int spawned = 0;
    for (int i = 0; i < MAX_BULLETS && spawned < currentBranches; i++)
    {
        if (!bullets[i].active)
        {
            bullets[i].active = true;
            bullets[i].x = shipX + 2.0f;
            bullets[i].y = shipY;

            if (currentBranches == 1)
                bullets[i].vy = 0.0f;
            else if (currentBranches == 2)
                bullets[i].vy = (spawned == 0) ? 5.0f : -5.0f;
            else if (currentBranches == 3)
                bullets[i].vy = (spawned == 0) ? 0.0f : ((spawned == 1) ? 8.0f : -8.0f);
            else if (currentBranches == 4)
            {
                if (spawned == 0)
                    bullets[i].vy = 10.0f;
                else if (spawned == 1)
                    bullets[i].vy = 4.0f;
                else if (spawned == 2)
                    bullets[i].vy = -4.0f;
                else if (spawned == 3)
                    bullets[i].vy = -10.0f;
            }
            spawned++;
        }
    }
}

void fireEnemyBullet(float x, float y, float vx, float vy)
{
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++)
    {
        if (!enemyBullets[i].active)
        {
            enemyBullets[i].active = true;
            enemyBullets[i].x = x;
            enemyBullets[i].y = y;
            enemyBullets[i].vx = vx;
            enemyBullets[i].vy = vy;
            break;
        }
    }
}

void fireBossBullets()
{
    if (currentBoss.type == 1)
    {
        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            if (!enemies[i].active)
            {
                enemies[i].active = true;
                enemies[i].x = currentBoss.x - 2.0f;
                enemies[i].y = currentBoss.y;
                enemies[i].speed = 12.0f;
                enemies[i].baseY = currentBoss.y;
                enemies[i].time = 0;
                break;
            }
        }
    }
    else if (currentBoss.type == 2)
    {
        float offsets[] = {0.0f, 3.0f, -3.0f};
        for (int j = 0; j < 3; j++)
        {
            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (!enemies[i].active)
                {
                    enemies[i].active = true;
                    enemies[i].x = currentBoss.x - 2.0f;
                    enemies[i].y = currentBoss.y + offsets[j];
                    enemies[i].speed = 10.0f;
                    enemies[i].baseY = currentBoss.y + offsets[j];
                    enemies[i].time = 0;
                    break;
                }
            }
        }
    }
    else if (currentBoss.type == 3)
    {
        for (int j = -2; j <= 2; j++)
        {
            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (!enemies[i].active)
                {
                    enemies[i].active = true;
                    enemies[i].x = currentBoss.x - 2.0f;
                    enemies[i].y = currentBoss.y + (j * 2.5f);
                    enemies[i].speed = 18.0f;
                    enemies[i].baseY = currentBoss.y + (j * 2.5f);
                    enemies[i].time = 0;
                    break;
                }
            }
        }
        for (int j = -4; j <= 4; j += 2)
        {
            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (!enemies[i].active)
                {
                    enemies[i].active = true;
                    enemies[i].x = currentBoss.x;
                    enemies[i].y = currentBoss.y + j;
                    enemies[i].speed = 6.0f;
                    enemies[i].baseY = currentBoss.y + j;
                    enemies[i].time = rand() % 10;
                    break;
                }
            }
        }
    }
}

void resetGame()
{
    shipX = -10.0f;
    shipY = 0.0f;
    playerHP = 5;
    score = 0;
    currentWave = 1;
    waveTimer = 0;
    bossesDefeated = 0;
    currentShootDelay = 12;
    currentBranches = 1;
    isDashing = false;
    dashTimer = 0;
    dashCooldown = 0;
    shakeTimer = 0;
    waveMessageTimer = 120;
    currentBoss.active = false;
    currentBoss.isDying = false;

    for (int i = 0; i < MAX_BULLETS; i++)
        bullets[i].active = false;
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++)
        enemyBullets[i].active = false;
    for (int i = 0; i < MAX_ENEMIES; i++)
        enemies[i].active = false;
    for (int i = 0; i < MAX_POWERUPS; i++)
        powerups[i].active = false;
    for (int i = 0; i < MAX_METEORS; i++)
        meteors[i].active = false;
    for (int i = 0; i < MAX_EXPLOSIONS; i++)
        explosions[i].active = false;

    lastFrameTime = glutGet(GLUT_ELAPSED_TIME);
}

void updateTimer(int value)
{
    int currentFrameTime = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = (currentFrameTime - lastFrameTime) / 1000.0f;
    lastFrameTime = currentFrameTime;

    if (deltaTime > 0.05f)
        deltaTime = 0.05f;

    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (explosions[i].active)
        {
            explosions[i].x += explosions[i].vx * deltaTime;
            explosions[i].y += explosions[i].vy * deltaTime;
            explosions[i].life -= 1.5f * deltaTime;
            if (explosions[i].life <= 0.0f)
                explosions[i].active = false;
        }
    }

    if (currentState == PLAYING)
    {
        camX = shipX * 0.15f;
        camY = 2.0f + (shipY * 0.1f);
        targetX = shipX * 0.05f;

        if (shakeTimer > 0)
            shakeTimer--;
        if (waveMessageTimer > 0)
            waveMessageTimer--;

        if (playerHP <= 0)
            currentState = GAME_OVER;
        else if (bossesDefeated >= 3)
            currentState = VICTORY;
        else
        {
            if (isDashing)
            {
                shipX += dashDirX * (speed * 3.0f * deltaTime);
                shipY += dashDirY * (speed * 3.0f * deltaTime);

                if (shipX > limitX)
                    shipX = limitX;
                if (shipX < -limitX)
                    shipX = -limitX;
                if (shipY > limitY)
                    shipY = limitY;
                if (shipY < -limitY)
                    shipY = -limitY;

                dashTimer--;
                if (dashTimer <= 0)
                    isDashing = false;
            }
            if (dashCooldown > 0)
                dashCooldown--;

            if (rand() % 500 < 2 + bossesDefeated)
                spawnMeteor();

            for (int i = 0; i < MAX_METEORS; i++)
            {
                if (meteors[i].active)
                {
                    meteors[i].x += meteors[i].speedX * deltaTime;
                    meteors[i].y += meteors[i].speedY * deltaTime;
                    meteors[i].rot += 100.0f * deltaTime;

                    if (meteors[i].y < -10.0f || meteors[i].x < -20.0f || meteors[i].x > 20.0f)
                    {
                        if (meteors[i].y < -10.0f)
                            spawnExplosion(meteors[i].x, -9.5f, 15, 8.0f, 0.8f, 0.4f, 0.1f);
                        meteors[i].active = false;
                    }

                    if (!isDashing && fabs(shipX - meteors[i].x) < 2.0f && fabs(shipY - meteors[i].y) < 2.0f)
                    {
                        spawnExplosion(meteors[i].x, meteors[i].y, 10, 5.0f, 0.8f, 0.4f, 0.1f);
                        meteors[i].active = false;
                        playerHP -= 3;
                        shakeTimer = 50;
                    }
                }
            }

            if (!currentBoss.active)
            {
                waveTimer++;
                if (waveTimer >= waveInterval)
                {
                    if (currentWave % 5 == 0 && currentWave <= 15)
                    {
                        spawnBoss();
                        waveMessageTimer = 180;
                    }
                    else
                    {
                        spawnEnemiesWave();
                        currentWave++;
                        waveMessageTimer = 90;
                    }
                    waveTimer = 0;
                }
            }
            else
            {
                if (currentBoss.isDying)
                {
                    currentBoss.y -= 10.0f * deltaTime;
                    currentBoss.rot += 180.0f * deltaTime;
                    if (currentBoss.y < -11.0f)
                    {
                        spawnExplosion(currentBoss.x, -9.5f, 40, 15.0f, 1.0f, 0.5f, 0.0f);
                        currentBoss.active = false;
                        bossesDefeated++;
                        currentWave++;
                        waveMessageTimer = 120;
                        shakeTimer = 60;
                    }
                }
                else
                {
                    currentBoss.time += deltaTime;
                    currentBoss.y = sin(currentBoss.time * currentBoss.speed) * 6.0f;
                    if (currentBoss.x > 8.0f)
                        currentBoss.x -= 2.0f * deltaTime;

                    if (currentBoss.type == 2)
                    {
                        currentBoss.healTimer += deltaTime;
                        if (currentBoss.healTimer >= 5.0f)
                        {
                            currentBoss.hp += 5;
                            if (currentBoss.hp > currentBoss.maxHp)
                                currentBoss.hp = currentBoss.maxHp;
                            currentBoss.healTimer = 0.0f;
                        }
                    }
                    else if (currentBoss.type == 3)
                    {
                        currentBoss.healTimer += deltaTime;
                        if (currentBoss.healTimer >= 3.0f)
                        {
                            currentBoss.hp += 15;
                            if (currentBoss.hp > currentBoss.maxHp)
                                currentBoss.hp = currentBoss.maxHp;
                            currentBoss.healTimer = 0.0f;
                        }

                        currentBoss.invulnTimer += deltaTime;
                        if (currentBoss.isInvulnerable)
                        {
                            if (currentBoss.invulnTimer >= 3.0f)
                            {
                                currentBoss.isInvulnerable = false;
                                currentBoss.invulnTimer = 0.0f;
                            }
                        }
                        else
                        {
                            if (currentBoss.invulnTimer >= 5.0f)
                            {
                                currentBoss.isInvulnerable = true;
                                currentBoss.invulnTimer = 0.0f;
                            }
                        }
                    }

                    if (currentBoss.type >= 2)
                    {
                        currentBoss.shootTimer += deltaTime;
                        if (currentBoss.shootTimer > 1.0f)
                        {
                            fireEnemyBullet(currentBoss.x - 2.0f, currentBoss.y, -35.0f, 0.0f);
                            if (currentBoss.type == 3)
                            {
                                fireEnemyBullet(currentBoss.x - 2.0f, currentBoss.y + 2.0f, -35.0f, 0.0f);
                                fireEnemyBullet(currentBoss.x - 2.0f, currentBoss.y - 2.0f, -35.0f, 0.0f);
                            }
                            currentBoss.shootTimer = 0.0f;
                        }
                    }

                    currentBoss.attackTimer += deltaTime;
                    if (currentBoss.attackTimer > 2.0f - (bossesDefeated * 0.3f))
                    {
                        fireBossBullets();
                        currentBoss.attackTimer = 0.0f;
                    }

                    if (!isDashing && fabs(shipX - currentBoss.x) < 4.0f && fabs(shipY - currentBoss.y) < 4.0f)
                    {
                        playerHP -= 2;
                        shakeTimer = 40;
                        shipX -= 3.0f;
                    }
                }
            }

            autoShootTimer++;
            if (autoShootTimer >= currentShootDelay)
            {
                fireAutoBullets();
                autoShootTimer = 0;
            }

            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullets[i].active)
                {
                    bullets[i].x += bulletSpeed * deltaTime;
                    bullets[i].y += bullets[i].vy * deltaTime;
                    if (bullets[i].x > 15.0f || bullets[i].y > 10.0f || bullets[i].y < -10.0f)
                    {
                        bullets[i].active = false;
                    }

                    if (currentBoss.active && !currentBoss.isDying && fabs(bullets[i].x - currentBoss.x) < 3.0f && fabs(bullets[i].y - currentBoss.y) < 3.0f)
                    {
                        bullets[i].active = false;
                        if (!currentBoss.isInvulnerable)
                        {
                            currentBoss.hp -= 1;
                            score += 5;
                            if (currentBoss.hp <= 0)
                            {
                                currentBoss.isDying = true;
                                score += 1000 * currentBoss.type;
                            }
                        }
                    }
                }
            }

            for (int i = 0; i < MAX_ENEMY_BULLETS; i++)
            {
                if (enemyBullets[i].active)
                {
                    enemyBullets[i].x += enemyBullets[i].vx * deltaTime;
                    enemyBullets[i].y += enemyBullets[i].vy * deltaTime;
                    if (enemyBullets[i].x < -15.0f || enemyBullets[i].x > 15.0f || enemyBullets[i].y > 10.0f || enemyBullets[i].y < -10.0f)
                    {
                        enemyBullets[i].active = false;
                    }

                    if (!isDashing && fabs(shipX - enemyBullets[i].x) < 1.5f && fabs(shipY - enemyBullets[i].y) < 1.5f)
                    {
                        enemyBullets[i].active = false;
                        playerHP -= 1;
                        shakeTimer = 20;
                    }
                }
            }

            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (enemies[i].active)
                {
                    enemies[i].x -= enemies[i].speed * deltaTime;
                    enemies[i].time += deltaTime * 5.0f;
                    enemies[i].y = enemies[i].baseY + 3.0f * sin(enemies[i].time);

                    if (rand() % 1000 < 5)
                    {
                        fireEnemyBullet(enemies[i].x, enemies[i].y, -30.0f, 0.0f);
                    }

                    if (enemies[i].x < -15.0f)
                        enemies[i].active = false;

                    if (!isDashing && fabs(shipX - enemies[i].x) < 2.5f && fabs(shipY - enemies[i].y) < 2.0f)
                    {
                        spawnExplosion(enemies[i].x, enemies[i].y, 5, 3.0f, 0.6f, 0.6f, 0.6f);
                        enemies[i].active = false;
                        playerHP -= 1;
                        shakeTimer = 35;
                    }

                    for (int b = 0; b < MAX_BULLETS; b++)
                    {
                        if (bullets[b].active && fabs(bullets[b].x - enemies[i].x) < 2.0f && fabs(bullets[b].y - enemies[i].y) < 1.5f)
                        {
                            spawnExplosion(enemies[i].x, enemies[i].y, 8, 4.0f, 0.8f, 0.8f, 0.8f);
                            enemies[i].active = false;
                            bullets[b].active = false;
                            score += 10;

                            if (rand() % 100 < 10)
                            {
                                for (int p = 0; p < MAX_POWERUPS; p++)
                                {
                                    if (!powerups[p].active)
                                    {
                                        powerups[p].active = true;
                                        powerups[p].x = enemies[i].x;
                                        powerups[p].y = enemies[i].y;
                                        powerups[p].type = (rand() % 100 < 20) ? 2 : rand() % 2;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            for (int i = 0; i < MAX_POWERUPS; i++)
            {
                if (powerups[i].active)
                {
                    powerups[i].x -= 5.0f * deltaTime;
                    if (powerups[i].x < -15.0f)
                        powerups[i].active = false;

                    if (fabs(shipX - powerups[i].x) < 2.5f && fabs(shipY - powerups[i].y) < 2.0f)
                    {
                        powerups[i].active = false;
                        if (powerups[i].type == 0)
                        {
                            if (currentShootDelay > 8)
                                currentShootDelay -= 2;
                        }
                        else if (powerups[i].type == 1)
                        {
                            if (currentBranches < 4)
                                currentBranches++;
                        }
                        else if (powerups[i].type == 2)
                        {
                            if (playerHP < 10)
                                playerHP++;
                        }
                    }
                }
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, updateTimer, 0);
}