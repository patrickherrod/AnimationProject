//
// File: graphUtils.cc
//
// Author: Douglas Harms
//
// Description:  This file contains various utility functions for graphics
//

#include "graphUtils.h"

void displayText (GLfloat startx, GLfloat starty, GLfloat startz,
		  GLfloat basex,  GLfloat basey,  GLfloat basez,
		  GLfloat upx,    GLfloat upy,    GLfloat upz,
		  float scale, const char s[])
  
// draw string s, with lower left corner at point (startx,starty,startz),
// along base line with vector (basex,basey,basez).  The up vector 
// (upx,upy,upz) and base vector define the plane the text will be 
// rendeered in.  scale is the scale factor.
{
  float baseLength;
  float d,d2;
  float uy,uz;
  GLfloat M[16];
  int i;
  const float epsilon=1.0e-3;
  
  // make base vector a unit vector
  baseLength = sqrt(basex*basex + basey*basey + basez*basez);
  basex /= baseLength;
  basey /= baseLength;
  basez /= baseLength;
  
  // calculate length of projection of base onto xz plane
  d = sqrt(basex*basex + basez*basez);
  
  // calculate transformation matrix, which depends on whether the base vector
  // is parallel to the y axis.
  if (d>=epsilon)
    {
      // base not parallel to y-axis
      // calculate up vector components after rotation about y and z axes
      uy = d*upy - (basex*basey*upx + basez*basey*upz)/d;
      uz = (basex*upz - basez*upx)/d;
      
      // calculate length pf projection of u onto zy plane
      d2 = sqrt(uy*uy + uz*uz);
      
      // define transformation matrix
      M[0] = basex;
      M[1] = basey;
      M[2] = basez;
      M[3] = 0.0;
      
      M[4] = -(basez*uz + basex*basey*uy)/(d*d2);
      M[5] = (uy*d)/d2;
      M[6] = (basex*uz - basez*basey*uy)/(d*d2);
      M[7] = 0.0;
      
      M[8] = (basex*basey*uz - basez*uy)/(d*d2);
      M[9] = (-uz*d)/d2;
      M[10] = (basez*basey*uz + basex*uy)/(d*d2);
      M[11] = 0.0;
      
      M[12] = startx;
      M[13] = starty;
      M[14] = startz;
      M[15] = 1.0;
    }
  else
    {
      // base is parallel to y-axis
      
      // calculate length of up vector projected onto xz plane
      d2 = sqrt(upx*upx + upz*upz);
      
      // define transformation matrix
      M[0] = 0.0;
      M[1] = 1.0;
      M[2] = 0.0;
      M[3] = 0.0;
      
      M[4] = upx/d2;
      M[5] = 0.0;
      M[6] = upz/d2;
      M[7] = 0.0;
      
      M[8] = upz/d2;
      M[9] = 0.0;
      M[10] = -upx/d2;
      M[11] = 0.0;
      
      M[12] = startx;
      M[13] = starty;
      M[14] = startz;
      M[15] = 1.0;
    }
  
  // add M to the view matrix after saving current matrix
  glPushMatrix();
  glMultMatrixf(M);
  
  // set the scale
  glScalef(scale, scale, scale);
  
  // render the string
  i=0;
  while (s[i])
    glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,s[i++]);
  
  // restore matrix
  glPopMatrix();
}

void displayText(GLfloat startx, GLfloat starty, GLfloat startz,
		 GLfloat scale, const char s[])
// draw string s, with lower left corner at point (startx,starty,startz)
// along a line parallel with the x axis, parallel to the x-y plane,
// and up toward the y-axis.
{
  int i;
  // translate and scale appropriately
  glPushMatrix();
  glTranslatef(startx,starty,startz);
  glScalef(scale,scale,scale);

  // draw the string
  i=0;
  while (s[i])
    glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,s[i++]);

  // restore transformation and return
  glPopMatrix();
}

void displayBitmapText (float x, float y, float z, const char *s)
{
  // displays string s starting at (x,y,z) using 8x13 raster font
  
  GLboolean posValid;
  GLboolean lightingEnabled;
  int i=0;
  
  // make sure lighting is disabled, after saving its current value
  lightingEnabled = glIsEnabled (GL_LIGHTING);
  if (lightingEnabled)
    glDisable (GL_LIGHTING);
  
  // draw the string
  glRasterPos3f (x,y,z);
  glGetBooleanv (GL_CURRENT_RASTER_POSITION_VALID, &posValid);
  while (s[i] && posValid)
    {
      glutBitmapCharacter(GLUT_BITMAP_8_BY_13, s[i++]);
      glGetBooleanv (GL_CURRENT_RASTER_POSITION_VALID, &posValid);
    }
  
  // reenable lighting if necessary
  if (lightingEnabled)
    glEnable (GL_LIGHTING);
}

