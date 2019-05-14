#include "player.h"

int Player::frameCount = 0;
Player::Player( const std::string& name) :
  MultiSprite2(name),
  collision(false),
  initialVelocity(getVelocity()),
  observers(),
  activeObservers()
{ }

Player::Player(const Player& s) :
  MultiSprite2(s), 
  collision(s.collision),
  initialVelocity(s.getVelocity()),
  observers(s.observers),
  activeObservers(s.activeObservers)
  { }

Player& Player::operator=(const Player& s) {
  MultiSprite2::operator=(s);
  collision = s.collision;
  initialVelocity = s.initialVelocity;
  return *this;
}

void Player::stop() { 
  setVelocity( Vector2f(0, 0) );
}

void Player::right() { 
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
  }
} 
void Player::left()  { 
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
  }
} 
void Player::up()    { 
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1] );
  }
} 
void Player::down()  { 
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
  }
}

void Player::update(Uint32 ticks) {
  //if ( !collision ) MultiSprite2::update(ticks);
  if (!collision) {
    Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
    setPosition(getPosition() + incr);

    if (getVelocityX() > 0) {
      advanceFrame(ticks, 1, 0);
    }
    else if (getVelocityX() < 0) {
      advanceFrame(ticks, 0, 0);
    }
    else {
      advanceFrame(ticks, 0, 1);
    }

    if ( getY() < 0) {
      setVelocityY( fabs( getVelocityY() ) );
    }
    if ( getY() > worldHeight-getScaledHeight()) {
      setVelocityY( -fabs( getVelocityY() ) );
    }

    if ( getX() < 40) {
      setVelocityX( 0 );
    }
    if ( getX() > worldWidth-getScaledWidth() - 100) {
      setVelocityX( 0 );
    }  
  }
  stop();
}

void Player::advanceFrame(Uint32 ticks, int right, int stand) {
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval) {
    if (stand == 1) {
      currentFrame = (currentFrame+1) % (numberOfFrames/4)
        + (numberOfFrames/4) + (stand * numberOfFrames/4);
    }
    else if (right == 1) {
      currentFrame = (currentFrame+1) % (numberOfFrames/4)
        + (right * numberOfFrames/4);
    }
    else {
      currentFrame = (currentFrame+1) % (numberOfFrames/4);
    }
    timeSinceLastFrame = 0;
  }
//right++;
}

void Player::setPassiveObservers(std::vector<Drawable*>& obs) {
  observers = obs;
}

void Player::setActiveObservers(std::vector<Drawable*>& obs) {
  activeObservers = obs;
}

void Player::notifyActiveObservers() {
  Vector2f pos = getPosition();
  for (auto i : activeObservers) {
    i->notify(pos);
  }
}
