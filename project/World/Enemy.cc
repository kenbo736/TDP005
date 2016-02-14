#include "Enemy.h"
#include "Player.h"

int Enemy::total_enemies{0};


int Enemy::Count()
{
  return total_enemies;
}

Enemy::Enemy(const Sprite& sprite, int x_boost, int y_boost) : Sprite(sprite)
{
  total_enemies++;
  this->y_boost = y_boost;
  this->x_boost = x_boost;
  y_start = rect.y + rand()%bop_range-(bop_range/2);
}

bool Enemy::onCollide(Sprite* target){
  Player* player = dynamic_cast<Player*>(target);
  if ( player != nullptr)
  {
    player->velocity.y -= y_boost;
    player->velocity.x += x_boost;
    remove_me = true;

    return true;
  }

  Enemy* enemy = dynamic_cast<Enemy*>(target);
  if ( enemy != nullptr )
  {
    enemy->remove_me = true;
    return false;
  }
}

void Enemy::update()
{
  velocity.y += acceleration_y;
  if(rect.y >= y_start + bop_range/4 || rect.y <= y_start - bop_range/4)
  {
    acceleration_y *= -1;
  }
}

Enemy::~Enemy()
{
    total_enemies--;
}
