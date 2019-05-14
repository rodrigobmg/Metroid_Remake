#include "player.h"
#include "gameData.h"
#include "imageFactory.h"

int Player::jumpHeight = 0;
int Player::deathTime = 0;
int Player::soundCount = 0;

void Player::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % images.size();
    timeSinceLastFrame = 0;
  }
}

Player::Player( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  samusRunningLeft( ImageFactory::getInstance().getImages(name+"RunningLeft") ),
  samusRunningRight( ImageFactory::getInstance().getImages(name+"RunningRight") ),
  samusJumpingLeft( ImageFactory::getInstance().getImages(name+"JumpingLeft") ),
  samusJumpingRight( ImageFactory::getInstance().getImages(name+"JumpingRight") ),
  samusBallLeft( ImageFactory::getInstance().getImages(name+"BallLeft") ),
  samusBallRight( ImageFactory::getInstance().getImages(name+"BallRight") ),
  samusBallIdle( ImageFactory::getInstance().getImages(name+"BallIdle") ),
  samusIdle( ImageFactory::getInstance().getImages(name+"Idle") ),
  samusIdleLeft( ImageFactory::getInstance().getImages(name+"IdleLeft") ),
  samusIdleRight( ImageFactory::getInstance().getImages(name+"IdleRight") ),
  samusDeathRight( ImageFactory::getInstance().getImages(name+"DeathRight") ),
  images( ImageFactory::getInstance().getImages(name) ),
  //explosion(nullptr),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  collision(false),
  initialVelocity(getVelocity()),
  observers(),
  activeObservers(),
  //oldPosition(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
  //            Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
  jumping(false),
  ball(false),
  isLeft(false),
  beamName( Gamedata::getInstance().getXmlStr(name+"/beam") ),
  beams( beamName ),
  minSpeed( Gamedata::getInstance().getXmlInt(beamName+"/speedX") ),
  sound()
{ }

Player::Player(const Player& s) :
  Drawable(s), 
  samusRunningLeft(s.samusRunningLeft),
  samusRunningRight(s.samusRunningRight),
  samusJumpingLeft(s.samusJumpingLeft),
  samusJumpingRight(s.samusJumpingRight),
  samusBallLeft(s.samusBallLeft),
  samusBallRight(s.samusBallRight),
  samusBallIdle(s.samusBallIdle),
  samusIdle(s.samusIdle),
  samusIdleLeft(s.samusIdleLeft),
  samusIdleRight(s.samusIdleRight),
  samusDeathRight(s.samusDeathRight),
  images(s.images),
  //explosion(nullptr),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  collision(s.collision),
  initialVelocity( s.initialVelocity ),
  observers(s.observers),
  activeObservers(s.activeObservers),
  //oldPosition(s.oldPosition),
  jumping(s.jumping),
  ball(s.ball),
  isLeft(s.isLeft),
  beamName(s.beamName),
  beams(s.beams),
  minSpeed(s.minSpeed),
  sound()
  { }

void Player::draw() const { 
  //if (explosion) explosion->draw();
  beams.draw();
  images[currentFrame]->draw(getX(), getY(), getScale());
}

void Player::stop() { 
  setVelocity( Vector2f(0, 0) );
}

void Player::right() { 
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
  }
  isLeft = false;
} 
void Player::left()  { 
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
  }
  isLeft = true;
} 
void Player::up()    { 
  if ( getY() > 0) {
    setVelocityY(-initialVelocity[1]);
    jumping = true;
  }
} 
void Player::down()  { 
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
  }
}

/*void Player::explode() {
  Sprite s(getName(), getPosition(), getVelocity(), images[currentFrame]);
  if (!explosion) explosion = new ExplodingSprite(s);
}*/

void Player::update(Uint32 ticks) { 
  /*if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }*/

  beams.update(ticks);
  if (getDead() && deathTime == 0) {
    images = samusDeathRight;
    currentFrame = 0;
    deathTime++;
  }
  else if (getDead() && deathTime < 35) {
    images = samusDeathRight;
    deathTime++;
  }
  else if (jumping && isLeft) {
    images = samusJumpingLeft;
    if (soundCount > 11) {
      sound[4];
      soundCount = 0;
    }
  }
  else if (jumping && !isLeft) {
    images = samusJumpingRight;
    if (soundCount > 11) {
      sound[4];
      soundCount = 0;
    }
  }
  else if (ball && getVelocityX() == 0) images = samusBallIdle;
  else if (ball && getVelocityX() < 0) {
    images = samusBallLeft;
    if (soundCount > 9) {
      sound[5];
      soundCount = 0;
    }
  }
  else if (ball && getVelocityX() > 0) {
    images = samusBallRight;
    if (soundCount > 9) {
      sound[5];
      soundCount = 0;
    }
  }
  //else if (getVelocityX() == 0) images = samusIdle;
  else if (getVelocityX() == 0 && isLeft) images = samusIdleLeft;
  else if (getVelocityX() == 0 && !isLeft) images = samusIdleRight;
  else if (getVelocityX() > 0) {
    images = samusRunningRight;
    if (soundCount > 27) {
      sound[2];
      soundCount = 0;
    }
  }
  else if (getVelocityX() < 0) {
    images = samusRunningLeft;
    if (soundCount > 27) {
      sound[2];
      soundCount = 0;
    }
  }
  
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  int newY = getY();
  if (!getDead()) {
    if (jumping && jumpHeight < 20) {
      newY -= 1;
      setY(newY);
      //setVelocityY(-initialVelocity[1]);
      jumpHeight++;
      //std::cout << "jump" << std::endl;
    }
    else {
      newY += 4;
      setY(newY);
      //setVelocityY(initialVelocity[1]);
      //std::cout << "no jump" << std::endl;
    }
  }
  stop();
  soundCount++;
}

void Player::setPassiveObservers(std::vector<Drawable*>& obs) {
  observers = obs;
}

void Player::setActiveObservers(std::vector<Drawable*>& obs) {
  activeObservers = obs;
}

void Player::addActiveObservers(Drawable* obs) {
  activeObservers.emplace_back(obs);
}

void Player::notifyActiveObservers() {
  Vector2f pos = getPosition();
  for (auto i : activeObservers) {
    i->notify(pos);
  }
}

/*void Player::setOldPosition(Vector2f n) {
  oldPosition = n;
}

Vector2f Player::getOldPosition() {
  return oldPosition;
}*/

void Player::setJump(bool t) {
  jumping = t;
}

void Player::setBall(bool t) {
  ball = t;
}

bool Player::getBall() {
  return ball;
}

void Player::shoot() {
  sound[0];
  if (!ball) {
    int dir = 1;
    if (isLeft) {
      dir = -1;
    }
    float x = getX()+getImage()->getWidth() + (dir * 10);
    float y = getY()+getImage()->getHeight()/2.2;
    // I'm not adding minSpeed to y velocity:
    beams.shoot( Vector2f(x, y), 
    // Vector2f((minSpeed+getVelocityX()) * dir, 0)
    Vector2f(minSpeed * dir, 0));
  }
}

bool Player::collidedWith(const Drawable* obj) const {
  return beams.collidedWith( obj );
}

void Player::playSound(int s) {
  sound[s];
}

void Player::toggleMusic() {
  sound.toggleMusic();
}
