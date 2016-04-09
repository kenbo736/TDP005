/* World.cc */
#include "World.h"
#include <algorithm>
#include <iostream>
#include <cstdlib>

using namespace std;

World::World(Graphics* graphics){
  this->graphics = graphics;
  reset();
}

World::~World(){
  for(Sprite* sp : sprite_list)
  {
    delete sp;
  }
}

void World::delete_gauge()
{
  removeSprite(powerGaugeX_back);
  removeSprite(powerGaugeX_front);
  powerGaugeX_back = nullptr;
  powerGaugeX_front = nullptr;

  removeSprite(powerGaugeY_back);
  removeSprite(powerGaugeY_front);
  powerGaugeY_back = nullptr;
  powerGaugeY_front = nullptr;

}

void World::update()
{

  player->update();
  if(player->buff == 2 && graphics->getMotionBlur() > 50)
  {
    graphics->setMotionBlur(0.05);
  }
  else if(player->buff != 2 && graphics->getMotionBlur() < 50)
  {
    graphics->setMotionBlur(0.5);
  }




if ( Enemy::Count() < 2 )
{
  Enemy* e = new Enemy(graphics->load_image("enemy.png"),
                       rand()%700+300, rand()%500+300);
  e->setAnimInfo(3, 3, false);
  e->rect.w = 273;
  e->rect.h = 110;
  addSprite(e);
  e->rect.x = getPlayer()->rect.x + (rand()%1000)+500;
  e->rect.y = getPlayer()->rect.y + (rand()%1000)-500 + int(getPlayer()->velocity.y/2);
  e->velocity.x = int(getPlayer()->velocity.x/1.7);
  e->gravity_mult = 0.0;
}

if ( Powerup::Count() < 1 )
{
  //Create Powerup with random buff from buffs
  Powerup* p = new Powerup(graphics->load_image("powerup.png"), rand()%3);
  p->setAnimInfo(3, 3, false);
  addSprite(p);
  p->rect.x = getPlayer()->rect.x + (rand()%1000)+500;
  p->rect.y = getPlayer()->rect.y + (rand()%1000)-500 + int(getPlayer()->velocity.y/2);
  p->velocity.x = int(getPlayer()->velocity.x/1.7);
  p->gravity_mult = 0.0;
}

graphics->setBackground(Powerup::backgrounds[player->buff]);

  for ( Sprite* sp : sprite_list )
  {

      Enemy* enemy = dynamic_cast<Enemy*>(sp);
      if ( enemy != nullptr )
      {
        enemy->update();
        if ( abs(getPlayer()->rect.x - enemy->rect.x) > 1000 )
        {
          enemy->remove_me = true;
          continue;
        }
      }

      Powerup* powerup = dynamic_cast<Powerup*>(sp);
      if ( powerup != nullptr )
      {
        if ( abs(getPlayer()->rect.x - powerup->rect.x) > 1000 )
        {
          powerup->remove_me = true;
        }
      }


  }




  if ( powerGaugeX_back != nullptr && powerGaugeX_front != nullptr
    && powerGaugeY_back != nullptr && powerGaugeY_front != nullptr )
  {

    //X-led
      SDL_Point gaugeOffsetX{ -80, 100 };
      powerGaugeX_back->rect = SDL_Rect
      {
        player->rect.x + gaugeOffsetX.x,
        player->rect.y + gaugeOffsetX.y,
        100,
        10
      };
      powerGaugeX_front->rect = powerGaugeX_back->rect;
      powerGaugeX_front->rect.w = 100*double(player->getLaunchVelocity().x)/player->power;
    //Y-led
      SDL_Point gaugeOffsetY{ -110, -20 };
      powerGaugeY_back->rect = SDL_Rect
      {
        player->rect.x + gaugeOffsetY.x,
        player->rect.y + gaugeOffsetY.y,
        10,
        100
      };
      powerGaugeY_front->rect = powerGaugeY_back->rect;
      powerGaugeY_front->rect.h -= 100+100*double(player->getLaunchVelocity().y)/player->power;
      powerGaugeY_front->rect.y -= -100-1*100*double(player->getLaunchVelocity().y)/player->power;
  } else {

    if ( abs(player->velocity.x) < 10 && abs(player->velocity.y) < 10 && player->rect.y > 30 )
    {
      reset();
    }
  }

  for ( Sprite* sp : sprite_list )
  	{
        Text* text = dynamic_cast<Text*>(sp);
        if ( text != nullptr )
        {
        	text->remove_me = true;
        }
	}

	//Score
  string score_message = "Score: ";
  score_message += to_string(getPlayer()->rect.x*47);
  Text* score = new Text(graphics->stringToSprite(score_message));
  score->draw_order = 100002;
  addSprite(score);
  if(!getPlayer()->hasLaunched())
  {
  	score->rect.y = getPlayer()->rect.y - 220;// + int(getPlayer()->velocity.y*0.016);
  	score->rect.x = getPlayer()->rect.x - 107;// + int(getPlayer()->velocity.x*0.016);
  }
  else
  {
  	score->rect.y = getPlayer()->rect.y - 220 + int(getPlayer()->velocity.y*0.016);
  	score->rect.x = getPlayer()->rect.x - 107 + int(getPlayer()->velocity.x*0.016);
  }
  score->rect.w = 25 * (to_string(getPlayer()->rect.x).size() + 7);
  score->rect.h = 40;

  //Fuel
  string fuel_message = "Fuel: ";
  fuel_message += to_string(getPlayer()->fuel);
  Text* fuel = new Text(graphics->stringToSprite(fuel_message));
  fuel->draw_order = 100003;
  addSprite(fuel);
  if(!getPlayer()->hasLaunched())
  {
  	fuel->rect.y = getPlayer()->rect.y + 165;// + int(getPlayer()->velocity.y*0.016);
  	fuel->rect.x = getPlayer()->rect.x + 270;// + int(getPlayer()->velocity.x*0.016);
  }
  else
  {
  	fuel->rect.y = getPlayer()->rect.y + 160 + int(getPlayer()->velocity.y*0.016);
  	fuel->rect.x = getPlayer()->rect.x + 270 + int(getPlayer()->velocity.x*0.016);
  }
  fuel->rect.w = 20 * (to_string(getPlayer()->fuel).size() + 6);
  fuel->rect.h = 35;
}


