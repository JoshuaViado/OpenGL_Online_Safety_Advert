// #include loads up library files, the order can matter
// generally load glut.h last
#include <stdio.h>  // this library is for standard input and output
#include <math.h>
#include "glut.h"// this library is for glut the OpenGL Utility Toolkit
// this is an example of creating a global variable called counter

float sx = 0.3;
float sy = 0.3;
float sx1 = 1.0;
float sy1 = 1.0;
float tx1 = 0.0;
float ty1 = 0.0;
float tx2 = 0.0;
float ty2 = 0.0;
float tx3 = 0.0;
float ty3 = 0.0;

int frame = 0;
int flash_counter = 0;
int text_frame = 0;
int v_width = 700;
int v_height = 500;

int trollface[13][2];
int eye[13][2];
int tweenPoly[13][2];
float proportion = 0.0;
char *beware[9] = { "B E W A R E",  "O F", "W H A T", "Y O U", "S E E K", "A N D", "W H A T", "Y O U", "G I V E" };


GLfloat vertices[8][3] =
{ { -70.0, 50.0, 0.0 },{ 70.0, 50.0, 0.0 },{ 70.0, -50.0, 0.0 },{ -70.0, -50.0, 0.0 },
{ -70.0, 50.0, 150.0 },{ 70.0, 50.0, 150.0 },{ 70.0,-50.0,150.0 },{ -70.0, -50.0, 150.0 } };

GLfloat colors[8][3] =
{ { 0.0, 0.0, 0.0 },{ 1.0, 0.0, 0.0 },
{ 1.0, 1.0, 0.0 },{ 0.0, 1.0, 0.0 },{ 0.0, 0.0, 1.0 },
{ 1.0, 0.0, 1.0 },{ 1.0, 0.0, 0.0 },{ 0.0, 1.0, 1.0 } };

GLfloat theta = 0.0;
GLfloat delta = 10.0;

void face(int a, int b, int c, int d)
{
	glBegin(GL_POLYGON);
	glVertex3fv(vertices[a]);
	glVertex3fv(vertices[b]);
	glVertex3fv(vertices[c]);
	glVertex3fv(vertices[d]);
	glEnd();
}

void pupil(void) {
	glLineWidth(2.0);
	glColor3fv(colors[1]);
	face(0, 1, 2, 3);
	glColor3fv(colors[2]);
	face(4, 0, 3, 7);
	glColor3fv(colors[3]);
	face(1, 5, 6, 2);
	glColor3fv(colors[4]);
	face(0, 4, 5, 1);
	glColor3fv(colors[5]);
	face(3, 7, 6, 2);
	glColor3fv(colors[6]);
	face(4, 5, 6, 7);
}

class wcPt2D {
public:
	float x, y;
};


void init3D(int w, int h) {
	//glClearColor(0.0, 0.0, 0.0, 0.0); // set what colour you want the background to be
	glMatrixMode(GL_PROJECTION); // set the matrix mode, we will look at this later
								 // set the projection window size in x an y. Window will then
								 // be mapped to the window created in the main function
	glLoadIdentity();
	glOrtho(-400.0, 400.00, -200.0, 200.0, -1000.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);

	trollface[0][0] = -200;
	trollface[0][1] = 100;

	trollface[1][0] = 200;
	trollface[1][1] = 100;

	trollface[2][0] = 200;
	trollface[2][1] = 50;

	trollface[3][0] = 250;
	trollface[3][1] = 50;

	trollface[4][0] = 250;
	trollface[4][1] = 0;

	trollface[5][0] = 200;
	trollface[5][1] = 0;

	trollface[6][0] = 150;
	trollface[6][1] = -50;

	trollface[7][0] = -225;
	trollface[7][1] = -100;

	trollface[8][0] = -250;
	trollface[8][1] = -60;

	trollface[9][0] = -250;
	trollface[9][1] = 0;

	trollface[10][0] = -300;
	trollface[10][1] = 0;

	trollface[11][0] = -300;
	trollface[11][1] = 50;

	trollface[12][0] = -250;
	trollface[12][1] = 50;

	eye[0][0] = -300;
	eye[0][1] = 50;

	eye[1][0] = -300;
	eye[1][1] = 50;

	eye[2][0] = -100;
	eye[2][1] = 100;

	eye[3][0] = -100;
	eye[3][1] = 100;

	eye[4][0] = 100;
	eye[4][1] = 100;

	eye[5][0] = 100;
	eye[5][1] = 100;

	eye[6][0] = 300;
	eye[6][1] = 50;

	eye[7][0] = 300;
	eye[7][1] = 50;

	eye[8][0] = 300;
	eye[8][1] = -50;

	eye[9][0] = 300;
	eye[9][1] = -50;

	eye[10][0] = 100;
	eye[10][1] = -100;

	eye[11][0] = -100;
	eye[11][1] = -100;

	eye[12][0] = -300;
	eye[12][1] = -50;

}

