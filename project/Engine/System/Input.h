/* Input.h */
#pragma once
#include "../../World/Sprite.h"
#include "../../World/World.h"

#include <SDL2/SDL.h>

class Input
{
public:
  Input(int fps);
  ~Input();
  bool update(World& world);

private:
  bool press_button(const SDL_Keycode& key, World& world, double deltatime);
  unsigned int last_time;
  int fps;
};
