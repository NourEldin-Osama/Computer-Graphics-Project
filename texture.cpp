#include <Windows.h>
#include <iostream>
#pragma comment(lib, "Winmm.lib")
#include <mmsystem.h>


#include "TextureBuilder.h"
#include <glut.h>
#include <iostream>


using namespace std;


// Global variables

int player_x = 0;
int enemy_x = 0;
int direction = 0;
int steps = 0;
int n_steps = 0;  // the number of steps the enemy moved
GLuint texID;  // For Texture
int rep=1;  // number of repets of Texture
constexpr auto GLUT_KEY_ESCAPE = 27;  // For Esc Key


// Game Setting

// 40 represent maximum number of steps before stop or change direction,
// Decrease the number if you want the enemy to change direction faster or stop more, default: 40
int n_steps_befroe_stop = 400;
int distance_enemy_move = 5;  // distance that the enemy move in one step, default: 50
int distance_player_move = 50;  // distance that the player move in one step, default: 50
int random_position = 800;  // the position of the firsr Powerup, default: 800
int n_steps_before_Powerup = 200;  // the number of steps before generate Powerup, default: 200
int start_position_powerup = 780;  // the start position of powerup in Y axis, default: 780
int enemyHealth = 100;  // this represent the percent of Enemy Health and it it Decreases when the Enemy is hit, default: 100
int stars_speed = 1;  // speed of rotate of stars "Background"
int level = 1;  // the level of the game
bool move_with_mouse = FALSE;

int y_powerup = 0;
bool show_powerup = FALSE;  // if True then it will Generate Powerup, default: FALSE
int powerup_type = 1;  // change powerup color 
bool enemy4_d = FALSE;

int Damage = 10;

int Bullet_x = 0;
int Bullet_y = 0;
int BulletSpeed = 40;
bool BulletFlag = FALSE;

int ObstacleSpeed = 4;
int Obstacle_x = 0;
int Obstacle_y = 0;
bool ObstacleFlag = FALSE;
bool Player_Shooted = FALSE;
bool Win = FALSE;
bool Game_Over = FALSE;

bool EnemyDefenderFlag = FALSE;
int EnemyDefender_x = 0;
bool EnemyDefender_direction = TRUE;
int EnemyDefenderSpeed = 20;

int BulletEnemyDefender_x = 0;
int BulletEnemyDefender_y = 0;
bool BulletEnemyDefender = FALSE;
int BulletEnemyDefenderSpeed = 4;


GLuint Texture_Win;  // For "WIN" Texture
GLuint Texture_Lose;  // For "Game Over" Texture
GLuint Texture_Background1;  // For "Background" Texture
GLuint Texture_Background2;  // For "Background" Texture
GLuint Texture_Background3;  // For "Background" Texture
//GLuint Texture_Win_1;  // For "WIN" Texture

// Simplify drawing

// For Drawing Quads
void drawQuad(float x, float y, float w, float h) {
	glBegin(GL_QUADS);
		glVertex2d(x, y);
		glVertex2d(x + w, y);
		glVertex2d(x + w, y + h);
		glVertex2d(x, y + h);
	glEnd();
}

// For Drawing Quads Rotated with theta
void drawQuadRotatable(float x, float y, float w, float h, int theta) {
	glPushMatrix();
		glTranslated((2 * x + w) / 2, (2 * y + h) / 2, 0);
		glRotated(theta, 0, 0, 1);
		glTranslated(-(2 * x + w) / 2, -(2 * y + h) / 2, 0);
		glBegin(GL_QUADS);
			glVertex2d(x, y);
			glVertex2d(x + w, y);
			glVertex2d(x + w, y + h);
			glVertex2d(x, y + h);
		glEnd();
	glPopMatrix();
}

// For Drawing Circles
void drawCircle(int x_axis, int y_axis, int inner_r, int outter_r, int start_theta, int all_theta) {
	glPushMatrix();
	glTranslatef(x_axis, y_axis, 0);
	GLUquadric* quadobj1 = gluNewQuadric();
	gluPartialDisk(quadobj1, inner_r, outter_r, 1000, 1000, start_theta, all_theta);
	glPopMatrix();
}

// For Drawing Circles with Border
void drawCircBord(int x_axis, int y_axis) {
	glColor3f(1, 1, 0);
	glPushMatrix();
	glTranslatef(x_axis, y_axis, 0);
	GLUquadric* quadobj1 = gluNewQuadric();
	gluPartialDisk(quadobj1, 0, 10, 1000, 1000, -90, 360);
	GLUquadric* quadobj2 = gluNewQuadric();
	glColor3f(0, 0, 0);
	gluPartialDisk(quadobj2, 10, 12, 1000, 1000, -90, 360);
	glPopMatrix();
}

// For Drawing Strings and Numbers on Screen
void printStringToScreen(int x, int y, char* string) {
    int len, i;
    //set the position of the text in the window using the x and y coordinates
    glRasterPos2f(x, y);
    //get the length of the string to display
    len = (int)strlen(string);
    //loop to display character by character
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
}



void WIN(bool var) {
    if (var) {
        glColor3d(1, 1, 1);
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, Texture_Win);
            glBegin(GL_QUADS);
                glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
                glTexCoord2f(1, 0); glVertex3f(1600, 0, 0);
                glTexCoord2f(1, 1); glVertex3f(1600, 800, 0);
                glTexCoord2f(0, 1); glVertex3f(0, 800, 0);
            glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
}

