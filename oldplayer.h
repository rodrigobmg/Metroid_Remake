#ifndef PLAYER__H
#define PLAYER__H

#include "multisprite2.h"

// In this example the player is derived from MultiSprite.
// However, there are many options.
class Player : public MultiSprite2 {
public:
  Player(const std::string&);
  Player(const Player&);
  virtual void update(Uint32 ticks);
  virtual void advanceFrame(Uint32 ticks, int, int);

  void collided() { collision = true; }
  void missed() { collision = false; }
  std::vector<Drawable*>& getActiveObservers() { return activeObservers; }
  void notifyActiveObservers();
  void setPassiveObservers(std::vector<Drawable*>&);
  void setActiveObservers(std::vector<Drawable*>&);
  Player& operator=(const Player&);

  void right();
  void left();
  void up();
  void down();
  void stop();
  
  static int frameCount;
private:
  bool collision;
  Vector2f initialVelocity;
  std::vector<Drawable*> observers;
  std::vector<Drawable*> activeObservers;
};
#endif
