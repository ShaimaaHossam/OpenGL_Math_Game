#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include<string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include <iostream>
#include <iterator>
#include <fstream>
#include <list>
#include <algorithm>
#include <bits/stdc++.h>
#include<iterator>
#include <sstream>
#include <string>

using namespace std;

class Square
{
public:
    int sqWidX; //width along x-axis
    int sqWidY; // width along y-axis
    int cx; //X center
    int cy; // Y center
};

int PhyWidth= 700;
int PhyHeight= 700;
int logWidth=100;
int logHeight=100;
int centerX=logWidth/2;
int centerY=logHeight/2;
int mouseX=centerX, mouseY=centerY;
int hover = 0;
int Menu = 0;
int status = 0;
int score = 0;
int speed=50;
int i=0;
char str[10000];
char Newstr[2]= {0,0};
char ch;
bool changeChars = false;
bool gameOver = false;
int x[100];  //first number
int y[100];  // second number
int ans[100]; //answer
char op[100]; //operator
int delta=0;
int playerRank=0;
int timer = 59;
int second = 1000;
int milliSeconds = 0;
int restartCounter=0;
bool hambozo = true;
bool addMyScore = 0;
std::list<int> highScore;
std::list<int>::iterator it;
//Functions
void init();
void display();
void Timer(int value);
void Mouse(int btn, int state,int x, int y);
void passiveMouse (int x,int y);
void Keyboard(unsigned char key, int x, int y);
void specialKeyboard(int key, int x, int y);
void printSome(char *str,int x,int y, float r, float g, float b);
void createButton(Square button, float r, float g, float b);
void MainMenu();
void PlayMenu();
void generateSquareObjects(Square sq);
void GameOverMenu();
void checkMenu1();
void initializeRandomNumbersToArray();
void writeScoreToFile();
void readHighScoresFromFile();
void printScore();

//Objects
Square start;
Square highScores;
Square exitbtn;

int main(int argc, char *argv[])
{

    initializeRandomNumbersToArray();
    glutInit(&argc, argv);
    glutInitWindowSize(700,700);
    glutInitWindowPosition(100,100);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE );
    glutCreateWindow("Game");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(Mouse);
    glutPassiveMotionFunc(passiveMouse);
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(specialKeyboard);
    glutMainLoop();

}
void init()
{
    glClearColor( 1, 1, 1, 1.0);
    glMatrixMode( GL_PROJECTION);
    gluOrtho2D( 0.0, 100, 0.0, 100);
}
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if(Menu==0)
    {
        MainMenu();

    }
    else if(Menu==1)
    {

        if(hambozo)
        {
            Timer(0);
            hambozo = false;
        }

        PlayMenu();


    }
    else if(Menu==2)
    {
        glClearColor(1,1,1,0);
        glClear(GL_COLOR_BUFFER_BIT);
        writeScoreToFile();
        GameOverMenu();
    }
    else if(Menu==3)
    {
        printSome("Highest Scores", 40, 70,0,0,0);
        readHighScoresFromFile();
    }
    glutSwapBuffers();
    glFlush();
}
void Timer(int value)
{
    if(Menu==1){
        milliSeconds+=50;
        if(milliSeconds==second){ //if one second passed, decrement timer
            timer--;
            second+=1000;
        }


        if(timer<=0){
            timer = 0;
            gameOver = true;
            Menu=2;
        }

        glutPostRedisplay();
        glutTimerFunc(50, Timer, value);
    }
}

