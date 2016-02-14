#pragma once
#include "Sprite.h"

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Text : public Sprite
{
public:
  Text(const Sprite&);
  ~Text();
};
