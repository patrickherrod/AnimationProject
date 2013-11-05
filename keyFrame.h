//
// File: keyFrame.h
//
// Author: Douglas Harms
//
// Description:  Define classes for key frames
//
#ifndef _KEYFRAME_H
#define _KEYFRAME_H
#include "artPtRegistry.h"
#include <fstream>
#include <iostream>
using namespace std;

class keyFrame {
 public:
  keyFrame(artPtRegistry *registry);
  ~keyFrame();
  void operator= (const keyFrame &src);
  void setFrame ();
  // sets the keyFrame to the current setting of the registry

  void setWorld();
  // sets the registry to the current value of the keyFrame

  void saveFrame (fstream &outfile);
  // writes the contents of keyFrame to file outfile

  void loadFrame (fstream &infile);
  // reads the next characters from infile and places them into the keyFrame.
  // No error checking is done, so the assumption is that the values in infile
  // were written by an eariler call to saveFrame.
  
  int time;            // time of this keyFrame
  artPtData *data;     // setting of all articulation points for this keyFrame

 private:
  artPtRegistry *artPts;
};

typedef keyFrame* keyFramePtr;

#endif
