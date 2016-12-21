#ifndef SMARTSPRITE_H
#define SMARTSPRITE_H
#include <iostream>
#include "sprite.h"
#include "ioManager.h"

// SmartSprite is an observer of subject SubjectSprite, which
// in this example is a spinning star.
class ExplodingSprite;
class SmartSprite : public Sprite {
public:
SmartSprite(const std::string&);
  SmartSprite(const std::string&, const Vector2f& pos, int w, int h);
  
  void draw() const;
  void update(Uint32 ticks);
  void goLeft();
  void goRight();
  void goUp();
  void goDown();
void explode();
bool Exploding();
  static void incrSafeDistance() {++safeDistance; }
  static void decrSafeDistance() { --safeDistance; }
  static float getSafeDistance() { return safeDistance; }
  void setPlayerPos(const Vector2f& p) { playerPos = p; }
  ExplodingSprite* getExplosion() {return explosion;}
private:	
 ExplodingSprite* explosion;
  enum MODE {NORMAL, EVADE};
  IOManager& io;	
  Vector2f playerPos;
  int playerWidth;
  int playerHeight;
  static float safeDistance;
  MODE currentMode;
   SmartSprite(const SmartSprite &);
  SmartSprite& operator=(const SmartSprite &rhs);
};

#endif