void tween(int source[13][2], int destination[13][2], int numPoints, double proportion, int tweenPoly[13][2])
{

	for (int i = 0; i < numPoints; i++)
	{
		// get the source point
		double sourceX = source[i][0];
		double sourceY = source[i][1];

		// get the destination point
		double destinationX = destination[i][0];
		double destinationY = destination[i][1];

		// get the difference between source and destination
		double differenceX = (destinationX - sourceX);
		double differenceY = (destinationY - sourceY);

		// tween point is source position + proportion
		// of distance between source and destination
		double tweenX = sourceX + (differenceX * proportion);
		double tweenY = sourceY + (differenceY * proportion);

		// create point with tween co-ordinates in tween array
		tweenPoly[i][0] = int(tweenX);
		tweenPoly[i][1] = int(tweenY);
	}


}

void renderSpacedBitmapString(float x, float y, void *font, char *string) {
	char *c;
	int x1 = x;
	for (c = string; *c != '\0'; c++) {
		glRasterPos2f(x1, y);
		glutBitmapCharacter(font, *c);
		x1 = x1 + glutBitmapWidth(font, *c);
	}
}

void circle(double radius, double xc, double yc, double zc)
{
	int i;
	double angle = 2 * 3.1415 / 20;	// circle is drawn using 20 line.
	double x, y, z;

	glBegin(GL_POLYGON);
	for (i = 0; i<20; i++)
	{
		x = radius * cos(i *angle) + xc;
		y = radius * sin(i * angle) + yc;
		z = zc;
		glVertex3f(x, y, z);
	}
	glEnd();
}

void Draw_Triangle() {
	glLineWidth(3.0);
	glColor3f(0.0, 0.0, 255.0);
	glBegin(GL_LINE_LOOP);
	glVertex3d(-200.0, -100.0, -900.0);
	glVertex3d(0.0, 190.0, -900.0);
	glVertex3d(200.0, -100.0, -900.0);
	glEnd();
}

void Draw_Triangle_2() {
	glColor3f(255.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex2d(200.0, 25.0);
	glVertex2d(60.0, 100.0);
	glVertex2d(230.0, 125.0);
	glEnd();
}

void draw_eye() {
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 255.0);
	glVertex3d(-300.0, 50.0, 500.0);
	glVertex3d(-100, 100.0, 500.0);
	glVertex3d(100.0, 100.0, 500.0);
	glColor3f(0.0, 255.0, 0.0);
	glVertex3d(300.0, 50.0, 500.0);
	glVertex3d(300.0, -50.0, 500.0);
	glColor3f(255.0, 0.0, 0.0);
	glVertex3d(100.0, -100.0, 500.0);
	glVertex3d(-100.0, -100.0, 500.0);
	glVertex3d(-300.0, -50.0, 500.0);
	glEnd();

}

void scaling_triangle() {
	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	glScalef(sx, sy, 0.3);
	renderSpacedBitmapString(-40.0, -30.0, GLUT_BITMAP_HELVETICA_18, " E R R O R ");
	Draw_Triangle();
	glPopMatrix();
	sx = sx*1.05;
	sy = sy*1.05;
}

void scaling_eye() {
	glPushMatrix();
	glScalef(sx, sy, 0.3);
	draw_eye();
	glPopMatrix();

	theta += delta;
	if (theta > 360.0) theta -= 360.0;

	glPushMatrix();
	///3D we can have a seperate rotation for each axis
	glScalef(sx, sy, 0.3);
	glRotatef(theta, 0.0, 1.0, 0.0);
	// translate object back to origin so it will rotate around its center
	glTranslatef(-20, 0.0, -50.0);
	pupil();
	glPopMatrix();

	sx = sx*1.03;
	sy = sy*1.03;
}

