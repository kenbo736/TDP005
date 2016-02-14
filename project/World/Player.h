/* Player.h */
#pragma once
#include "Sprite.h"

class Player : public Sprite
{
public:
  Player(const Sprite& sprite);
  virtual bool onCollide(Sprite* target);
  void launch();
  bool hasLaunched() const;
  SDL_Point getLaunchVelocity();
  void startBuff(int buff);
  void update();

  int power = 1000;
  int fuel = 100;
  int buff = 0;
private:
  bool hasStarted = false;
  int buff_start_time;
};
