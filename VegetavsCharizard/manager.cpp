#include <iostream>
#include <string>
#include <iomanip>
#include "multisprite.h"
#include "sprite.h"
#include "smartSprite.h"
#include "gamedata.h"
#include "manager.h"
#include "twoWayMultiSprite.h"
#include <algorithm>
#include "frameFactory.h"
#include "bullet.h"
#include "smallSprite.h"
//#include "explodingAI.h"
#include "explodingSprite.h"

Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  
  for (unsigned i = 0; i < sprites.size(); ++i) {
    
    delete sprites[i];
  }
    
 std::list<SmartSprite*>::iterator it=dragonBalls.begin();
 while(it!=dragonBalls.end())
  { //std::cout<<"DragonBalls destructor"<<std::endl;
    delete *it;
    ++it;

  }
   std::list<SmartSprite*>::iterator dit=deleteBalls.begin();
 while(dit!=deleteBalls.end())
  { //std::cout<<"DragonBalls destructor"<<std::endl;
    delete *dit;
    ++dit;

  }
  

  SDL_FreeSurface(scaledSpriteSurface);
  delete hudcontrols;
  delete hudhealthvegeta;
  delete hudhealthcharizard;
  if(resetvar){
Manager mang;
mang.play();
}
  
}

class ScaledSpriteCompare {
public:
  bool operator()(const ScaledSprite* lhs, const ScaledSprite* rhs) {
    return lhs->getScale() < rhs->getScale();
  }
};

Manager::Manager() :
cld(Collider::getInstance()),
resetvar(false),
  vegetaGodMode(false),
  charizardGodMode(false),
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  hudcontrols(new HUD("HUD")),
  hudhealthvegeta(new HUD("HealthVegeta")),
  hudhealthcharizard(new HUD("HealthChari")),
  drawHud(false),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
   scaledSpriteSurface( io.loadAndSet(Gamedata::getInstance().getXmlStr("dragonball/file"), 
              Gamedata::getInstance().getXmlBool("dragonball/transparency")) ),
  world("mountains", Gamedata::getInstance().getXmlInt("mountains/factor") ),
last("cloud", Gamedata::getInstance().getXmlInt("cloud/factor")),
  mountains("trees", Gamedata::getInstance().getXmlInt("trees/factor") ),
  viewport( Viewport::getInstance() ),
  scaledSprites(),
  charizard(NULL),
  vegeta(NULL),
  sprites(),
  currentSprite(),dragonBalls(),deleteBalls(),
  numberofdb(Gamedata::getInstance().getXmlInt("numberOfSprites")),
  gameover(false),
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") )
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  sprites.push_back(new Charizard("charizard"));
  
  sprites.push_back(new Vegeta("bat"));
  


  for(int i =0 ; i<numberofdb ; ++i )
  
  //  dragonBalls.push_back(new SmartSprite("dragonball"));
    {  dragonBalls.push_back(new SmartSprite("dragonball"));}
    


  std::list<SmartSprite*>::const_iterator str = dragonBalls.begin();
  while(str!= dragonBalls.end())
  { 
    dynamic_cast<Vegeta*>(sprites[1])->attach(*str);
    ++str;
  }

  charizard=dynamic_cast<Charizard*>(sprites[0]);
  vegeta=dynamic_cast<Vegeta*>(sprites[1]);

  currentSprite = sprites.begin();
  currentSprite++;
  viewport.setObjectToTrack(*currentSprite);
  
   
  
 

 
}

void Manager::makescaledSprites() {
  /*unsigned numberOfscaledSprites = Gamedata::getInstance().getXmlInt("numberOfSprites");
  scaledSprites.reserve( numberOfscaledSprites );

  for (unsigned i = 0; i < numberOfscaledSprites; ++i) {
    scaledSprites.push_back( new ScaledSprite("dragonball", scaledSpriteSurface) );
  }
  sort(scaledSprites.begin(), scaledSprites.end(), ScaledSpriteCompare());*/
}

void Manager::printscaledSprites() const {
  /*for (unsigned i = 0; i < scaledSprites.size(); ++i) {
    std::cout << scaledSprites[i]->getScale() << std::endl;
  }*/
}

