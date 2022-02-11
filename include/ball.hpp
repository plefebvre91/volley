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

#ifndef VL_BALL_HPP
#define VL_BALL_HPP

#include "interfaces.hpp"
#include "entity.hpp"
#include "observer.hpp"

namespace vl {
  class Ball: public Entity,  public ISolidObject, public ISoftObject, public IControllableObject {
  public:
    Ball(const char* file, const sf::Vector2f& position, float friction);
    void setObserver(IObserver* observer);
    void notify(const Entity& entity, Event event);
    bool isCollidingWith(const IPhysicalObject& object) const;
    void bounce(const IPhysicalObject& object);
    void handleEvent(vl::Event e);
    void update(float dt);

  private:
    IObserver* _observer;
  };
};

#endif
