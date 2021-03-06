//
// File: partControl.cc
//
// Description:  Declares functions to control the articulation points
// registered.
//	Original Code By: Prof. Doug Harms
//	Modified By: Pat Herrod
//	November 26, 2012
#include "graphUtils.h"
#include "artPtRegistry.h"
#include "keyFrame.h"
#include <stdio.h> // sprintf
#include <GL/glut.h>
#include <GL/glu.h>
#include <osg/MatrixTransform>
#include <string>
#include <stdlib.h>  // exit()

using namespace std;

// declare variables for the artPt controller
const float FONT_WIDTH = 10.4;
const float FONT_HEIGHT = 13.0;

static int currFrame;
static int ctWindow;
static int rbWindow;
static int animationWindow;
static int animationWindowW, animationWindowH;
static int numFrames;
int time1;
static keyFramePtr keyFrames[100];
static int numArtPts;

static keyFramePtr deltaFrame;
static keyFramePtr currentFrame;

static artPtRegistry *artPts;

bool deltaFlag;
bool animation;
bool loop;

// function prototypes
int animationInit(void);
void animationReshape (int w, int h);
void animationDisplay(void);
void animationKeyboard (unsigned char ch, int x,int y);
void timer(int val);
void addFrame();
void deleteFrame();

// function implementation
int animationInit(int cWindow, int rWindow, artPtRegistry *registry) {
  int currWindow;
  artPts = registry;
  numArtPts = artPts->numArtPts();
  
  numFrames = 0;

  animation = false;
  loop = false;
  
  // save the part window number
  ctWindow = cWindow;
  rbWindow = rWindow;
  
  // initialize currently selected part and paramter
  currFrame = 0;
  
  deltaFrame = new keyFrame(artPts);
  currentFrame = new keyFrame(artPts);
  
  keyFrames[0] = new keyFrame(artPts);
  keyFrames[0]->setFrame();
  // create part control window and set up callbacks, etc.
  
  // save the current window for later restoration
  currWindow = glutGetWindow();
  
  // create a window and register callbacks
  glutInitWindowPosition (850,400);
  glutInitWindowSize (FONT_WIDTH*10,18*FONT_HEIGHT);
  animationWindow = glutCreateWindow ("Animation Controller");
  
  glutDisplayFunc (animationDisplay);
  glutKeyboardFunc (animationKeyboard);
  glutReshapeFunc (animationReshape);
  
  // set up window parameters
  glClearColor (0.0, 0.0, 0.0, 0.0);
  
  // restore previous current window and return
  glutSetWindow (currWindow);

  // return glut ID of control window
  return animationWindow;
}

void animationDisplay(void){
  float yPos,xPos;
  int i,j;
  int val;
  char str[10];
  std::string name;
  
  glutSetWindow (animationWindow);
  glClear (GL_COLOR_BUFFER_BIT);
  
  // display all part names
  // display table heading
  glColor3f (1.0,1.0,1.0);
  yPos = animationWindowH-FONT_HEIGHT;
  xPos = 0;
  displayText (xPos, yPos, 0.0, 0.1, "Frame");
  yPos -= FONT_HEIGHT;
  
  
  for (i=0;i <= numFrames;i++) {
    xPos = 0;

    val = keyFrames[i]->time;
    sprintf (str, "%1i", val);
    
    if (i == currFrame)
      glColor3f (1.0, 0.0, 0.0);
    displayText (xPos, yPos, 0.0, 0.1, str);
    if (i == currFrame)
      glColor3f (1.0, 1.0, 1.0);
//    xPos += 20*FONT_WIDTH;
    
    yPos -= FONT_HEIGHT;
  }
  
  glutSwapBuffers();
}

