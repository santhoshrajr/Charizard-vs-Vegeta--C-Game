#include <cmath>
#include "viewport.h"
#include "smartSprite.h"
#include "explodingSprite.h"

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

SmartSprite::SmartSprite(const std::string& name):
Sprite(name),   explosion(NULL),io(IOManager::getInstance()),
playerPos(),
 playerWidth(0),
 playerHeight(0),
 currentMode(EVADE) 
{}



SmartSprite::SmartSprite(const std::string& name, 
                         const Vector2f& pos, int w, int h) :
 Sprite(name, pos, 
   Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"), 
            Gamedata::getInstance().getXmlInt(name+"/speedY"))
 ),    explosion(NULL),
 io(IOManager::getInstance()), 
 playerPos(),
 playerWidth(w),
 playerHeight(h),
 currentMode(EVADE) 
{ }
SmartSprite::SmartSprite(const SmartSprite& sp):
Sprite(sp),explosion(sp.explosion),
io(IOManager::getInstance()),
playerPos(sp.playerPos),
playerWidth(sp.playerWidth),
playerHeight(sp.playerHeight),
currentMode(EVADE)
{}
bool SmartSprite::Exploding()
{
  if(explosion)
    return true;
  else return false;
}
void SmartSprite::goLeft()  {    // std::cout<<"In Evade mode"<<std::endl;
  if (X() > 0) velocityX( -abs(velocityX()) ); 
}
void SmartSprite::goRight() { velocityX( fabs(velocityX()) ); }
void SmartSprite::goUp()    { velocityY( -fabs(velocityY()) ); }
void SmartSprite::goDown()  { velocityY( fabs(velocityY()) ); }

void SmartSprite::draw() const {
  if (explosion) {
    explosion->draw();
    return;
  }
  Sprite::draw();
  int x = 20+ X() - Viewport::getInstance().X();
  int y = 20+ Y() - Viewport::getInstance().Y();
  std::stringstream strm;
  strm << currentMode;
  //std::cout<<currentMode
  io.printMessageAt( strm.str(), x, y); 
}

void SmartSprite::explode() { 
  if ( explosion ) return;
  Sprite sprite(getName(), getPosition(), getVelocity(), getFrame());

  //std::cout<<"Check Sprite explosions"<<std::endl;
  explosion = new ExplodingSprite(sprite); 
}
float SmartSprite::safeDistance = 75;
void SmartSprite::update(Uint32 ticks) {
  if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }
  Sprite::update(ticks);
  float x= X()+getFrame()->getWidth()/2;
  float y= Y()+getFrame()->getHeight()/2;
  float ex= playerPos[0]+playerWidth/2;
  float ey= playerPos[1]+playerHeight/2;
  float distanceToEnemy = ::distance( x, y, ex, ey );

  if  ( currentMode == NORMAL ) {
    if(distanceToEnemy < safeDistance) currentMode = EVADE;
  }
  else if  ( currentMode == EVADE ) {

    if(distanceToEnemy > safeDistance) currentMode=NORMAL;
    else {
      if ( x < ex ) goLeft();
      if ( x > ex ) goRight();
      if ( y < ey ) goUp();
      if ( y > ey ) goDown();
    }
  }
}
