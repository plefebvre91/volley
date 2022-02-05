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


#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

namespace vl {

enum class player_state {
  VL_JUMPING,
};


class player {
  public:
    player() = default;
    player(const char* image_file);
    ~player();

    void move(const sf::Vector2f& v);
    void jump();
    void update(double dt);
    sf::Sprite* get_sprite() const;
    const sf::Vector2f& get_position();
    const sf::Vector2f& get_velocity();
    const sf::Vector2f& get_acceleration();

    void set_position(const sf::Vector2f&);

    void set_physics_attributes(const sf::Vector2f&, const sf::Vector2f&, const sf::Vector2f&);

  private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    double rotation;

    sf::Texture* texture;
    sf::Sprite* sprite;
  };
}
#endif
