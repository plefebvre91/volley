#include <SFML/Graphics.hpp>
#include <cmath>

namespace vl {
  namespace utils {
    float sd(const sf::Vector2f& a, const sf::Vector2f& b) {
      auto x = a.x - b.x;
      auto y = a.y - b.y;
      return x*x + y*y;
    }

    // Build vecotr from 2 points
    sf::Vector2f v(const sf::Vector2f& a, const sf::Vector2f& b) {
      return b - a;
    }

    // Build a normalize vector
    sf::Vector2f nv(const sf::Vector2f& a, const sf::Vector2f& b) {
      auto u = v(b,a);
      auto n = std::hypotf(u.x, u.y);

      return u/n;
    }
  }
}
