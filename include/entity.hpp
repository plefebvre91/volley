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


#ifndef VL_ENTITY_HPP
#define VL_ENTITY_HPP

#include "interfaces.hpp"
#include "states.hpp"
#include "constants.hpp"

namespace vl {
  class Entity: public IPhysicalObject, public IDynamicObject {
  public:
    Entity() = default;
    Entity(const char* file, const sf::Vector2f& position);
    ~Entity() = default;

    void update(double dt);

    inline const sf::Vector2f& getPosition() const {
      return _position;
    }

    inline void setPosition(const sf::Vector2f& position) {
      _position = position;
    }

    inline const sf::Sprite& getSprite() const {
      return _sprite;
    }

    inline const sf::Vector2f& getVelocity() const {
      return _velocity;
    }

    inline const sf::Vector2f& getAcceleration() const {
      return _acceleration;
    }

    inline void move(const sf::Vector2f& v) {
      _velocity.x = v.x;
    }

    inline void jump(float force) {
      _acceleration += sf::Vector2f(0.0, -force);
    }

    inline void reset() {
      _position = sf::Vector2f(0, 0);
    }

    inline void rotate(float angle) {
      _sprite.rotate(angle);
    }

    inline const sf::Vector2u getSize() const {
      return _texture.getSize();
    }


  protected:
    sf::Vector2f _position;
    sf::Vector2f _velocity;
    sf::Vector2f _acceleration;
    sf::Sprite _sprite;
    sf::Texture _texture;
    vl::State _state;
    float _friction;
  };
};

#endif
