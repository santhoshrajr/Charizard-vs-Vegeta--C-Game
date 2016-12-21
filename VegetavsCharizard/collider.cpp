#include <iostream>
#include "collider.h"
Collider::Collider() : strategy(new PerPixelCollisionStrategy){}
Collider::~Collider(){
  delete strategy;
}
bool Collider::collidedWithVegeta(const flamethrower c,
	const Vegeta* d) const  {
  return strategy->execute(c, *d);
}

bool Collider::collidedWithChari(const smallSprite s,
	const Charizard* z) const  {
  return strategy->execute(s, *z);
}

bool Collider::collidedWithDragonBall(const Vegeta* c,
	const SmartSprite* d) const  {
  return strategy->execute(*c, *d);
}

Collider& Collider::getInstance(){
  static Collider collider;
  return collider;
}
