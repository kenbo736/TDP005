/* Level.h */
#pragma once
#include "Sprite.h"
#include "Player.h"
#include "Enemy.h"
#include "Powerup.h"
#include "Text.h"
#include "../Engine/Graphics/Graphics.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

class World
{
public:
  World(Graphics* graphics);
  ~World();
  std::vector<Sprite*> sprite_list;
  void reset();
  void update();
  Player* getPlayer();
  void setPlayer(const std::string& file_name, int x, int y);
  void removeSprite(const Sprite* sprite);
  void addSprite(Sprite* sprite);
  void deleteGauge();


private:

  Player* player = nullptr;
  Graphics* graphics;

  Sprite* powerGaugeX_front;
  Sprite* powerGaugeX_back;
  Sprite* powerGaugeY_front;
  Sprite* powerGaugeY_back;

  void spawnEnemy();
  void spawnPowerup();
  void cleanSprites();
  void changeBackground();
  void spawnGauge();
  void updateGauge();
  void updateScore();
  void updateFuel();
};
