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

#include "entity.hpp"
#include "constants.hpp"

namespace vl {
  Entity::Entity(const char* file, const sf::Vector2f& position):
    _position(position), _velocity(), _acceleration(), _sprite(), _texture(), _friction(VL_DEFAULT_FRICTION) {
      _texture.loadFromFile(file);
      _sprite.setTexture(_texture);
      _sprite.setPosition(position);
  }

  void Entity::update(double dt) {
    auto size = _texture.getSize();

    _velocity.x += _acceleration.x;
    _velocity.y += _acceleration.y;

    _position.x += _velocity.x;
    _position.y += _velocity.y;

    // // A DEPLACER DANS BOUNCE ?
    if(std::abs(_velocity.x) != 0.0f) _velocity.x *= _friction;
     if(std::abs(_velocity.x) < 0.01f) _velocity.x = 0.0f;

    _acceleration.y = 0.0f;
    _acceleration.x = 0.0f;

    // Is in the air
    if (_position.y < VL_FLOOR - (size.y/2)) {
      _velocity.y += VL_GRAVITY*dt;
    }

    // Rebound
    else if(_position.y >  VL_FLOOR - (size.y/2)) {
      if (_velocity.y > 0.0f)
        _velocity.y *= -VL_BOUND_RESTITUTION;
      else {
        _position.y = VL_FLOOR - (size.y/2);
        _state = vl::State::IDLE;
      }
    }
    _sprite.setPosition(_position);
  }

}