void Mouse(int btn, int state,int x, int y)
{
    if(Menu==0)
    {
        if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            mouseX = x;
            mouseX=0.5+1.0*mouseX*logWidth/PhyWidth;
            mouseY = PhyHeight - y;
            mouseY=0.5+1.0*mouseY*logHeight/PhyHeight;


            if( mouseX>=(start.cx-start.sqWidX/2) && mouseX<=(start.cx+start.sqWidX/2) && mouseY >=(start.cy-start.sqWidY/2) && mouseY <=(start.cy+start.sqWidY/2))
            {
                Menu = 1;
                status = 1;
            }
            if( mouseX>=(highScores.cx-highScores.sqWidX/2) && mouseX<=(highScores.cx+highScores.sqWidX/2) && mouseY >=(highScores.cy-highScores.sqWidY/2) && mouseY <=(highScores.cy+highScores.sqWidY/2))
            {
                Menu = 3;
                status = 3;
            }
            if( mouseX>=(exitbtn.cx-exitbtn.sqWidX/3) && mouseX<=(exitbtn.cx+exitbtn.sqWidX/3) && mouseY >=(exitbtn.cy-exitbtn.sqWidY/3) && mouseY <=(exitbtn.cy+exitbtn.sqWidY/3))
            {
                exit(1);
            }
        }


        glutPostRedisplay();

    }
}
void passiveMouse (int x,int y)
{
    if(Menu==0)
    {
        mouseX = x;
        mouseX=0.5+1.0*mouseX*logWidth/PhyWidth;
        mouseY = PhyHeight - y;
        mouseY=0.5+1.0*mouseY*logHeight/PhyHeight;
        glutPostRedisplay();

        if( mouseX>=(start.cx-start.sqWidX/2) && mouseX<=(start.cx+start.sqWidX/2) && mouseY >=(start.cy-start.sqWidY/2) && mouseY <=(start.cy+start.sqWidY/2))
        {
            hover = 1;
        }
        else if(mouseX>=(highScores.cx-highScores.sqWidX/2) && mouseX<=(highScores.cx+highScores.sqWidX/2) && mouseY >=(highScores.cy-highScores.sqWidY/2) && mouseY <=(highScores.cy+highScores.sqWidY/2))
        {
            hover = 2;
        }
        else if(mouseX>=(exitbtn.cx-exitbtn.sqWidX/3) && mouseX<=(exitbtn.cx+exitbtn.sqWidX/3) && mouseY >=(exitbtn.cy-exitbtn.sqWidY/3) && mouseY <=(exitbtn.cy+exitbtn.sqWidY/3))
        {
            hover = 3;
        }
        else
            hover = 0;
    }

}
void Keyboard(unsigned char key, int x, int y)
{
    Newstr[0]=key;
    strcat(str,Newstr);
    //glutPostRedisplay();
}
void specialKeyboard(int key, int x, int y)
{
    if(Menu==2 || Menu==3)
    {
        if(key==GLUT_KEY_F2)
            Menu=0;
        if(key== GLUT_KEY_F1)
        {
            initializeRandomNumbersToArray();
            timer=59;
            score=0;
            delta=0;
            Menu=1;
        }
        //glutPostRedisplay();
    }

}
void printSome(char *str,int x,int y, float r, float g, float b)
{
    glColor3f(r,g,b);
    glRasterPos2d(x,y);
    for (int i=0; i<strlen(str); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,str[i]);
    }
    glFlush();
}
void createButton(Square button, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2f(button.cx-button.sqWidX/2, button.cy-button.sqWidY/2); // x, y
    glVertex2f(button.cx+button.sqWidX/2, button.cy-button.sqWidY/2); // x, y
    glVertex2f(button.cx+button.sqWidX/2, button.cy+button.sqWidY/2); // x, y
    glVertex2f(button.cx-button.sqWidX/2, button.cy+button.sqWidY/2); // x, y
    glEnd();
}
void MainMenu()
{

    //Initialize Button Start
    start.sqWidX = 18;
    start.sqWidY = 7;
    start.cx = 50;
    start.cy = 60;


    //Initialize Button Highscorea
    highScores.sqWidX = 18;
    highScores.sqWidY = 7;
    highScores.cx = 50;
    highScores.cy = 50;


    //Initialize Button Exit
    exitbtn.sqWidX = 18;
    exitbtn.sqWidY = 7;
    exitbtn.cx = 50;
    exitbtn.cy = 40;

    //Draw buttons
    if(hover==1)
    {
        createButton(start, 0.137255, 0.137255,  0.556863);
        createButton(highScores, 0.137255, 0.419608,  0.556863);
        createButton(exitbtn, 0.137255, 0.419608,  0.556863);
    }
    else if(hover==2)
    {
        createButton(highScores, 0.137255, 0.137255,  0.556863);
        createButton(start, 0.137255, 0.419608,  0.556863);
        createButton(exitbtn, 0.137255, 0.419608,  0.556863);
    }
    else if(hover==3)
    {
        createButton(exitbtn, 0.137255, 0.137255,  0.556863);
        createButton(highScores, 0.137255, 0.419608,  0.556863);
        createButton(start, 0.137255, 0.419608,  0.556863);
    }
    else
    {
        createButton(start, 0.137255, 0.419608,  0.556863);
        createButton(highScores, 0.137255, 0.419608,  0.556863);
        createButton(exitbtn, 0.137255, 0.419608,  0.556863);
    }
    //print text
    printSome("Ma+h Her0", centerX-strlen("Ma+h Her0"), 90, 0.0, 0.0, 0.0);
    printSome("Play", start.cx-2.5, start.cy-1, 1,1,1);
    printSome("Highscores", highScores.cx-8, highScores.cy-1, 1,1,1);
    printSome("Exit", exitbtn.cx-3, exitbtn.cy-1, 1,1,1);

}
void PlayMenu()
{

    glClearColor(1,1,1,0);
    glClear(GL_COLOR_BUFFER_BIT);
    char temp[20];
    sprintf(temp,"SCORE: %d",score);
    printSome(temp, 5, 96, 0, 0, 0); //Print score at the top left corner
    sprintf(temp, "TIMER: %d", timer); //Print timer at the top right corner
    printSome(temp, 74, 96, 0, 0, 0);

//DRAW VERTICAL LINE IN THE MIDDLE OF THE SCREEN
    glColor3f(0,0,0);
    glBegin(GL_LINES);
    glVertex2i(50,0);
    glVertex2i(50,100);
    glEnd();

//START DRAWING SQUARES
    Square sq;
    sq.sqWidX=18;
    sq.sqWidY=10;
    sq.cx=50;
    sq.cy=-10;
    generateSquareObjects(sq);
    if(abs(delta)>120)
    {
        delta=0;
        i++;
        strcpy(str,"");
    }
    delta++;


    sprintf(temp, "%d", x[i]);
    printSome(temp, sq.cx-sq.sqWidX/2+3, sq.cy-sq.sqWidY/2+3+delta,1,1,1);
    sprintf(temp, "%c", op[i]);
    printSome(temp, sq.cx-sq.sqWidX/2+8, sq.cy-sq.sqWidY/2+3+delta,1,1,1);
    sprintf(temp, "%d", y[i]);
    printSome(temp, sq.cx-sq.sqWidX/2+12, sq.cy-sq.sqWidY/2+3+delta,1,1,1);
    char answer[10];
    sprintf(answer,"%d",ans[i]);
    if(str[strlen(str)-1]==13)
    {
        str[strlen(str)-1] = '\0';
        if(strcmp(str, answer)==0)
        {
            score++;
            strcpy(str,"");
            delta=0;
            i++;
        }
        else
            strcpy(str, "");


    }
}
void generateSquareObjects(Square sq)
{

    glColor3f(0.35, 0.35, 0.7);
    glBegin(GL_POLYGON);
    glVertex2f(sq.cx-sq.sqWidX/2, sq.cy-sq.sqWidY/2+delta);
    glVertex2f(sq.cx+sq.sqWidX/2, sq.cy-sq.sqWidY/2+delta);
    glVertex2f(sq.cx+sq.sqWidX/2, sq.cy+sq.sqWidY/2+delta);
    glVertex2f(sq.cx-sq.sqWidX/2, sq.cy+sq.sqWidY/2+delta);
    glEnd();

}
void GameOverMenu()
{
    char temp[40];

    sprintf(temp, "Your   Score  is  %d", score);
    printSome(temp, centerX-strlen(temp)/2-5, centerY-strlen(temp)+30, 0,0,0);

    sprintf(temp, "Your   Rank   is  %d", playerRank);
    printSome(temp, centerX-strlen(temp)/2-5, centerY-strlen(temp)+15, 0,0,0);

    printSome("Play Again: Press F1", 10, 10, 0.137255, 0.137255,  0.556863 );
    printSome("Back to Menu: Press F2", 55, 10, 0.137255, 0.137255,  0.556863 );


}

