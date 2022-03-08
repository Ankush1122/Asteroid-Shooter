#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <stdio.h>

#define AMMO 10
#define AST 10
#define MOVEACCELERATION 0.02
#define ASTROSPEED 0.008
#define FIRESPEED 0.08
#define REFRESH 20

float speed = 0, xbat = 0, ybat = -0.8, xfire[AMMO], yfire[AMMO],
		xasteroid[AST], yasteroid[AST];
int i, j, numoffire = 0, numoasteroid = 0, astroVisible[AST], fireVisible[AMMO];

//generate random position for asteroid
float ran(void) {
	float r = ((float) (rand() % 20) - 10) / 10;
	return r;
}

void keyboard(unsigned char Key, int x, int y) {
	switch (Key) {
	case 'd':
		speed = speed + MOVEACCELERATION;
		break;
	case 'a':
		speed = speed - MOVEACCELERATION;
		break;
	case 32:
		xfire[numoffire % 10] = xbat;
		yfire[numoffire % 10] = ybat;
		fireVisible[numoffire % 10] = 1;
		numoffire++;
		break;
	}
}

void AstroGenerateTime(int Value) {
	xasteroid[numoasteroid % 10] = ran();
	yasteroid[numoasteroid % 10] = 1;
	astroVisible[numoasteroid % 10] = 1;
	numoasteroid++;
	glutTimerFunc(2000, AstroGenerateTime, 0);
}

void Timer(int Value) {

	for (i = 0; i < AST; i++) {
		yfire[i] += FIRESPEED;
		for (j = 0; j < AMMO; j++) {
			if (xfire[i] < (xasteroid[j] + 0.08)
					&& xfire[i] > (xasteroid[j] - 0.08)
					&& yfire[i] < (yasteroid[j] + 0.08)
					&& yfire[i] > (yasteroid[j] - 0.08)) {
				astroVisible[j] = 0;
			}
		}
	}

	for (i = 0; i < AST; i++) {
		if (xbat <= (xasteroid[i] + 0.08) && xbat >= (xasteroid[i] - 0.08)
				&& ybat <= (yasteroid[i] + 0.08)
				&& ybat >= (yasteroid[i] - 0.08) && astroVisible[i] == 1)
			exit(0);
	}
	for (i = 0; i < AST; i++) {
		if (yasteroid[i] <= -1.15 && astroVisible[i] == 1) {
			exit(0);
		}
	}

	if (xbat < -1.0) {
		xbat = -1.0;
		speed = 0;
	}
	if (xbat > 1.0) {
		xbat = 1.0;
		speed = 0;
	}

	xbat = xbat + speed;

	if (speed >= 0.0004) {
		speed = speed - 0.002;
	} else if (speed <= (-0.0004)) {
		speed = speed + 0.002;
	} else
		speed = 0;

	for (i = 0; i < 10; i++) {
		yasteroid[i] -= ASTROSPEED;
	}

	glutPostRedisplay();
	glutTimerFunc(REFRESH, Timer, 0);
}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(xbat, ybat, 0);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	glVertex2d(0, 0);
	glVertex2d(-0.08, -0.08);
	glVertex2d(0.08, -0.08);
	glEnd();

	glColor3f(1.0f, 0.0f, 0.0f);
	for (i = 0; i < AMMO; i++) {
		if (fireVisible[i] == 1) {
			glLoadIdentity();
			glTranslatef(xfire[i], yfire[i], 0);
			glutSolidSphere(0.02, 6, 4);
		}
	}

	for (i = 0; i < 10; i++) {
		if (astroVisible[i] == 1) {
			glLoadIdentity();
			glTranslatef(xasteroid[i], yasteroid[i], 0);
			glutWireSphere(0.08, 8, 4);
		}
	}

	glutSwapBuffers();
}

int main(int argc, char **argv) {
	for (i = 0, j = 0; i < AST || j < AMMO; i++, j++) {
		astroVisible[i] = 0;
		fireVisible[j] = 0;
	}
	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(400, 140);
	glutCreateWindow("Space Ship Game ");
	glutDisplayFunc(Display);
	glutTimerFunc(200, Timer, 0);
	glutTimerFunc(REFRESH, AstroGenerateTime, 0);
	glutKeyboardFunc(keyboard);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glutMainLoop();
	return 0;
}
