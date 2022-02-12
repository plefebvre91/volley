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

#include "character.hpp"
#include "constants.hpp"

namespace vl {
  Character::Character(const char* file, const sf::Vector2f& position, float friction)
    :Entity(file, position) {
      _sprite.setOrigin(sf::Vector2f(_texture.getSize() / 2u));
      _friction = friction;
    }

  void Character::handleEvent(vl::Event e) {
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
        _state = vl::State::GOING_RIGHT;
        move(sf::Vector2f(VL_MOVE_STEP, 0.0f));
      break;

      case Event::LEFT:
        _state = vl::State::GOING_LEFT;
        move(sf::Vector2f(-VL_MOVE_STEP, 0.0f));
      break;

      default:  break;
    }
  }
}
