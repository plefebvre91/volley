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


#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "utils.hpp"
#include "constants.hpp"

namespace vl {

enum class PlayerState {
  IDLE,
  JUMPING,
  GOING_RIGHT,
  GOING_LEFT,
};

enum class Event {
  JUMP,
  RESET,
  RIGHT,
  LEFT,
};


class Player {
  public:
    Player() = default;
    Player(const char* image_file, float a = VL_DEFAULT_ABSORPTION);
    ~Player();
    void setPlayableArea(const sf::FloatRect&);

    void move(const sf::Vector2f& v);
    void jump();
    void update(double dt);
    void handleEvent(Event);
    sf::Sprite* getSprite() const;
    const sf::Vector2f& getPosition();
    const sf::Vector2f& getVelocity();
    const sf::Vector2f& getAcceleration();
    void resetPosition();
    void setPosition(const sf::Vector2f& p);
    void bounceIfCollide(const Player& anotherPlayer);
    void rotate();

    void setPhysicsAttributes(const sf::Vector2f&, const sf::Vector2f&, const sf::Vector2f&);

  private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    float rotation;
    float absorption;
    sf::Texture* texture;
    sf::Sprite* sprite;
    PlayerState state;
    sf::FloatRect area;
  };
}
#endif
