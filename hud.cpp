#include<sstream>
#include "hud.h"
#include "renderContext.h"

Hud::Hud(const std::string& name) :  
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(
                    Gamedata::getInstance().getXmlInt(name+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(name+"/speedY")) 
           ),
  renderer(RenderContext::getInstance().getRenderer()),
  barRect( { Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
             Gamedata::getInstance().getXmlInt(name+"/startLoc/y"),
             Gamedata::getInstance().getXmlInt(name+"/width"),
             Gamedata::getInstance().getXmlInt(name+"/height")}
  ),
  deltaTime(0),
  barColor(
    {(Uint8)Gamedata::getInstance().getXmlInt(name+"/barColor/r"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/barColor/g"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/barColor/b"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/barColor/a")}
  ),
  backColor(
    {(Uint8)Gamedata::getInstance().getXmlInt(name+"/backColor/r"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/backColor/g"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/backColor/b"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/backColor/a")}
  ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() )
{}

void Hud::update(Uint32 ticks) {
  deltaTime += ticks;
}

void Hud::draw() const {

  // First set the blend mode so that alpha blending will work;
  // the default blend mode is SDL_BLENDMODE_NONE!
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor( 
    renderer, backColor.r, backColor.g, backColor.b, backColor.a );
  // Draw the  background
  SDL_RenderFillRect( renderer, &barRect );

  SDL_SetRenderDrawColor( 
    renderer, barColor.r, barColor.g, barColor.b, barColor.a );
  int x = static_cast<int>(getX());
  int y = static_cast<int>(getY());
  SDL_Rect hudRect = {x, y, barRect.w, barRect.h};
  SDL_RenderFillRect( renderer, &hudRect );

  SDL_SetRenderDrawColor( 
    renderer, barColor.r, barColor.g, barColor.b, barColor.a );
  SDL_RenderDrawRect( renderer, &barRect );

  SDL_Color c = {255, 0, 0, 255};
  std::stringstream fps; 
  fps << "FPS: " << std::to_string(clock.getFps());
  io.writeText(fps.str(), 680, 30, c);
  std::stringstream move1, move2, move3, move4, move5, god, restart, music, 
                    goal;
  move1 << "Jump: W";
  move2 << "Morph Ball: S";
  move3 << "Move left: A";
  move4 << "Move right: D";
  move5 << "Shoot: SPACE";
  god << "Godmode: G";
  restart << "Restart : R";
  music << "Music : M";
  goal << "Destroy Mother Brain and save the Metroid Baby.";
  io.writeText(move1.str(), 680, 60, c);
  io.writeText(move2.str(), 680, 90, c);
  io.writeText(move3.str(), 680, 120, c);
  io.writeText(move4.str(), 680, 150, c);
  io.writeText(move5.str(), 680, 180, c);
  io.writeText(god.str(), 680, 210, c);
  io.writeText(restart.str(), 680, 240, c);
  io.writeText(music.str(), 680, 270, c);
  io.writeText(goal.str(), 30, 60, c);
  
}
