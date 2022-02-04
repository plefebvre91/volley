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

player::player(const char* image_file)
    :position(sf::Vector2f(200,200)),velocity(),acceleration(),rotation(0) {
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

void player::move(const sf::Vector2f& v) {
  velocity.x = v.x;
}

void player::jump() {
  acceleration.y = -5;
}

void player::update(double dt) {
  if(position.y < 500)                  //If you are above ground
      velocity.y += VL_GRAVITY*dt;    //Add gravity
    else if(position.y > 500) { //If you are below ground
      if (velocity.y > 0.0)
        velocity.y *= -0.6;
      else
        position.y = 500;                 //That's not supposed to happen, put him back up
    }

  velocity.x += acceleration.x;
  velocity.y += acceleration.y;

  rotation = (velocity.x * 180) / (32 * 3.14);

  position.x += velocity.x;
  position.y += velocity.y;

  if(std::abs(velocity.x) != 0) velocity.x *= 0.95;
  if(std::abs(velocity.x) < 0.01) velocity.x = 0;
  if(std::abs(rotation) != 0) rotation *= 0.95;
  if(std::abs(rotation) < 0.01) rotation = 0;

  acceleration.y = 0;
  acceleration.x = 0;

  sprite->setPosition(position);
  sprite->rotate(rotation);

}
