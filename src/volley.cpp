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

#include "volley.hpp"
#include "constants.hpp"
#include <X11/Xlib.h>
#include <thread>


static void _render(volley* app) {
  app->render();
}
static void _update(volley* app) {
  app->update();
}

volley::volley():p("ball.png")
{
  XInitThreads();
  window = new
    sf::RenderWindow(sf::VideoMode(VL_WINDOW_SIZE, VL_WINDOW_SIZE), VL_APP_TITLE);
  window->setActive(false);
  window->setFramerateLimit(VL_FPS);
}

void volley::render() {
  sf::Clock clock;

  while(window->isOpen()){
    window->clear();
    window->draw(*p.get_sprite());
    window->display();
  }
}

void volley::update() {
  sf::Clock clock;
  while(window->isOpen()){
    double dt = clock.restart().asSeconds();
  	p.update(dt);
    std::this_thread::sleep_for(std::chrono::milliseconds(VL_UPDATE_THREAD_MS));
  }
}

void volley::handle_events()
{
  // Start the game loop
  while (window->isOpen())
  {
    // Process events
    sf::Event event;
    while (window->pollEvent(event))
    {
      if(event.type == sf::Event::KeyPressed) {
        switch(event.key.code) {
        case sf::Keyboard::Space: p.jump(); break;
        case sf::Keyboard::Escape: window->close(); break;
        default: break;
        }
      }

      if (event.type == sf::Event::Closed)
        window->close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
      p.move(sf::Vector2f(-5.0, 0));

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
      p.move(sf::Vector2f(5.0, 0));

    std::this_thread::sleep_for(std::chrono::milliseconds(VL_EVENT_THREAD_MS));
  }
}

void volley::run()
{
  std::thread render_thread(&_render, this);
  std::thread update_thread(&_update, this);
  handle_events();
  update_thread.join();
  render_thread.join();
}

volley::~volley()
{
  delete window;
}
