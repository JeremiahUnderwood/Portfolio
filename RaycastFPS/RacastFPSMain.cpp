#include <GL/glut.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <map>
#include "Textures/Textures.cpp"
#include "LoadBMPObj.cpp"
#define PI 3.1415926535897
#define P2 PI/2
#define P3 3*PI/2
#define DEGREE PI/180


float degToRad(float a) { return a*PI/180.0;}
float FixAng(float a){ if(a>=2 * PI){ a-=2 * PI;} if(a<0){ a+=2 * PI;} return a;}

LoadBMPObj imageQuad;

//Player//
float px, py, pdx, pdy, pa; //Player position, displacement, and angle
typedef struct {
	int w, a, s, d, leftA, rightA; //button state on/off
}ButtonKeys;
ButtonKeys Keys;

void drawPlayer(){
	glColor3f(1, 1, 0);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2i(px, py);
	glEnd();
	
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2i(px, py);
	glVertex2i(px + pdx*5, py + pdy*5);
	glEnd();
}
//PlayerEnd//

//Map//
int mapX = 8, mapY = 8, mapS = 64;
int mapW[] = {
	3, 3, 3, 3, 3, 3, 3, 3,
	3, 0, 0, 0, 3, 0, 0, 3,
	6, 0, 0, 0, 5, 0, 0, 3,
	3, 0, 0, 0, 3, 0, 0, 3,
	3, 3, 3, 3, 3, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 3,
	3, 3, 3, 3, 3, 3, 3, 3,
} ;

int mapF[] = {
	3, 3, 3, 3, 3, 3, 3, 3,
	3, 4, 4, 4, 3, 4, 4, 3,
	3, 4, 4, 4, 4, 4, 4, 3,
	3, 4, 4, 4, 3, 4, 4, 3,
	3, 3, 3, 3, 3, 4, 4, 3,
	3, 4, 4, 4, 4, 4, 4, 3,
	3, 4, 4, 4, 4, 4, 4, 3,
	3, 3, 3, 3, 3, 3, 3, 3,
} ;

int mapC[] = {
	3, 3, 3, 3, 3, 3, 3, 3,
	3, 1, 1, 1, 3, 1, 1, 3,
	3, 1, 1, 1, 1, 1, 1, 3,
	3, 1, 1, 1, 3, 1, 1, 3,
	3, 3, 3, 3, 3, 1, 1, 3,
	3, 1, 1, 1, 1, 1, 1, 3,
	3, 1, 1, 1, 1, 1, 1, 3,
	3, 3, 3, 3, 3, 3, 3, 3,
} ;

void drawMap2D(){
	int x, y, xo, yo;
	for (y=0;y<mapY;y++){
		for(x=0;x<mapX;x++){
			if(mapW[y*mapX +x] > 0){glColor3f(1,1,1);} else{glColor3f(0,0,0);} //map[y*mapX +x] is done since this uses a 1D array instead of a 2D array. It would be the equivalent of map[y][x]
			xo = x * mapS; yo = y * mapS;
			glBegin(GL_QUADS);
			glVertex2i(xo+1, yo+1);
			glVertex2i(xo+1, yo + mapS-1);
			glVertex2i(xo + mapS-1, yo + mapS-1);
			glVertex2i(xo + mapS-1, yo+1);
			glEnd();
		}
	}
}

//MapEnd//

//Hash Map//

vector<vector<int>> textureHash;
vector<int> imageHash;
void initMap(){
	
	textureHash.push_back(nullTexture); //0
	textureHash.push_back(checkerTexture);//1
	textureHash.push_back(arrowTexture);//2
	textureHash.push_back(brickTexture);//3
	textureHash.push_back(tileTexture);//4
	textureHash.push_back(doorTexture);//5
	textureHash.push_back(nullTexture);//6
	textureHash.push_back(stayOutTexture);//7	
	
	
	imageHash.push_back(0); //0
	imageHash.push_back(0); //1
	imageHash.push_back(0); //2
	imageHash.push_back(0); //3
	imageHash.push_back(0); //4
	imageHash.push_back(0); //5
	imageHash.push_back(1); //6
}

