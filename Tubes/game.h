#ifndef GAME_H
#define GAME_H

#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

enum GameState
{
    MENU,
    PLAYING,
    GAME_OVER,
    VICTORY
};

extern GameState currentState;

#define MAX_BULLETS 100
#define MAX_ENEMY_BULLETS 100
#define MAX_POWERUPS 10
#define MAX_ENEMIES 50
#define MAX_METEORS 5
#define MAX_EXPLOSIONS 200

struct Bullet
{
    float x, y, vy;
    bool active;
};
struct EnemyBullet
{
    float x, y, vx, vy;
    bool active;
};
struct PowerUp
{
    float x, y;
    int type;
    bool active;
};
struct Enemy
{
    float x, y, baseY, speed, time;
    bool active;
};
struct Meteor
{
    float x, y, speedX, speedY, rot;
    bool active;
};
struct ExplosionParticle
{
    float x, y, vx, vy, life;
    bool active;
};
struct Boss
{
    float x, y, hp, maxHp, speed, attackTimer, shootTimer, time;
    int type;
    bool active;
    float healTimer;
    float invulnTimer;
    bool isInvulnerable;
    bool isDying;
    float rot;
};

extern float shipX, shipY, speed;
extern int playerHP, score;
extern bool isDashing;
extern int dashTimer, dashCooldown;
extern float dashDirX, dashDirY;

extern int autoShootTimer, currentShootDelay, currentBranches;
extern Bullet bullets[MAX_BULLETS];
extern EnemyBullet enemyBullets[MAX_ENEMY_BULLETS];
extern float bulletSpeed;

extern PowerUp powerups[MAX_POWERUPS];
extern Enemy enemies[MAX_ENEMIES];
extern Meteor meteors[MAX_METEORS];
extern ExplosionParticle explosions[MAX_EXPLOSIONS];
extern Boss currentBoss;

extern int waveTimer, waveInterval, currentWave, bossesDefeated, waveMessageTimer;
extern float limitX, limitY;

extern float camX, camY, camZ, targetX, targetY, targetZ;
extern int shakeTimer;

extern float deltaTime;
extern int lastFrameTime;

extern GLuint bgTexture;
extern GLuint meteorTex;
extern GLUquadric *quadObj;

GLuint loadBMP_custom(const char *imagepath);
void drawText(float x, float y, const char *string);
void drawTextCentered(float y, const char *string);
void drawDropShadow(float x, float y, float radius);
void drawPlayerShip();
void drawEnemyShip(float ex, float ey);
void drawBoss();
void drawMeteor(float mx, float my, float rot);
void drawBullets();
void drawEnemyBullets();
void drawExplosions();
void drawPowerUps();
void drawParallaxBackground3D();
void drawMenuBorder();
void drawUI();
void drawMenuScreen();
void drawGameOverScreen();
void drawVictoryScreen();
void display();

void spawnEnemiesWave();
void spawnBoss();
void spawnMeteor();
void spawnExplosion(float x, float y, int count, float speedMod, float r, float g, float b);
void fireAutoBullets();
void fireEnemyBullet(float x, float y, float vx, float vy);
void fireBossBullets();
void resetGame();
void updateTimer(int value);

#endif