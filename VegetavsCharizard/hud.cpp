#include"hud.h"
#include "gamedata.h"
HUD::HUD(const std::string& hudname):
io( IOManager::getInstance() ),
  name(hudname),
  HudStartX(Gamedata::getInstance().getXmlInt(hudname+"/start/x")),
  HudStartY(Gamedata::getInstance().getXmlInt(hudname+"/start/y")),
  HUD_WIDTH(Gamedata::getInstance().getXmlInt(hudname+"/helt")),
  HUD_HEIGHT(Gamedata::getInstance().getXmlInt(hudname+"/height")){}
void HUD::drawHUD(SDL_Surface*  screen,  int x,  int y) const {

  const Uint32 RED = SDL_MapRGB(screen->format, 0xff, 0, 0);
  Draw_AALine(screen, x, y+Gamedata::getInstance().getXmlInt("HUD/HUD_HEIGHT")/2, 
                      x+Gamedata::getInstance().getXmlInt("HUD/HUD_WIDTH"),y+Gamedata::getInstance().getXmlInt("HUD/HUD_HEIGHT")/2, 
                      Gamedata::getInstance().getXmlInt("HUD/HUD_HEIGHT"), 0xff, 0xff, 0xff, 0xff/2);
  Draw_AALine(screen, x,y, x+ Gamedata::getInstance().getXmlInt("HUD/HUD_WIDTH"),y, RED);
}

void HUD::drawHUDdynamic(SDL_Surface* screen,int helt)const{
  //io.printMessageAt("Health",HudStartX+10,HudStartY-HUD_HEIGHT);
  Draw_AALine(screen, HudStartX, HudStartY+HUD_HEIGHT/2, 
                      HudStartX+helt,HudStartY+HUD_HEIGHT/2, 
                      HUD_HEIGHT, 0xff, 0x00, 0x00, 0xff);
}
