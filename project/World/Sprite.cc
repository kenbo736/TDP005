/* Sprite.cc */
#include "Sprite.h"

Sprite::Sprite(SDL_Texture* image)
{
  this->image = image;
  rect = SDL_Rect{0, 0, 50, 50};
  velocity = SDL_Point{0,0};
  draw_order = 0;
  remove_me = false;
  gravity_mult = 1.0;
  friction = 0.0f;
}

Sprite::Sprite(const Sprite& other) : Sprite(other.image)
{}

bool Sprite::operator<(const Sprite& other) const
{
  return draw_order < other.draw_order;
}

bool compare(const Sprite* a, const Sprite* b)
{
  return *a < *b;
}

bool collides(const Sprite& a, const Sprite& b)
{
  return collides( a.rect, b.rect );
}

bool collides(const SDL_Rect& a, const SDL_Rect& b)
{
  return ((a.x + a.w >= b.x && a.x <= b.x + b.w) && (a.y + a.h >= b.y && a.y <= b.y + b.h));
}

void Sprite::setAnimInfo(int x, int y, bool play_once)
{
  if (x<1) x=1;
  if (y<1) y=1;
  anim_x = x;
  anim_y = y;
  remove_on_anim_end = play_once;
}
