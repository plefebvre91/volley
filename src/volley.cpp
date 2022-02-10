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
#include "utils.hpp"
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

    ball = new vl::Ball("ball.png", sf::Vector2f(5*VL_WINDOW_WIDTH/8, 0), VL_BALL_FRICTION);
    players[0] = new vl::Character("player.png", sf::Vector2f(VL_WINDOW_WIDTH/4, 0), VL_PLAYER_FRICTION);
    players[1] = new vl::Character("player2.png", sf::Vector2f(3*VL_WINDOW_WIDTH/4, 0), VL_PLAYER_FRICTION);
    //players[0]->setPlayableArea(sf::FloatRect(VL_MARGIN, VL_MARGIN, VL_WINDOW_WIDTH/2 - 4*VL_MARGIN, VL_WINDOW_HEIGHT - VL_MARGIN));
    //players[1]->setPlayableArea(sf::FloatRect(VL_WINDOW_WIDTH/2 + 4*VL_MARGIN, VL_MARGIN, VL_WINDOW_WIDTH/2 - VL_MARGIN, VL_WINDOW_HEIGHT - VL_MARGIN));
    //players[2]->setPlayableArea(sf::FloatRect(VL_MARGIN, VL_MARGIN, VL_WINDOW_WIDTH - VL_MARGIN, VL_WINDOW_HEIGHT - VL_MARGIN));


    for (auto& shadow: shadows) {
      shadow = new sf::CircleShape(VL_SHADOW_WIDTH/2, 10);
      shadow->setFillColor(sf::Color(200,200,200));
      shadow->setOrigin(VL_SHADOW_WIDTH/2, 0);
      shadow->setScale(1.0,0.3);
    }

    _sceneObjects[0] = new Entity("beach.png", sf::Vector2f(0,0));
    _sceneObjects[1] = new Entity("tree.png", sf::Vector2f(80, 250));
    _sceneObjects[2] = new Entity("net.png", sf::Vector2f(450,415));
  }

  void Volley::render() {
    sf::Clock clock;

    while(window->isOpen()){
      window->clear();

      window->draw(_sceneObjects[0]->getSprite());

      for (auto& shadow: shadows)
        window->draw(*shadow);


      window->draw(players[0]->getSprite());

      if (ball->getPosition().x < VL_WINDOW_WIDTH/2) {
        window->draw(ball->getSprite());
        window->draw(_sceneObjects[2]->getSprite());
        window->draw(players[1]->getSprite());
      } else {
        window->draw(_sceneObjects[2]->getSprite());
        window->draw(players[1]->getSprite());
        window->draw(ball->getSprite());
      }

      window->draw(_sceneObjects[1]->getSprite());

      window->display();
    }
  }

  Volley::~Volley() {
    for (auto player: players)
      delete player;
    delete ball;
    for (auto element: _sceneObjects)
      delete element;

    delete window;
  }

  void Volley::resolveCollisions() {
    auto angle = (ball->getVelocity().x * 180.0) / (32.0 * 3.14 * 3);
    ball->rotate(angle);

    if (ball->isCollidingWith(*players[0])) {
      ball->bounce(*players[0]);
    }

    if (ball->isCollidingWith(*players[1])) {
      ball->bounce(*players[1]);
    }

    const sf::Vector2f p = _sceneObjects[2]->getPosition();
    const sf::Vector2u s = _sceneObjects[2]->getSize();

    sf::FloatRect net_box(sf::Vector2f(p.x+s.x/2.0 - 10, p.y+50), sf::Vector2f(20, 300));

    if (net_box.contains(ball->getPosition())) {
      std::cout << "Filet!\n";
    }
  }

  void Volley::update() {
    sf::Clock clock;
    while(window->isOpen()){
      auto dt = clock.restart().asSeconds();

      resolveCollisions();

      ball->update(dt);

      for (auto& player: players)
        player->update(dt);


      for (int i=0; i<VL_NB_PLAYERS; i++) {
        shadows[i]->setPosition(players[i]->getPosition().x, 690);
        shadows[i]->setScale(players[i]->getPosition().y/700, 0.3*players[i]->getPosition().y/700);
      }

      shadows[2]->setPosition(ball->getPosition().x, 690);
      shadows[2]->setScale(ball->getPosition().y/700, 0.3*ball->getPosition().y/700);

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
          case sf::Keyboard::G: ball->handleEvent(vl::Event::JUMP); break;
          case sf::Keyboard::V: ball->handleEvent(vl::Event::LEFT); break;
          case sf::Keyboard::B: ball->handleEvent(vl::Event::RIGHT); break;
          case sf::Keyboard::Escape: window->close(); break;
          case sf::Keyboard::Space:
            ball->setPosition(sf::Vector2f(5*VL_WINDOW_WIDTH/8, 0));
            players[0]->setPosition(sf::Vector2f(VL_WINDOW_WIDTH/4, 0));
            players[1]->setPosition(sf::Vector2f(3*VL_WINDOW_WIDTH/4, 0));
            break;

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
