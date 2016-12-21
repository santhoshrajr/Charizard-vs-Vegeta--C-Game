#include <cmath>
#include "flamethrower.h"
#include "gamedata.h"
#include "ioManager.h"
flamethrower::flamethrower(const string& n, const Vector2f& pos, const Vector2f& vel,const Frame* frm):Sprite(n, pos, vel,frm),startPos(pos),distance(0){ 
	startPos=getPosition();
}

flamethrower::flamethrower(const flamethrower& s) :Sprite(s),startPos(s.startPos),distance(s.distance){ }
void flamethrower::update(Uint32 ticks){
   Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
   setPosition(getPosition() + incr);
   distance+=(hypot(incr[0],incr[1]));
}
bool flamethrower::goneFar(){
   return distance>Gamedata::getInstance().getXmlInt("flamethrower/range");
}
void flamethrower::reset() {
   distance = 0;
}