void rotate_pupil() {
	draw_eye();
	//if (theta > 360.0) theta -= 360.0;

	glPushMatrix();
	///3D we can have a seperate rotation for each axis
	glRotatef(theta, 0.0, 0.0, 1.0);
	// translate object back to origin so it will rotate around its center
	//glTranslatef(-20, 0.0, -50.0);
	pupil();
	glPopMatrix();

	theta = theta + 5.5;
}

void we_r_watching() {
	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	tx1 = tx1 + 7;
	glTranslatef(tx1, ty1, 0);
	renderSpacedBitmapString(-1000, -210, GLUT_BITMAP_HELVETICA_18, "W E    A R E    W A T C H I N G .    W E    A R E    W A T C H I N G .   ");
	renderSpacedBitmapString(-1000, -190, GLUT_BITMAP_HELVETICA_18, "W E    A R E    W A T C H I N G .    W E    A R E    W A T C H I N G .   ");
	renderSpacedBitmapString(-1000, -170, GLUT_BITMAP_HELVETICA_18, "W E    A R E    W A T C H I N G .    W E    A R E    W A T C H I N G .   ");
	renderSpacedBitmapString(-1000, -150, GLUT_BITMAP_HELVETICA_18, "W E    A R E    W A T C H I N G .    W E    A R E    W A T C H I N G .   ");
	renderSpacedBitmapString(-1000, -130, GLUT_BITMAP_HELVETICA_18, "W E    A R E    W A T C H I N G .    W E    A R E    W A T C H I N G .   ");
	renderSpacedBitmapString(-1000, -110, GLUT_BITMAP_HELVETICA_18, "W E    A R E    W A T C H I N G .    W E    A R E    W A T C H I N G .   ");
	glPopMatrix();


	glPushMatrix();
	tx2 = tx2 - 10;
	glTranslatef(tx2, ty2, 0);
	renderSpacedBitmapString(400, 150, GLUT_BITMAP_HELVETICA_18, "W E    A R E    W A T C H I N G .   ");
	renderSpacedBitmapString(400, 130, GLUT_BITMAP_HELVETICA_18, "W E    A R E    W A T C H I N G .   ");
	renderSpacedBitmapString(400, 110, GLUT_BITMAP_HELVETICA_18, "W E    A R E    W A T C H I N G .   ");
	glPopMatrix();
}

