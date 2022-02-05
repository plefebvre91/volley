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

#include "player.hpp"
#include "constants.hpp"

using namespace vl;

player::player(const char* image_file, float a)
    :position(sf::Vector2f(200,200)),velocity(),acceleration(),rotation(0) , absorption(a){
  texture = new sf::Texture();
  texture->loadFromFile(image_file);


  sprite = new sf::Sprite(*texture);
  const sf::Vector2u& s = sprite->getTexture()->getSize();
  sprite->setOrigin(s.x/2, s.y/2);
}

player::~player() {
  delete sprite;
  delete texture;
}

sf::Sprite* player::get_sprite() const {
  return sprite;
}

const sf::Vector2f& player::get_position() {
  return position;
}

const sf::Vector2f& player::get_velocity() {
  return velocity;
}

const sf::Vector2f& player::get_acceleration() {
  return acceleration;
}

void player::set_physics_attributes(const sf::Vector2f& p, const sf::Vector2f& v, const sf::Vector2f& a) {
  position = p;
  velocity = v;
  acceleration = a;
}

void player::move(const sf::Vector2f& v) {
  velocity.x = v.x;
}

void player::jump() {
  acceleration.y = -VL_JUMP_STEP;
}

void player::update(double dt) {
  velocity.x += acceleration.x;
  velocity.y += acceleration.y;

  position.x += velocity.x;
  position.y += velocity.y;

  if(std::abs(velocity.x) != 0) velocity.x *= absorption;
  if(std::abs(velocity.x) < 0.01) velocity.x = 0;

  acceleration.y = 0;
  acceleration.x = 0;

  sprite->setPosition(position);
}
