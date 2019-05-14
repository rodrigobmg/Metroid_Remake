#include "SDL.h"
#include "gameData.h"
#include "drawable.h"

class Hud : public Drawable {
public:
  Hud(const std::string&);
  void draw() const;
  void update(Uint32);
  //void reset() { currentLength = barRect.w; }

  Hud(const Hud&) = delete;
  Hud& operator=(const Hud&) = delete;

  virtual int getScaledWidth() const  { return barRect.w; }
  virtual int getScaledHeight() const { return barRect.h; }
  virtual const Image* getImage() const { 
    throw std::string("No image in Hud");
  }
  virtual const SDL_Surface* getSurface() const {
    throw std::string("No surface in Hud");
  }
private:
  SDL_Renderer* renderer;
  SDL_Rect barRect;
  int deltaTime;
  SDL_Color barColor;
  SDL_Color backColor;
  IoMod& io;
  Clock& clock;
};