void Manager::checkForCollisions()
{
  
  static SDLSound sound;
   std::list<flamethrower>::const_iterator fth = charizard->flamethrowerList.begin();
   std::list<smallSprite>::const_iterator pls = vegeta->bullets.bulletList.begin();
    while(fth != charizard->flamethrowerList.end())
    {
        if(!vegetaGodMode && cld.collidedWithVegeta(*fth,vegeta))
     { 
      
          vegeta->explode();  
          sound[0];
     }
      
     ++ fth;
    }

    while( pls != vegeta->bullets.bulletList.end())
    {
      if(!charizardGodMode && cld.collidedWithChari(*pls, charizard))
      {
      charizard->explode();
      sound[0];
      }
     ++ pls;
    }


    std::list<SmartSprite*>::iterator sprite = dragonBalls.begin();
    //std::list<SmartSprite*>::iterator deleteSprite = deleteBalls.begin();
    while(sprite!=dragonBalls.end())
    {     

        SmartSprite* ss=dynamic_cast<SmartSprite*>(*sprite);
        
       //SmartSprite* ds = dynamic_cast<SmartSprite*> (*deleteSprite);
        //ExplodingAI* e = dynamic_cast<ExplodingAI*>(*sprite);    
        if(cld.collidedWithDragonBall(vegeta,ss) )
      {

        //ss->explode();
        sound[0];
        //SmartSprite* temp = *sprite;
        numberofdb--; 
        deleteBalls.push_back(*sprite);
        sprite = dragonBalls.erase(sprite);
        
        if(numberofdb < 1)
        { charizard->lost = true; }
        
      }
   else  

      sprite++;

     }
     
  

    SDL_Flip(screen);
}

void Manager::draw()  const{

  world.draw();
  
   last.draw();

  mountains.draw();
  
  std::vector<Drawable*>::const_iterator ptr = sprites.begin();
  while ( ptr != sprites.end() ) {
    (*ptr)->draw();
    ++ptr;
  }

  std::list<SmartSprite*>::const_iterator itr = dragonBalls.begin();
  while ( itr != dragonBalls.end() ) {
    (*itr)->draw();
    ++itr;
  }
  
    io.printMessageAt("Vegeta Health", 700, 30);

  hudhealthvegeta->drawHUDdynamic(screen,vegeta->getHealth());
 
io.printMessageAt("Charizard Health", 700, 80);
  hudhealthcharizard->drawHUDdynamic(screen,charizard->getHealth());
 
  if(charizardGodMode)
  { io.printMessageAt("Charizard God Mode On", 650, 140);}
if(vegetaGodMode)
{
  io.printMessageAt("Vegeta God Mode On", 650, 160);
}
std::string db = "Dragon Balls: ";
std::stringstream sstm;
sstm<<db<<numberofdb;
std::string r = sstm.str();

 io.printMessageAt(r, 700, 190);
  
  if(clock.getSeconds()<2){
    hudcontrols->drawHUD(screen, Gamedata::getInstance().getXmlInt("HUD/startX"), Gamedata::getInstance().getXmlInt("HUD/startX"));

    io.printMessageAt("Let Vegeta Collect all 7 Dragon balls or kill Vegeta", 10, 70);
  clock.display();
  io.printMessageAt("Press A to Turn Vegeta Left", 10, 110);
  io.printMessageAt("Press D to Turn Vegeta Right", 10, 130);
  io.printMessageAt("Press arrow keys fpr Charizard", 10, 150);
 io.printMessageAt("God Mode, v for Vegeta and c for Charizard", 10, 170);
  io.printMessageValueAt("BulletList: ", static_cast<Charizard*>(charizard)->getFlameListSize(), 10, 190);
     io.printMessageValueAt("FreeList: ",  static_cast<Charizard*> (charizard)->getFreeListSize(), 10, 210);
      io.printMessageValueAt("PlasmaList: ", static_cast<Vegeta*>(vegeta)->getBulletlistSize(), 10, 230);
       io.printMessageAt("Press E to Shoot Plasma with Vegeta", 10, 250);
  io.printMessageAt("Press R_Ctrl to Soot Fireballs with Charlizard", 10, 270);
    
   
}
   else if(drawHud == true){
  hudcontrols->drawHUD(screen, Gamedata::getInstance().getXmlInt("HUD/startX"), Gamedata::getInstance().getXmlInt("HUD/startX"));

    io.printMessageAt("Press F1 to toggle HUD", 10, 70);
    clock.display();
    io.printMessageAt("Press A to Turn Vegeta Left", 10, 110);
  io.printMessageAt("Press D to Turn Vegeta Right", 10, 130);
  io.printMessageAt("Press arrow keys to turn Charizard", 10, 150);
    io.printMessageAt("God Mode, v for Vegeta and c for Charizard", 10, 170);
   io.printMessageValueAt("BulletList: ", static_cast<Charizard*>(charizard)->getFlameListSize(), 10, 190);
     io.printMessageValueAt("FreeList: ",  static_cast<Charizard*> (charizard)->getFreeListSize(), 10, 210);
     io.printMessageValueAt("PlasmaList: ", static_cast<Vegeta*>(vegeta)->getBulletlistSize(), 10, 230);
     io.printMessageAt("Press E to Shoot Plasma with Vegeta", 10, 250);
  io.printMessageAt("Press R_Ctrl to Soot Fireballs with Charlizard", 10, 270);
     
 }

 if(vegeta->lost){
io.printMessageAt("Vegeta has Lost",300 , 200);
io.printMessageAt("Press r to restart the game",300 , 220);
}

if(charizard->lost){
io.printMessageAt("charizard has Lost",300 , 200);
io.printMessageAt("Press r to restart the game",300 , 220);
}
 
  
  io.printMessageAt(title, 10, 450);
  
  viewport.draw();

  SDL_Flip(screen);
}