void GameOver(bool var) {
    if (var) {
        glColor3d(1, 1, 1);
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, Texture_Lose);
            glBegin(GL_QUADS);
                glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
                glTexCoord2f(1, 0); glVertex3f(1600, 0, 0);
                glTexCoord2f(1, 1); glVertex3f(1600, 800, 0);
                glTexCoord2f(0, 1); glVertex3f(0, 800, 0);
            glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
}

void drawExplosion(int x, int y, double s, int theta, bool Shooted) {
    if (Shooted) {
        glPushMatrix();
            glTranslated(x + 315, y - 60, 0);
            glPushMatrix();
                glTranslated(471, 260, 0);
                glScaled(s, s, 1);
                glRotated(theta, 0, 0, 1);
                glTranslated(-471, -260, 0);

                glColor3d(0.98, 0.92, 0.6);
                glBegin(GL_POLYGON);
                    glVertex2d(480, 450);
                    glVertex2d(660, 300);
                    glVertex2d(545, 330);
                    glVertex2d(565, 290);
                    glVertex2d(530, 305);
                    glVertex2d(560, 95);
                    glVertex2d(495, 205);
                    glVertex2d(485, 125);
                    glVertex2d(460, 215);
                    glVertex2d(425, 65);
                    glVertex2d(400, 240);
                    glVertex2d(325, 180);
                    glVertex2d(345, 335);
                    glVertex2d(285, 340);
                    glVertex2d(405, 450);
                glEnd();

                glBegin(GL_POLYGON);
                    glColor3d(0.96, 0.56, 0.06);
                    glVertex2d(475, 450);
                    glColor3d(0.95, 0.21, 0.07);
                    glVertex2d(570, 340);
                    glVertex2d(500, 370);
                    glVertex2d(510, 270);
                    glVertex2d(465, 350);
                    glVertex2d(445, 220);
                    glVertex2d(430, 355);
                    glVertex2d(380, 285);
                    glVertex2d(405, 400);
                    glVertex2d(335, 365);
                    glVertex2d(410, 450);
                glEnd();
            glPopMatrix();
        glPopMatrix();
    }
}

void drawHealthBar(int health) {
    if (health>100) {
        health = 100;
    }
    if (health<0) {
        health = 0;
    }
    glColor3d(0, 0, 0);
    drawQuad(295, 760, 1010, 30);
    glColor3d(1, 0, 0);
    drawQuad(300, 765, 10 * health, 20);
}

void drawPowerup(int x, int y, int c) {
    if (c) {
        glColor3f(1, 0, 0);
    }
    else {
        glColor3f(0, 0, 1);
    }
    glPushMatrix();
    glTranslated(x-200, y-550, 0);
        glPushMatrix();
        glTranslated(218, 600, 0);
        glScaled(0.5, 0.5, 1);
        glTranslated(-218, -600, 0);
            drawCircle(200, 600, 0, 20, -90, 180);
            drawCircle(236, 600, 0, 20, -90, 180);
            glBegin(GL_TRIANGLES);
            glVertex2d(180, 600);
            glVertex2d(256, 600);
            glVertex2d(218, 550);
            glEnd();
        glPopMatrix();
    glPopMatrix();
}

void generatePowerup() {
    if (show_powerup) {
        drawPowerup(random_position, y_powerup, powerup_type);  // 780 - 0
        y_powerup -= 10;
    }
    // -80 if you want to cross the bottom line
    if (y_powerup <= 20) {
        show_powerup = FALSE;
    }
    if (n_steps % n_steps_before_Powerup == 0) {
        random_position = (rand() % 1550 + 1);  // random position will show in (x axis)
        show_powerup = TRUE;
        y_powerup = start_position_powerup;
        if (level != 4 || enemy4_d) {
            powerup_type = 1;
        }
        else {
            powerup_type = 0;
        }
        //powerup_type = (rand() % 2);  // determine which type to generate
    }
}

void drawObstacle(int x, int y, double s, int theta) {
    glPushMatrix();
        glTranslated(x + 402, y + 220, 0);
        glPushMatrix();
            // Center 397, 335
            glTranslated(397, 335, 0);
            glScaled(s, s, 1);
            glRotated(theta, 0, 0, 1);
            glTranslated(-397, -335, 0);
            glColor3f(0.71f, 0.7f, 0.7f);
            glBegin(GL_POLYGON);
                glVertex2d(320, 300);
                glVertex2d(400, 300);
                glVertex2d(500, 335);
                glVertex2d(400, 370);
                glVertex2d(320, 370);
            glEnd();
            glColor3f(0.69f, 0.06f, 0.07f);
            drawQuad(295, 300, 25, 70);

            glColor3f(0, 0, 0);
            glLineWidth(3);
            glBegin(GL_LINE_LOOP);
                glVertex2d(295, 300);
                glVertex2d(320, 300);
                glVertex2d(400, 300);
                glVertex2d(500, 335);
                glVertex2d(400, 370);
                glVertex2d(320, 370);
                glVertex2d(295, 370);
            glEnd();

            glBegin(GL_LINE_LOOP);
                glVertex2d(320, 300);
                glVertex2d(400, 300);
                glVertex2d(400, 370);
                glVertex2d(320, 370);
            glEnd();

        glPopMatrix();
    glPopMatrix();
}

