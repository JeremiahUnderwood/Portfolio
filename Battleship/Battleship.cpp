#include <GL/glut.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <string>
#include <map>
#include <vector>
#include "Equipment.cpp"

using namespace std;

enum gameMode { //Game modes in relation to user input
	GM_IDLE = 0,
	GM_INPUT = 1,
	GM_VICTORY = 2
};
gameMode GMode; //Variable to storing global game mode

map<string, int> commands; //Hash map for command strings
string displayWords = "Press Enter:"; //words to display


void initCommands(){
	commands["hit"] = 1;
	commands["CheatMode"] = 2;
}

int playingField[100] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

vector<Equipment> enemies = {};

void glInit(){
	glClearColor(0, 0, 0, 0);
	gluOrtho2D(0, 600, 600, 0);
}

void initEquipment(){
	enemies.push_back(Equipment("Trebuchet", 3, 3, 3, 2));
	enemies.push_back(Equipment("Ballista", 5, 8, 3, 2));
	enemies.push_back(Equipment("Ogre", 8, 4, 5, 3));
	
}

void drawMap(int offset){
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			switch (playingField[i + 10 * j]){
				case 0:
					glColor3f(0, 0, 1);
					break;
				case 1:
					glColor3f(.75, .75, 1);
					break;
				case 2:
					glColor3f(1, .75, 0);
					break;
				case 3:
					glColor3f(1, 0, 0);
					break;
			}
			glBegin(GL_QUADS);
			glVertex2i(40*i + 1 + offset, 40*j + 1 + offset);
			glVertex2i(40*(i+1) - 1 + offset, 40*j + 1 + offset);
			glVertex2i(40*(i+1) - 1 + offset, 40*(j+1) - 1 + offset);
			glVertex2i(40*i + 1 + offset, 40*(j+1) - 1 + offset);
			glEnd();
		}
	}
}

bool VictoryCheck(){
	bool victory = true;
	for (int i = 0; i < enemies.size(); i++){
		if(enemies[i].isAlive()) {victory = false;}
	}
	return victory;
}

void hitCommand(string command, int endOfCommand){
	int xCoord = 0;
	int yCoord = 0;
	bool hit = false;
	string enemyName = "";
	if (command.length() >= 7){
		if ((int)command[endOfCommand + 1] -48 == 1 && (int)command[endOfCommand + 2] -48 == 0){
			xCoord = 10;
			endOfCommand += 3;
		}
		else{
			xCoord = (int)command[endOfCommand + 1] -48;
			endOfCommand += 2;
		}
		if ((int)command[endOfCommand + 1] - 48 == 1 && (int)command[endOfCommand + 2] -48 == 0){
			yCoord = 10;
		}
		else{
			yCoord = (int)command[endOfCommand + 1] -48;
		}
		if ((xCoord > 0 && xCoord < 11) && (yCoord > 0 && yCoord < 11)){
			
			for (int i = 0; i < enemies.size(); i++){
				if (enemies[i].isHit(xCoord, yCoord)){
					hit = true;
					enemyName = enemies[i].GetName();
					enemies[i].Hit();
					int graphValue = 0;
					if (!enemies[i].isAlive()){
						for (int j = enemies[i].GetY() - enemies[i].GetRad(); j <= enemies[i].GetY() + enemies[i].GetRad(); j++){
							for(int k = enemies[i].GetX() - enemies[i].GetRad(); k <= enemies[i].GetX() + enemies[i].GetRad(); k++){
								playingField[((10 - j) * 10) + k - 1] = 3;
							}
						}
					}
					else{playingField[((10 - yCoord) * 10) + xCoord - 1] = 2;}
				}
			}
			if (hit == false){
				playingField[((10 - yCoord) * 10) + xCoord - 1] = 1;
				displayWords = "Miss.";
			}
			else{
				displayWords = "Hit " + enemyName;
			}
		}
		else {displayWords = "Invalid Input";}
	}
	else {displayWords = "Invalid Input";}
	if (VictoryCheck()){
		GMode = GM_VICTORY;
	}
}

void GetFucked(){
	for (int i = 0; i < enemies.size(); i++){
		int eHealth = enemies[i].GetHealth(); //needs to be passed to eHealth as to not end early
		for (int l = 0; l <= eHealth; l++){
			enemies[i].Hit();
		}
		for (int j = enemies[i].GetY() - enemies[i].GetRad(); j <= enemies[i].GetY() + enemies[i].GetRad(); j++){
			for(int k = enemies[i].GetX() - enemies[i].GetRad(); k <= enemies[i].GetX() + enemies[i].GetRad(); k++){
				playingField[((10 - j) * 10) + k - 1] = 3;
			}
		}
	}
	if (VictoryCheck()){
		GMode = GM_VICTORY;
	}
}

void inputFunction(string func){
	string command = func; //I don't remember why I did this but too late to change it
	int endOfCommand = command.find(" ");
	switch(commands[command.substr(0, endOfCommand)]){
		case 1:
			hitCommand(command, endOfCommand);
			break;
		case 2:
			GetFucked();
			break;
		default : 
			displayWords = command + " is not a command";
	}
	glutPostRedisplay();
	
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	drawMap(100);
	glColor3f(0.5, 0.25, 0.25);
	glRasterPos2i(20, 20);
	for (int i = 0; i < displayWords.size(); i++){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, displayWords[i]);
	}
	if (GMode == GM_VICTORY){
		string vText = "ALL ENEMIES DEFEATED";
		glColor3f(1, 1, 1);
		glRasterPos2i(160, 290);
		for (int i = 0; i < vText.size(); i++){
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, vText[i]);
		}
	}
	glutSwapBuffers();
}

void resize(int w, int h){
	
}

void keyDown(unsigned char key, int x, int y){
	if (GMode == GM_IDLE){
		switch (key){
			case (char)(27): //escape key
				glutDestroyWindow(glutGetWindow());
				break;
			case((char)(13)):
				displayWords = "";
				GMode = GM_INPUT;
				break;
		}
	}
	else if (GMode == GM_INPUT){
		switch(key){
			case (char)(27): //escape key
				glutDestroyWindow(glutGetWindow());
				break;
			case((char)(13)):
				GMode = GM_IDLE;
				inputFunction(displayWords);
				break;
			case((char)(8)):
				if (displayWords.size() > 0){
					displayWords.pop_back();	
				}
				break;
			default :
				displayWords += key;
		
		}
	}
	/*
	if (key == (char)(27)){
		glutDestroyWindow(glutGetWindow());
	}
	if ()*/
	glutPostRedisplay();
}

int main(int argc, char** argv){
	initEquipment();
	initCommands();
	glutInit(&argc, argv);
 	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
 	glutInitWindowSize(600,600);
 	glutInitWindowPosition(0, 0);
 	glutCreateWindow("Trebuchet Game");
 	glInit();
 	glutDisplayFunc(display);
 	glutReshapeFunc(resize);
 	glutKeyboardFunc(keyDown);
 	glutMainLoop();
 	return 0;
	
}
