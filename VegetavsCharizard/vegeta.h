#ifndef VEGETA_H
#define VEGETA_H
#include <vector>
#include "drawable.h"
#include "sprite.h"
#include <string>
#include "gamedata.h"
#include "ioManager.h"
#include "bullet.h"
#include "explodingSprite.h"
#include "sound.h"
#include "smartSprite.h"


class Vegeta: public Drawable
{
public:
 Vegeta(const std::string&);
 Vegeta(const Vegeta&);
 virtual ~Vegeta(){delete explosion;}
 virtual void draw()const;
 virtual void update(Uint32 ticks);
 virtual const Frame* getFrame() const {return frames[currentFrame];}
 void changeExplode(bool isCase){ hasExploded = isCase; }
  void reset();
  virtual bool collidedWith(const Drawable* ,const Drawable* );
  void shoot();
  bool checkCollision(const Drawable* );
  void explode();
  bool getExplode() { return hasExploded; }
  int smartCheck(const Drawable* );
  
  int getBulletlistSize();
  int getFreelistSize();
  int getHealth()const {return health;}
  void  setHealth( int h){ health =h ;}
 void attach( SmartSprite* o ) { observers.push_back(o); } 
  void detach( SmartSprite* o );

 protected:
 friend class Manager;
 ExplodingSprite* explosion;
 const std::vector<Frame*> frames;
 std::list<SmartSprite*> observers;
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;

  int movSpeedX;
  int movSpeedY;
  int direction;
   bool lost;

  bool VegetaLeft;
   bool VegetaRight;
   bool VegetaIdle;
  bool VegetaLookRight;
  bool VegetaLookLeft;
  bool VegetaShoot;
  std::string bulletName;
  float bulletInterval;
  float timeSinceLastBullet;
  float minBulletSpeed;
  Bullet bullets;
  bool hasExploded;
  int health;
 
  
    


 void advanceFrame(Uint32 ticks);
  Vegeta& operator=(const Vegeta&);
  };
#endif
