#include "twoWayMultiSprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "explodingSprite.h"

void TwoWayMultiSprite::advanceFrame(Uint32 ticks)
{
   timeSinceLastFrame += ticks;
    if (timeSinceLastFrame > frameInterval)
    {
     currentFrame = (currentFrame + 1) % numberOfFrames;
     timeSinceLastFrame = 0;
    }
}

const Frame* TwoWayMultiSprite::getFrame() const
{
   return getframes()[currentFrame];
}

const std::vector<Frame *> TwoWayMultiSprite::getframes() const
{
   if (velocityX() > 0){
    
     return frames_RHS;
   }
   else
   {
  
     return frames_LHS;
   }
}

TwoWayMultiSprite::TwoWayMultiSprite( const std::string& name1,const std::string& name2):
   Drawable(name1, Vector2f(Gamedata::getInstance().getXmlInt(name1 + "/startLoc/x"), 
           Gamedata::getInstance().getXmlInt(name1 + "/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name1 + "/speedX"), 
           Gamedata::getInstance().getXmlInt(name1 + "/speedY"))),
	   explosion(NULL),
     frames_RHS(FrameFactory::getInstance().getFrames(name1)),
	   frames_LHS(FrameFactory::getInstance().getFrames(name2)),
	   worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
	   worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
	   currentFrame(0),
	   numberOfFrames(Gamedata::getInstance().getXmlInt(name1 + "/frames")),
	   frameInterval(Gamedata::getInstance().getXmlInt(name1 + "/frameInterval")),
	   timeSinceLastFrame(0),
	   frameWidth(frames_RHS[0]->getWidth()),
	   frameHeight(frames_RHS[0]->getHeight())
 {}

void TwoWayMultiSprite::draw() const {
    if (explosion) {
    explosion->draw();
    return;
  }
	Uint32 x = static_cast<Uint32>(X());
	Uint32 y = static_cast<Uint32>(Y());
	getframes()[currentFrame]->draw(x, y);
}

void TwoWayMultiSprite::explode() { 
  if ( explosion ) return;
  Sprite sprite(getName(), getPosition(), getVelocity(), getFrame());
  explosion = new ExplodingSprite(sprite); 
}

void TwoWayMultiSprite::update(Uint32 ticks) {
   if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }

     advanceFrame(ticks);
 Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }  
 }