void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::switchSprite() {
  /*++currentSprite;
  if ( currentSprite == sprites.end() ) {
    currentSprite = sprites.begin();
  }
  viewport.setObjectToTrack(*currentSprite);
  viewport.setObjectToTrack(vegeta[0]);*/
  /*currentSprite = (currentSprite+1) % sprites.size();
  viewport.setObjectToTrack(sprites[currentSprite]);
  viewport.setObjectToTrack(vegeta[currentSprite]);*/
  /*currentSprite = (currentSprite+1) % sprites.size();
  viewport.setObjectToTrack(sprites[currentSprite]);*/

}

void Manager::update() {
  clock.update();
  
  Uint32 ticks = clock.getElapsedTicks();

  
 
   std::vector<Drawable*>::const_iterator ptr = sprites.begin();
  while ( ptr != sprites.end() ) {
    (*ptr)->update(ticks);
    ++ptr;
  }

   std::list<SmartSprite*>::const_iterator itr = dragonBalls.begin();
  while ( itr != dragonBalls.end() ) {
    (*itr)->update(ticks);
    ++itr;
  }
  
  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  checkForCollisions() ;

  world.update();
  mountains.update();
  
  viewport.update(); // always update viewport last
}

void Manager::play() {
  SDL_Event event;
  bool done = false;
  static SDLSound sound;
  
  while ( not done ) {
  if(vegeta->getHealth()<=0)
    vegeta->lost=true;
  else if(charizard->getHealth()<=0)
    charizard->lost=true;
    while ( SDL_PollEvent(&event) ) {
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
          break;
        }

        if ( keystate[SDLK_2] ) {
          vegeta->explode();
        }
        if ( keystate[SDLK_t] ) {
          switchSprite();
        }
        if ( keystate[SDLK_c] ) {
          charizardGodMode=!charizardGodMode;
          
        }
        if ( keystate[SDLK_v] ) {
          vegetaGodMode=!vegetaGodMode;
         
        }

        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
         if (keystate[SDLK_RCTRL]) {
        charizard->shoot("greenorb",
          charizard->getPosition()+Vector2f(60,10),
          Vector2f(Gamedata::getInstance().getXmlInt("greenorb/speedX")+Gamedata::getInstance().getXmlInt("flamethrower/speedx"),
            Gamedata::getInstance().getXmlInt("greenorb/speedY")+Gamedata::getInstance().getXmlInt("flamethrower/speedy")),
          FrameFactory::getInstance().getFrame("greenorb"));
        sound[1];
        }
   if (keystate[SDLK_F1]) {
          if(drawHud == true) drawHud = false ;
          else if(drawHud == false) drawHud = true;
        }
        if (keystate[SDLK_r]) {
    resetvar=true;
                done=true;
                vegeta->lost=false;
                charizard->lost=false;
                break;
        }
         
    
        
}


    }
    draw();
  if(!vegeta->lost && !charizard->lost)
  
    update();
  }
}
