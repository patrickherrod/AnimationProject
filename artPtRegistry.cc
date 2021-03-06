
//
// File: artPtRegistry.cc
//
// Description: Defines the class (and implementation) for registering parts
//	Original Code By: Prof. Doug Harms
//	Modified By: Pat Herrod
//	November 6, 2012
#include "artPtRegistry.h"
#include <assert.h>


artPtRegistry::artPtRegistry ()
{
  artPtCount = 0;
}

void artPtRegistry::createRegistry (osg::Node *root)
{
  // traverse tree rooted at "root" and add all articulation points to this
  // registry
  osg::Group* currGroup;
  osg::Transform *currTransform;
  osg::ref_ptr<osg::PositionAttitudeTransform> pat;

  // nothing to do if tree is null
  if (!root)
    return;

  // check to see if root is an articulation point
  if (root->getNumDescriptions()==1 &&
      (currGroup=root->asGroup()) != 0L &&
      (currTransform=currGroup->asTransform()) != 0L &&
      (pat=currTransform->asPositionAttitudeTransform()) != 0L)
    // root is an articulation point so add it to the registry
    addArtPt (pat,root->getDescription(0));

  // now traverse all children (if there are any)
  currGroup = root->asGroup();  // note:currGroup will be NULL if not a group
  if (currGroup)
    for (int i=0;i<currGroup->getNumChildren();i++)
      createRegistry (currGroup->getChild(i));
}


int artPtRegistry::addArtPt (osg::ref_ptr<osg::PositionAttitudeTransform> p,
			     const std::string &name)
{
  // add p to registry and return the registration number
  artPoints[artPtCount] = p;
  artPtName [artPtCount] = name;
  artPtDataArr [artPtCount].rotX = 0.0;
  artPtDataArr [artPtCount].rotY = 0.0;
  artPtDataArr [artPtCount].rotZ = 0.0;
  artPtDataArr [artPtCount].posX = 0.0;
  artPtDataArr [artPtCount].posY = 0.0;
  artPtDataArr [artPtCount].posZ = 0.0;
  return artPtCount++;
}

osg::ref_ptr<osg::PositionAttitudeTransform> artPtRegistry::lookupArtPt (int p)
{
  // return the ref_ptr of part p
  assert (p < artPtCount);
  return artPoints[p];
}

std::string artPtRegistry::getArtPtName (int p)
{
  assert (p < artPtCount);
  return artPtName [p];
}

void artPtRegistry::setArtPtData (int p, artPtData data)
{
  assert (p < artPtCount);
  artPtDataArr[p] = data;
  artPoints[p]->setAttitude(osg::Quat
			    (data.rotX*M_PI/180.0,osg::Vec3(1.0,0.0,0.0),
			     data.rotY*M_PI/180.0,osg::Vec3(0.0,1.0,0.0),
			     data.rotZ*M_PI/180.0,osg::Vec3(0.0,0.0,1.0)));
  artPoints[p]->setPosition(osg::Vec3(data.posX,data.posY,data.posZ));
}

artPtData artPtRegistry::getArtPtData (int p)
{
  assert (p < artPtCount);
  return artPtDataArr[p];
}

int artPtRegistry::numArtPts(void)
{
  return artPtCount;
}
