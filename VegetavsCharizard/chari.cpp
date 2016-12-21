#include "chari.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "explodingSprite.h"
Charizard:: ~Charizard(){
  delete explosion;
  std::list<flamethrower>::iterator it=flamethrowerList.begin();
  std::list<flamethrower>::iterator itf=freeList.begin();
  while(it!=flamethrowerList.end()){
  it=flamethrowerList.erase(it);
}
flamethrowerList.clear();
while(itf!=freeList.end()){
  itf=freeList.erase(itf);
} 
freeList.clear();
  
}

void Charizard::shoot(const std::string& name,const Vector2f& pos,const Vector2f& vel,const Frame* frm){
  if(freeList.empty()){
  if(currentFrame<4)

  flamethrowerList.push_back(flamethrower(name,pos,vel*2,frm));
  else 
  flamethrowerList.push_back(flamethrower(name,getPosition(),Vector2f(-500,320),frm));

}
  else {
    flamethrower fth=freeList.front();
    freeList.pop_front();
    fth.reset();
            if(currentFrame<4)
            {
fth.setPosition(pos);
  fth.setVelocity(vel*2);
}
  else 
  {
    fth.setPosition(getPosition());
  fth.setVelocity(Vector2f(-500,320));
  
  }
  
  flamethrowerList.push_back(fth);
}
}


Charizard::Charizard(const std::string& name):
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")
), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"), 
 
                    Gamedata::getInstance().getXmlInt(name+"/speedY")
) 
           ),
  frames(FrameFactory::getInstance().getFrames(name)),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  explosion(NULL),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  health(Gamedata::getInstance().getXmlInt("HealthChari/helt")),
  movSpeedX( Gamedata::getInstance().getXmlInt(name+"/speedX") ),
  movSpeedY( Gamedata::getInstance().getXmlInt(name+"/speedY") ),
  direction( 1 ),
    CharizardLeft(false),
  CharizardRight(false),
  CharizardUp(false),
  CharizardDown(false),
  CharizardIdle(true),
  lost(false),
  flamethrowerList(),
  freeList()
{ }

Charizard::Charizard(const Charizard& s) :
  Drawable(s), 
  frames(s.frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  explosion(NULL),

  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  health(s.health),
  movSpeedX( s.movSpeedX ),
  movSpeedY( s.movSpeedY ),
  direction( s.direction ),
  CharizardLeft(s.CharizardLeft),
  CharizardRight(s.CharizardRight),
  CharizardUp(s.CharizardUp),
  CharizardDown(s.CharizardDown),
  CharizardIdle(s.CharizardIdle),
  lost(s.lost),
  flamethrowerList(s.flamethrowerList),
  freeList(s.freeList)
{ }

void Charizard::draw() const 
{
  if (explosion) {
    explosion->draw();
    return;
  }
    Uint32 x =static_cast<Uint32>(X());
    Uint32 y =static_cast<Uint32>(Y());
    frames[currentFrame]->draw(x,y); 
    std::list<flamethrower>::const_iterator itB=flamethrowerList.begin();
    while(itB!=flamethrowerList.end()){
    (*itB).draw();
    itB++;
    }
    


}

void Charizard::explode() { 
  if ( explosion ) return;
  else{
  setHealth(getHealth()-20);
  Sprite sprite(getName(), getPosition(), getVelocity(), frames[6]);
  explosion = new ExplodingSprite(sprite);
  }
}


void Charizard::update(Uint32 ticks) 
{

  if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    } 
    return;
}
  advanceFrame(ticks);
  Uint8 *keyPressed = SDL_GetKeyState(NULL);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  std::list<flamethrower>::iterator it=flamethrowerList.begin();
while(it!=flamethrowerList.end()){
  (*it).update(ticks);
      if(it->goneFar()){
        freeList.push_back(*it);
  it=flamethrowerList.erase(it);
      }
it++;
}

  if((keyPressed[SDLK_LEFT] && keyPressed[SDLK_RIGHT]) ||
    (keyPressed[SDLK_UP] && keyPressed[SDLK_DOWN]) )
  {
    CharizardIdle =true;
    CharizardLeft =false;
    CharizardRight =false;

  }



  //vertical movement
  if(keyPressed[SDLK_UP])
  {
    if(keyPressed[SDLK_DOWN])
    {
      CharizardIdle=true;
      velocityY(0);
      velocityX(0);
    }
    else
    {
    if(Y() > 0)
      velocityY( -movSpeedY );
    else
      Y(0);
    }
  }
  else if(keyPressed[SDLK_DOWN])
  {
    if(Y() < worldHeight-frameHeight)
      velocityY( movSpeedY );
    else
      Y(worldHeight-frameHeight);
  }
  else
   { velocityY(0);}
  


  if(!keyPressed[SDLK_LEFT] && !keyPressed[SDLK_RIGHT])
  {CharizardIdle =true;
  CharizardLeft =false;
  CharizardRight =false;}
  //horizontal movement
  if(keyPressed[SDLK_RIGHT])
  {
    if(keyPressed[SDLK_LEFT])
    {
      CharizardIdle=true;
      CharizardLeft =false;
  CharizardRight =false;
  velocityX(0);
  velocityY(0);
    }
    else{
    CharizardIdle =false;
  CharizardLeft =false;
  CharizardRight =true;
    if(X() < worldWidth-frameWidth)
      velocityX( movSpeedY );
    else
      X(worldWidth-frameWidth);
    direction = 1;
    }   
  }
  else if(keyPressed[SDLK_LEFT])
  {
      CharizardIdle =false;
  CharizardLeft =true;
  CharizardRight =false;
    if(X() > 0)
      velocityX( -movSpeedY );
    else
      X(0);
    direction = 0;
  }
  else
    velocityX(0);
  
 
  
}

void Charizard :: advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval )
    {
        if( CharizardRight && direction) {
         
    currentFrame = (currentFrame+1) %(numberOfFrames/2);
  if(currentFrame==0)currentFrame=0;
		timeSinceLastFrame = 0;
	}
        if( CharizardIdle ) {
         
     if(currentFrame<4)
    {
        currentFrame = (currentFrame+1) %(numberOfFrames/2);
  if(currentFrame==0)currentFrame=0;

    }
      else
      {
           currentFrame = (currentFrame+1) %(numberOfFrames);
    if(currentFrame==0)currentFrame=numberOfFrames/2;
      }
		timeSinceLastFrame = 0;
	}
    if (timeSinceLastFrame > frameInterval )
    {
        if( CharizardLeft && (direction<1)) {
         
     currentFrame = (currentFrame+1) %(numberOfFrames);
          
    if(currentFrame==0)currentFrame=numberOfFrames/2;
		timeSinceLastFrame = 0;
	}
    }
    }
}