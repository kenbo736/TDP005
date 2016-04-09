/* Graphics.h */
#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <vector>
#include <string>
#include <map>
#include <algorithm>


#include "../../World/Sprite.h"

class Graphics
{
public:
  Graphics(SDL_Window* window);
  ~Graphics();
  void setBackground(const std::string& file_name);
  void setGround(const std::string& file_name);
  void draw(std::vector<Sprite*>& sprite_list, int camera_x, int camera_y);
  Sprite load_image(const std::string& file_name);
  Sprite stringToSprite(const std::string&);

  void setMotionBlur( double percent );
  int getMotionBlur();

private:
  SDL_Window* window;
  SDL_Renderer* renderer;
  Sprite* background;
  Sprite* ground;
  TTF_Font* font;
  std::map<std::string, SDL_Texture*> images;
  SDL_Texture* rendered_image;
  SDL_Texture* screen_texture;
  int motion_blur = 127;
  int anim_tick_ms = 25;
  int anim_timestamp = 0;
};
