#include "Powerup.h"
#include "Player.h"
#include <iostream>

int Powerup::total_powerups{0};

std::vector<void (*)(Player*)> Powerup::buffs;

std::vector<std::string> Powerup::backgrounds(3);


int Powerup::Count()
{
  return total_powerups;
}

std::vector<void (*)(Player* player)> Powerup::getBuffs()
{
  return buffs;
}

Powerup::Powerup(const Sprite& sprite, int buff_index) : Sprite(sprite)
{
  total_powerups++;
  if(buffs.size() < 3)
  {
    buffs.push_back(invertGravity);
    buffs.push_back(hyperSpeed);
    buffs.push_back(fuelRefill);
    backgrounds[0] = "sky.png";
    backgrounds[1] = "space.jpg";
    backgrounds[2] = "LSD.png";
  }
  this->buff = buffs[buff_index];
}

bool Powerup::onCollide(Sprite* target){
  Player* player = dynamic_cast<Player*>(target);
  if ( player != nullptr)
  {
    buff(player);
    remove_me = true;
    return true;
  }

  return false;
}

void Powerup::invertGravity(Player* player)
{
  player->gravity_mult = -1.0;
  player->startBuff(1);
}

void Powerup::hyperSpeed(Player* player)
{
  player->friction = 0.0;
  player->startBuff(2);
}

void Powerup::fuelRefill(Player* player)
{
  player->fuel += 100;
}

Powerup::~Powerup()
{
    total_powerups--;
}
