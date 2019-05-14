#ifndef MULTISPRITE2__H
#define MULTISPRITE2__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class MultiSprite2 : public Drawable {
public:
  MultiSprite2(const std::string&);
  MultiSprite2(const MultiSprite2&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { 
    return images[currentFrame]; 
  }
  int getScaledWidth()  const { 
    return getScale()*images[currentFrame]->getWidth() - 50; 
  } 
  int getScaledHeight()  const { 
    return getScale()*images[currentFrame]->getHeight(); 
  } 
  virtual const SDL_Surface* getSurface() const { 
    return images[currentFrame]->getSurface();
  }

protected:
  std::vector<Image *> images;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  virtual void advanceFrame(Uint32 ticks, int);
  MultiSprite2& operator=(const MultiSprite2&);
};
#endif
