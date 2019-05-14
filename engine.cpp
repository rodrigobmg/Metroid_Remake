#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "multisprite2.h"
#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"

int Engine::updateCount = 0;
Engine::~Engine() { 
  for (Drawable* i : sprites) delete i;
  for (Drawable* i : activeSprites) delete i;
  delete player;
  delete col;
  delete rcol;
  delete hud;
  delete bar;
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc.getRenderer() ),
  world("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  world2("back2", Gamedata::getInstance().getXmlInt("back2/factor") ),
  //world3("back3", Gamedata::getInstance().getXmlInt("back3/factor") ),
  viewport( Viewport::getInstance() ),
  sprites(),
  activeSprites(),
  player(new Player("Samus")),
  currentSprite(0),
  col(new PerPixelCollisionStrategy),
  rcol(new RectangularCollisionStrategy),
  hud(new Hud("Hud")),
  bar(new HealthBar("HealthBar")),
  god(false),
  gameOver(false),
  over(false),
  kills(0),
  deaths(0),
  //sound(),
  makeVideo( false )
{
  addSprites();
  player->setScale(1.75);
  Viewport::getInstance().setObjectToTrack(player);
  player->setVelocityX(0);
  player->setVelocityY(0);
  player->setPassiveObservers(sprites);
  std::cout << "Loading complete" << std::endl;
}

void Engine::spawnSprite() {
  if (sprites.size() == 13) {
    sprites.emplace_back(new Sprite("SmallFloor"));   //13
    sprites.emplace_back(new Sprite("SmallFloor"));   //14
    sprites.emplace_back(new Sprite("SmallFloor"));   //15
    for (unsigned int i = 13; i < 16; i++) {
      sprites[i]->setPosition(Vector2f((i % 2) ? 55 : 343, 
                                        1170 - (i - 11) * 110));
    }
    sprites.emplace_back(new Sprite("SmallFloor"));   //16
    sprites.emplace_back(new Sprite("SmallFloor"));   //17
    sprites.emplace_back(new Sprite("SmallFloor"));   //18
    sprites.emplace_back(new Sprite("SmallFloor"));   //19
    sprites.emplace_back(new Sprite("SmallFloor"));   //20
    sprites.emplace_back(new Sprite("SmallFloor"));   //21
    for (unsigned int i = 16; i < sprites.size() - 1; i++) {
      sprites[i]->setPosition(Vector2f((i % 2) ? 1593 : 1885, 
                                        1140 - (i - 14) * 110));
    }
    sprites[21]->setPosition(Vector2f(1593, 350));
    for (auto *i : sprites) i->setScale(2.00);

    sprites.emplace_back(new Sprite("Ship"));         //22
    sprites[22]->setScale(1.50);
  }
  else if (gameOver) {
    sprites.emplace_back(new Sprite("BabyMetroid"));  //12
    sprites[12]->setScale(.40);
  }
  else {
  srand(time(NULL));
  if (rand() % 2) {
    activeSprites.emplace_back(new Sprite("Guy"));
    activeSprites[activeSprites.size() - 1]->setScale(2.00);
  }
  else {
    activeSprites.emplace_back(new MultiSprite("FlyingGuy"));
    activeSprites[activeSprites.size() - 1]->setScale(3.00);
  }

  bool xsign, ysign;
  int x, y;
  xsign = rand() % 2;
  ysign = rand() % 2;
  x = rand() % 400 + 100;
  if (!xsign) x = -x;
  y = rand() % 400 + 100;
  if (!ysign) y = -y;
  activeSprites[activeSprites.size() - 1]->setVelocityX(x);
  activeSprites[activeSprites.size() - 1]->setVelocityY(y);
  player->addActiveObservers(activeSprites[activeSprites.size() - 1]);
  }
}

void Engine::addSprites() {
  activeSprites.emplace_back(new Sprite("MBrain"));
  sprites.emplace_back(new Sprite("Floor"));        //0
  sprites.emplace_back(new Sprite("TopFloor"));     //1
  sprites.emplace_back(new Sprite("SmallFloor"));   //2
  sprites.emplace_back(new Sprite("SmallFloor"));   //3
  sprites.emplace_back(new Sprite("SmallFloor"));   //4
  sprites.emplace_back(new Sprite("SmallFloor"));   //5
  sprites.emplace_back(new Sprite("MidFloor"));     //6
  for (unsigned int i = 2; i < 6; i++) {
    sprites[i]->setPosition(Vector2f((i % 2) ? 1170 : 1470, 1130 - i * 100));
  }
  sprites.emplace_back(new Sprite("MedWall"));      //7
  sprites.emplace_back(new Sprite("MedWall"));      //8
  sprites[8]->setPosition(Vector2f(1130, 660));
  sprites.emplace_back(new Sprite("SmallWall"));    //9
  sprites.emplace_back(new Sprite("Wall"));         //10
  sprites.emplace_back(new Sprite("Wall"));         //11
  sprites[11]->setPosition(Vector2f(1940, 0));
  for (auto i : sprites) i->setScale(2);
  activeSprites[0]->setScale(2.50);
  activeSprites[0]->setVelocity(Vector2f(0,0));
  activeSprites[0]->setHealth(100);
}

void Engine::draw(bool h) const {
  world.draw();
  world2.draw();

  for (Drawable* i : sprites) i->draw();
  for (Drawable* i : activeSprites) i->draw();
  player->draw();
  //world3.draw();
  if (h) hud->draw();
  bar->draw();

  SDL_Color c = {255, 0, 0, 255};
  std::stringstream congrats("Congratulations, you saved the baby Metroid!");
  std::stringstream k;
  k << "Total kills: " << kills;
  std::stringstream d;
  d << "Total deaths: " << deaths;
  if (over) {
    io.writeText(congrats.str(), 200, 300, c);
    io.writeText(k.str(), 200, 330, c);
    io.writeText(d.str(), 200, 360, c);
    clock.pause();
  }
  std::stringstream name("William Sease");
  io.writeText(name.str(), 25, 450, c);

  //viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  for (Drawable* i : sprites) i->update(ticks);
  for (Drawable* i : activeSprites) i->update(ticks);
  player->update(ticks);

//Detecting collisions player and enemies.
  auto it = activeSprites.begin();
  while (it != activeSprites.end()) {
    if (col->execute(*player, **it) && !god) {
      if (!static_cast<Sprite*>(*it)->getDead()) bar->update(ticks);
    }
    it++;
  }
  
  if (clock.getTicks() % 10 == 0) player->notifyActiveObservers();
  updateCount++;
  
//Detecting collisions between enemies and beams.
  for (auto* i : activeSprites) {
    if (i->getHealth() < 1 && !i->getDead()) {
      //player->playSound(1);
      i->explode();
      static_cast<Sprite*>(i)->setDead(true);
      kills++;
    }
    else if (player->collidedWith(i)) {
      i->setHealth(i->getHealth() - 3);
    }
  }
  
//Detecting collisions with walls.
  for (unsigned int i = 7; i < 12; i++) {
    if (player->collidedWith(sprites[i])) {}
    else if (rcol->execute(*player, *sprites[i])) { 
      player->setX(player->getOldPosition()[0]);
    }
    /*for (auto *s : activeSprites) {
      if (rcol->execute(*s, *sprites[i])) {
        s->setX(s->getOldPosition()[0]);
      }
    }*/
  }
//Detecting collisions with floor.
  for (unsigned int i = 0; i < 7; i++) {
    if (player->collidedWith(sprites[i])) {}
    if (rcol->execute(*player, *sprites[i])) {
      player->setY(player->getOldPosition()[1]);
      player->setJump(false);
      player->jumpHeight = 0;
    }
  }

  /*if (rcol->execute(*player, *sprites[2])) {
    player->setY(player->getOldPosition()[1]);
  }*/
  //Fancy collision detection not working yet.
  /*int c = col->execute2(*player, *sprites[0]);
  if (c == 1 || c == 2) player->setPosition(player->getOldPosition());
  else if (c == 3) {
    player->setY(player->getOldPosition()[1]);
    player->setJump(false);
    player->jumpHeight = 0;
  }
  else if (c == 4) {
    player->setPosition(player->getOldPosition());
    player->setJump(false);
    player->jumpHeight = 0;
  }
  else player->setOldPosition(Vector2f(player->getX(), player->getY()));*/

//For making enemies collide with walls and floors.
  /*for (auto *i : activeSprites) {
    i->setOldPosition(Vector2f(i->getX(), i->getY()));
  }*/
  
  if (bar->getHealth() == 0) {
    player->playSound(6);
    player->setDead(true);
    //player->setPosition(player->getOldPosition());
    if (player->deathTime == 35) {
      bar->reset();
      player->deathTime = 0;
      if (!gameOver) player->setPosition(Vector2f(150, 200));
      else player->setPosition(Vector2f(700, 1000));
      deaths++;
      player->setDead(false);
    }
  }

//Boss battle stuff
  srand(time(NULL));
  if (activeSprites[0]->getDead() && !gameOver) {
    player->playSound(7);
    gameOver = true;
    spawnSprite();
  }
  else if ((clock.getTicks() % 100 == 0 || clock.getTicks() % 33 == 0) && 
           activeSprites[0]->getHealth() < 33 && !gameOver) {
    spawnSprite();
  }
  else if ((clock.getTicks() % 200 == 0 || clock.getTicks() % 66 == 0) && 
           activeSprites[0]->getHealth() < 66 && !gameOver) {
    spawnSprite();
  }
  else if (clock.getTicks() % 200 == 0 && !gameOver) {
    spawnSprite();
  }

//Conclusion stuff
  if (sprites.size() == 13) {
    if (sprites[12]->getDead()) spawnSprite();
    if (rcol->execute(*player, *sprites[12])) {
      sprites[12]->setDead(true);
    }
  }
  else if (sprites.size() > 13) {
    for (unsigned int i = 13; i < sprites.size() - 1; i++) {
      if (player->collidedWith(sprites[i])) {}
      if (rcol->execute(*player, *sprites[i])) {
        player->setY(player->getOldPosition()[1]);
        player->setJump(false);
        player->jumpHeight = 0;
      }
    }
    if (col->execute(*player, *sprites[22])) {
      over = true;
    }
  }
  player->setOldPosition(Vector2f(player->getX(), player->getY()));

  world.update();
  world2.update();
  //world3.update();
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  currentSprite++;
  int size = activeSprites.size();
  if ( currentSprite >= size ) {
    currentSprite = -1;
    Viewport::getInstance().setObjectToTrack(player);
  }
  else {
    Viewport::getInstance().setObjectToTrack(activeSprites[currentSprite]);
  }
}

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  bool help = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_SPACE] ) {
          //sound[0];
          player->shoot();
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if ( keystate[SDL_SCANCODE_G] ) {
          god = (god) ? false : true;
        }
        if ( keystate[SDL_SCANCODE_R] ) {
          clock.unpause();
          return true;
        }
        if ( keystate[SDL_SCANCODE_M] ) {
          player->toggleMusic();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
        if (keystate[SDL_SCANCODE_F1] && !help) {
          help = true;
        }
        else if (keystate[SDL_SCANCODE_F1] && help) {
          help = false;
        }
        if (keystate[SDL_SCANCODE_S]) {
          //static_cast<Player*>(player)->down();
          if (player->getBall()) player->setBall(false);
          else player->setBall(true);
          player->playSound(3);
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        static_cast<Player*>(player)->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        static_cast<Player*>(player)->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        if (player->jumpHeight < 20) {
          static_cast<Player*>(player)->up();
        }
        player->setJump(true);
        player->setBall(false);
      }
      draw(help);
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
  return false;
}
