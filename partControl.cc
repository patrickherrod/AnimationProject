
//
// File: partControl.cc
//
// Description:  Declares functions to control the articulation points
// registered.
//	Original Code By: Prof. Doug Harms
//	Modified By: Pat Herrod
//	November 6, 2012

#include "artPtRegistry.h"
#include "graphUtils.h"
#include <stdio.h> // sprintf
#include <GL/glut.h>
#include <GL/glu.h>
#include <osg/MatrixTransform>
#include <string>
#include <stdlib.h>  // exit()

using namespace std;

// declare variables for the artPt controller
const int NUM_PARAMS = 6;			// display 3 paramters
const float FONT_WIDTH = 10.4;
const float FONT_HEIGHT = 13.0;

static int currPart, currParam;
static int controlWindow;
static int robotWindow;
static int controlWindowW, controlWindowH;

static artPtRegistry *artPts;

// function prototypes
int controlInit(void);
void controlReshape (int w, int h);
void controlDisplay(void);
void controlKeyboard (unsigned char ch, int x,int y);

// function implementation
int controlInit(int pWindow,artPtRegistry *registry) {
  int currWindow;
  int numArtPts;

  // remember the articulation points registry in global variable artPts
  artPts = registry;

  // get the number of articulation points, which will be used to know how
  // big the window should be
  numArtPts = artPts->numArtPts();

  // save the part window number
  robotWindow = pWindow;
  
  // initialize currently selected part and paramter
  currPart = currParam = 0;
  
  // create part control window and set up callbacks, etc.
  
  // save the current window for later restoration
  currWindow = glutGetWindow();
  
  // create a window and register callbacks
  glutInitWindowPosition (850,10);
  glutInitWindowSize ((20+NUM_PARAMS*8)*FONT_WIDTH,(numArtPts+1)*FONT_HEIGHT+5);
  controlWindow = glutCreateWindow ("Part Controller");
  
  glutDisplayFunc (controlDisplay);
  glutKeyboardFunc (controlKeyboard);
  glutReshapeFunc (controlReshape);
  
  // set up window parameters
  glClearColor (0.0, 0.0, 0.0, 0.0);
  
  // restore previous current window and return
  glutSetWindow (currWindow);

  // return glut ID of control window
  return controlWindow;
}

void controlDisplay(void){
  float yPos,xPos;
  int numParts = artPts->numArtPts();
  int i,j;
  float val;
  char str[10];
  std::string name;
  artPtData data;
  
  glutSetWindow (controlWindow);
  glClear (GL_COLOR_BUFFER_BIT);
  
  // display all part names
  // display table heading
  glColor3f (1.0,1.0,1.0);
  yPos = controlWindowH-FONT_HEIGHT;
  xPos = 0;
  displayText (xPos, yPos, 0.0, 0.1, "Part Name");

  xPos += 20*FONT_WIDTH;
  displayText (xPos, yPos, 0.0, 0.1, "  RotX    RotY    RotZ    PosX    PosY    PosZ");
  yPos -= FONT_HEIGHT;
  
  for (i=0;i<numParts;i++) {
    xPos = 0;
    name = artPts->getArtPtName(i);
    data = artPts->getArtPtData(i);
    if (i == currPart)
      glColor3f (0.0, 1.0, 0.0);
    displayText (xPos, yPos, 0.0, 0.1, name.c_str());
    if (i == currPart)
      glColor3f (1.0, 1.0, 1.0);
    xPos += 20*FONT_WIDTH;
    
    for (j=0;j<NUM_PARAMS;j++) {
      // set val to current parameter value
      switch (j) {
      case 0: val = data.rotX; break;
      case 1: val = data.rotY; break;
      case 2: val = data.rotZ; break;
      case 3: val = data.posX; break;
      case 4: val = data.posY; break;
      case 5: val = data.posZ; break;
      }
      
      if (i == currPart && j==currParam)
	glColor3f (0.0, 1.0, 0.0);
      sprintf (str, "%6.1f", val);
      displayText (xPos, yPos, 0.0, 0.1, str);
      if (i == currPart && j==currParam)
	glColor3f (1.0, 1.0, 1.0);
      
      xPos += 8*FONT_WIDTH;
    }
    
    yPos -= FONT_HEIGHT;
  }
  
  glutSwapBuffers();
}

void controlKeyboard (unsigned char ch, int x,int y){
  artPtData data;
  float *val;
  int currWindow;
  
  glutSetWindow (controlWindow);

  switch (ch) {
  case '2':  // down
    currPart++;
    if (currPart >= artPts->numArtPts())
      currPart = artPts->numArtPts()-1;
    glutPostRedisplay();
    break;
  case '8':  // up
    currPart--;
    if (currPart < 0)
      currPart = 0;
    glutPostRedisplay();
    break;
  case '4':  // left
    currParam--;
    if (currParam < 0)
      currParam = 0;
    glutPostRedisplay();
    break;
  case '6':  // right
    currParam++;
    if (currParam >= NUM_PARAMS)
      currParam = NUM_PARAMS-1;
    glutPostRedisplay();
    break;
  case '=':
  case '+':
  case '-':
  case '_':
    // get part and set val to point to selected field
    data = artPts->getArtPtData (currPart);
    switch (currParam) {
    case 0: val = &(data.rotX); break;
    case 1: val = &(data.rotY); break;
    case 2: val = &(data.rotZ); break;
    case 3: val = &(data.posX); break;
    case 4: val = &(data.posY); break;
    case 5: val = &(data.posZ); break;
    }
    
    // alter selected field
    switch (ch){
    case '=': *val += 1.0; break;
    case '+': *val += 10.0; break;
    case '-': *val -= 1.0; break;
    case '_': *val -= 10.0; break;
    }
        
    // store changed data in registry
    artPts->setArtPtData (currPart, data);

    // redisplay both windows
    glutPostRedisplay();
    currWindow = glutGetWindow();
    glutSetWindow(robotWindow);
    glutPostRedisplay();
    glutSetWindow (currWindow);
    break;

  case 27:
  case 'Q':
  case 'q':
    glutDestroyWindow (robotWindow);
    glutDestroyWindow (controlWindow);	
    exit (0);
    break;
  }
}

void controlReshape (int w, int h){
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D (0.0, w, 0.0, h);
  glMatrixMode(GL_MODELVIEW);
  
  // save the width and height
  controlWindowW = w;
  controlWindowH = h;
}