//RayCasting//

float dist(float ax, float ay, float bx, float by, float ang){
	return sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay));
}

float fixAng(float angle){
	if (angle >= 360){
		angle +- 360;
	}
	if (angle < 0){
		angle += 360;
	}
	return angle;
}

void drawRays3D(){
	bool imageBit = false; int imageCorner1X, imageCorner1Y, imageCorner2X, imageCorner2Y, imageLHight, imageRHight; float xStart, xStop;
	int r, mx, my, mp, dof; float rx, ry, ra, xo, yo, disT;
	ra = pa - DEGREE * 30; if (ra < 0){ra += 2 * PI;} if (ra > 2 * PI){ra -= 2 * PI;}
	vector<float> imageCoords;
	for (r=0;r<60;r++){
		int xWallText = 0, yWallText = 0, finalWallText = 0;
		//Check Horizontal Lines//
		dof = 0;
		float disH = 1000000000000000, hx = px, hy = py;
		float aTan = -1/tan(ra);
		if (ra>PI){ ry=(((int)py>>6)<<6)-0.0001; rx = (py-ry)*aTan+px; yo = -64; xo = -yo *aTan;}
		if (ra<PI){ ry=(((int)py>>6)<<6)+64; rx = (py-ry)*aTan+px; yo = 64; xo = -yo * aTan;}
		if(ra==0 || ra == PI) {rx =px; ry=py; dof=8;}
		while (dof < 8){
			mx = (int)(rx)>>6; my = (int)(ry)>>6; mp = my*mapX+mx; //find sector
			if(mp > 0 && mp<mapX*mapY && mapW[mp] > 0){xWallText = mapW[mp]; hx = rx; hy = ry; disH = dist(px, py, hx, hy, ra); dof = 8;}//hitwall
			else{rx += xo; ry += yo; dof+=1;}
		}
		
		//Check Vertical Lines//
		dof = 0;
		float disV = 1000000000000000, vx = px, vy = py;
		float nTan = -tan(ra);
		if (ra>P2 && ra<P3){ rx=(((int)px>>6)<<6)-0.0001; ry = (px-rx)*nTan+py; xo = -64; yo = -xo *nTan;}
		if (ra<P2 || ra>P3){ rx=(((int)px>>6)<<6)+64; ry = (px-rx)*nTan+py; xo = 64; yo = -xo * nTan;}
		if(ra==0 || ra == PI) {rx =px; ry=py; dof=8;}
		while (dof < 8){
			mx = (int)(rx)>>6; my = (int)(ry)>>6; mp = my*mapX+mx; //find sector
			if(mp > 0 && mp<mapX*mapY && mapW[mp] > 0){yWallText = mapW[mp]; vx = rx; vy = ry; disV = dist(px, py, vx, vy, ra); dof = 8;}//hitwall
			else{rx += xo; ry += yo; dof+=1;}
		}
		
		float shade = 1;
		if (disV < disH) {finalWallText = yWallText; rx = vx; ry = vy; disT = disV; glColor3f(0, 0, 0.9); shade = 0.66;}
		if (disV > disH) {finalWallText = xWallText; rx = hx; ry = hy; disT = disH; glColor3f(0, 0, 0.7);}
		
		//glColor3f(1, 0, 0);
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex2i(px, py);
		glVertex2i(rx, ry);
		glEnd();
		
		//Draw Walls//
		float ca = pa - ra; if (ca < 0){ca += 2 * PI;} if (ca > 2 * PI){ca -= 2 * PI;} disT = disT * cos(ca); //fixes fisheye
		float lineH = (mapS*320)/disT; 
		float lineHBuffer = lineH;
		float ty_step = 32/(float)lineH;
		float ty_off = 0;
		if (lineH > 320) {ty_off = (lineH-320)/2.0;lineH = 320;} //offset the texture reading so the texture doesnt appear to fall down
		float lineO = 160 - lineH/2;
		
		int y;
		float ty = ty_off * ty_step;
		
		float tx;
		if (shade == 1){
			tx = (int)(rx / 2.0)%32; if (ra < 180 * DEGREE){tx = 31 - tx;}
		}
		else{
			tx = (int)(ry / 2.0)%32; if (ra>90 * DEGREE && ra<270 * DEGREE){tx = 31 - tx;}
		}
		
		float e = .035 * disT;
		float invE = 64 / e;
		
		
		if(imageHash[finalWallText] == 0){
			for (y = 0; y<lineH; y++){
				
				float c = textureHash[finalWallText][(int)(ty) * 32 + (int)(tx)] * shade;
				if (finalWallText == 6){
					if (!imageBit){
						imageLHight = lineHBuffer;
						imageCorner1X = r*8 + 530; imageCorner1Y =  y + 160 - lineHBuffer/2;
						xStart = tx / 31;
						imageBit = true;
					}
					else{
						imageRHight = lineHBuffer;
						imageCorner2X = r*8 + 530; imageCorner2Y =  lineHBuffer + 160 - lineHBuffer/2;
						xStop = tx / 31;
					}
				} //draw full image pt 1
				glColor3f(c, c, c);
				glPointSize(8);
				glBegin(GL_POINTS);
				glVertex2i(r*8 + 530, y + lineO);
				glEnd();
				ty += ty_step;
			}
		}
		else{
			imageCoords.push_back(r*8 + 530);
			//int iX = r*8 + 530;
			imageCoords.push_back(lineO);
			//int iY = lineO;
			//tx = 31 - tx;
			//float txr = tx;
			//ty = 31 - ty;
			//float tyr = 31 - ty;
			imageCoords.push_back(tx);
			imageCoords.push_back(lineH);
			imageCoords.push_back(lineH / lineHBuffer/ 2);
			//float lineHRatio = lineH / lineHBuffer/ 2;
			//imageQuad.showImage(iX-4, iY+4 + lineH, iX+4, iY+4 + lineH, iX+4, iY-4, iX-4, iY-4, tx/32, 0.5 - lineHRatio, (tx + 1)/32, 0.5 - lineHRatio, (tx + 1)/32, 0.5 + lineHRatio, tx/32, 0.5 + lineHRatio);
			ty += ty_step;							
		}
		
		//DrawFloors//
		for (y = lineO + lineH; y < 320; y++){
			float dy=y-(320/2.0), deg = ra, raFix = cos(FixAng(pa - ra));
			tx = px/2 + cos(deg) * 158 * 32 / dy / raFix;
			ty = py/2 + sin(deg) * 158 * 32 / dy / raFix;
			int mp = mapF[(int)(ty/32.0) * mapX + (int)(tx/32.0)];
			float c = textureHash[mp][((int)(ty) & 31) * 32 + ((int)(tx) & 31)] * 0.7; // textureHash[finalWallText]
			glColor3f(c, c, c);
			glPointSize(8);
			glBegin(GL_POINTS);
			glVertex2i(r*8 + 530, y);
			glEnd();
			
			//Draw Cieling
			mp = mapC[(int)(ty/32.0) * mapX + (int)(tx/32.0)];
			c = textureHash[mp][((int)(ty) & 31) * 32 + ((int)(tx) & 31)] * 0.7; // textureHash[finalWallText]
			glColor3f(c, c, c);
			glPointSize(8);
			glBegin(GL_POINTS);
			glVertex2i(r*8 + 530, 320 - y);
			glEnd();
		}
		
		
		ra += DEGREE; if (ra < 0){ra += 2 * PI;} if (ra > 2 * PI){ra -= 2 * PI;} // Iterate
	}
	
	int iMax = imageCoords.size()/5;
	float iX;
	float iY;
	float tx;
	float lineH;
	float lineHRatio;
	for (float i = 0; i < iMax; i++){
		float startPercent = imageCoords[2] / 32;
		float endPercent = (imageCoords[imageCoords.size() - 3] + 1) / 32;
		iX = imageCoords[i * 5];
		iY = imageCoords[i * 5 + 1];
		tx = imageCoords[i * 5 + 2];
		lineH = imageCoords[i * 5 + 3];
		float x1 = i/iMax; x1 = x1 * (endPercent - startPercent); x1 = x1 + startPercent;
		float x2 =  (i + 1)/iMax; x2 = x2 * (endPercent - startPercent); x2 = x2 + startPercent;
		lineHRatio = imageCoords[i * 5 + 4];
		//imageQuad.showImage(iX-4, iY+4 + lineH, iX+4, iY+4 + lineH, iX+4, iY-4, iX-4, iY-4, tx/32, 0.5 - lineHRatio, (tx + 1)/32, 0.5 - lineHRatio, (tx + 1)/32, 0.5 + lineHRatio, tx/32, 0.5 + lineHRatio);
		//imageQuad.showImage(iX-4, iY+4 + lineH, iX+4, iY+4 + lineH, iX+4, iY-4, iX-4, iY-4, i/iMax, 0.5 - lineHRatio, (i + 1)/iMax, 0.5 - lineHRatio, (i + 1)/iMax, 0.5 + lineHRatio, i/iMax, 0.5 + lineHRatio);
		imageQuad.showImage(iX-4, iY-4 + lineH, iX+4, iY-4 + lineH, iX+4, iY+4, iX-4, iY+4, x1, 0.5 - lineHRatio, x2, 0.5 - lineHRatio, x2, 0.5 + lineHRatio, x1, 0.5 + lineHRatio);
	}
}

