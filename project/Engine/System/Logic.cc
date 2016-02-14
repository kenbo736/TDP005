/* Logic.cc */
#include <iostream>
#include <utility>
#include "Logic.h"
using namespace std;
Logic::Logic(SDL_Window* window, int fps)
{
  this->window = window;
  last_time = SDL_GetTicks();
  this->fps = fps;
  gravity = 1000;
  bottom = 100;
}

Logic::~Logic(){}


void Logic::apply_gravity_and_friction(World& world, double deltatime)
{
  for ( Sprite* sp : world.sprite_list )
  {
    if ( !sp->moveable ) continue;
    double friction_mult = (1.0-(sp->friction*deltatime));
    if ( friction_mult < 0.0 )
    {
      friction_mult = 0.0;
    }
    else if ( friction_mult > 1.0 )
    {
      friction_mult = 1.0;
    }
    sp->velocity.x *= friction_mult;
    sp->velocity.y *= friction_mult;

    sp->velocity.y += deltatime * gravity * sp->gravity_mult;

  }
}


void Logic::move_sprites(World& world, double deltatime )
{
  // uppdatera positioner:
  for(int i = 0; i < world.sprite_list.size(); i++)
  {
    if ( world.sprite_list[i]->remove_me || !world.sprite_list[i]->moveable ) continue;

    SDL_Rect new_pos = world.sprite_list[i]->rect;

    new_pos.x += int(double(world.sprite_list[i]->velocity.x)*deltatime);
    new_pos.y += int(double(world.sprite_list[i]->velocity.y)*deltatime);

    bool can_move = true;
    // studsa:
    if ( new_pos.y+world.sprite_list[i]->rect.h > bottom )
    {
      can_move = false;
      world.sprite_list[i]->velocity.y = -int(double(world.sprite_list[i]->velocity.y) * 0.75);
      world.sprite_list[i]->velocity.x = int(double(world.sprite_list[i]->velocity.x) * 0.75);
    }
    for (int j = 0; j < world.sprite_list.size(); j++)
    {
      if (i==j) continue;
      if (collides(new_pos,world.sprite_list[j]->rect)
        && world.sprite_list[i]->onCollide(world.sprite_list[j]))
      {
          can_move = false;
      }
    }
    if (can_move)
    {
      world.sprite_list[i]->rect = new_pos;
    }
  }
}

void Logic::remove_old_sprites(World& world)
{
  for (int i = 0; i < world.sprite_list.size(); i++)
  {
    if ( world.sprite_list[i] == world.getPlayer() ) continue;
    if ( world.sprite_list[i]->remove_me )
    {
      delete world.sprite_list[i];
      world.sprite_list[i] = nullptr;
      world.sprite_list[i] = world.sprite_list[ world.sprite_list.size()-1 ];
      world.sprite_list.pop_back();
      i--;
    }
  }
}

void Logic::think(World& world)
{
  unsigned int now = SDL_GetTicks();
  double deltatime = double(now-last_time)/1000;
  double frametime = 1.0 / fps;
  if ( deltatime > frametime )
  {
    deltatime = frametime;
  }
  last_time = now;

  world.update();

  //TODO:fixa fiender
  //TODO:spawna powerups
    //LSD
    //Space
    //green cloud (slowdown)
  apply_gravity_and_friction(world, deltatime);
  move_sprites(world, deltatime);
  remove_old_sprites(world);
}
