/* Game.cc */
#include "Game.h"
#include <iostream>

using namespace std;

// Builds the screen
Game::Game()
{
  const int SCREEN_WIDTH = 640;
  const int SCREEN_HEIGHT = 480;
	// Initialize SDL
  if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cerr << "SDL_Init Error: " << SDL_GetError() << endl;
		exit(1);
	}

  window = SDL_CreateWindow("Banana Boy",
  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
  SCREEN_WIDTH, SCREEN_HEIGHT, 0);

  fps = 60;

  // Skicka fönstret till grafikmotorn
  graphics = new Graphics(window);
  logic = new Logic(window,fps);
  input = new Input(fps);

  world = new World(graphics);
}

// Destructor
Game::~Game()
{
  delete graphics;
  delete logic;
  delete input;

  delete world;

  SDL_DestroyWindow(window);
  SDL_Quit();
}

// Builds the screen
void Game::run()
{
  bool running = true;
  while(running)
  {
    const unsigned int start = SDL_GetTicks();
    running = input->update(*world);
    logic->think(*world);
    graphics->draw(world->sprite_list,
      world->getPlayer()->rect.x, world->getPlayer()->rect.y);

    const unsigned int delta = SDL_GetTicks() - start;
    const unsigned int frametime = (unsigned int)(1000.0/double(fps));
    if(delta < frametime)
    {
      SDL_Delay(frametime - delta);
      //cout << 1000.0 / double(frametime) << "fps" << endl;
    } else{
      //cout << 1000.0 / double(delta) << "fps" << endl;
    }
  }
}
