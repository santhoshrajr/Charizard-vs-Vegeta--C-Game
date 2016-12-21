#ifndef CHARI_H
#define CHARI_H
#include <vector>
#include "drawable.h"
#include "sprite.h"
#include <string>
#include "gamedata.h"
#include "ioManager.h"
#include <list>
#include "flamethrower.h"

class ExplodingSprite;

class Charizard: public Drawable
{
public:
 Charizard(const std::string&);
 Charizard(const Charizard&);
  virtual ~Charizard();
 
 virtual void draw()const;
 virtual void update(Uint32 ticks);
  void shoot(const std::string& name,const Vector2f& pos,const Vector2f& vel,const Frame* frm);

 virtual const Frame* getFrame() const {return frames[currentFrame];}
  int getHealth()const{return health;}
  void setHealth(int h){health=h;}
  friend class Manager;
 unsigned int getFlameListSize() const { return flamethrowerList.size(); }
  unsigned int getFreeListSize() const { return freeList.size(); }
  virtual void explode();
 private:
 const std::vector<Frame*> frames;
  int worldWidth;
  int worldHeight;
  ExplodingSprite* explosion;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;
  int health;
  int movSpeedX;
  int movSpeedY;
  int direction;
  bool CharizardLeft;
  bool CharizardRight;
  bool CharizardUp;
  bool CharizardDown;
  bool CharizardIdle;
  bool lost;
  std::list<flamethrower>flamethrowerList;
  std::list<flamethrower>freeList;

 void advanceFrame(Uint32 ticks);
  Charizard& operator=(const Charizard&);
  };
#endif
