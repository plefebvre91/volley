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

#include "player.hpp"
#include "constants.hpp"

namespace vl {
  Player::Player(const char* file, float a)
      :position(), velocity(),acceleration(),rotation(0) , absorption(a), state(PlayerState::IDLE), area(){
    texture = new sf::Texture();
    texture->loadFromFile(file);

    sprite = new sf::Sprite(*texture);
    const sf::Vector2u& s = sprite->getTexture()->getSize();
    sprite->setOrigin(s.x/2, s.y/2);
  }

  Player::~Player() {
    delete sprite;
    delete texture;
  }

  void Player::setPlayableArea(const sf::FloatRect& a) {
      area = a;
  }

  void Player::resetPosition() {
    position.x = area.left + area.width/2;
    position.y = area.top + 3*area.height/4;
  }

  void Player::rotate() {
    auto rotation = (velocity.x * 180) / (32 * 3.14);
    sprite->rotate(rotation);
  }

  void Player::bounceIfCollide(const Player& anotherPlayer) {
    if(vl::utils::sd(anotherPlayer.position-sf::Vector2f(0,65), position) < VL_DIST_BEFORE_COLLISION) {
      const sf::Vector2f& v = vl::utils::nv(position, (anotherPlayer.position-sf::Vector2f(0,65))) ;
      acceleration.x = 2*v.x;
      acceleration.y = 2*v.y;
    }
  }

  sf::Sprite* Player::getSprite() const {
    return sprite;
  }

  const sf::Vector2f& Player::getPosition() {
    return position;
  }

  void Player::setPosition(const sf::Vector2f& p) {
    position = p;
  }

  const sf::Vector2f& Player::getVelocity() {
    return velocity;
  }

  const sf::Vector2f& Player::getAcceleration() {
    return acceleration;
  }

  void Player::setPhysicsAttributes(const sf::Vector2f& p, const sf::Vector2f& v, const sf::Vector2f& a) {
    position = p;
    velocity = v;
    acceleration = a;
  }

  void Player::move(const sf::Vector2f& v) {
    if (area.contains(position+v)) {
      velocity.x = v.x;
    }
  }

  void Player::jump() {
    acceleration.y = -VL_JUMP_STEP;
  }

  void Player::handleEvent(Event e) {
    switch (e) {
      case Event::RESET:
        state = PlayerState::IDLE;
        resetPosition();
      break;

      case Event::JUMP:
        if (state != PlayerState::JUMPING) {
          state = PlayerState::JUMPING;
          jump();
        }
      break;

      case Event::RIGHT:
        if (state != PlayerState::JUMPING) {
          state = PlayerState::GOING_RIGHT;
          move(sf::Vector2f(VL_MOVE_STEP, 0.0));
        }
      break;

      case Event::LEFT:
        if (state != PlayerState::JUMPING) {
          state = PlayerState::GOING_LEFT;
          move(sf::Vector2f(-VL_MOVE_STEP, 0.0));
        }
      break;

      default:  break;
    }
  }

  void Player::update(double dt) {
    auto size = sprite->getTexture()->getSize();

    velocity.x += acceleration.x;
    velocity.y += acceleration.y;

    position.x += velocity.x;
    position.y += velocity.y;

    if(std::abs(velocity.x) != 0) velocity.x *= absorption;
    if(std::abs(velocity.x) < 0.01) velocity.x = 0;

    acceleration.y = 0;
    acceleration.x = 0;

    // Is in the air
    if (position.y < VL_FLOOR - (size.y/2)) {
      velocity.y += VL_GRAVITY*dt;
    }

    // Rebound
    else if(position.y >  VL_FLOOR - (size.y/2)) {
      if (velocity.y > 0.0)
        velocity.y *= -VL_BOUND_RESTITUTION;
      else {
        position.y = VL_FLOOR - (size.y/2);
        state = PlayerState::IDLE;
      }
    }

    sprite->setPosition(position);
  }
}