void animationKeyboard (unsigned char ch, int x,int y){
  float *val;
  int currWindow;
  
  glutSetWindow (animationWindow);

  switch (ch) {
  case 'E':
  case 'e':
    keyFrames[currFrame]->setFrame();
    break;
  case '2':  // down
    currFrame++;
    if (currFrame > numFrames)
      currFrame = numFrames;
    keyFrames[currFrame]->setWorld();
    glutPostRedisplay();
    currWindow = glutGetWindow();
    glutSetWindow(rbWindow);
    glutPostRedisplay();
    glutSetWindow(ctWindow);
    glutPostRedisplay();
    glutSetWindow(currWindow);
    break;
  case '8':  // up
    currFrame--;
    if (currFrame < 0)
      currFrame = 0;
    keyFrames[currFrame]->setWorld();
    glutPostRedisplay();
    currWindow = glutGetWindow();
    glutSetWindow(rbWindow);
    glutPostRedisplay();
    glutSetWindow(ctWindow);
    glutPostRedisplay();
    glutSetWindow(currWindow);
    break;
  case 'P':
  case 'p':
    animation = true;
    deltaFlag = false;
    currFrame = 0;
    time1 = 0;
    keyFrames[currFrame]->setWorld();
    glutPostRedisplay();
    currWindow = glutGetWindow();
    glutSetWindow(rbWindow);
    glutPostRedisplay();
    glutSetWindow(ctWindow);
    glutPostRedisplay();
    glutSetWindow(currWindow);
    glutTimerFunc(100,timer,0);
    break;
  case 'S':
  case 's':
    animation = false;
    loop = false;
    break;
  case 'L':
  case 'l':
    loop = true;
    deltaFlag = false;
    currFrame = 0;
    time1 = 0;
    keyFrames[currFrame]->setWorld();
    glutPostRedisplay();
    currWindow = glutGetWindow();
    glutSetWindow(rbWindow);
    glutPostRedisplay();
    glutSetWindow(ctWindow);
    glutPostRedisplay();
    glutSetWindow(currWindow);
    glutTimerFunc(100,timer,0);
    break;
  case 'I':
  case 'i':
    if (currFrame == numFrames) {
      addFrame();
      glutPostRedisplay();
    }
    else if ((keyFrames[currFrame]->time + 1) != keyFrames[currFrame+1]->time) {
      addFrame();
      glutPostRedisplay();
    }
    break;
  case 'D':
  case 'd':
    if ((numFrames > 0) && (currFrame != 0)) {
    deleteFrame();
    glutPostRedisplay();
    }
    break;
  case 19:
    break;
  case 15:
    break;
  case '=':
    if ((currFrame != 0) &&
      ((currFrame == numFrames) ||
      ((keyFrames[currFrame]->time) < (keyFrames[currFrame+1]->time) - 1)))
    keyFrames[currFrame]->time++;
    glutPostRedisplay();
    break;
  case '+':
    if ((currFrame != 0) &&
      ((currFrame == numFrames) ||
      ((keyFrames[currFrame]->time) < ((keyFrames[currFrame+1]->time) - 11))))
    keyFrames[currFrame]->time += 10;
    glutPostRedisplay();
    break;
  case '-':
    if ((currFrame != 0) &&
      ((keyFrames[currFrame]->time) > (keyFrames[currFrame-1]->time) + 1))
    keyFrames[currFrame]->time--;
    glutPostRedisplay();
    break;
  case '_':
    if ((currFrame != 0) &&
      ((keyFrames[currFrame]->time) > ((keyFrames[currFrame-1]->time) + 11)))
    keyFrames[currFrame]->time -= 10;
    glutPostRedisplay();
    break;        

    // redisplay all three windows
    glutPostRedisplay();
    currWindow = glutGetWindow();
    glutSetWindow(rbWindow);
    glutPostRedisplay();
    glutSetWindow(ctWindow);
    glutPostRedisplay();
    glutSetWindow (currWindow);
    break;

  case 27:
  case 'Q':
  case 'q':
    glutDestroyWindow (rbWindow);
    glutDestroyWindow (ctWindow);
    glutDestroyWindow (animationWindow);
    exit (0);
    break;
  }
}

