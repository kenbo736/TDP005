#include <iostream>

/* Input.cc */
#include "Input.h"

Input::Input(int fps)
{
  this->fps = fps;
}

Input::~Input(){}

bool Input::update(World& world)
{
  unsigned int now = SDL_GetTicks();
  double deltatime = double(now-last_time)/1000;
  double frametime = 1.0 / fps;
  if ( deltatime > frametime )
  {
    deltatime = frametime;
  }
  last_time = now;
  // Handle events
  SDL_PumpEvents();
  for(SDL_Event e; SDL_PollEvent(&e);)
  {
    switch(e.type)
    {
      case SDL_QUIT:
         return false;
      case SDL_KEYDOWN:
        if(!press_button(e.key.keysym.sym, world,deltatime))
        {
          return false;
        }
        break;
      default:
         break;
    }
  }
  return true;
}

bool Input::press_button(const SDL_Keycode& key, World& world,double deltatime)
{
  const int speed_add = 543*deltatime;
  SDL_Point movement{0, 0};
  switch (key){
    case SDLK_ESCAPE:
      return false;
    case SDLK_LEFT:
    case SDLK_a:
      //movement.x = -speed_add;
      break;
    case SDLK_UP:
    case SDLK_w:
      movement.y = -speed_add;
      world.getPlayer()->fuel--;
      break;
    case SDLK_RIGHT:
    case SDLK_d:
      movement.x = speed_add;
      world.getPlayer()->fuel--;
      break;
    case SDLK_DOWN:
    case SDLK_s:
      movement.y = speed_add;
      world.getPlayer()->fuel--;
      break;
    case SDLK_SPACE:
       if(world.getPlayer() != nullptr){
         if ( !world.getPlayer()->hasLaunched() )
         {
           world.setPlayer("captain-america.png", world.getPlayer()->rect.x, world.getPlayer()->rect.y);
           world.getPlayer()->launch();
           world.deleteGauge();
         }
       }
       break;
    case SDLK_r:
      if ( !world.getPlayer()->hasLaunched() ) world.deleteGauge();
      world.reset();
      break;
  }

  if(world.getPlayer() != nullptr && world.getPlayer()->hasLaunched() && world.getPlayer()->fuel > 0)
  {
    //world.getPlayer()->velocity.x += movement.x;
    world.getPlayer()->velocity.y += movement.y*8;
  }

  if ( world.getPlayer()->fuel < 0) world.getPlayer()->fuel = 0;

  return true;
}
