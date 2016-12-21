#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "hud.h"
#include "scaledSprite.h"
#include "vegeta.h"
#include "chari.h"
#include "health.h"
#include "collider.h"
#include "multisprite.h"
#include <list>
#include "sound.h"
class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void switchSprite();
  Collider& cld;
  bool resetvar;
private:
  bool vegetaGodMode;
  bool charizardGodMode;
  const bool env;
  HUD* hudcontrols;
  HUD* hudhealthvegeta;
  HUD* hudhealthcharizard;
  //Health bar;
  bool drawHud ;
  const IOManager& io;
  Clock& clock;

  SDL_Surface * const screen;
  SDL_Surface * const scaledSpriteSurface;
  World world;
 World last;
  World mountains;
  
  Viewport& viewport;
  std::vector<ScaledSprite*> scaledSprites;
  Charizard* charizard;
  Vegeta* vegeta;
  
  std::vector<Drawable*> sprites;
 
  
  std::vector<Drawable*>::iterator currentSprite;
  std::list<SmartSprite*> dragonBalls;
  std::list<SmartSprite*> deleteBalls;
  int numberofdb;
  bool gameover;

  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;

  void draw() const;
  void update();
  void makescaledSprites();
  void printscaledSprites() const;
  void checkForCollisions();
  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};
