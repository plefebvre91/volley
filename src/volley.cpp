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


namespace vl {

  static void _render(vl::Volley* app) {
    app->render();
  }
  static void _update(vl::Volley* app) {
    app->update();
  }

  Volley::Volley() {
    XInitThreads();
    window = new
      sf::RenderWindow(sf::VideoMode(VL_WINDOW_WIDTH, VL_WINDOW_HEIGHT), VL_APP_TITLE);
    window->setActive(false);
    window->setFramerateLimit(VL_FPS);

    players[0] = new vl::Player("player2.png", VL_PLAYER_ABSORPTION);
    players[1] = new vl::Player("player2.png", VL_PLAYER_ABSORPTION);
    players[2] = new vl::Player("ball.png", VL_BALL_ABSORPTION);

    players[0]->setPlayableArea(sf::FloatRect(VL_MARGIN, VL_MARGIN, VL_WINDOW_WIDTH/2 - VL_MARGIN, VL_WINDOW_HEIGHT - VL_MARGIN));
    players[1]->setPlayableArea(sf::FloatRect(VL_WINDOW_WIDTH/2 + VL_MARGIN, VL_MARGIN, VL_WINDOW_WIDTH/2 - VL_MARGIN, VL_WINDOW_HEIGHT - VL_MARGIN));
    players[2]->setPlayableArea(sf::FloatRect(VL_MARGIN, VL_MARGIN, VL_WINDOW_WIDTH - VL_MARGIN, VL_WINDOW_HEIGHT - VL_MARGIN));

    for (auto& player: players)
      player->resetPosition();

    players[2]->move(sf::Vector2f(-4.0, 0));

    for (auto& shadow: shadows) {
      shadow = new sf::CircleShape(VL_SHADOW_WIDTH/2, 10);
      shadow->setFillColor(sf::Color(200,200,200));
      shadow->setOrigin(VL_SHADOW_WIDTH/2, 0);
      shadow->setScale(1.0,0.3);
    }

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

  void Volley::render() {
    sf::Clock clock;

    while(window->isOpen()){
      window->clear();

      window->draw(*background);

      for (auto& shadow: shadows)
        window->draw(*shadow);

      window->draw(*(players[0]->getSprite()));
      window->draw(*net);


      window->draw(*(players[1]->getSprite()));
      window->draw(*(players[2]->getSprite()));
      window->draw(*tree);

      window->display();
    }
  }

  Volley::~Volley() {
    for (auto player: players)
      delete player;

    delete window;
  }



  void Volley::resolveCollisions() {
    std::array<sf::Vector2f, 3> positions {
      players[0]->getPosition(),
      players[1]->getPosition(),
      players[2]->getPosition()
    };

    std::array<sf::Vector2f, 3> velocities {
      players[0]->getVelocity(),
      players[1]->getVelocity(),
      players[2]->getVelocity()
    };

    std::array<sf::Vector2f, 3> accelerations {
      players[0]->getVelocity(),
      players[1]->getVelocity(),
      players[2]->getVelocity()
    };

    auto rotation = (velocities[2].x * 180) / (32 * 3.14);
    players[2]->getSprite()->rotate(rotation);

    if (vl::utils::sd(positions[0]-sf::Vector2f(0,65), positions[2]) < VL_DIST_BEFORE_COLLISION) {
      const sf::Vector2f& v = vl::utils::nv(positions[2], (positions[0]-sf::Vector2f(0,65))) ;
      accelerations[2].x = 10*v.x;
      accelerations[2].y = 10*v.y;
      players[2]->setPhysicsAttributes(positions[2], velocities[2], accelerations[2]);
    }
    else if (vl::utils::sd(positions[1]-sf::Vector2f(0,65), positions[2]) < VL_DIST_BEFORE_COLLISION) {
      const sf::Vector2f& v = vl::utils::nv(positions[2], (positions[1]-sf::Vector2f(0,65))) ;
      accelerations[2].x = 10*v.x;
      accelerations[2].y = 10*v.y;
      players[2]->setPhysicsAttributes(positions[2], velocities[2], accelerations[2]);
    }
    else {
      // Nothing to do...
    }

    const sf::Vector2f p = net->getPosition();
    const sf::Vector2u s = net->getTexture()->getSize();

    sf::FloatRect net_box(sf::Vector2f(p.x+s.x/2.0 - 10, p.y+50), sf::Vector2f(20, 300));

    if (net_box.contains(positions[2])) {
      std::cout << "Net hit!\n";
    }
  }

  void Volley::update() {
    sf::Clock clock;
    while(window->isOpen()){
      auto dt = clock.restart().asSeconds();

      for (auto& player: players)
        player->update(dt);

      resolveCollisions();

      for (int i=0; i<VL_NB_PLAYERS; i++) {
        shadows[i]->setPosition(players[i]->getPosition().x, 690);
        shadows[i]->setScale(players[i]->getPosition().y/700, 0.3*players[i]->getPosition().y/700);
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(VL_UPDATE_THREAD_MS));
    }
  }

  void Volley::handleEvents()
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
          case sf::Keyboard::Z: players[0]->handleEvent(vl::Event::JUMP); break;
          case sf::Keyboard::I: players[1]->handleEvent(vl::Event::JUMP); break;
          case sf::Keyboard::G: players[2]->handleEvent(vl::Event::JUMP); break;
          case sf::Keyboard::V: players[2]->handleEvent(vl::Event::LEFT); break;
          case sf::Keyboard::B: players[2]->handleEvent(vl::Event::RIGHT); break;
          case sf::Keyboard::Escape: window->close(); break;
          default: break;
          }
        }

        if (event.type == sf::Event::Closed)
          window->close();
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        players[0]->handleEvent(vl::Event::LEFT);

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        players[0]->handleEvent(vl::Event::RIGHT);

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
        players[1]->handleEvent(vl::Event::LEFT);

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        players[1]->handleEvent(vl::Event::RIGHT);

      std::this_thread::sleep_for(std::chrono::milliseconds(VL_EVENT_THREAD_MS));
    }
  }

  void Volley::run()
  {
    std::thread render_thread(&_render, this);
    std::thread update_thread(&_update, this);
    handleEvents();
    update_thread.join();
    render_thread.join();
  }
}
