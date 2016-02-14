#pragma once
#include "Sprite.h"
#include <cstdlib>
#include <cmath>

class Enemy : public Sprite
{
public:
  Enemy(const Sprite& sprite, int x_boost, int y_boost);
  ~Enemy();
  static int Count();
  virtual bool onCollide(Sprite* target);
  void update();
private:
  static int total_enemies;
  int x_boost;
  int y_boost;
  int y_start;
  int acceleration_y = 500;
  const int bop_range = 1000;
};