void checkObstacleCollision() {
    if (ObstacleFlag) {
        Obstacle_y -= ObstacleSpeed;  // 2 will be replaced
        if (Obstacle_y < -450) {
            ObstacleFlag = FALSE;
            Obstacle_y = 0;
        }
    }
    // Upper line 300
    if (Obstacle_y < -320 && (player_x - 135<Obstacle_x)&&(Obstacle_x < player_x + 135)) {
        Player_Shooted = TRUE;
        PlaySound(TEXT("sound/explosion-hit.wav"), NULL, SND_ASYNC | SND_FILENAME);
    }
}

void drawBullet(int x, int y, double s, int theta) {
    glPushMatrix();
        glTranslated(x + 730, y - 100, 0);
        glPushMatrix();
            // Center 70,300
            glTranslated(70, 300, 0);
            glScaled(s, s, 1);
            glRotated(theta, 0, 0, 1);
            glTranslated(-70, -300, 0);

            glColor3f(0.73f, 0.69f, 0.68f);
            glBegin(GL_POLYGON);
                glVertex2d(50, 420);
                glVertex2d(50, 180);
                glVertex2d(90, 180);
                glVertex2d(90, 420);
            glEnd();

            glColor3f(0.95f, 0.4f, 0);
            glLineWidth(3);
            glBegin(GL_LINE_LOOP);
                glVertex2d(50, 420);
                glVertex2d(50, 180);
                glVertex2d(90, 180);
                glVertex2d(90, 420);
            glEnd();
        glPopMatrix();
    glPopMatrix();
}

void checkBulletCollision() {
    if (BulletFlag) {
        Bullet_y += BulletSpeed;  // 10 will be replaced
        if (Bullet_y>500) {
            BulletFlag = FALSE;
            Bullet_y = 0;
        }
    }
    if (Bullet_y > 320 && (enemy_x - 275 < Bullet_x) && (Bullet_x < enemy_x + 275)) {
        enemyHealth -= Damage;  // 1 will be replaced
        BulletFlag = FALSE;
        Bullet_y = 0;
        PlaySound(TEXT("sound/explosion-hit.wav"), NULL, SND_ASYNC | SND_FILENAME);
    }
    if (EnemyDefenderFlag&&Bullet_y > 120 && (EnemyDefender_x - 200 < Bullet_x) && (Bullet_x < EnemyDefender_x + 200)) {
        BulletFlag = FALSE;
        Bullet_y = 0;
    }
}

void checkEnemyDefenderBulletCollision() {
    if (BulletEnemyDefender) {
        BulletEnemyDefender_y -= BulletEnemyDefenderSpeed;  // 2 will be replaced
        if (BulletEnemyDefender_y < -200) {
            BulletEnemyDefender = FALSE;
            BulletEnemyDefender_y = 0;
        }
    }
    // Upper line 300
    if (BulletEnemyDefender_y < -150 && (player_x - 135 < BulletEnemyDefender_x) && (BulletEnemyDefender_x < player_x + 135)) {
        Player_Shooted = TRUE;
    }
}

void increasedifficulty() {
    Damage -= 5;
    BulletEnemyDefenderSpeed += 2;
    ObstacleSpeed += 2;
    stars_speed += 1;
    distance_enemy_move += 5;
    EnemyDefenderSpeed += 5;
}

void checkEnemyHealth() {
    if (enemyHealth <= 0) {
        level++;
        increasedifficulty();
        enemyHealth = 100;
    }
    if (level == 5) {
        Win = TRUE;
        PlaySound(TEXT("sound/win.wav"), NULL, SND_ASYNC | SND_FILENAME);
    }
}

void enemyRandomMoving() {
    if (steps == 0) {
        direction = (rand() % 3);  // right or left or in the same position
        steps = (rand() % n_steps_befroe_stop + 1);  // 40 represent number of steps before stop
    }
    else if (steps > 0) {
        if (direction == 2) {
            enemy_x += distance_enemy_move;
        }
        else if (direction == 1) {
            enemy_x -= distance_enemy_move;
        }
        steps--;
    }

    if (enemy_x > 525) {
        direction = 1;
    }
    else if (enemy_x < -525) {
        direction = 2;
    }

    // to reduce stop time
    if (direction == 0 && steps > 4) {
        steps = 4;
    }

    n_steps++;
    //cout << direction << " , " << steps << " , " << enemy_x << endl;  // for testing
}

void checkPowerupCollision() {
    // Upper line 300
    if (show_powerup && y_powerup < 100 && (player_x - 135 < random_position-800) && (random_position-800 < player_x + 135)) {
        if (powerup_type) {
            Damage += 10;            
        }
        else {
            enemy4_d = TRUE;
        }
        show_powerup = FALSE;
        y_powerup = 0;
        PlaySound(TEXT("sound/powerup.wav"), NULL, SND_ASYNC | SND_FILENAME);
    }
}


