#include "aaline.h"
#include "ioManager.h"
#include <iostream>
#include <SDL.h>
#include <string>
class HUD{
public:
HUD(const std::string& hudname);
const IOManager& io;
void drawHUD(SDL_Surface*  screen,  int x,  int y) const;
  void drawHUDdynamic(SDL_Surface* screen,int helt)const;
private:
  string name;
  const int HudStartX;
  const int HudStartY;
  const int HUD_WIDTH;
  const int HUD_HEIGHT;
};
