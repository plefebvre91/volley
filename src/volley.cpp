/* MIT License

Copyright (c) 2022 Pierre Lefebvre

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
#include "assets.hpp"
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

    // Create SFML window
    _window = new
      sf::RenderWindow(sf::VideoMode(VL_WINDOW_WIDTH, VL_WINDOW_HEIGHT), VL_APP_TITLE);
    _window->setActive(false);
    _window->setFramerateLimit(VL_FPS);

    // Create players sprite
    _players[0] = new vl::Character(VL_ASSET_IMG_PLAYER1, sf::Vector2f(VL_WINDOW_WIDTH/4, 660), VL_PLAYER_FRICTION);
    _players[0]->setPlayableArea(sf::FloatRect(VL_MARGIN, VL_MARGIN, VL_WINDOW_WIDTH/2 - 4*VL_MARGIN, VL_WINDOW_HEIGHT - VL_MARGIN));

    _players[1] = new vl::Character(VL_ASSET_IMG_PLAYER2, sf::Vector2f(3*VL_WINDOW_WIDTH/4, 600), VL_PLAYER_FRICTION);
    _players[1]->setPlayableArea(sf::FloatRect(VL_WINDOW_WIDTH/2 + 4*VL_MARGIN, VL_MARGIN, VL_WINDOW_WIDTH/2 - 4*VL_MARGIN, VL_WINDOW_HEIGHT - VL_MARGIN));

    // Create ball sprite
    _ball = new vl::Ball(VL_ASSET_IMG_BALL, sf::Vector2f(5*VL_WINDOW_WIDTH/8, 200), VL_BALL_FRICTION);
    _ball->setPlayableArea(sf::FloatRect(VL_MARGIN, VL_MARGIN, VL_WINDOW_WIDTH - VL_MARGIN, VL_WINDOW_HEIGHT - VL_MARGIN));
    _ball->setObserver(this);

    // Initialize score value
    _score = new Score();
    _scores[0] = 0u;
    _scores[1] = 0u;
    _lastPlayer = 0u;

    // Create shadows
    for (auto& shadow: _shadows) {
      shadow = new sf::CircleShape(VL_SHADOW_WIDTH/2, 10u);
      shadow->setFillColor(sf::Color(200u,200u,200u));
      shadow->setOrigin(VL_SHADOW_WIDTH/2, 0u);
      shadow->setScale(1.0f, 0.3f);
    }

    // Create other NP objects
    _sceneObjects[0] = new vl::Entity(VL_ASSET_IMG_BEACH, sf::Vector2f(0.0f, 0.0f));
    _sceneObjects[1] = new vl::Entity(VL_ASSET_IMG_TREE, sf::Vector2f(80.0f, 250.0f));
    _sceneObjects[2] = new vl::Entity(VL_ASSET_IMG_NET, sf::Vector2f(450.0f, 415.0f));

    _sounds[0] = new vl::Sound(VL_ASSET_SND_LOSE);
    _sounds[1] = new vl::Sound(VL_ASSET_SND_BOUNCE);
  }

  Volley::~Volley() {
    for (auto element: _sounds)
      delete element;

    for (auto element: _sceneObjects)
      delete element;

    for (auto element: _shadows)
      delete element;

    delete _score;
    delete _ball;

    for (auto element: _players)
      delete element;

    delete _window;
  }

  void Volley::render() {
    sf::Clock clock;

    while(_window->isOpen()){
      _window->clear();

      _window->draw(_sceneObjects[0]->getSprite());

      for (auto& shadow: _shadows)
        _window->draw(*shadow);

      _window->draw(_players[0]->getSprite());

      if (_ball->getPosition().x < VL_WINDOW_WIDTH/2) {
        _window->draw(_ball->getSprite());
        _window->draw(_sceneObjects[2]->getSprite());
        _window->draw(_players[1]->getSprite());
      } else {
        _window->draw(_sceneObjects[2]->getSprite());
        _window->draw(_players[1]->getSprite());
        _window->draw(_ball->getSprite());
      }

      _window->draw(_sceneObjects[1]->getSprite());
      _window->draw(_score->getSprite());

      _window->display();
    }
  }


  void Volley::resolveCollisions() {
    auto angle = (_ball->getVelocity().x * 180.0f) / (32.0f * 3.14f);
    _ball->rotate(angle);

    if (_ball->isCollidingWith(*_players[0])) {
      _ball->bounce(*_players[0]);
      _lastPlayer = 0u;
      _sounds[1]->play();
    }

    if (_ball->isCollidingWith(*_players[1])) {
      _ball->bounce(*_players[1]);
      _lastPlayer = 1u;
      _sounds[1]->play();
    }

    const sf::Vector2f p = _sceneObjects[2]->getPosition();
    const sf::Vector2u s = _sceneObjects[2]->getSize();

    sf::FloatRect net_box(sf::Vector2f(p.x+s.x/2.0f - 10.0f, p.y+50.0f), sf::Vector2f(20.0f, 300.0f));

    if (net_box.contains(_ball->getPosition())) {
      _scores[1-_lastPlayer]++;
      _score->update(_scores[0], _scores[1]);
      _sounds[0]->play();
      reset();

    }
  }

  void Volley::reset() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000u));
    _ball->setPosition(sf::Vector2f(5*VL_WINDOW_WIDTH/8, 0));
    _players[0]->setPosition(sf::Vector2f(VL_WINDOW_WIDTH/4, 600));
    _players[1]->setPosition(sf::Vector2f(3*VL_WINDOW_WIDTH/4, 600));

    _ball->stop();
    _players[0]->stop();
    _players[1]->stop();
  }

  void Volley::update() {
    sf::Clock clock;
    while(_window->isOpen()){
      auto dt = clock.restart().asSeconds();

      resolveCollisions();

      _ball->update(dt);

      for (auto& player: _players)
        player->update(dt);


      for (int i=0; i<VL_NB_PLAYERS; i++) {
        _shadows[i]->setPosition(_players[i]->getPosition().x, 690);
        _shadows[i]->setScale(_players[i]->getPosition().y/700, 0.3*_players[i]->getPosition().y/700);
      }

      _shadows[2]->setPosition(_ball->getPosition().x, 690);
      _shadows[2]->setScale(_ball->getPosition().y/700, 0.3*_ball->getPosition().y/700);

      std::this_thread::sleep_for(std::chrono::milliseconds(VL_UPDATE_THREAD_MS));
    }
  }

  void Volley::handleEvents()
  {
    // Start the game loop
    while (_window->isOpen())
    {
      // Process events
      sf::Event event;
      while (_window->pollEvent(event))
      {
        if(event.type == sf::Event::KeyPressed) {
          switch(event.key.code) {
          case sf::Keyboard::Z: _players[0]->handleEvent(vl::Event::JUMP); break;
          case sf::Keyboard::I: _players[1]->handleEvent(vl::Event::JUMP); break;
          case sf::Keyboard::G: _ball->handleEvent(vl::Event::JUMP); break;
          case sf::Keyboard::V: _ball->handleEvent(vl::Event::LEFT); break;
          case sf::Keyboard::B: _ball->handleEvent(vl::Event::RIGHT); break;
          case sf::Keyboard::Escape: _window->close(); break;
          case sf::Keyboard::Space:
            _ball->setPosition(sf::Vector2f(5*VL_WINDOW_WIDTH/8, 0));
            _players[0]->setPosition(sf::Vector2f(VL_WINDOW_WIDTH/4, 0));
            _players[1]->setPosition(sf::Vector2f(3*VL_WINDOW_WIDTH/4, 0));
            break;

          default: break;
          }
        }

        if (event.type == sf::Event::Closed)
          _window->close();
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        _players[0]->handleEvent(vl::Event::LEFT);

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        _players[0]->handleEvent(vl::Event::RIGHT);

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
        _players[1]->handleEvent(vl::Event::LEFT);

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        _players[1]->handleEvent(vl::Event::RIGHT);

      std::this_thread::sleep_for(std::chrono::milliseconds(VL_EVENT_THREAD_MS));
    }
  }

  void Volley::onNotify(const vl::Event& event) {
    if (event == vl::Event::BALL_FELL) {
      if (_ball->getPosition().x < VL_WINDOW_WIDTH/2)
        _scores[1]++;
      else
        _scores[0]++;

      _score->update(_scores[0], _scores[1]);
      _sounds[0]->play();

      reset();
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
