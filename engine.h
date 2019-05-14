#include <vector>
#include <SDL.h>
#include "renderContext.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "hud.h"
#include "healthBar.h"
//#include "sound.h"

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();
  void switchSprite();

private:
  const RenderContext& rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World world, world2;
  Viewport& viewport;

  std::vector<Drawable*> sprites;
  std::vector<Drawable*> activeSprites;
  Player* player;
  int currentSprite;
  PerPixelCollisionStrategy* col;
  RectangularCollisionStrategy* rcol;
  Hud* hud;
  HealthBar* bar;

  bool god;
  bool gameOver;
  bool over;

  int kills, deaths;

  void spawnSprite();

  //SDLSound sound;

  static int updateCount;

  bool makeVideo;

  void draw(bool) const;
  void update(Uint32);
  void addSprites();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  void printScales() const;
  void checkForCollisions();
};
