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


#ifndef VL_INTERFACES_HPP
#define VL_INTERFACES_HPP

#include <SFML/Graphics.hpp>
#include "events.hpp"

namespace vl {
  /**
   * Base interface for a simple object with a position
   */
  struct IPhysicalObject {
    /**
     * Getter for position
     * @return object's position
     */
    virtual const sf::Vector2f& getPosition() const = 0;

    /**
     * Setter for position
     * @param new position
     */
    virtual void setPosition(const sf::Vector2f& p) = 0;
  };

  /**
   * Base interface for a collidable object
   */
  struct ISolidObject {
    /**
     * Setter for position
     * @param another object
     * @return true if the 2 objects are colliding
     */
    virtual bool isCollidingWith(const IPhysicalObject& object) const = 0;
  };

  /**
   * Base interface for a dynamic object
   */
  struct IDynamicObject {
    /**
     * Getter for velocity
     * @return object's velocity
     */
    virtual const sf::Vector2f& getVelocity() const = 0;

    /**
     * Getter for acceleration
     * @return object's acceleration
     */
    virtual const sf::Vector2f& getAcceleration() const = 0;

    /**
     * Move the object
     * @param velocity
     */
    virtual void move(const sf::Vector2f& v) = 0;

    /**
     * Make a jump
     * @param vertical force
     */
    virtual void jump(float force) = 0;

    /**
     * Reset object
     */
    virtual void reset() = 0;

    /**
     * Rotate object
     * @param rotation angle
     */
    virtual void rotate(float angle) = 0;

    /**
     * Make the object immobile
     */
    virtual void stop() = 0;

    /**
     * Update object attributes
     * @param time since last update
     */
    virtual void update(double dt) = 0;
  };

  /**
   * Base interface for a controllable object
   */
  struct IControllableObject {
    /**
     * Events handler
     */
    virtual void handleEvent(vl::Event) = 0;
  };

  /**
   * Base interface for a collidable object
   */
  struct ISoftObject {
    /**
     * Make a bounce
     * @param other object which impulsed the bounce
     */
    virtual void bounce(const IPhysicalObject& object) = 0;
  };
}

#endif
