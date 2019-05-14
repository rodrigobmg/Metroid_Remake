#include "multisprite2.h"
#include "imageFactory.h"
#include "gameData.h"

void MultiSprite2::advanceFrame(Uint32 ticks, int right) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
          currentFrame = (currentFrame+1) % (numberOfFrames/2) +
            (right*numberOfFrames/2);
	  timeSinceLastFrame = 0;
	}
}

MultiSprite2::MultiSprite2( const std::string& name ) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( ImageFactory::getInstance().getImages(name) ),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

MultiSprite2::MultiSprite2(const MultiSprite2& s) :
  Drawable(s), 
  images(s.images),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight )
  { }

MultiSprite2& MultiSprite2::operator=(const MultiSprite2& s) {
  Drawable::operator=(s);
  images = (s.images);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  return *this;
}

void MultiSprite2::draw() const { 
  images[currentFrame]->draw(getX(), getY(), getScale());
}

void MultiSprite2::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if (getVelocityX() >= 0) {
    advanceFrame(ticks, 1);
  }
  else if (getVelocityX() < 0) {
    advanceFrame(ticks, 0);
  }

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 40) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth() - 100) {
    setVelocityX( -fabs( getVelocityX() ) );
  }  

}