void matrix_code() {
	glColor3f(0.0, 1.0, 0.0);
	glPushMatrix();
	ty1 = ty1 - 10;
	glTranslatef(tx1, ty1, 0.0);
	renderSpacedBitmapString(-400, 780, GLUT_BITMAP_HELVETICA_18, "0101110000000000000000001111111111111111000000000010000000000000111100000000011011010101010");
	renderSpacedBitmapString(-400, 760, GLUT_BITMAP_HELVETICA_18, "0101110000000000000000001111111111111111000000000010000000000000111100000000011011010101010");
	renderSpacedBitmapString(-400, 740, GLUT_BITMAP_HELVETICA_18, "0101110000000000000000001111111111111111000000000010000000000000111100000000011011010101010");
	renderSpacedBitmapString(-400, 720, GLUT_BITMAP_HELVETICA_18, "0101110000000000000000001111111111111111000000000010000000000000111100000000011011010101010");
	renderSpacedBitmapString(-400, 700, GLUT_BITMAP_HELVETICA_18, "0101110000000000000000001111111111111111000000000010000000000000111100000000011011010101010");
	renderSpacedBitmapString(-400, 680, GLUT_BITMAP_HELVETICA_18, "0101110000000000000000001111111111111111000000000010000000000000111100000000011011010101010");
	renderSpacedBitmapString(-400, 660, GLUT_BITMAP_HELVETICA_18, "0101110000000000000000001111111111111111000000000010000000000000111100000000011011010101010");
	renderSpacedBitmapString(-400, 640, GLUT_BITMAP_HELVETICA_18, "0101110000000000000000001111111111111111000000000010000000000000111100000000011011010101010");
	renderSpacedBitmapString(-400, 620, GLUT_BITMAP_HELVETICA_18, "0101110000000000000000001111111111111111000000000010000000000000111100000000011011010101010");
	renderSpacedBitmapString(-400, 600, GLUT_BITMAP_HELVETICA_18, "0101110000000000000000001111111111111111000000000010000000000000111100000000011011010101010");
	renderSpacedBitmapString(-400, 580, GLUT_BITMAP_HELVETICA_18, "0101110000000000000000001111111111111111000000000010000000000000111100000000011011010101010");
	renderSpacedBitmapString(-400, 560, GLUT_BITMAP_HELVETICA_18, "0101110000000000000000001111111111111111000000000010000000000000111100000000011011010101010");
	renderSpacedBitmapString(-400, 540, GLUT_BITMAP_HELVETICA_18, "1111010000000000001110000001111111100011111111000101100000010011110000000111111101101010101");
	renderSpacedBitmapString(-400, 520, GLUT_BITMAP_HELVETICA_18, "0000000110000000001110000111111111111111100000000001000001111001111000000111101010111100101");
	renderSpacedBitmapString(-400, 500, GLUT_BITMAP_HELVETICA_18, "0001110001000000000000001111110000111111000000000010000000111000111000000111100010101010101");
	renderSpacedBitmapString(-400, 480, GLUT_BITMAP_HELVETICA_18, "0000000000000000000000011111000001111001111111000000000010000000011100000111111011010000111");
	renderSpacedBitmapString(-400, 460, GLUT_BITMAP_HELVETICA_18, "11010100000001110000011110000000011110000111111000111100001000011100000000111100010101011111");
	renderSpacedBitmapString(-400, 440, GLUT_BITMAP_HELVETICA_18, "00000000000000000000000111111000000111111000000000010000111110000111100000111100010101010111");
	renderSpacedBitmapString(-400, 420, GLUT_BITMAP_HELVETICA_18, "00000011100000010001110111111111101111110000000000100001111110000111100000111111111111111111");
	renderSpacedBitmapString(-400, 400, GLUT_BITMAP_HELVETICA_18, "11111111111111111010101011000011110000011111111111000000000010000000000000111100010101010101");
	renderSpacedBitmapString(-400, 380, GLUT_BITMAP_HELVETICA_18, "00000000000000000000000111111111111111100000000001000000000111110000110000111100000010111101");
	renderSpacedBitmapString(-400, 360, GLUT_BITMAP_HELVETICA_18, "0000000000000000000000011111111111111110000000000100000000000111010101010111100010101110000");
	renderSpacedBitmapString(-400, 340, GLUT_BITMAP_HELVETICA_18, "0000101010101000000001001111111100111110000000000100101111011100000000000111100001010101011");
	renderSpacedBitmapString(-400, 320, GLUT_BITMAP_HELVETICA_18, "0111111011111110000000000000000001111111111111111000000000010000000000000111100010101010111");
	renderSpacedBitmapString(-400, 300, GLUT_BITMAP_HELVETICA_18, "1100000000000111110000000000111111111100000111111000000000010000000000000111100010101010101");
	renderSpacedBitmapString(-400, 280, GLUT_BITMAP_HELVETICA_18, "1110101011000000000000000000000001111111111111111000000000010000000000000111100010111010101");
	renderSpacedBitmapString(-400, 260, GLUT_BITMAP_HELVETICA_18, "011100000000000000000000011111111111000100001111000000000010000000000000111100010101011111");
	renderSpacedBitmapString(-400, 260, GLUT_BITMAP_HELVETICA_18, "000000000000000000000001010101010101011111111111000000000010000000000000111100010101110101");
	renderSpacedBitmapString(-400, 240, GLUT_BITMAP_HELVETICA_18, "000000000000000000000001111111000011100110111111000000000010000000000000111100000010110111");
	renderSpacedBitmapString(-400, 220, GLUT_BITMAP_HELVETICA_18, "000000000000000000000011111111110101010101111111000000000010000000000000111100010101010101");
	renderSpacedBitmapString(-400, 200, GLUT_BITMAP_HELVETICA_18, "110101010000000000000000000000001111111111111111000000000010000000000000111101010101011110");
	glPopMatrix();


}

void viewports() {
	glViewport(0, 0, 350, 250);
	draw_eye();
	pupil();
	glViewport(0, 250, 350, 250);
	draw_eye();
	pupil();
	glViewport(350, 0, 350, 250);
	draw_eye();
	pupil();
	glViewport(350, 250, 350, 250);
	draw_eye();
	pupil();
}