void initializeRandomNumbersToArray()
{
    srand(time(0));
    for(int i=0; i<100; i++)
    {
        x[i] = rand()%15 + 1;
        y[i] = rand()%15 + 1;
        switch(rand() % 3)
        {
        case 0:
            op[i] = '+';
            ans[i] = x[i] + y[i];
            break;
        case 1:
            op[i] = '-';
            ans[i] = x[i] - y[i];
            break;
        default:
            op[i] = '*';
            ans[i] = x[i] * y[i];
            break;
        }

    }
}

void writeScoreToFile()
{
    std::ofstream file ("GameScores.txt",std::ios::app);

    file <<score<<std::endl;
}

void readHighScoresFromFile()
{
    std::ifstream file ("GameScores.txt");
    std::string buffer;
    int j;
    while(file >> buffer)
    {
        std::istringstream(buffer) >> j ;
        highScore.push_back(j);
    }
    highScore.sort();
    highScore.reverse();
    highScore.unique();
    int c = 1;
    int n=60;
    char temp[20];
    char num[2];
    for(it= highScore.begin(); it!=highScore.end();++it){
        sprintf(temp, "%d", *it);
        printSome(temp, 50, n,0,0,0);
        sprintf(num, "%d. ", c);
        printSome(num, 30,n,0,0,0);
        n-=5;
        c++;
        if(c==6)
            break;
    }



}
