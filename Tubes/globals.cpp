#include "game.h"

GameState currentState = MENU;

float shipX = -10.0f;
float shipY = 0.0f;
float speed = 15.0f;
int playerHP = 5;
int score = 0;

bool isDashing = false;
int dashTimer = 0;
int dashCooldown = 0;
float dashDirX = 1.0f;
float dashDirY = 0.0f;

int autoShootTimer = 0;
int currentShootDelay = 12;
int currentBranches = 1;
Bullet bullets[MAX_BULLETS];
EnemyBullet enemyBullets[MAX_ENEMY_BULLETS];
float bulletSpeed = 35.0f;
PowerUp powerups[MAX_POWERUPS];

Enemy enemies[MAX_ENEMIES];
Meteor meteors[MAX_METEORS];
ExplosionParticle explosions[MAX_EXPLOSIONS];
Boss currentBoss = {0, 0, 0, 0, 0, 0, 0, 0, 0, false, 0.0f, 0.0f, false, false, 0.0f};

int waveTimer = 0;
int waveInterval = 300;
int currentWave = 1;
int bossesDefeated = 0;
int waveMessageTimer = 0;

float camX = 0.0f;
float camY = 2.0f;
float camZ = 25.0f;
float targetX = 0.0f;
float targetY = 0.0f;
float targetZ = 0.0f;
int shakeTimer = 0;

GLuint bgTexture = 0;
GLuint meteorTex = 0;
GLUquadric *quadObj = NULL;

float limitX = 13.0f;
float limitY = 9.0f;

float deltaTime = 0.0f;
int lastFrameTime = 0;