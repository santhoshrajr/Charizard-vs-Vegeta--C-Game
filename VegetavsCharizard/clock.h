#include <SDL.h>
#include <string>

class Manager;

class Clock {
public:
  static Clock& getInstance();
  unsigned int getTicks() const;
  unsigned int getTotalTicks() const { return sumOfAllTicks; }
  void toggleSloMo();
  

  bool isStarted() const { return started; }
  bool isPaused() const  { return paused;  }
  int getFps() const;
  unsigned int getFrames() const  { return frames;  }
  unsigned int getSeconds() const { return getTicks()/1000;  }
  unsigned int capFrameRate() const;

  void start();
  void pause();
  void unpause();
  void display() const;
  void update();
  unsigned int getElapsedTicks();
 

private:
  
 
  bool started;
  bool paused;
  bool sloMo;
  const bool framesAreCapped;
  const int frameCap;

  unsigned int frames;

  unsigned int tickSum;
  unsigned int sumOfAllTicks;
  unsigned int timeAtStart;
  unsigned int timeAtPause;
  unsigned int currTicks;
  unsigned int prevTicks;
  unsigned int ticks;

  

  

  Clock();
  Clock(const Clock&);
  Clock&operator=(const Clock&);
};
