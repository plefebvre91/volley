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
  struct IPhysicalObject {
    IPhysicalObject() = default;
    virtual ~IPhysicalObject() = default;
    virtual const sf::Vector2f& getPosition() const = 0;
    virtual void setPosition(const sf::Vector2f& p) = 0;
  };

  struct ISolidObject {
    ISolidObject() = default;
    virtual ~ISolidObject() = default;
    virtual bool isCollidingWith(const IPhysicalObject& object) const = 0;
  };

  struct IDynamicObject {
    IDynamicObject() = default;
    virtual ~IDynamicObject() = default;
    virtual const sf::Vector2f& getVelocity() const = 0;
    virtual const sf::Vector2f& getAcceleration() const = 0;

    virtual void move(const sf::Vector2f& v) = 0;
    virtual void jump(float force) = 0;
    virtual void reset() = 0;
    virtual void rotate(float angle) = 0;

    virtual void update(double dt) = 0;
  };

  struct IControllableObject {
    IControllableObject() = default;
    virtual void handleEvent(vl::Event) = 0;
  };

  struct ISoftObject {
    ISoftObject() = default;
    virtual ~ISoftObject() = default;
    virtual void bounce(const IPhysicalObject& object) = 0;
  };
}

#endif
