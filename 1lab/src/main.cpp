#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <cmath>
#include <random>

int main() {
  std::random_device gen;
  std::uniform_real_distribution<float> dist(0, 2);

  sf::RenderWindow window(sf::VideoMode({1200, 1200}), "rejection method");
  window.setFramerateLimit(60);

  sf::CircleShape point(1.f);
  bool draw_dots = false;

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
        draw_dots = !draw_dots;
        window.clear();
      }
    }
    for (int i = 0; i < 1000; i++) {
      float x = dist(gen);
      float y = dist(gen);
      if (x >= (2 - y) && x <= (2 - asin(y / 2)) && y >= (2 - x) &&
          y <= (2 * std::sin(2 - x)))
        point.setFillColor(sf::Color::Green);
      else
        point.setFillColor(sf::Color::Red);
      point.setPosition({x * 500 + 100.f, 1000.f - y * 500 + 100.f});
      if (draw_dots)
        window.draw(point);
    }
    window.display();
  }
}
