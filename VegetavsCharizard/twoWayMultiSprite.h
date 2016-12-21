#include <string>
#include <vector>
#include "drawable.h"

class ExplodingSprite;
class TwoWayMultiSprite : public Drawable {
public:
    TwoWayMultiSprite(const std::string&, const std::string&);


  virtual ~TwoWayMultiSprite() { } 

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const; 
 const std::vector<Frame *> getframes() const;
 void explode();
protected: 
  ExplodingSprite* explosion;
   const std::vector<Frame *> frames_RHS;
  const std::vector<Frame *> frames_LHS;
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;
    TwoWayMultiSprite(const TwoWayMultiSprite&);
     TwoWayMultiSprite& operator=(const TwoWayMultiSprite&);
  void advanceFrame(Uint32 ticks);


};

