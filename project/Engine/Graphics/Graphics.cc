/* Graphics.cc */
#include "Graphics.h"
#include <iostream>
#include <string>
using namespace std;

// Builds the graphics
Graphics::Graphics(SDL_Window* window) : window{window}
{
  int w, h;

	SDL_GetWindowSize(window, &w, &h);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);

  rendered_image = SDL_CreateTexture( renderer,
                                 SDL_PIXELFORMAT_RGBA8888,
                                 SDL_TEXTUREACCESS_TARGET,
                                 w,h);

  screen_texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGBA8888,
    SDL_TEXTUREACCESS_TARGET, w,h);

  // Make scaled rendering look smooth
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	// Render at virtual resolution
	SDL_RenderSetLogicalSize(renderer, w, h);


    anim_timestamp = SDL_GetTicks();

    TTF_Init();

    font = TTF_OpenFont("./Fonts/Commodore-64-Pixeled.ttf", 20);
    if(!font)
    {
      cout << TTF_WasInit() << endl;
      cout << "TTF_OpenFont failed: " << TTF_GetError() << endl;
      exit(1);
    }
}

// Destructor
Graphics::~Graphics()
{
  for(auto& key : images)
  {
    SDL_DestroyTexture(key.second);
  }

  TTF_CloseFont(font);

  delete background;
  background = nullptr;

  SDL_DestroyRenderer(renderer);
  renderer = nullptr;
}

Sprite Graphics::stringToSprite(const string& message)
{
  SDL_Color color = {255, 255, 255};
  const char* text = message.c_str();
  SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text, color);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
  SDL_FreeSurface(surfaceMessage);
  Sprite sprite(texture);
  return sprite;
}

void Graphics::setBackground(const string& file_name)
{
  background = new Sprite(load_image(file_name));

  int screen_width;
  int screen_height;

  SDL_GetWindowSize(window, &screen_width, &screen_height);
  background->rect = SDL_Rect{0, 0, screen_width, screen_height};
}

void Graphics::setGround(const string& file_name)
{
  ground = new Sprite(load_image(file_name));

  int screen_width;
  int screen_height;

  SDL_GetWindowSize(window, &screen_width, &screen_height);
  ground->rect = SDL_Rect{0, 100, screen_width, screen_height};
}


void Graphics::draw(vector<Sprite*>& sprite_list, int camera_x, int camera_y)
{
  int new_time = SDL_GetTicks();
  bool update_animations = new_time-anim_timestamp > anim_tick_ms;
  if ( update_animations ) anim_timestamp = new_time;
  // Clear screen
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_SetRenderTarget(renderer, rendered_image);
  SDL_RenderClear(renderer);

  // Adjust the balls x and y positionnu
  const SDL_Point draw_offset{ 140, 240 };

  // Draw background
  int screen_width;
  int screen_height;

  SDL_GetWindowSize(window, &screen_width, &screen_height);

  int mod_camera_x = camera_x % screen_width;
  mod_camera_x += screen_width;
  mod_camera_x %= screen_width;
  mod_camera_x = screen_width-mod_camera_x;
  int mod_camera_y = camera_y % screen_height;
  mod_camera_y += screen_height;
  mod_camera_y %= screen_height;
  mod_camera_y = screen_height-mod_camera_y;

  SDL_Rect bg_dst[4]
  {
    { mod_camera_x, mod_camera_y, background->rect.w, background->rect.h },
    { mod_camera_x-screen_width, mod_camera_y, background->rect.w, background->rect.h },
    { mod_camera_x, mod_camera_y-screen_height, background->rect.w, background->rect.h },
    { mod_camera_x-screen_width, mod_camera_y-screen_height, background->rect.w, background->rect.h }
  };


  for (int i = 0; i < 4; i++)
  {
    SDL_RenderCopy(renderer, background->image, nullptr, &bg_dst[i]);
  }

  SDL_Rect g_dst[2]
  {
    { mod_camera_x,                             ground->rect.y-camera_y+draw_offset.y,
      ground->rect.w, ground->rect.h },
    { mod_camera_x-screen_width,                ground->rect.y-camera_y+draw_offset.y,
      ground->rect.w, ground->rect.h },
  };

  for (int i = 0; i < 2; i++)
  {
    SDL_RenderCopy(renderer, ground->image, nullptr, &g_dst[i]);
  }

  //sortera efter draw order så att saker ritas på rätt djup
  sort(sprite_list.begin(), sprite_list.end(), &compare);
  for(Sprite* sprite : sprite_list)
  {
    // hantera animerade sprites genom att göra en modifierad source rect
    int w, h;
    int anim_index = sprite->anim_x*sprite->anim_index_y + sprite->anim_index_x;
    SDL_QueryTexture(sprite->image,nullptr,nullptr,&w,&h);
    w /= sprite->anim_x;
    h /= sprite->anim_y;

    if ( update_animations )
    {
      if ( ++sprite->anim_index_x == sprite->anim_x )
      {
        sprite->anim_index_x = 0;
        if ( ++sprite->anim_index_y == sprite->anim_y )
        {
          sprite->anim_index_y = 0;
          if ( sprite->remove_on_anim_end )
          {
            sprite->remove_me = true;
          }
        }
      }
    }

    SDL_Rect src{ sprite->anim_index_x * w,sprite->anim_index_y * h, w,h};

    SDL_Rect dst{sprite->rect.x-camera_x+draw_offset.x,
      sprite->rect.y-camera_y+draw_offset.y,
      sprite->rect.w, sprite->rect.h};

    //SDL_Rect dst{ 0, 0, 50, 50 };
    SDL_RenderCopy(renderer, sprite->image, &src, &dst);
  }

  SDL_SetRenderTarget(renderer, screen_texture);
  SDL_Rect screen_rect{0,0,screen_width,screen_height};
  SDL_SetTextureBlendMode(rendered_image,SDL_BLENDMODE_BLEND);

  SDL_SetTextureAlphaMod(rendered_image,motion_blur);
  SDL_RenderCopy(renderer,rendered_image,nullptr,&screen_rect);
  SDL_SetRenderTarget(renderer, nullptr);
  SDL_RenderCopy(renderer,screen_texture,nullptr,&screen_rect);

  // Show the newly drawn things
  SDL_RenderPresent(renderer);
}

Sprite Graphics::load_image(const string& file_name)
{
  const string images_path = "./Images/";
  if(this->images.count(file_name) == 0)
  {
    SDL_Surface* surface = IMG_Load((images_path+file_name).c_str());
    if ( surface == nullptr )
    {
      cout << "could not load image " << images_path+file_name << endl;
      exit(1);
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    images[file_name] = texture;

    SDL_FreeSurface(surface);
  }
  return Sprite(images.at(file_name));
}


void Graphics::setMotionBlur( double percent )
{
  if ( percent < 0.05 ) percent = 0.05;
  if ( percent > 1.0 ) percent = 1.0;
  motion_blur = int( 255*percent );
}

int Graphics::getMotionBlur()
{
  return motion_blur;
}
