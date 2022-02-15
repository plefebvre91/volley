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

#include <SFML/Graphics.hpp>
#include "ball.hpp"
#include "utils.hpp"
#include "constants.hpp"

namespace vl {
  Ball::Ball(const char* file, const sf::Vector2f& position,  float friction):
    Entity(file, position) {
    _sprite.setOrigin(sf::Vector2f(_texture.getSize() / 2u));
    _friction = friction;
  }

  bool Ball::isCollidingWith(const IPhysicalObject& object) const {
    return vl::utils::sd(object.getPosition()-sf::Vector2f(0.0f, 65.0f), _position) < VL_DIST_BEFORE_COLLISION;
  }

  void Ball::bounce(const IPhysicalObject& object) {
    const sf::Vector2f& v = vl::utils::nv(_position, (object.getPosition()-sf::Vector2f(0.0 ,65.0f))) ;
    _acceleration.x = std::min<float>(10.0f*v.x, 2.0f);
    _acceleration.y = std::min<float>(10.0f*v.y, 2.0f);
  }

  void Ball::setObserver(IObserver* observer) {
    _observer = observer;
  }

  void Ball::notify(Event event) {
    _observer->onNotify(event);
  }

  void Ball::handleEvent(vl::Event e) {
    switch (e) {
      case Event::RESET:
        _state = vl::State::IDLE;
        reset();
      break;

      case Event::JUMP:
        if (_state != vl::State::JUMPING) {
          _state = vl::State::JUMPING;
          jump(VL_JUMP_STEP);
        }
      break;

      case Event::RIGHT:
        move(sf::Vector2f(VL_MOVE_STEP, 0.0f));
      break;

      case Event::LEFT:
        move(sf::Vector2f(-VL_MOVE_STEP, 0.0f));
      break;

      default:  break;
    }
  }

  void Ball::update(float dt) {
    Entity::update(dt);

    if (_position.x < VL_MARGIN) {
      _position.x = VL_MARGIN;
      _velocity.x *= -VL_BOUND_RESTITUTION/2.0f;
    }

    else if (_position.x > (VL_WINDOW_WIDTH - VL_MARGIN)) {
      _position.x  = (VL_WINDOW_WIDTH - VL_MARGIN);
      _velocity.x *= -VL_BOUND_RESTITUTION/2.0f;
    }

    if (_position.y < 0) {
      _velocity.y *= -VL_BOUND_RESTITUTION/2.0f;
      _position.y = 1.0f;
    }

    auto size = getSize();
    if(_position.y >  VL_FLOOR - (size.y/2.0f) - 5.0f) {
      notify(vl::Event::BALL_FELL);
    }
  }
}
