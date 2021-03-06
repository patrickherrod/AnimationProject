//
// File: artPtRegistry.h
//
// Description: Defines the class (and implementation) for registering parts
//	Original Code By: Prof. Doug Harms
//	Modified By: Pat Herrod
//	November 6, 2012
#include <string>
#include <osg/PositionAttitudeTransform>
#ifndef _ART_PT_REGISTRY_H_
#define _ART_PT_REGISTRY_H_

class artPtData {
 public:
 artPtData():rotX(0),rotY(),rotZ(),posX(),posY(),posZ() {}
  float rotX,rotY,rotZ,posX,posY,posZ;
};

class artPtRegistry {
 public:
  artPtRegistry();
  void createRegistry (osg::Node *root);
  osg::ref_ptr<osg::PositionAttitudeTransform> lookupArtPt (int p);
  std::string getArtPtName (int p);
  void setArtPtData (int p, artPtData data);
  artPtData getArtPtData (int p);
  int numArtPts(void);
  
 private:
  int addArtPt (osg::ref_ptr<osg::PositionAttitudeTransform> p,
		const std::string &name);
  osg::ref_ptr<osg::PositionAttitudeTransform> artPoints[100];
  artPtData artPtDataArr [100];
  std::string artPtName [100];
  int artPtCount;
};

#endif