// YD
void star(int x, int y, double s, int theta) {
    glPushMatrix();
        glTranslated(x-300, y-400, 0);
        glPushMatrix();
            glTranslated(308, 408, 0);
            glRotated(theta, 0, 0, 1);
            glScaled(s, s, 1);
            glTranslated(-308, -408, 0);
            drawQuad(300, 400, 15, 15);
            glBegin(GL_TRIANGLES);
                glVertex2d(300, 415);
                glVertex2d(315, 415);
                glVertex2d(307, 430);

                glVertex2d(315, 415);
                glVertex2d(315, 400);
                glVertex2d(330, 407);

                glVertex2d(300, 400);
                glVertex2d(315, 400);
                glVertex2d(307, 390);

                glVertex2d(300, 415);
                glVertex2d(300, 400);
                glVertex2d(285, 407);

                glVertex2d(300, 407);
                glVertex2d(307, 415);
                glVertex2d(275, 435);

                glVertex2d(307, 415);
                glVertex2d(315, 407);
                glVertex2d(340, 435);

                glVertex2d(315, 407);
                glVertex2d(307, 400);
                glVertex2d(340, 380);

                glVertex2d(307, 400);
                glVertex2d(300, 407);
                glVertex2d(275, 380);
            glEnd();
        glPopMatrix();
    glPopMatrix();
}
void enemy1(int x, int y) {
    glPushMatrix();
    glTranslated(x, y - 35, 0);
    glColor3f(0.06, 0.77, 0.8);
    drawQuad(715, 570, 170, 10); //1
    glColor3f(0.06, 0.77, 0.8);
    drawQuad(630, 650, 60, 10); //31
    glBegin(GL_QUADS); //bulletL
    glVertex2d(610, 665);
    glVertex2d(640, 665);
    glVertex2d(630, 635);
    glVertex2d(620, 635);
    glEnd();
    glBegin(GL_QUADS); //bulletR
    glVertex2d(960, 665);
    glVertex2d(990, 665);
    glVertex2d(980, 635);
    glVertex2d(970, 635);
    glEnd();
    glColor3f(0.46f, 0.14f, 0.36f);
    glBegin(GL_TRIANGLES); //5
    glVertex2d(660, 675);
    glVertex2d(685, 675);
    glVertex2d(685, 620);
    glEnd();
    glColor3f(0.06, 0.77, 0.8);
    drawQuad(910, 650, 60, 10); //30
    glColor3f(0.46f, 0.14f, 0.36f);
    glBegin(GL_TRIANGLES); //6
    glVertex2d(940, 675);
    glVertex2d(915, 675);
    glVertex2d(915, 620);
    glEnd();
    drawQuad(784, 675, 30, 13); //9
    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLES); //2
    glVertex2d(800, 550);
    glVertex2d(840, 600);
    glVertex2d(760, 600);
    glEnd();
    drawQuad(760, 600, 80, 40); //3
    drawQuad(685, 640, 230, 35); //4
    glColor3f(0.06, 0.77, 0.8);
    drawQuad(700, 647, 40, 20); //7
    drawQuad(860, 647, 40, 20); //8
    drawQuad(747, 652, 7, 7); //p
    drawQuad(767, 652, 7, 7); //p
    drawQuad(787, 652, 7, 7); //p
    drawQuad(807, 652, 7, 7); //p
    drawQuad(827, 652, 7, 7); //p
    drawQuad(847, 652, 7, 7); //p
    drawQuad(780, 600, 40, 40); //22
    drawQuad(700, 695, 10, 40); //14
    drawQuad(900, 695, 10, 40); //12
    drawQuad(600, 707, 20, 50); //15
    drawQuad(980, 707, 20, 50); //13
    glBegin(GL_TRIANGLES); //23
    glVertex2d(800, 575);
    glVertex2d(780, 600);
    glVertex2d(820, 600);
    glEnd();
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS); //11
    glVertex2d(798, 700);
    glVertex2d(798, 685);
    glVertex2d(600, 730);
    glVertex2d(600, 745);
    glEnd();
    glBegin(GL_QUADS); //10
    glVertex2d(798, 700);
    glVertex2d(798, 685);
    glVertex2d(1000, 730);
    glVertex2d(1000, 745);
    glEnd();
    drawQuad(580, 730, 20, 15); //18
    drawQuad(1000, 730, 20, 15); //16
    glColor3f(0.46f, 0.14f, 0.36f);
    drawQuad(540, 710, 40, 45); //19
    drawQuad(1020, 710, 40, 45); //17
    glBegin(GL_LINES);
    glVertex2d(560, 720); //L
    glVertex2d(560, 785);
    glVertex2d(575, 720);
    glVertex2d(530, 785);
    glVertex2d(565, 750);
    glVertex2d(590, 785);
    glVertex2d(1040, 720); //R
    glVertex2d(1040, 785);
    glVertex2d(1050, 720);
    glVertex2d(1010, 785);
    glVertex2d(1050, 755);
    glVertex2d(1070, 785);
    glEnd();
    glColor3f(1, 1, 1);
    drawCircle(625, 665, 0, 15, -90, 180);
    drawCircle(975, 665, 0, 15, -90, 180);
    glPopMatrix();
}
void enemy2(int x, int y) {
    glPushMatrix();
    glTranslated(x - 10, y - 35, 0);
    glColor3f(0.08f, 0.05f, 0.49f);
    drawQuad(774, 540, 70, 5); //1
    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLES); //2
    glVertex2d(810, 530);
    glVertex2d(820, 550);
    glVertex2d(800, 550);
    glEnd();
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS); //3
    glVertex2d(780, 550);
    glVertex2d(840, 550);
    glVertex2d(847, 570);
    glVertex2d(772, 570);
    glEnd();
    glColor3f(0.08f, 0.05f, 0.49f);
    drawQuad(660, 600, 300, 50); //4
    glColor3f(1, 1, 1);
    drawQuad(660, 630, 300, 3); //4+
    drawQuad(660, 615, 300, 3); //4+
    glBegin(GL_QUADS); //7
    glVertex2d(660, 600);
    glVertex2d(605, 600);
    glVertex2d(605, 630);
    glVertex2d(660, 650);
    glEnd();
    glBegin(GL_QUADS); //6
    glVertex2d(960, 600);
    glVertex2d(1020, 600);
    glVertex2d(1020, 630);
    glVertex2d(960, 650);
    glEnd();
    glColor3f(1, 1, 1);
    drawQuad(772, 570, 75, 100); //5 
    glBegin(GL_TRIANGLES); //8
    glVertex2d(772, 670);
    glVertex2d(847, 670);
    glVertex2d(810, 775);
    glEnd();
    glColor3f(0.08f, 0.05f, 0.49f);
    glBegin(GL_POLYGON); //10
    glVertex2d(810, 740);
    glVertex2d(820, 775);
    glVertex2d(840, 790);
    glVertex2d(890, 760);
    glVertex2d(890, 740);
    glEnd();
    glBegin(GL_POLYGON); //9
    glVertex2d(810, 740);
    glVertex2d(800, 775);
    glVertex2d(780, 790);
    glVertex2d(730, 760);
    glVertex2d(730, 740);
    glEnd();
    glColor3f(1, 1, 1);
    drawQuad(780, 745, 5, 40); //LL
    drawQuad(770, 745, 5, 30);
    drawQuad(760, 745, 5, 20);
    drawQuad(750, 745, 5, 13);
    drawQuad(740, 745, 5, 7);
    drawQuad(835, 745, 5, 40);//LR
    drawQuad(845, 745, 5, 30);
    drawQuad(855, 745, 5, 20);
    drawQuad(865, 745, 5, 13);
    drawQuad(875, 745, 5, 7);
    drawQuad(809, 770, 1, 24);//middle
    glColor3f(0.08f, 0.05f, 0.49f);
    drawQuad(800, 570, 20, 100); //11
    drawCircle(810, 700, 0, 22, -90, 360);//12
    drawCircle(635, 620, 0, 15, -90, 360);//R
    drawCircle(985, 620, 0, 15, -90, 360);//L
    glEnd();
    drawQuad(805, 550, 10, 5); //md
    drawQuad(793, 560, 10, 5); //md
    drawQuad(818, 560, 10, 5); //md
    drawQuad(785, 570, 5, 5); //p
    drawQuad(785, 580, 5, 5); //p
    drawQuad(785, 590, 5, 5); //p
    drawQuad(785, 600, 5, 5); //p
    drawQuad(785, 610, 5, 5); //p
    drawQuad(785, 620, 5, 5); //p
    drawQuad(785, 630, 5, 5); //p
    drawQuad(785, 640, 5, 5); //p
    drawQuad(785, 650, 5, 5); //p
    drawQuad(785, 660, 5, 5); //p
    drawQuad(830, 570, 5, 5); //p
    drawQuad(830, 580, 5, 5); //p
    drawQuad(830, 590, 5, 5); //p
    drawQuad(830, 600, 5, 5); //p
    drawQuad(830, 610, 5, 5); //p
    drawQuad(830, 620, 5, 5); //p
    drawQuad(830, 630, 5, 5); //p
    drawQuad(830, 640, 5, 5); //p
    drawQuad(830, 650, 5, 5); //p
    drawQuad(830, 660, 5, 5); //p
    drawQuad(830, 630, 5, 5); //p
    glPopMatrix();
}
void enemy3(int x, int y) {
    glPushMatrix();
    glTranslated(x - 15, y - 10, 0);
    glColor3f(0.58f, 0.88f, 0.95f);
    glBegin(GL_TRIANGLES); //1
    glVertex2d(814, 520);
    glVertex2d(750, 600);
    glVertex2d(875, 600);
    glEnd();
    drawQuad(750, 600, 125, 145); //2
    glColor3f(0.58f, 0.88f, 0.95f);
    glBegin(GL_QUADS); //3
    glVertex2d(750, 600);
    glVertex2d(750, 745);
    glVertex2d(630, 680);
    glVertex2d(620, 650);
    glEnd();
    glBegin(GL_QUADS); //4
    glVertex2d(875, 600);
    glVertex2d(875, 745);
    glVertex2d(995, 680);
    glVertex2d(1005, 650);
    glEnd();
    glColor3f(0.24f, 0.16f, 0.94f);
    glBegin(GL_QUADS); //3+
    glVertex2d(740, 620);
    glVertex2d(740, 715);
    glVertex2d(650, 675);
    glVertex2d(640, 655);
    glEnd();
    glBegin(GL_QUADS); //4+
    glVertex2d(885, 620);
    glVertex2d(885, 715);
    glVertex2d(975, 675);
    glVertex2d(985, 655);
    glEnd();
    glColor3f(0.58f, 0.88f, 0.95f);
    glBegin(GL_TRIANGLES); //5
    glVertex2d(740, 740);
    glVertex2d(785, 765);
    glVertex2d(814, 745);
    glEnd();
    glBegin(GL_TRIANGLES); //6
    glVertex2d(814, 745);
    glVertex2d(845, 765);
    glVertex2d(885, 740);
    glEnd();
    glColor3f(0.24f, 0.16f, 0.94f);
    glBegin(GL_QUADS); //L
    glVertex2d(770, 759);
    glVertex2d(785, 765);
    glVertex2d(785, 553);
    glVertex2d(770, 573);
    glEnd();
    glBegin(GL_QUADS); //R
    glVertex2d(843, 766);
    glVertex2d(858, 759);
    glVertex2d(858, 575);
    glVertex2d(843, 555);
    glEnd();
    glColor3f(0.24f, 0.16f, 0.94f);
    glBegin(GL_QUADS); //FisrtL
    glVertex2d(785, 595);
    glVertex2d(785, 570);
    glVertex2d(814, 538);
    glVertex2d(814, 562);
    glEnd();
    glBegin(GL_QUADS); //SecondR
    glVertex2d(843, 595);
    glVertex2d(843, 570);
    glVertex2d(814, 538);
    glVertex2d(814, 562);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2d(785, 635);
    glVertex2d(785, 610);
    glVertex2d(814, 578);
    glVertex2d(814, 602);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2d(843, 635);
    glVertex2d(843, 610);
    glVertex2d(814, 578);
    glVertex2d(814, 602);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2d(785, 675);
    glVertex2d(785, 650);
    glVertex2d(814, 618);
    glVertex2d(814, 642);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2d(843, 675);
    glVertex2d(843, 650);
    glVertex2d(814, 618);
    glVertex2d(814, 642);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2d(785, 715);
    glVertex2d(785, 690);
    glVertex2d(814, 658);
    glVertex2d(814, 682);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2d(843, 715);
    glVertex2d(843, 690);
    glVertex2d(814, 658);
    glVertex2d(814, 682);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2d(785, 755);
    glVertex2d(785, 730);
    glVertex2d(814, 698);
    glVertex2d(814, 722);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2d(843, 755);
    glVertex2d(843, 730);
    glVertex2d(814, 698);
    glVertex2d(814, 722);
    glEnd();
    glColor3f(0.97f, 0.04f, 0.32f);
    drawQuad(750, 598, 5, 149);
    drawQuad(870, 598, 5, 149);
    glPopMatrix();
}
void enemy_defender(int x, int y, bool var) {
    if (var) {
        glPushMatrix();
        glTranslated(x, y - 100, 0);
        //glColor3f(1, 1, 1);
        drawQuad(610, 490, 415, 25);
        drawCircle(815, 450, 0, 20, -90, 180);
        glColor3f(0, 0, 1);
        drawQuad(570, 500, 40, 5);
        drawQuad(1025, 500, 40, 5);
        drawQuad(813, 470, 5, 20);
        glBegin(GL_QUADS);
        glVertex2d(540, 480);
        glVertex2d(570, 480);
        glVertex2d(580, 510);
        glVertex2d(530, 510);
        glEnd();
        drawCircle(555, 510, 0, 25, -90, 180);
        glBegin(GL_QUADS);
        glVertex2d(1055, 480);
        glVertex2d(1085, 480);
        glVertex2d(1095, 510);
        glVertex2d(1045, 510);
        glEnd();
        drawCircle(1070, 510, 0, 25, -90, 180);
        glBegin(GL_QUADS);
        glVertex2d(795, 450);
        glVertex2d(835, 450);
        glVertex2d(825, 430);
        glVertex2d(804, 430);
        glEnd();
        glPopMatrix();
    }
}
void drawNY(int x, int y, float s, float c1, float c2, float c3, int theta) {
	glPushMatrix();
		glTranslated(x, y, 0);
		glPushMatrix();
			glColor3d(c1, c2, c3);
			glTranslated(162, 145, 0);
			glRotated(theta, 0, 0, 1);
			glScaled(s, s, 1);
			glTranslated(-162, -145, 0);

			glPushMatrix();
			glTranslated(-240, -220, 0);
				drawQuad(340, 320, 10, 60);
				drawQuadRotatable(338, 344, 80, 10, -40);
				drawQuadRotatable(400, 385, 65, 10, 30);
				drawQuadRotatable(350, 385, 65, 10, -30);
				drawQuad(401, 320, 10, 60);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}