void drawArc(float r, float theta, int increments)
{
  // draws a theta degree arc of radius r from the x-axis toward the y-axis
  
  float delta;
  
  // translate theta from degrees to radians
  theta = theta/180.0*M_PI;
  
  // determine delta theta
  delta = theta/increments;
  
  // draw the arc
  glBegin(GL_LINE_STRIP);
  for (;theta>0.0;theta-=delta)
    glVertex3f(r*cos(theta),r*sin(theta),0.0);
  glVertex3f(r, 0.0, 0.0);
  glEnd();
}

void drawVector (float px, float py, float pz, float vx, float vy, float vz)
// draws vector v from P
{
  GLfloat I[16];			// will be the identity matrix
  GLfloat M[16];			// a transformation matrix
  
  int i;
  float d;
  float length;
  
  // initialize I
  for (i=0;i<16;i++)
    I[i] = 0.0;
  I[0] = 1.0;
  I[5] = 1.0;
  I[10] = 1.0;
  I[15] = 1.0;
  
  // save current matrix
  glPushMatrix();
  
  // translate by (px, py, pz)
  glTranslatef (px, py, pz);
  
  // calculate some constants
  length = sqrt(vx*vx + vy*vy + vz*vz);
  d = sqrt(vx*vx + vz*vz);
  
  // define y-axis rotation and add it in
  for (i=0; i<16;i++)
    M[i] = I[i];
  if (d != 0.0)
    {
      M[0] = M[10] = vx/d;
      M[2] = vz/d;
      M[8] = -vz/d;
    }
  glMultMatrixf (M);
  
  // define z-axis rotation and add it in
  for (i=0; i<16;i++)
    M[i] = I[i];
  M[0] = M[5] = d/length;
  M[1] = vy/length;
  M[4] = -vy/length;
  
  glMultMatrixf (M);
  
  // draw the vector along the x-axis
  
  
  glBegin(GL_LINES);
  glVertex3f (0.0, 0.0, 0.0);
  glVertex3f (length, 0.0, 0.0);
  glVertex3f (length, 0.0, 0.0);
  glVertex3f (length-0.03, 0.03, 0.0);
  glVertex3f (length, 0.0, 0.0);
  glVertex3f (length-0.03, -0.03, 0.0);
  glEnd();
  
  // restore the matrix
  glPopMatrix();
}

void drawSphere (float theta,int numSlices,int numStacks)
// draws a partial sphere through theta degrees from the x axis using
// numSlices and numStacks elements
{
  const int MAX_STACKS=100;
  float deltaTheta=theta/numSlices;
  //float deltaZ = 2.0/numStacks;
  float deltaPhi = M_PI/numStacks;	  // note this is in radians
  float phi;
  float x[MAX_STACKS],y[MAX_STACKS],z[MAX_STACKS];
  float x1,y1,x2,y2;
  int i,j;
  const float S=sin(deltaTheta/180.0*M_PI);
  const float C=cos(deltaTheta/180.0*M_PI);
  
  // calculate coordinates of first slice (theta=0)
  x[0] = x[numStacks] = 0.0;
  y[0] = y[numStacks] = 0.0;
  z[0] = 1.0;
  z[numStacks] = -1.0;
  phi = M_PI/2.0;
  for (i=1;i<numStacks;i++)
    {  phi -= deltaPhi;
      z[i] = sin(phi);
      // z[i] = z[i-1] + deltaZ;
      x[i] = sqrt(1-z[i]*z[i]);
      y[i] = 0.0;
    }
  
  // normalize all normal vectors
  glEnable (GL_NORMALIZE);
  
  // draw all slices
  for (j=0;j<numSlices;j++)
    {	// initialize for first stack
      x1 = y1 = 0.0;
      // draw all stacks of next slice
      for (i=0;i<numStacks;i++)
	{	// left coordinates are [i] and [i+1]
	  // right coordinates are (x1,y1,z[i]) and (x2,y2,x[i+1])
	  // calculate (x2,y2)
	  x2 = x[i+1]*C - y[i+1]*S;
	  y2 = x[i+1]*S + y[i+1]*C;
	  
	  // draw the polygon, with normals at each vertex
	  glBegin (GL_POLYGON);
	  glNormal3f (x[i],y[i],z[i]);
	  glVertex3f (x[i],y[i],z[i]);
	  glNormal3f (x[i+1],y[i+1],z[i+1]);
	  glVertex3f (x[i+1],y[i+1],z[i+1]);
	  glNormal3f (x2,y2,z[i+1]);
	  glVertex3f (x2,y2,z[i+1]);
	  glNormal3f (x1,y1,z[i]);
	  glVertex3f (x1,y1,z[i]);
	  glEnd();
	  
	  // set ith element for next slice
	  x[i] = x1;
	  y[i] = y1;
	  
	  // set x1 and y1 for next stack
	  x1 = x2;
	  y1 = y2;
	}
    }
}

