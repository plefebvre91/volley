/* MIT License

Copyright (c) 2019 Pierre Lefebvre

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */

#include "volley.hpp"
#include "constants.hpp"
#include "utils.h"
#include <X11/Xlib.h>
#include <thread>
#include <iostream>


static void _render(volley* app) {
  app->render();
}
static void _update(volley* app) {
  app->update();
}

volley::volley() {
  XInitThreads();
  window = new
    sf::RenderWindow(sf::VideoMode(VL_WINDOW_WIDTH, VL_WINDOW_HEIGHT), VL_APP_TITLE);
  window->setActive(false);
  window->setFramerateLimit(VL_FPS);

  players[0] = new vl::player("player2.png", VL_PLAYER_ABSORPTION);
  players[1] = new vl::player("player2.png", VL_PLAYER_ABSORPTION);
  players[2] = new vl::player("ball.png", VL_BALL_ABSORPTION);

  players[0]->move(sf::Vector2f(-6.0, 0));
  players[1]->move(sf::Vector2f(11.0, 0));
  players[2]->move(sf::Vector2f(0.0, 3.0));

  background_texture = new sf::Texture();
  background_texture->loadFromFile("beach.png");
  background = new sf::Sprite(*background_texture);

  tree_texture = new sf::Texture();
  tree_texture->loadFromFile("tree.png");
  tree = new sf::Sprite(*tree_texture);
  tree->setPosition(80, 250);

  net_texture = new sf::Texture();
  net_texture->loadFromFile("net.png");
  net = new sf::Sprite(*net_texture);
  net->setPosition(450,415);
}

void volley::render() {
  sf::Clock clock;

  while(window->isOpen()){
    window->clear();

    window->draw(*background);
    window->draw(*(players[0]->get_sprite()));
    window->draw(*net);
    window->draw(*(players[1]->get_sprite()));
    window->draw(*(players[2]->get_sprite()));
    window->draw(*tree);

    window->display();
  }
}

volley::~volley() {
  for (auto player: players)
    delete player;

  delete window;
}

void volley::resolve_gravity(double dt) {
  for (auto& player: players) {
    sf::Vector2f position = player->get_position();
    sf::Vector2f velocity = player->get_velocity();
    sf::Vector2f acceleration = player->get_acceleration();
    sf::Vector2u size = player->get_sprite()->getTexture()->getSize();

    if(position.x > VL_BOUND_RIGHT) {
      position.x = VL_BOUND_RIGHT - 2;
      velocity.x *= -VL_BOUND_RESTITUTION;
    }
    else if(position.x < VL_BOUND_LEFT) {
      position.x = VL_BOUND_LEFT + 2;
      velocity.x *= -VL_BOUND_RESTITUTION;
    }
    if(position.y <= (size.y/2 + 20)) {
      position.y =  (size.y/2 + 20);
      velocity.y *= -VL_BOUND_RESTITUTION;
    }
    if(position.y < VL_FLOOR - (size.y/2))
      velocity.y += VL_GRAVITY*dt;
    else if(position.y >  VL_FLOOR - (size.y/2)) { //If you are below ground
      if (velocity.y > 0.0)
        velocity.y *= -VL_BOUND_RESTITUTION;
      else
        position.y =  VL_FLOOR - (size.y/2);                 //That's not supposed to happen, put him back up
    }

    player->set_physics_attributes(position, velocity, acceleration);
  }


}


void volley::resolve_collisions() {
  std::array<sf::Vector2f, 3> positions {
    players[0]->get_position(),
    players[1]->get_position(),
    players[2]->get_position()
  };

  std::array<sf::Vector2f, 3> velocities {
    players[0]->get_velocity(),
    players[1]->get_velocity(),
    players[2]->get_velocity()
  };

  std::array<sf::Vector2f, 3> accelerations {
    players[0]->get_velocity(),
    players[1]->get_velocity(),
    players[2]->get_velocity()
  };

  auto rotation = (velocities[2].x * 180) / (32 * 3.14);
  players[2]->get_sprite()->rotate(rotation);

  if (vl::utils::sd(positions[0]-sf::Vector2f(0,65), positions[2]) < VL_DIST_BEFORE_COLLISION) {
    const sf::Vector2f& v = vl::utils::nv(positions[2], (positions[0]-sf::Vector2f(0,65))) ;
    accelerations[2].x = 10*v.x;
    accelerations[2].y = 10*v.y;
    players[2]->set_physics_attributes(positions[2], velocities[2], accelerations[2]);
  }
  else if (vl::utils::sd(positions[1]-sf::Vector2f(0,65), positions[2]) < VL_DIST_BEFORE_COLLISION) {
    const sf::Vector2f& v = vl::utils::nv(positions[2], (positions[1]-sf::Vector2f(0,65))) ;
    accelerations[2].x = 10*v.x;
    accelerations[2].y = 10*v.y;
    players[2]->set_physics_attributes(positions[2], velocities[2], accelerations[2]);
  }
  else {
    // Nothing to do...
  }

  sf::Vector2f p = net->getPosition();
  sf::Vector2u s = net->getTexture()->getSize();

  sf::FloatRect net_box(sf::Vector2f(p.x+s.x/2.0 - 10, p.y+50), sf::Vector2f(20, 300));

  if (net_box.contains(positions[2])) {
    std::cout << "Net hit!\n";
  }
}

void volley::update() {
  sf::Clock clock;
  while(window->isOpen()){
    auto dt = clock.restart().asSeconds();

    for (auto& player: players)
      player->update(dt);

    resolve_gravity(dt);
    resolve_collisions();


    std::this_thread::sleep_for(std::chrono::milliseconds(VL_UPDATE_THREAD_MS));
  }
}

void volley::handle_events()
{
  // Start the game loop
  while (window->isOpen())
  {
    // Process events
    sf::Event event;
    while (window->pollEvent(event))
    {
      if(event.type == sf::Event::KeyPressed) {
        switch(event.key.code) {
        case sf::Keyboard::Z: players[0]->jump(); break;
        case sf::Keyboard::I: players[1]->jump(); break;
        case sf::Keyboard::G: players[2]->jump(); break;
        case sf::Keyboard::V: players[2]->move(sf::Vector2f(-5,0)); break;
        case sf::Keyboard::B: players[2]->move(sf::Vector2f(5,0)); break;
        case sf::Keyboard::Escape: window->close(); break;
        default: break;
        }
      }

      if (event.type == sf::Event::Closed)
        window->close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
      players[0]->move(sf::Vector2f(VL_MOVE_LEFT, 0));

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
      players[0]->move(sf::Vector2f(VL_MOVE_RIGHT, 0));

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
      players[1]->move(sf::Vector2f(VL_MOVE_LEFT, 0));

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
      players[1]->move(sf::Vector2f(VL_MOVE_RIGHT, 0));

    std::this_thread::sleep_for(std::chrono::milliseconds(VL_EVENT_THREAD_MS));
  }
}

void volley::run()
{
  std::thread render_thread(&_render, this);
  std::thread update_thread(&_update, this);
  handle_events();
  update_thread.join();
  render_thread.join();
}
