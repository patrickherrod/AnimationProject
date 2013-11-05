//
// File: graphUtils.h
//
// Author: Douglas Harms
//
// Description:  This file contains various utility functions for graphics
//

#ifndef _GRAPHUTILS_H
#define _GRAPHUTILS_H
#include <GL/glut.h>
#include <math.h>

// define useful types
typedef GLfloat point[3];
typedef GLfloat point2[2];

// prototype useful functions
void displayText (GLfloat startx, GLfloat starty, GLfloat startz,
		  GLfloat basex,  GLfloat basey,  GLfloat basez,
		  GLfloat upx,    GLfloat upy,    GLfloat upz,
		  float scale, const char s[]);
// draw string s, with lower left corner at point (startx,starty,startz),
// along base line with vector (basex,basey,basez).  The up vector 
// (upx,upy,upz) and base vector define the plane the text will be 
// rendeered in.  scale is the scale factor.

void displayText(GLfloat startx, GLfloat starty, GLfloat startz,
		 GLfloat scale, const char s[]);
// draw string s, with lower left corner at point (startx,starty,startz)
// along a line parallel with the x axis, parallel to the x-y plane,
// and up toward the y-axis.

void displayBitmapText (float x, float y, float z, const char *s);
// displays string s starting at (x,y,z) using 8x13 raster font

void drawArc(float r, float theta, int increments=10);
// draws a theta degree arc of radius r from the x-axis toward the y-axis

void drawVector (float px, float py, float pz, float vx, float vy, float vz);
// draws vector v from P

void drawSphere (float theta,int NumSlices,int numStacks);
// draws a partial sphere through theta degrees from the x axis using
// numSlices and numStacks elements

#endif