void inner_trollface() {
	glColor3f(1.0, 1.0, 1.0);
	//left eye
	glBegin(GL_LINE_LOOP);
	glVertex2d(-120, 50);
	glVertex2d(-120, 60);
	glVertex2d(-40, 60);
	glVertex2d(-40, 50);
	glEnd();
	//right eye
	glBegin(GL_LINE_LOOP);
	glVertex2d(20, 50);
	glVertex2d(20, 70);
	glVertex2d(120, 70);
	glVertex2d(120, 50);
	glEnd();
	//nose
	glBegin(GL_LINE_STRIP);
	glVertex2d(0, 40);
	glVertex2d(-20, 30);
	glVertex2d(-20, 10);
	glVertex2d(-60, 0);
	glVertex2d(-40, -15);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2d(-225, 20);
	glVertex2d(-100, -20);
	glVertex2d(0, -20);
	glVertex2d(210, 30);
	glVertex2d(100, -50);
	glVertex2d(-180, -70);
	glEnd();
}

void draw_trollface() {
	//glColor3f(1.0, 1.0, 1.0);
	//face outline
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2d(-200, 100);
	glVertex2d(200, 100);
	glVertex2d(200, 50);
	glVertex2d(250, 50);
	glVertex2d(250, 0);
	glVertex2d(200, 0);
	glVertex2d(150, -50);
	glVertex2d(-225, -100);
	glVertex2d(-250, -60);
	glVertex2d(-250, 0);
	glVertex2d(-300, 0);
	glVertex2d(-300, 50);
	glVertex2d(-250, 50);
	glEnd();
}

void morph_eye_to_troll() {
	int i;

	glColor3f(1.0, 1.0, 1.0);
	tween(eye, trollface, 13, proportion, tweenPoly);

	glBegin(GL_LINE_LOOP);
	for (i = 0; i<13; i++)
		glVertex2i(tweenPoly[i][0], tweenPoly[i][1]);	// Morphing W to Arrow.
	glEnd();

	proportion += 0.01;

	if (proportion >1.0) proportion = 0.0;
}

void beware_text() {
	text_frame++;
	glColor3f(0.0, 1.0, 0.0);

	if (text_frame >= 0) {
		renderSpacedBitmapString(-300, 180, GLUT_BITMAP_HELVETICA_18, beware[0]);
	}
	if (text_frame >= 20) {
		renderSpacedBitmapString(0, 180, GLUT_BITMAP_HELVETICA_18, beware[1]);
	}
	if (text_frame >= 40) {
		renderSpacedBitmapString(300, 180, GLUT_BITMAP_HELVETICA_18, beware[2]);
	}
	if (text_frame >= 60) {
		renderSpacedBitmapString(-300, 0, GLUT_BITMAP_HELVETICA_18, beware[3]);
	}
	if (text_frame >= 80) {
		renderSpacedBitmapString(0, 0, GLUT_BITMAP_HELVETICA_18, beware[4]);
	}
	if (text_frame >= 100) {
		renderSpacedBitmapString(300, 0, GLUT_BITMAP_HELVETICA_18, beware[5]);
	}
	if (text_frame >= 120) {
		renderSpacedBitmapString(-300, -160, GLUT_BITMAP_HELVETICA_18, beware[6]);
	}
	if (text_frame >= 140) {
		renderSpacedBitmapString(0, -160, GLUT_BITMAP_HELVETICA_18, beware[7]);
	}
	if (text_frame >= 160) {
		renderSpacedBitmapString(300, -160, GLUT_BITMAP_HELVETICA_18, beware[8]);
	}
}

void refl_triangle() {
	Draw_Triangle_2();
	glPushMatrix();
	glScalef(-1, -1, 1);
	Draw_Triangle_2();
	glPopMatrix();
}

void rotate_rtriangle() {
	glPushMatrix();
	glRotatef(theta, 0.0, 0.0, 1.0);
	refl_triangle();
	glPopMatrix();

	theta = theta + 1.7;

	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	glScalef(sx, sy, 0.3);
	renderSpacedBitmapString(-40.0, -30.0, GLUT_BITMAP_HELVETICA_18, " E R R O R ");
	glPopMatrix();
	sx = sx*1.05;
	sy = sy*1.05;
}