void timer(int val) {
  int currWindow;
  int time2;
  int deltaTime;
  
  if (animation || loop) {
    if (time1 < keyFrames[numFrames]->time-1) {
      glutTimerFunc(100,timer,0);
      time2 = keyFrames[currFrame+1]->time;
    }
    else {
      time2 = keyFrames[numFrames]->time;
    }
    
  }
  if (time1 < (time2-1)) {
    if (!deltaFlag) {     
      deltaFlag = true;
      deltaTime = time2 - time1;
      for (int i = 0; i < numArtPts; i++) {
	deltaFrame->data[i].rotX = (keyFrames[currFrame+1]->data[i].rotX - keyFrames[currFrame]->data[i].rotX)/deltaTime;
	deltaFrame->data[i].rotY = (keyFrames[currFrame+1]->data[i].rotY - keyFrames[currFrame]->data[i].rotY)/deltaTime;
	deltaFrame->data[i].rotZ = (keyFrames[currFrame+1]->data[i].rotZ - keyFrames[currFrame]->data[i].rotZ)/deltaTime;
	deltaFrame->data[i].posX = (keyFrames[currFrame+1]->data[i].posX - keyFrames[currFrame]->data[i].posX)/deltaTime;
	deltaFrame->data[i].posY = (keyFrames[currFrame+1]->data[i].posY - keyFrames[currFrame]->data[i].posY)/deltaTime;
	deltaFrame->data[i].posZ = (keyFrames[currFrame+1]->data[i].posZ - keyFrames[currFrame]->data[i].posZ)/deltaTime;
      }
    }
    currentFrame->setFrame();
    for (int i = 0; i < numArtPts; i++) {
      currentFrame->data[i].rotX += deltaFrame->data[i].rotX;
      currentFrame->data[i].rotY += deltaFrame->data[i].rotY;
      currentFrame->data[i].rotZ += deltaFrame->data[i].rotZ;
      currentFrame->data[i].posX += deltaFrame->data[i].posX;
      currentFrame->data[i].posY += deltaFrame->data[i].posY;
      currentFrame->data[i].posZ += deltaFrame->data[i].posZ;
    }
    
    currentFrame->setWorld();
    glutPostRedisplay();
    currWindow = glutGetWindow();
    glutSetWindow(rbWindow);
    glutPostRedisplay();
    glutSetWindow(ctWindow);
    glutPostRedisplay();
    glutSetWindow(currWindow);
  } else {
   deltaFlag = false;
   currFrame++;
   keyFrames[currFrame]->setWorld();
   glutPostRedisplay();
   currWindow = glutGetWindow();
   glutSetWindow(rbWindow);
   glutPostRedisplay();
   glutSetWindow(ctWindow);
   glutPostRedisplay();
   glutSetWindow(currWindow);
 }
  time1++;
  if ((time1 == keyFrames[numFrames]->time) && loop) {
    time1 = 0;
    currFrame = 0;
    keyFrames[currFrame]->setWorld();
    glutPostRedisplay();
    currWindow = glutGetWindow();
    glutSetWindow(rbWindow);
    glutPostRedisplay();
    glutSetWindow(ctWindow);
    glutPostRedisplay();
    glutSetWindow(currWindow);
    glutTimerFunc(100,timer,0);
  }
}

void animationReshape (int w, int h){
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D (0.0, w, 0.0, h);
  glMatrixMode(GL_MODELVIEW);
  
  // save the width and height
  animationWindowW = w;
  animationWindowH = h;
}

void addFrame() {
  keyFrames[currFrame]->setFrame();
  for(int i = numFrames; i > currFrame; i--) {
    keyFrames[i+1] = keyFrames[i];
  }
  keyFrames[currFrame+1] = new keyFrame(artPts);
  keyFrames[currFrame+1]->time = (keyFrames[currFrame]->time) + 1;
  numFrames++;
  currFrame++;
  keyFrames[currFrame]->setFrame();
}


void deleteFrame() {
  keyFrames[currFrame]->~keyFrame();
  for(int i = (currFrame+1); i <= numFrames; i++) {
    keyFrames[i-1] = keyFrames[i];
  }
  numFrames--;
  currFrame--;
}

