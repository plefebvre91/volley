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


#ifndef VOLLEY_HPP
#define VOLLEY_HPP

#include <SFML/Graphics.hpp>
#include "constants.hpp"
#include "entity.hpp"
#include "character.hpp"
#include "ball.hpp"
#include "observer.hpp"
#include "score.hpp"

namespace vl {
  class Volley: public IObserver
  {
  public:
    /**
     * Constructor
     */
    Volley();

    /**
     * Constructor
     */
    ~Volley();

    /**
     * Launch application
     */
    void run();

    /**
     * Render sprites
     */
    void render();

    /**
     * Update state
     */
    void update();

    /**
     * Event notification handler
     */
    void onNotify(const Entity& entity, vl::Event event);


  private:
    void handleEvents();
    void resolveCollisions();
    void resolveGravity(double dt);
    void reset();
    std::array<vl::Character*, VL_NB_PLAYERS> players;
    std::array<sf::CircleShape*, 3> shadows;
    std::array<vl::Entity*, 3> _sceneObjects;
    vl::Ball* ball;
    sf::RenderWindow* window;
    unsigned int _lastPlayer;
    unsigned int _scores[2];
    Score* _score;

  };
}

#endif