//RayCastingEnd//

float frame1 = 0, frame2, fps, sens = .025, mouseSens = 0.002, wSens = .04;

float mouseD = 0; //changeInMouseX
float mouseSpeed = 0;
void display()
{
	frame2 = glutGet(GLUT_ELAPSED_TIME); fps = frame2-frame1; frame1 = glutGet(GLUT_ELAPSED_TIME);
	
	//mouseSpeed += (mouseD - mouseSpeed)/1.33;
	//if ((mouseD - mouseSpeed) >= -1){mouseSpeed == mouseD;}
	//if ((mouseD - mouseSpeed) <= 1){mouseSpeed == mouseD;}
	mouseSpeed = mouseD;
	
	int xo;
	if(pdx < 0){xo = -10;} else {xo = 10;}
	int yo;
	if(pdy < 0){yo = -10;} else {yo = 10;}
	int ipx = px/64.0, ipx_plus_xo = (px + xo)/64.0, ipx_minus_xo = (px - xo)/64.0;
	int ipy = py/64.0, ipy_plus_yo = (py + yo)/64.0, ipy_minus_yo = (py - yo)/64.0;
	
	if (Keys.a == 1){
		if (mapW[ipy * mapX + (int)((px + yo)/64)] == 0){px += pdy * wSens * fps;}
		if (mapW[(int)((py - xo)/64) * mapX + ipx] == 0){py -= pdx * wSens * fps;}
	}
	if (Keys.d == 1){
		if (mapW[ipy * mapX + (int)((px - yo)/64)] == 0){px -= pdy * wSens * fps;}
		if (mapW[(int)((py + xo)/64) * mapX + ipx] == 0){py += pdx * wSens * fps;}
	}
	if (Keys.w== 1){
		if (mapW[ipy * mapX + ipx_plus_xo] == 0) {px += pdx * wSens * fps;}
		if (mapW[ipy_plus_yo * mapX + ipx] == 0) {py += pdy * wSens * fps;}
	}
	if (Keys.s == 1){
		if (mapW[ipy * mapX + ipx_minus_xo] == 0) {px -= pdx * wSens * fps;}
		if (mapW[ipy_minus_yo * mapX + ipx] == 0) {py -= pdy * wSens * fps;}
	}
	
	if (Keys.leftA == 1) {pa -= 0.1 * sens * fps; if (pa < 0){pa += 2 * PI;} pdx = cos(pa) * 5; pdy = sin(pa) * 5;}
	if (Keys.rightA == 1) {pa += 0.1 * sens * fps; if (pa > 2*PI){pa -= 2 * PI;} pdx = cos(pa) * 5; pdy = sin(pa) * 5;}
	pa += mouseSpeed * mouseSens; if (pa < 0){pa += 2 * PI;} if (pa > 2*PI){pa -= 2 * PI;} pdx = cos(pa) * 5; pdy = sin(pa) * 5;
	//printf("%f", mouseD);
	mouseD = 0;
	glutPostRedisplay();
	
	glClear(GL_COLOR_BUFFER_BIT);
 	drawMap2D();
 	drawPlayer();
 	drawRays3D();
 	//imageQuad.showImage(100, 400, 300, 400, 300, 100, 100, 100);
 	glutSwapBuffers();
}