void player(int x, int y) {
    glPushMatrix();
    glTranslated(x - 10, y - 15, 0);
    glColor3f(0.77f, 0.47f, 0.69f);
    glBegin(GL_TRIANGLES); //1
    glVertex2d(810, 205);
    glVertex2d(830, 160);
    glVertex2d(790, 160);
    glEnd();
    glColor3f(0.9f, 0.93f, 0.58f);
    glBegin(GL_QUADS); //2
    glVertex2d(790, 160);
    glVertex2d(830, 160);
    glVertex2d(835, 140);
    glVertex2d(785, 140);
    glEnd();
    drawQuad(785, 75, 50, 65); //3
    glBegin(GL_QUADS); //4
    glVertex2d(735, 115);
    glVertex2d(785, 140);
    glVertex2d(785, 105);
    glVertex2d(735, 105);
    glEnd();
    glBegin(GL_QUADS); //5
    glVertex2d(835, 140);
    glVertex2d(885, 115);
    glVertex2d(885, 105);
    glVertex2d(835, 105);
    glEnd();
    glColor3f(0.77f, 0.47f, 0.69f);
    glBegin(GL_TRIANGLES); //L
    glVertex2d(675, 115);
    glVertex2d(705, 115);
    glVertex2d(690, 150);
    glEnd();
    glBegin(GL_TRIANGLES); //R
    glVertex2d(915, 115);
    glVertex2d(945, 115);
    glVertex2d(930, 150);
    glEnd();
    glColor3f(0.9f, 0.93f, 0.58f);
    drawQuad(885, 105, 30, 10);
    drawQuad(700, 105, 30, 10);
    glColor3f(0.77f, 0.47f, 0.69f);
    drawQuad(730, 105, 5, 25); //6
    drawQuad(885, 105, 5, 25); //7
    drawQuad(915, 105, 30, 20);
    drawQuad(675, 105, 30, 20);
    drawQuad(875, 120, 5, 15); //R
    drawQuad(865, 125, 5, 15);
    drawQuad(855, 130, 5, 15);
    drawQuad(845, 135, 5, 15);
    drawQuad(835, 140, 5, 15);
    drawQuad(740, 120, 5, 15); //L
    drawQuad(750, 125, 5, 15);
    drawQuad(760, 130, 5, 15);
    drawQuad(770, 135, 5, 15);
    drawQuad(780, 140, 5, 15);
    drawQuad(785, 79, 50, 5); //inner
    drawQuad(785, 87, 50, 5); //inner
    glBegin(GL_TRIANGLES); //8
    glVertex2d(785, 95);
    glVertex2d(785, 75);
    glVertex2d(750, 75);
    glEnd();
    glBegin(GL_TRIANGLES); //9
    glVertex2d(835, 95);
    glVertex2d(835, 75);
    glVertex2d(870, 75);
    glEnd();
    drawQuad(790, 65, 15, 10); //10
    drawQuad(818, 65, 15, 10); //11

    drawNY(650, -20, .4, 0, 0, 0, 45);
    glPopMatrix();
}


