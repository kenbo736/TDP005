#pragma once
#include "Sprite.h"
#include <string>
#include "Player.h"
#include <vector>

class Powerup : public Sprite
{
public:
  Powerup(const Sprite& sprite, int buff_index);
  ~Powerup();
  static int Count();
  virtual bool onCollide(Sprite* target);
  static std::vector<void (*)(Player* player)> getBuffs();

  static std::vector<std::string> backgrounds;

  static void invertGravity(Player* player);
  static void hyperSpeed(Player* player);
  static void fuelRefill(Player* player);

private:
  static int total_powerups;
  void (*buff)(Player* player);

  static std::vector<void (*)(Player*)> buffs;
};
