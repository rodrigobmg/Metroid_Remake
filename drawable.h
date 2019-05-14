#ifndef DRAWABLE__H
#define DRAWABLE__H
#include <SDL.h>
#include <iostream>
#include <string>
#include "vector2f.h"
#include "image.h"
#include "clock.h"
#include "ioMod.h"
 
// Drawable is an Abstract Base Class (ABC) that specifies the methods
// that derived classes may or must have.
class Drawable {
public:
  Drawable(const std::string& n, const Vector2f& pos, const Vector2f& vel): 
    name(n), position(pos), velocity(vel), scale(1.0), 
    oldPosition(Vector2f(0,0)), isDead(false), health(1) {}

  Drawable(const Drawable& s) : 
    name(s.name), position(s.position), velocity(s.velocity), 
    scale(s.scale), oldPosition(s.oldPosition), isDead(false), health(1) {}

  virtual ~Drawable() {}

  virtual void draw() const = 0;
  virtual void update(Uint32 ticks) = 0;

  float getScale() const  { return scale; }
  void  setScale(float s) { scale = s; }
  virtual int getScaledWidth() const = 0;
  virtual int getScaledHeight() const = 0;
  virtual const SDL_Surface* getSurface() const = 0;

  const std::string& getName() const { return name; }
  void setName(const std::string& n) { name = n;    }

  virtual const Image* getImage() const = 0;

  float getX() const  { return position[0]; }
  void  setX(float x) { position[0] = x;    }

  float getY() const  { return position[1]; }
  void  setY(float y) { position[1] = y;    }

  const Vector2f& getVelocity() const    { return velocity; }
  void  setVelocity(const Vector2f& vel) { velocity = vel;  }
  const Vector2f& getPosition() const    { return position; }
  void  setPosition(const Vector2f& pos) { position = pos;  }

  float getVelocityX() const   { return velocity[0]; }
  void  setVelocityX(float vx) { velocity[0] = vx;   }
  float getVelocityY() const   { return velocity[1]; }
  void  setVelocityY(float vy) { velocity[1] = vy;   }

  void setOldPosition(Vector2f n) { oldPosition = n; }
  Vector2f getOldPosition() { return oldPosition; }

  void notify(Vector2f& pos) {
    float playerx = pos[0];
    float playery = pos[1] + 15;
    Vector2f opos = getPosition();
    float x = playerx - opos[0];
    float y = playery - opos[1];
    float dis = hypot(x, y);
    
    if (dis < 300) { 
      if (x > 0) setVelocityX(fabs(getVelocityX()));
      else if (x < 0) setVelocityX(-fabs(getVelocityX()));
      if (y > 0) setVelocityY(fabs(getVelocityY())); 
      else if (y < 0) setVelocityY(-fabs(getVelocityY())); 
    }
  }

  virtual void explode() {}
  void setDead(bool d) { isDead = d; }
  bool getDead() const { return isDead; } 
  void setHealth(int h) { health = h; }
  int getHealth() { return health; }

private:
  std::string name;
  Vector2f position;
  Vector2f velocity;
  float scale;
  Vector2f oldPosition;
  bool isDead;

  int health;
};
#endif
