/* Sprite.h */
#pragma once
#include <SDL2/SDL.h>
#include <string>

class Sprite
{
  friend class Graphics;
public:
  SDL_Rect rect;
  SDL_Point velocity;
  double gravity_mult;
  double friction;
  int draw_order;
  bool remove_me;
  bool moveable = true;

  virtual bool onCollide(Sprite* target){return false;}
  bool operator<(const Sprite& other) const;
  virtual ~Sprite(){}
  Sprite(const Sprite& other);

  void setAnimInfo(int x, int y, bool play_once);
private:
  Sprite(SDL_Texture* texture);
  SDL_Texture* image;
  int anim_x = 1;
  int anim_y = 1;
  int anim_index_x = 0;
  int anim_index_y = 0;
  bool remove_on_anim_end = false;
};
bool compare(const Sprite* a, const Sprite* b);

bool collides(const Sprite& a, const Sprite& b);
bool collides(const SDL_Rect& a, const SDL_Rect& b);
