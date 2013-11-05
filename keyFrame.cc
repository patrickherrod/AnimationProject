//
// File: keyFrame.cc
//
// Author: Douglas Harms
//
// Description:  Define classes for key frames
//
#include "keyFrame.h"

keyFrame::keyFrame(artPtRegistry *registry) {
  artPts = registry;

  int numParts = artPts->numArtPts();
  
  data = new artPtData [numParts];
  time = 0;
}

keyFrame::~keyFrame() {
  delete [] data;
}

void keyFrame::operator= (const keyFrame &src){
  int numArtPts = artPts->numArtPts();
  int i;
  
  time = src.time;
  for (i=0;i<numArtPts;i++)
    data[i] = src.data[i];
}
  
void keyFrame::setFrame () {
  // set the frame based on the current setting of the world
  int numArtPts = artPts->numArtPts();
  
  // go through all the articulation points in the robot
  for (int i=0;i<numArtPts;i++) {
    // get data from the ith articulation point
    data[i] = artPts->getArtPtData (i);
  }
}

void keyFrame::setWorld () {
  artPtData apd;

  // set the robot to this frame
  int numArtPts = artPts->numArtPts();

  // go through all articulation points in the robot
  for (int i=0;i<numArtPts;i++) {
    
    // copy field of the keyframe to the articulation point
    apd = data[i];

    // set the articulation point
    artPts->setArtPtData (i,apd);
  }
}

void keyFrame::saveFrame (fstream &outfile){
  // write this frame to the textfile
  int numArtPts = artPts->numArtPts();
  
  outfile << time << endl;
  
  for (int i=0; i<numArtPts;i++) {
    outfile << data[i].posX << ' ';
    outfile << data[i].posY << ' ';
    outfile << data[i].posZ << ' ';
    outfile << data[i].rotX << ' ';
    outfile << data[i].rotY << ' ';
    outfile << data[i].rotZ << ' ';
    outfile << endl;
  }
}
void keyFrame::loadFrame (fstream &infile) {
  // read this frame from the textfile
  int numArtPts = artPts->numArtPts();
  
  infile >> time;
  
  for (int i=0; i<numArtPts;i++) {
    infile >> data[i].posX;
    infile >> data[i].posY;
    infile >> data[i].posZ;
    infile >> data[i].rotX;
    infile >> data[i].rotY;
    infile >> data[i].rotZ;
  }
}