//Input//
void ButtonDown(unsigned char key, int x, int y){  //check if key has been pressed
	if (key == 'a') {Keys.a = 1;}
	if (key == 'd') {Keys.d = 1;}
	if (key == 'w') {Keys.w = 1;}
	if (key == 's') {Keys.s = 1;}
	
	if (key == 'e'){                    //interact
		int xo;
		if(pdx < 0){xo = -50;} else {xo = 50;}
		int yo;
		if(pdy < 0){yo = -50;} else {yo = 50;}
		int ipx = px/64.0, ipx_plus_xo = (px + xo)/64.0;
		int ipy = py/64.0, ipy_plus_yo = (py + yo)/64.0;
		
		if (mapW[ipy_plus_yo * mapX + ipx] == 5){
			mapW[ipy_plus_yo * mapX + ipx] = 0;
		}
		else if (mapW[ipy * mapX + ipx_plus_xo] == 5){
			mapW[ipy * mapX + ipx_plus_xo] = 0;
		}
	}
	
	glutPostRedisplay();
}

void ButtonUp(unsigned char key, int x, int y){  //check if key has been pressed
	if (key == 'a') {Keys.a = 0;}
	if (key == 'd') {Keys.d = 0;}
	if (key == 'w') {Keys.w = 0;}
	if (key == 's') {Keys.s = 0;}
	glutPostRedisplay();
}

