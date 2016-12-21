#include "vegeta.h"
#include "gamedata.h"
#include "frameFactory.h"

Vegeta::Vegeta(const std::string& name):
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")
), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/movSpeedX"), 
 
                    Gamedata::getInstance().getXmlInt(name+"/movSpeedY")
) 
           ),
  explosion(NULL),
  frames(FrameFactory::getInstance().getFrames(name)),
  observers(),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  movSpeedX( Gamedata::getInstance().getXmlInt(name+"/movSpeedX") ),
  movSpeedY( Gamedata::getInstance().getXmlInt(name+"/movSpeedY") ),
  direction( 1 ),lost(false),
   VegetaLeft(false),
  VegetaRight(false),
  VegetaIdle(true),
  VegetaLookRight(false),
  VegetaLookLeft(false),
  VegetaShoot(false),
   bulletName("plasma"),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/bulletInterval")),
  timeSinceLastBullet(0),
  minBulletSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/minBulletSpeed")),
  bullets(bulletName),
  hasExploded( false ), health(Gamedata::getInstance().getXmlInt("HealthChari/helt"))
   
{ }

Vegeta::Vegeta(const Vegeta& s) :
  Drawable(s), 
  explosion(NULL),
  frames(s.frames),
  observers(s.observers),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  movSpeedX( s.movSpeedX ),
  movSpeedY( s.movSpeedY ),
  direction( s.direction ),
  lost(s.lost),
  VegetaLeft(s.VegetaLeft),
  VegetaRight(s.VegetaRight),
  VegetaIdle(s.VegetaIdle),
   VegetaLookRight(s.VegetaLookRight),
  VegetaLookLeft(s.VegetaLookLeft),
  VegetaShoot(false),
  bulletName(s.bulletName),
  bulletInterval(s.bulletInterval),
  timeSinceLastBullet(s.timeSinceLastBullet),
  minBulletSpeed(s.minBulletSpeed),
  bullets(s.bullets), 
  hasExploded( s.hasExploded ),health(s.health)

{ }

void Vegeta::draw() const 
{if (explosion) 
  {
    explosion->draw();
    return;
  }
  if( !hasExploded )
  {
    Uint32 x =static_cast<Uint32>(X());
    Uint32 y =static_cast<Uint32>(Y());
    frames[currentFrame]->draw(x,y); 
    bullets.draw();
    

  }
}

void Vegeta::detach( SmartSprite* o ) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

void Vegeta::explode() { 
  
  if ( explosion ) return;
  else if(getHealth() > 0)
    {    
     setHealth(getHealth()-20);
    Sprite sprite(getName(), getPosition(), getVelocity(), frames[5]);
    //std::cout<< getName() << getPosition() << getVelocity() << frames[currentFrame]<<std::endl;
  //currentFrame = 0;
  //sprite.explode();
  explosion = new ExplodingSprite(sprite);
 // sprite.explode();
  }
  else
      lost =true;
}
/*
void Vegeta::reset()
{setHealth(100);
  X(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/x"));
  Y(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/y")); 
  if( hasExploded )
    hasExploded = false;
}
*/

bool Vegeta::collidedWith(const Drawable* obj1,const Drawable* obj2)
{
  if(execute(*obj1, *obj2))
  {//std::cout<<"Collided"<<std::endl;
    return true;
  }
  return false;
}
  
int Vegeta::smartCheck(const Drawable* obj1)
{
  int i;
  i = bullets.smartCheck(obj1);
  return i;
}

int Vegeta::getBulletlistSize()
{return bullets.getBulletlistSize();}

int Vegeta::getFreelistSize()
{return bullets.getFreelistSize();}

bool Vegeta::checkCollision(const Drawable* obj)
{
  if(bullets.collidedWith( obj, 10) )
  {
    return true;
  }
  return false;
}


void Vegeta::update(Uint32 ticks) 
{
  if (explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }

  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    (*ptr)->setPlayerPos( getPosition() );
    ++ptr;
  }
advanceFrame(ticks);
  Uint8 *keyPressed = SDL_GetKeyState(NULL);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

if(keyPressed[SDLK_e])
  { 
   static SDLSound sound;
    VegetaShoot = true;
    shoot();
    sound[1];
  }

  
  //vertical movement
  if(keyPressed[SDLK_w])
  {
    if(keyPressed[SDLK_s])
    {
      
      velocityY(0);
      velocityX(0);
    }
    else{
    if(Y() > 0)
      velocityY( -movSpeedY );
    else
      Y(0);
  }
  }
  else if(keyPressed[SDLK_s])
  {
    if(Y() < worldHeight-frameHeight)
      velocityY( movSpeedY );
    else
      Y(worldHeight-frameHeight);
  }
  else
   { velocityY(0);}
  
  if(!keyPressed[SDLK_d] && !keyPressed[SDLK_a])
  {VegetaIdle =true;
  VegetaLeft =false;
  VegetaRight =false;
  VegetaShoot =false;
  }
  //horizontal movement
    //horizontal movement
  if(keyPressed[SDLK_d])
  {
    if(keyPressed[SDLK_a])
    {
      VegetaIdle=true;
      VegetaLeft =false;
  VegetaRight =false;
  velocityX(0);
  velocityY(0);
    }
    else{
    VegetaIdle =false;
  VegetaLeft =false;
  VegetaRight =true;
    if(X() < worldWidth-frameWidth)
      velocityX( movSpeedY );
    else
      X(worldWidth-frameWidth);
    direction = 1;
    }   
  }
  else if(keyPressed[SDLK_a])
  {
      VegetaIdle =false;
  VegetaLeft =true;
  VegetaRight =false;
    if(X() > 0)
      velocityX( -movSpeedY );
    else
      X(0);
    direction = 0;
  }
  else
    velocityX(0);
  
  

  
    

  timeSinceLastBullet +=ticks;
 bullets.update(ticks);
}

void Vegeta::shoot()
{
  //see if its time to shoot 
  if(timeSinceLastBullet > bulletInterval) 
  {
    Vector2f vel = getVelocity();
    float x;
    float y = Y()+ frameHeight/2;
    if(direction == 1) 
    {
      x = X() + frameWidth;
      vel[0] += minBulletSpeed;
      
    }
    else 
    {
      x=X();
      vel[0] -= minBulletSpeed;
    }
    bullets.shoot(Vector2f(x,y-50),vel);
    
    timeSinceLastBullet = 0;
  }
}

void Vegeta :: advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval )
    {
        if( VegetaRight && direction) {
         
    currentFrame = ((currentFrame+1) % 3) + 14;
		timeSinceLastFrame = 0;
	}

 if( VegetaIdle  ) {
        
    currentFrame = ((currentFrame+1) % 4 ) +6 ;
		timeSinceLastFrame = 0;
	}

  
  
       if( VegetaIdle && VegetaLookLeft ) {
         
    currentFrame = ((currentFrame+1) % 4 ) +6 ;
		timeSinceLastFrame = 0;
	}

        if( VegetaIdle && VegetaLookRight ) {
         
    currentFrame = ((currentFrame+1) % 4 )  +10;
		timeSinceLastFrame = 0;
	}
  
    if (timeSinceLastFrame > frameInterval )
    {
        if( VegetaLeft && (direction<1)) {
         
    currentFrame = ((currentFrame+1) % 3) + 17;
		timeSinceLastFrame = 0;
    	}
    }


    }
}