void enemy_defender_move() {
    if (EnemyDefender_x < 400 && EnemyDefender_direction == TRUE)
    {
        EnemyDefender_x += EnemyDefenderSpeed;
        if (EnemyDefender_x > 400-1)
        {
            EnemyDefender_direction = false;
        }
    }
    else
    {
        EnemyDefender_x -= EnemyDefenderSpeed;
        if (EnemyDefender_x < -(399))
        {
            EnemyDefender_direction = TRUE;
        }
    }
}

// Dynamic Background
void D_Background(double theta) {
    glColor3d(1, 1, 0);
    star(100, 100, 0.6,30 + theta);
    star(100, 400, 0.6,30 + theta);
    star(200, 300, 0.6,30 + theta);
    star(300, 450, 0.6,30 + theta);
    star(200, 600, 0.6,30 + theta);
    star(1300, 600, 0.6,30 + theta);
    star(1400, 450, 0.6,30 + theta);
    star(1300, 300, 0.6,30 + theta);
    star(1500, 250, 0.6,30 + theta);
}

void Background(int num) {
    glColor3d(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    if (num == 1) {
        glBindTexture(GL_TEXTURE_2D, Texture_Background1);
    }
    else if (num == 2) {
        glBindTexture(GL_TEXTURE_2D, Texture_Background2);
    }
    else if (num == 3) {
        glBindTexture(GL_TEXTURE_2D, Texture_Background3);
    }
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2f(1, 0); glVertex3f(1600, 0, 0);
    glTexCoord2f(1, 1); glVertex3f(1600, 800, 0);
    glTexCoord2f(0, 1); glVertex3f(0, 800, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3d(1, 1, 1);

    Background(level);
    D_Background(n_steps * stars_speed);

 //   glEnable(GL_TEXTURE_2D);
	//glPushMatrix();
	//glBindTexture(GL_TEXTURE_2D, Texture_Win);
	//glBegin(GL_QUADS);
	//    glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	//    glTexCoord2f(rep, 0.0f); glVertex3f(1600, 0, 0);
	//    glTexCoord2f(rep, rep); glVertex3f(1600, 800, 0);
	//    glTexCoord2f(0.0f, rep); glVertex3f(0, 800, 0);
	//glEnd();
	//glPopMatrix();
 //   glDisable(GL_TEXTURE_2D);
    
 
    generatePowerup();

    drawHealthBar(enemyHealth);
    // bullet enemy defender
    if (BulletEnemyDefender) {
        drawObstacle(BulletEnemyDefender_x, BulletEnemyDefender_y - 220, .3, -90);
    }
    if (ObstacleFlag) {
        drawObstacle(Obstacle_x, Obstacle_y, .6, -90);
    }
    if (BulletFlag) {
        drawBullet(Bullet_x, Bullet_y, .3, 0);
    }
    if (level == 1) {
        enemy1(enemy_x, 0);  // w, h = 550, 230
    }
    else if (level == 2) {
        enemy2(enemy_x, 0);  // w, h = 425, 250
    }
    else if (level == 3) {
        enemy3(enemy_x, 0);  // w, h = 380, 240
    }
    else if (level == 4 && enemy4_d) {
        enemy3(enemy_x, 0);  // w, h = 380, 240
    }
    enemy_defender(EnemyDefender_x-15, 0, EnemyDefenderFlag);
    player(player_x, 0);  // w, h = 270, 140
    drawExplosion(player_x, 0, 0.5, 180, Player_Shooted);

    glColor3d(1, 0, 0);
    char* Number[100];
    sprintf((char*)Number, "Level: %d", level);
    printStringToScreen(100, 760, (char*)Number);


    /// <summary>
    /// edit to don't draw anything if win or lose
    /// </summary>
    GameOver(Game_Over);
    WIN(Win);
    
	glFlush();
}

void player_moving(int d) {
	// d -> direction
	// 2 -> right
	// 1 -> left
	if (d == 2) {
		if (player_x <= 600) {
			player_x += distance_player_move;
		}
	}
	if (d == 1) {
		if (player_x >= -600) {
			player_x -= distance_player_move;
		}
	}
}

void Animation() {
	Sleep(50);
    // Game continue while the player still not Shooted
    if (!Player_Shooted) {
	    enemyRandomMoving();
        checkObstacleCollision();
        checkBulletCollision();
        checkEnemyHealth();
        enemy_defender_move();
        checkEnemyDefenderBulletCollision();
        checkPowerupCollision();
    }
    // Skip if the Game Ends
    else if (Game_Over || Win) {
    }
    // Give 3 seconds before showing Game Over
    else {
        PlaySound(TEXT("sound/GameOver.wav"), NULL, SND_ASYNC | SND_FILENAME);
        Sleep(3000);
        Game_Over = TRUE;
    }
    //cout << BulletEnemyDefender_y << " " << BulletEnemyDefender_x << " "<< BulletEnemyDefender << endl;

    
    //cout << Bullet_y << endl;

	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y) {
	if (key == 'D' || key == 'd') player_moving(2);
	if (key == 'A' || key == 'a') player_moving(1);
	if (key == 'M' || key == 'm') move_with_mouse=!move_with_mouse;
    if (key == ' ' && !BulletFlag) {
        BulletFlag = TRUE;
        Bullet_x = player_x;
        PlaySound(TEXT("sound/e1.wav"), NULL, SND_ASYNC | SND_FILENAME);
    }
	if (key == GLUT_KEY_ESCAPE) exit(EXIT_SUCCESS);

	glutPostRedisplay();
}

void Timer(int valu) {
    if (!ObstacleFlag) {
        ObstacleFlag = TRUE;
        PlaySound(TEXT("sound/e2.wav"), NULL, SND_ASYNC | SND_FILENAME);
        Obstacle_x = enemy_x;
    }
    glutPostRedisplay();
    glutTimerFunc(3000, Timer, 0);
}

void Enemy_Defender_Stay_Timer(int valu) {
    if (EnemyDefenderFlag) {
        EnemyDefenderFlag = FALSE;
    }
    glutPostRedisplay();
}

void Enemy_Defender_Timer(int valu) {
    if (!EnemyDefenderFlag) {
        EnemyDefenderFlag = TRUE;
        BulletEnemyDefender = TRUE;
        BulletEnemyDefender_y = 0;
        BulletEnemyDefender_x = EnemyDefender_x;
        EnemyDefender_x = 0;
    }
    glutPostRedisplay();
    glutTimerFunc(5000, Enemy_Defender_Stay_Timer, 0);
    glutTimerFunc(25000, Enemy_Defender_Timer, 0);
}

void Special_Keyboard(int key, int x, int y) {
	if (key == GLUT_KEY_UP);
	if (key == GLUT_KEY_RIGHT) player_moving(2);
	if (key == GLUT_KEY_LEFT) player_moving(1);
	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && !BulletFlag) {
        BulletFlag = TRUE;
        Bullet_x = player_x;
    }
	if (button == GLUT_RIGHT_BUTTON);

	glutPostRedisplay();
}

void Keyboard_Up(unsigned char key, int x, int y) {
	if (key == ' ');

	glutPostRedisplay();
}

void MouseMove(int x, int y) {
    if (move_with_mouse) {
        player_x = x - 800;
    }
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	int x, y;
	x = 1600;
	y = 800;

	glutInitWindowSize(x, y);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Program");
    PlaySound(TEXT("sound/Start.wav"), NULL, SND_ASYNC | SND_FILENAME);
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(Keyboard_Up);
	glutSpecialFunc(Special_Keyboard);
	glutMouseFunc(Mouse);
	glutIdleFunc(Animation);
    glutPassiveMotionFunc(MouseMove);
    glutTimerFunc(4000, Timer, 0);
    glutTimerFunc(3000, Enemy_Defender_Timer, 0); ///////

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glClearColor(0.2, 0.2, 0.2, 0.0f);  // Background Color
	
    // Load Texture

    loadBMP(&Texture_Lose, "textures/start_1.bmp", false);  // Start Background
	loadBMP(&Texture_Win, "textures/Win.bmp", false);  // Win Background
	loadBMP(&Texture_Lose, "textures/GameOver.bmp", false);  // Game Over Background
	loadBMP(&Texture_Background1, "textures/b1.bmp", false);  // Game Over Background
	loadBMP(&Texture_Background2, "textures/b2.bmp", false);  // Game Over Background
	loadBMP(&Texture_Background3, "textures/b3.bmp", false);  // Game Over Background
	//loadBMP(&texID, "textures/3.bmp", false);
	//loadBMP(&Texture_Win_1, "textures/win1.bmp", false);
	
	gluOrtho2D(0,x,0,y);

	glutMainLoop();
}