void specialDown(int key, int x, int y){
	if (key == GLUT_KEY_LEFT) {Keys.leftA = 1;}
	if (key == GLUT_KEY_RIGHT) {Keys.rightA = 1;}
	//if (key == GLUT_KEY_UP) {py -= 5;}
	//if (key == GLUT_KEY_DOWN) {py += 5;}
	glutPostRedisplay();
}

void specialUp(int key, int x, int y){
	if (key == GLUT_KEY_LEFT) {Keys.leftA = 0;}
	if (key == GLUT_KEY_RIGHT) {Keys.rightA = 0;}
	//if (key == GLUT_KEY_UP) {py -= 5;}
	//if (key == GLUT_KEY_DOWN) {py += 5;}
	glutPostRedisplay();
}
//End Input//

void init(){
	glClearColor(0.3, 0.3, 0.3, 0);
	gluOrtho2D(0, 1024, 512, 0);
	px = 400; py = 400; pdx = cos(pa) * 5; pdy = sin(pa) * 5;
}

void resize(int w, int h){
	glutReshapeWindow(1024, 512);
}

float iframe1, iframe2, ifps;
//int mouseX;
void drag(int x, int y){
	iframe2 = glutGet(GLUT_ELAPSED_TIME); ifps = iframe2-iframe1; iframe1 = glutGet(GLUT_ELAPSED_TIME);
	int mouseX2 = x;
	mouseD += (mouseX2 - 400);
	//mouseX = mouseX2;
	glutWarpPointer(400, 400);

}

int main(int argc, char** argv)
{ 
 	glutInit(&argc, argv);
 	initMap();
 	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
 	glutInitWindowSize(1024,512);
 	glutInitWindowPosition(200, 200);
 	glutCreateWindow("RaycastScene");
 	imageQuad.initialize();
 	init();
 	glutDisplayFunc(display);
 	glutPassiveMotionFunc(drag);
 	glutReshapeFunc(resize);
 	glutKeyboardFunc(ButtonDown);
 	glutKeyboardUpFunc(ButtonUp);
 	glutSpecialFunc(specialDown);
 	glutSpecialUpFunc(specialUp);
 	glutSetCursor(GLUT_CURSOR_NONE);
 	glutMainLoop();
 	return 0;
}