void World::reset(){






    graphics->setBackground("sky.jpg");
    graphics->setGround("ground.png");
    graphics->setMotionBlur(0.5);

  setPlayer("powerup.png", 0, 0);
  player->setAnimInfo(3,3,true);



    powerGaugeX_front = new Sprite(graphics->load_image("green.png"));
    powerGaugeX_back  = new Sprite(graphics->load_image("red.png"));
    powerGaugeX_front->draw_order = 10001;
    powerGaugeX_back->draw_order = 10000;
    addSprite(powerGaugeX_back);
    addSprite(powerGaugeX_front);

      powerGaugeY_front = new Sprite(graphics->load_image("green.png"));
      powerGaugeY_back  = new Sprite(graphics->load_image("red.png"));
      powerGaugeY_front->draw_order = 10001;
      powerGaugeY_back->draw_order = 10000;
      addSprite(powerGaugeY_back);
      addSprite(powerGaugeY_front);


      powerGaugeX_back->moveable = false;
      powerGaugeY_back->moveable = false;
      powerGaugeX_front->moveable = false;
      powerGaugeY_front->moveable = false;


      for ( Sprite* sp : sprite_list )
      {
          Enemy* enemy = dynamic_cast<Enemy*>(sp);
          if ( enemy != nullptr )
          {
            enemy->remove_me = true;
          }

          Powerup* powerup = dynamic_cast<Powerup*>(sp);
          if ( powerup == nullptr ) continue;
            powerup->remove_me = true;
      }

}

Player* World::getPlayer()
{
  return player;
}

void World::addSprite(Sprite* sprite)
{
  sprite_list.push_back(sprite);
}

void World::removeSprite(const Sprite* sprite)
{
  auto pos = find(sprite_list.begin(), sprite_list.end(), sprite);
  if(pos != sprite_list.end())
  {
    sprite_list.erase(pos);
  }
}

void World::setPlayer(const string& file_name, int x, int y)
{
  if(player != nullptr)
  {
    removeSprite(player);
    delete player;
    player = nullptr;
  }
  player = new Player(graphics->load_image(file_name));
  player->rect = SDL_Rect{x, y, 50, 50};
  addSprite(player);
}
