#ifndef FLAMETHROWER__H
#define FLAMETHROWER__H
#include <string>
#include "sprite.h"

class flamethrower : public Sprite {
public:
  flamethrower(const std::string&, 
         const Vector2f& pos, const Vector2f& vel, const Frame*);
  flamethrower(const std::string&, const Vector2f& pos, const Vector2f& vel);

  flamethrower(const flamethrower& s);
  virtual ~flamethrower() { } 
  void reset();
bool goneFar();
  flamethrower& operator=(const flamethrower&);
  void update(Uint32);	
private:
  Vector2f startPos;
  float distance;
};
#endif