void zoom_viewport() {

	glPushMatrix();
	glViewport(0, 0, v_width, v_height);
	glPopMatrix();

	v_width = v_width - 1;
	v_height = v_height - 1;

	glPushMatrix();
	glScalef(sx1, sy1, 1.0);
	glTranslatef(tx1, ty1, 1.0);
	Draw_Triangle();
	glPopMatrix();
	sx1 = sx1*1.0019;
	sy1 = sy1*1.0019;
	tx1 = tx1 + 0.45;
	ty1 = ty1 + 0.45;


}

void computer() {
	flash_counter++;
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3i(-100, 50, 0);
	glVertex3i(100, 50, 0);
	glVertex3i(100, -50, 0);
	glVertex3i(-100, -50, 0);
	glEnd();

	glBegin(GL_POLYGON);
	if (flash_counter % 2 == 0) {
		glColor3f(1.0, 1.0, 1.0);
	}
	else {
		glColor3f(1.0, 0.0, 0.0);
	}
	glVertex3i(-80, 35, 10);
	glVertex3i(80, 35, 10);
	glVertex3i(80, -35, 10);
	glVertex3i(-80, -35, 10);
	glEnd();

	glColor3f(1, 1, 1);
	circle(60, 0, -50, 20);

	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(-150, -75, 25);
	glVertex3f(150, -75, 25);
	glVertex3f(150, -200, 25);
	glVertex3f(-150, -200, 25);
	glEnd();


}

void timeline() {
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0, 1.0, 0.0);
	glLineWidth(3.0);
	glVertex2d(-1400, -175);
	glVertex2d(-400, -175);
	glEnd();
}

void skip_ad() {
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2i(300, -125);
	glVertex2i(400, -125);
	glVertex2i(400, -150);
	glVertex2i(300, -150);
	glEnd();

	renderSpacedBitmapString(305, -145, GLUT_BITMAP_HELVETICA_12, "Skip Ad");


	glPushMatrix();
	glTranslatef(tx3, ty3, 1.0);
	timeline();
	glPopMatrix();
	tx3 = tx3 + 0.7;

}





