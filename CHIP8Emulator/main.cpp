#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <chrono>
#include "chip8.cpp"

const int updateTime = 2000; //time between each frame (makes for approx 60 fps)
int timeToNextUpdate = 2000; //actual time to next update

auto time2 = std::chrono::system_clock::now();;
auto time1 = time2;
unsigned long deltaTime;

void glInit(){
	glClearColor(0, 0, 0, 0);
	gluOrtho2D(0, 640, 320, 0);
}

void keyUp(unsigned char key, int x, int y){
	if (key == '1' || key == '!'){CHIP8::input[0x1] = 0;}
	if (key == '2' || key == '@'){CHIP8::input[0x2] = 0;}
	if (key == '3' || key == '#'){CHIP8::input[0x3] = 0;}
	if (key == '4' || key == '$'){CHIP8::input[0xC] = 0;}
	if (key == 'q' || key == 'Q'){CHIP8::input[0x4] = 0;}
	if (key == 'w' || key == 'W'){CHIP8::input[0x5] = 0;}
	if (key == 'e' || key == 'E'){CHIP8::input[0x6] = 0;}
	if (key == 'r' || key == 'R'){CHIP8::input[0xD] = 0;}
	if (key == 'a' || key == 'A'){CHIP8::input[0x7] = 0;}
	if (key == 's' || key == 'S'){CHIP8::input[0x8] = 0;}
	if (key == 'd' || key == 'D'){CHIP8::input[0x9] = 0;}
	if (key == 'f' || key == 'F'){CHIP8::input[0xE] = 0;}
	if (key == 'z' || key == 'Z'){CHIP8::input[0xA] = 0;}
	if (key == 'x' || key == 'X'){CHIP8::input[0x0] = 0;}
	if (key == 'c' || key == 'C'){CHIP8::input[0xB] = 0;}
	if (key == 'v' || key == 'V'){CHIP8::input[0xF] = 0;}
}

void keyDown(unsigned char key, int x, int y){
	if (key == '1' || key == '!'){CHIP8::input[0x1] = 1;}
	if (key == '2' || key == '@'){CHIP8::input[0x2] = 1;}
	if (key == '3' || key == '#'){CHIP8::input[0x3] = 1;}
	if (key == '4' || key == '$'){CHIP8::input[0xC] = 1;}
	if (key == 'q' || key == 'Q'){CHIP8::input[0x4] = 1;}
	if (key == 'w' || key == 'W'){CHIP8::input[0x5] = 1;}
	if (key == 'e' || key == 'E'){CHIP8::input[0x6] = 1;}
	if (key == 'r' || key == 'R'){CHIP8::input[0xD] = 1;}
	if (key == 'a' || key == 'A'){CHIP8::input[0x7] = 1;}
	if (key == 's' || key == 'S'){CHIP8::input[0x8] = 1;}
	if (key == 'd' || key == 'D'){CHIP8::input[0x9] = 1;}
	if (key == 'f' || key == 'F'){CHIP8::input[0xE] = 1;}
	if (key == 'z' || key == 'Z'){CHIP8::input[0xA] = 1;}
	if (key == 'x' || key == 'X'){CHIP8::input[0x0] = 1;}
	if (key == 'c' || key == 'C'){CHIP8::input[0xB] = 1;}
	if (key == 'v' || key == 'V'){CHIP8::input[0xF] = 1;}
}

void resize(int w, int h){
	
}

void drawScreen(){
	glClear(GL_COLOR_BUFFER_BIT);
	for (int xPos = 0; xPos < 64; xPos++){
		for (int yPos = 0; yPos < 32; yPos++){
			if (CHIP8::display[yPos * 64 + xPos]){
				glColor3f(1, 1, 1);
				glBegin(GL_QUADS);
				glVertex2i(xPos * 10, yPos * 10);
				glVertex2i((xPos + 1) * 10, yPos * 10);
				glVertex2i((xPos + 1) * 10, (yPos + 1) * 10);
				glVertex2i(xPos * 10, (yPos + 1) * 10);
				glEnd();
			}
		}
	}
}

void display(){
	time1 = time2;
    time2 = std::chrono::system_clock::now();
    deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(time2-time1).count();
    timeToNextUpdate -= deltaTime;
    if (timeToNextUpdate <= 0) {
    	timeToNextUpdate += updateTime;
    	CHIP8::update();
    	drawScreen();
    }
    glutSwapBuffers();
    glutPostRedisplay();
}

int main (int argc, char** argv){
	
    bool escapeProgram = false;
    
    time2 = std::chrono::system_clock::now();
	time1 = time2;
	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(time2-time1).count();
    
    CHIP8::LoadROM("Rom/PONG.ch8");
    CHIP8::InitDefaultData();
    
    glutInit(&argc, argv);
 	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
 	glutInitWindowSize(640,320);
 	glutInitWindowPosition(0, 0);
 	glutCreateWindow("CHIP8");
 	glInit();
 	glutDisplayFunc(display);
 	glutReshapeFunc(resize);
 	glutKeyboardFunc(keyDown);
 	glutKeyboardUpFunc(keyUp);
 	glutMainLoop();
    
    return 0;
    
}
