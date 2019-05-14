#include <iostream>
#include <sstream>
#include <cmath>
//#include "ioMod.h"
#include "gameData.h"
#include "beamPool.h"

CollisionStrategy* getStrategy(const string& name) {
  std::string sName = Gamedata::getInstance().getXmlStr(name+"/strategy");
  if ( sName == "midpoint" ) return new MidPointCollisionStrategy;
  if ( sName == "rectangular" ) return new RectangularCollisionStrategy;
  if ( sName == "perpixel" ) return new PerPixelCollisionStrategy;
  throw std::string("No strategy in getStrategy");
}

BeamPool::~BeamPool() {
  delete strategy;
}

BeamPool::BeamPool(const std::string& n) :
  name(n),
  strategy( getStrategy(name) ),
  frameInterval(Gamedata::getInstance().getXmlInt(name+"/interval")),
  timeSinceLastFrame( 0 ),
  beamList(),
  freeList()
{ }

BeamPool::BeamPool(const BeamPool& b) :
  name(b.name),
  strategy(b.strategy),
  frameInterval(b.frameInterval),
  timeSinceLastFrame( b.timeSinceLastFrame ),
  beamList(b.beamList), 
  freeList(b.freeList)
{ }

bool BeamPool::collidedWith(const Drawable* obj) const {
  std::list<Beam>::iterator ptr = beamList.begin();
  while (ptr != beamList.end()) {
    if ( strategy->execute(*ptr, *obj) && !obj->getDead()) {
      freeList.push_back(*ptr);
      ptr = beamList.erase(ptr);
      return true;
    }
    ++ptr;
  }
  return false;
}

void BeamPool::shoot(const Vector2f& position, const Vector2f& velocity) {
  if (timeSinceLastFrame > frameInterval) {
    // If no beam in pool, make one:
    if ( freeList.empty() ) {
      Beam b(name);
      b.setPosition( position );
      b.setVelocity( velocity );
      b.setScale(1.5);
      beamList.push_back( b );
    }
    else {
      Beam b = freeList.front();
      freeList.pop_front();
      b.reset();
      b.setVelocity(velocity);
      b.setPosition(position);
      beamList.push_back( b );
    }
    timeSinceLastFrame = 0;
  }
}

void BeamPool::draw() const { 
  /*std::stringstream stream;
  stream << "Active beams: " << beamList.size();
  IoMod::getInstance().
    writeText(stream.str(), 500, 30);
  stream.clear();
  stream.str("");
  stream << "Beam pool: " << freeList.size();
  IoMod::getInstance().
    writeText(stream.str(), 500, 60);*/
  for ( Beam beam : beamList ) {
    beam.draw();
  }
}

void BeamPool::update(Uint32 ticks) { 
  timeSinceLastFrame += ticks;
  std::list<Beam>::iterator ptr = beamList.begin();
  while (ptr != beamList.end()) {
    ptr->update(ticks);
    if (ptr->goneTooFar()) {  // Check to see if we should free a Beam
      freeList.push_back(*ptr);
      ptr = beamList.erase(ptr);
    }   
    else ++ptr;
  }
}
