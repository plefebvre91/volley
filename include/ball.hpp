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
    /**
     * Constructor
     * @param image file
     * @param intial position
     */
    Ball(const char* file, const sf::Vector2f& position, float friction);

    /**
     * Add an observer
     * @param a observer object
     */
    void setObserver(IObserver* observer);

    /**
     * Notify an event to the observer
     * @param an object
     * @param an event
     */
    void notify(const Entity& entity, Event event);

    /**
     * ISolidObject interface implementation
     * @param another object
     * @return true is the 2 objects are collinding
     */
    bool isCollidingWith(const IPhysicalObject& object) const;

    /**
     * ISoftObject interface implementation
     * @param another object
     */
    void bounce(const IPhysicalObject& object);

    /**
     * Event handler
     * @param an event
     */
    void handleEvent(vl::Event e);

    /**
     * Upadte object at each frame
     * @param time since last update
     */
    void update(float dt);

  private:
    IObserver* _observer;
  };
};

#endif
