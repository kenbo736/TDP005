/* Player.cc */
#include "Player.h"
#include <cmath>
#include <SDL2/SDL.h>

Player::Player(const Sprite& sprite) : Sprite(sprite)
{
  gravity_mult = 1.0;
  friction = 0.1f;
}

bool Player::onCollide(Sprite* target)
{
  return false;
}

bool Player::hasLaunched() const
{
  return hasStarted;
}

SDL_Point Player::getLaunchVelocity()
{
    double timestamp = double(SDL_GetTicks()) / 500;
    int x = int(power * cos(timestamp*1.5));
    int y = int(power * sin(timestamp));
    return SDL_Point{abs(x),-abs(y)};
}

void Player::launch()
{
  if (!hasStarted)
  {
    velocity = getLaunchVelocity();
    velocity.x *= 3;
    velocity.y *= 3;
    hasStarted = true;
  }
}

void Player::update()
{
  if(SDL_GetTicks() - buff_start_time >= 3000)
  {
    buff = 0;
    gravity_mult = 1.0;
    friction = 0.1f;
  }
}

void Player::startBuff(int buff)
{
  buff_start_time = SDL_GetTicks();
  this->buff = buff;
}
