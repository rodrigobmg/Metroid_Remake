#ifndef PLAYER__H
#define PLAYER__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "beamPool.h"
#include "sound.h"

class Sprite;
//class ExplodingSprite;

class Player : public Drawable {
public:
  Player(const std::string&);
  Player(const Player&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { 
    return images[currentFrame]; 
  }
  int getScaledWidth()  const { 
    return getScale()*images[currentFrame]->getWidth(); 
  } 
  int getScaledHeight()  const { 
    return getScale()*images[currentFrame]->getHeight(); 
  } 
  virtual const SDL_Surface* getSurface() const { 
    return images[currentFrame]->getSurface();
  }
  std::vector<Drawable*>& getActiveObservers() { return activeObservers; }
  void notifyActiveObservers();
  void setPassiveObservers(std::vector<Drawable*>&);
  void setActiveObservers(std::vector<Drawable*>&);
  void addActiveObservers(Drawable*);
  //void setOldPosition(Vector2f);
  //Vector2f getOldPosition();

  void right();
  void left();
  void up();
  void down();
  void stop();

  void setJump(bool);
  void setBall(bool);
  bool getBall();
  static int jumpHeight;
  static int deathTime;

  void shoot();
  bool collidedWith(const Drawable*) const;

  //virtual void explode();
  void playSound(int);
  void toggleMusic();

private:
  std::vector<Image *> samusRunningLeft;
  std::vector<Image *> samusRunningRight;
  std::vector<Image *> samusJumpingLeft;
  std::vector<Image *> samusJumpingRight;
  std::vector<Image *> samusBallLeft;
  std::vector<Image *> samusBallRight;
  std::vector<Image *> samusBallIdle;
  std::vector<Image *> samusIdle;
  std::vector<Image *> samusIdleLeft;
  std::vector<Image *> samusIdleRight;
  std::vector<Image *> samusDeathRight;
  std::vector<Image *> images;
  //ExplodingSprite* explosion;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  bool collision;
  Vector2f initialVelocity;
  std::vector<Drawable*> observers;
  std::vector<Drawable*> activeObservers;

  //Vector2f oldPosition;

  bool jumping;
  bool ball;
  bool isLeft;

  std::string beamName;
  BeamPool beams;
  float minSpeed;

  SDLSound sound;
  static int soundCount;

  void advanceFrame(Uint32 ticks);
  Player& operator=(const Player&);
};
#endif
