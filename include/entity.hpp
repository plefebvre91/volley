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


#ifndef VL_ENTITY_HPP
#define VL_ENTITY_HPP

#include "interfaces.hpp"
#include "states.hpp"

namespace vl {
  class Entity: public IPhysicalObject, public IDynamicObject {
  public:
    /**
     * Constructor
     * @param image file
     * @param intial position
     */
    Entity(const char* file, const sf::Vector2f& position);

    /**
     * Upadte object at each frame
     * @param time since last update
     */
    void update(double dt);

    /**
     * IPhysicalObject interface implementation
     * @return object position
     */
    inline const sf::Vector2f& getPosition() const {
      return _position;
    }

    /**
     * IPhysicalObject interface implementation
     * @param new position
     */
    inline void setPosition(const sf::Vector2f& position) {
      _position = position;
    }

    /**
     * Sprite getter
     * @param object's sprite
     */
    inline const sf::Sprite& getSprite() const {
      return _sprite;
    }

    /**
     * IDynamicObject interface implementation
     * @return object velocity
     */
    inline const sf::Vector2f& getVelocity() const {
      return _velocity;
    }

    /**
     * IDynamicObject interface implementation
     * @return object acceleration
     */
    inline const sf::Vector2f& getAcceleration() const {
      return _acceleration;
    }

    /**
     * IDynamicObject interface implementation
     * @param velocity
     */
    inline void move(const sf::Vector2f& v) {
      if (_area.contains(_position+v))
        _velocity.x = v.x;
    }

    /**
     * IDynamicObject interface implementation
     */
    inline void stop() {
      _velocity = sf::Vector2f(0, 0);
    }

    /**
     * IDynamicObject interface implementation
     * @param vertical force
     */
    inline void jump(float force) {
      _acceleration += sf::Vector2f(0.0, -force);
    }

    /**
     * Reset object position
     */
    inline void reset() {
      _position = sf::Vector2f(0, 0);
    }

    /**
     * IDynamicObject interface implementation
     * @param rotation angle
     */
    inline void rotate(float angle) {
      _sprite.rotate(angle);
    }

    /**
     * Image size getter
     * @return sprite size
     */
    inline const sf::Vector2u getSize() const {
      return _texture.getSize();
    }

    /**
     * Define external bounds for the object
     * @param bounds coords
     */
    void setPlayableArea(const sf::FloatRect& area) {
        _area = area;
    }

  protected:
    sf::Vector2f _position;
    sf::Vector2f _velocity;
    sf::Vector2f _acceleration;
    sf::Sprite _sprite;
    sf::Texture _texture;
    sf::FloatRect _area;
    vl::State _state;
    float _friction;
  };
};

#endif
