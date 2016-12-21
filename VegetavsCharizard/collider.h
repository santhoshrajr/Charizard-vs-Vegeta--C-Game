#include <cmath>
#include "collisionStrategy.h"
#include "vegeta.h"
#include "flamethrower.h"
#include "chari.h"
#include "bullet.h"
#include "smallSprite.h"
#include "smartSprite.h"
class Collider{
  CollisionStrategy* strategy;
  Collider();
  ~Collider();
public:
  static Collider& getInstance();
  bool collidedWithVegeta(const flamethrower c,
  	const Vegeta* d) const ;
  bool collidedWithChari(const smallSprite s, const Charizard* z)const;
  bool collidedWithDragonBall(const Vegeta* c,const SmartSprite* d) const ;
  
 
private:
  Collider(const Collider&);
  Collider operator=(const Collider&); 
};