void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0); // set colour to white

	frame++;


	if ((frame >= 0) && (frame <= 200)) {
		//beware_text();
		glClearColor(0.0, 1.0, 0.0, 0.0);
		//zoom_viewport();
		//computer();

		glPushMatrix();
		glScalef(sx1, sy1, 1.0);
		computer();
		glPopMatrix();
		sx1 = sx1*1.01;
		sy1 = sy1*1.01;
	}

	if ((frame >= 200) && (frame <= 1400)) {
		skip_ad();
	}

	if ((frame >= 200) && (frame < 250)) {
		glClearColor(0.0, 0.0, 0.0, 0.0);
		scaling_triangle();
	}

	if (frame == 250) {						// reset scale
		sx = 0.3;
		sy = 0.3;
	}

	if ((frame >= 250) && (frame <= 300)) {
		glClearColor(0.0, 1.0, 0.0, 0.0);
		scaling_triangle();
	}

	if (frame == 300) {						// reset scale
		sx = 0.3;
		sy = 0.3;
	}

	if ((frame >= 300) && (frame <= 350)) {
		glClearColor(1.0, 1.0, 0.0, 0.0);
		scaling_triangle();
	}

	if (frame == 350) {						// reset scale
		sx = 0.3;
		sy = 0.3;
	}

	if ((frame >= 350) && (frame <= 400)) {
		glClearColor(0.0, 0.0, 0.0, 0.0);
		scaling_triangle();
	}

	if (frame == 400) {						// reset scale
		sx = 0.3;
		sy = 0.3;
	}

	if ((frame >= 400) && (frame <= 500)) {
		rotate_rtriangle();
	}

	if (frame == 500) {						// reset scale
		sx = 0.3;
		sy = 0.3;
	}

	if ((frame >= 500) && (frame <= 550)) {
		scaling_eye();
	}

	if (frame == 550) {						// reset scale
		sx = 0.3;
		sy = 0.3;
		theta = 0;
	}

	if ((frame >= 550) && (frame <= 600)) {
		rotate_pupil();
	}

	if ((frame >= 600) && (frame <= 800)) {
		viewports();
		glViewport(0, 0, 700, 500);
		we_r_watching();
	}

	if (frame == 800) {	// reset scale
		glViewport(0, 0, 700, 500);
		sx = 0.3;
		sy = 0.3;
		tx1 = 0.0;
		ty1 = 0.0;
		tx2 = 0.0;
		ty2 = 0.0;
	}

	if ((frame >= 800) && (frame <= 850)) {
		draw_eye();
		pupil();
		matrix_code();
	}

	if (frame == 850) {						// reset scale
		sx = 0.3;
		sy = 0.3;
		sx1 = 1.0;
		sy1 = 1.0;
		tx1 = 0.0;
		ty1 = 0.0;
		tx2 = 0.0;
		ty2 = 0.0;
	}

	if ((frame >= 850) && (frame <= 900)) {
		draw_eye();

		glPushMatrix();
		glScalef(sx1, sy1, 1.0);
		pupil();
		glPopMatrix();
		sx1 = sx1*0.9;
		sy1 = sy1*0.9;
	}

	if ((frame >= 900) && (frame <= 1000)) {
		morph_eye_to_troll();
	}

	if ((frame >= 1000) && (frame <= 1050)) {
		draw_trollface();

		glPushMatrix();
		glScalef(sx, sy, 0.3);
		inner_trollface();
		glPopMatrix();
		sx = sx*1.02;
		sy = sy*1.02;
	}

	if (frame == 1050) {						// reset scale
		sx = 0.3;
		sy = 0.3;
		sx1 = 1.0;
		sy1 = 1.0;
		tx1 = 0.0;
		ty1 = 0.0;
		tx2 = 0.0;
		ty2 = 0.0;
		flash_counter = 0;
		text_frame = 0;
	}

	if ((frame >= 1050) && (frame <= 1100)) {
		draw_trollface();
		inner_trollface();
	}

	if ((frame >= 1100) && (frame <= 1400)) {
		beware_text();
	}

	if ((frame >= 1400) && (frame <= 1450)) {
		computer();
	}

	if ((frame >= 1450) && (frame <= 1500)) {
		glClearColor(0, 0, 0, 0);
	}

	if ((frame >= 1500) && (frame <= 1600)) {

		glColor3f(1.0, 1.0, 1.0);
		renderSpacedBitmapString(-100, 0, GLUT_BITMAP_HELVETICA_18, "STAY SAFE ONLINE");
		renderSpacedBitmapString(-100, -40, GLUT_BITMAP_HELVETICA_18, "nspcc.org.uk");
	}

	if (frame >= 1600) { // reset
		glViewport(0, 0, 700, 500);
		sx = 0.3;
		sy = 0.3;
		sx1 = 1.0;
		sy1 = 1.0;
		tx1 = 0.0;
		ty1 = 0.0;
		tx2 = 0.0;
		ty2 = 0.0;
		tx3 = 0.0;
		ty3 = 0.0;

		frame = 0;
		flash_counter = 0;
		text_frame = 0;
		v_width = 700;
		v_height = 500;

	}




	fprintf(stdout, "Frame number= %i\n", frame);
	glFlush();
	glutSwapBuffers();


}

void Timer(int value)
{
	if (value) glutPostRedisplay();
	glutTimerFunc(40, Timer, value);
}

void visibility(int state)
{
	switch (state)
	{
	case GLUT_VISIBLE:
		Timer(1);
		break;
	case GLUT_NOT_VISIBLE:
		Timer(0);
		break;
	default:
		break;
	}
}



int main(int argc, char** argv) {
	glutInit(&argc, argv);  //perform the GLUT initialization
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(700, 500); // set window position
	glutInitWindowPosition(100, 100); // set window size
	glutCreateWindow("Hello"); // create a display with a given caption for the title bar
	init3D(800, 400); // call init function defined above
					  //glutIdleFunc(idle); //  define what function to call when the program is idle
	glutDisplayFunc(display); // define what function to call to draw 
							  // the last function in the program puts the program into infinite loop
							  // effectively the loop repeatedly calls the idle function 

							  //glOrtho(-2.0, 2.0, -2.0, 2.0, 0.0, 0.0);
	glutVisibilityFunc(visibility);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();

	// this line exits the program
	return 0;
}