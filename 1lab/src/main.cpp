#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <array>
#include <cmath>
#include <random>
#include <vector>

constexpr int N_cuts = 1000;

class upper_curve {
public:
  std::array<double, N_cuts + 1> x_points;
  std::array<double, N_cuts + 1> y_points;
  std::array<double, N_cuts> lengths;
  double length;

  double function(double x) const { return 2.0 * sin(2.0 - x); }
  double inverse_function(double y) const { return 2 - std::asin(y / 2.0); }
  void cut_from_to(double a, double b) {
    double dx = (b - a) / N_cuts;
    for (int i = 0; i <= N_cuts; i++) {
      x_points[i] = a + i * dx;
      y_points[i] = function(x_points[i]);
    }
    double sum = 0.0;
    for (int i = 0; i < N_cuts; i++) {
      double dl = std::sqrt(pow(dx, 2) + pow(y_points[i + 1] - y_points[i], 2));
      lengths[i] = dl;
      sum += dl;
    }
    length = sum;
  }
};

class down_curve {
public:
  std::array<double, N_cuts + 1> x_points;
  std::array<double, N_cuts + 1> y_points;
  std::array<double, N_cuts> lengths;
  double length;
  double function(double x) const { return 2.0 - x; }
  double inverse_function(double y) const { return 2.0 - y; }
  void cut_from_to(double a, double b) {
    double dx = (b - a) / N_cuts;
    for (int i = 0; i <= N_cuts; i++) {
      x_points[i] = a + i * dx;
      y_points[i] = function(x_points[i]);
    }
    double sum = 0.0;
    for (int i = 0; i < N_cuts; i++) {
      double dl = std::sqrt(pow(dx, 2) + pow(y_points[i + 1] - y_points[i], 2));
      lengths[i] = dl;
      sum += dl;
    }
    length = sum;
  }
};

double count_left_bound(const upper_curve &ub, const down_curve &db) {
  double x = 0.0;
  double dx = 1e-7;
  while (ub.function(x) - db.function(x) < 0) {
    x += dx;
  }
  return x;
}

double count_right_bound(const upper_curve &ub, const down_curve &db) {
  double x = 2.0;
  double dx = 1e-7;
  while (ub.function(x) - db.function(x) < 0) {
    x -= dx;
  }
  return x;
}

std::vector<sf::CircleShape>
generate_n_curve_dots(std::random_device &gen,
                      std::uniform_real_distribution<double> &line_dist,
                      down_curve &db, upper_curve &ub, int n) {
  std::vector<sf::CircleShape> dots;
  for (int i = 0; i < n; i++) {
    double dot_distance = line_dist(gen);
    if (dot_distance < db.length) {
      int cut_number = 0;
      double sum = 0.0;
      while (sum < dot_distance) {
        sum += db.lengths[cut_number++];
      }
      cut_number--;
      sum -= db.lengths[cut_number];
      double dl = dot_distance - sum;
      double x = db.x_points[cut_number] +
                 (db.x_points[cut_number + 1] - db.x_points[cut_number]) * dl;
      double y = db.y_points[cut_number] +
                 (db.y_points[cut_number + 1] - db.y_points[cut_number]) * dl;
      sf::CircleShape point(1.f);
      point.setFillColor(sf::Color::White);
      point.setPosition({static_cast<float>(x) * 500 + 100.f,
                         1000.f - static_cast<float>(y) * 500 + 100.f});
      dots.push_back(point);
    } else {
      dot_distance -= db.length;
      int cut_number = 0;
      double sum = 0.0;
      while (sum < dot_distance) {
        sum += ub.lengths[cut_number++];
      }
      cut_number--;
      sum -= ub.lengths[cut_number];
      double dl = dot_distance - sum;
      double x = ub.x_points[cut_number] +
                 (ub.x_points[cut_number + 1] - ub.x_points[cut_number]) * dl;
      double y = ub.y_points[cut_number] +
                 (ub.y_points[cut_number + 1] - ub.y_points[cut_number]) * dl;
      sf::CircleShape point(1.f);
      point.setFillColor(sf::Color::White);
      point.setPosition({static_cast<float>(x) * 500 + 100.f,
                         1000.f - static_cast<float>(y) * 500 + 100.f});
      dots.push_back(point);
    }
  }
  return dots;
}

inline void
draw_n_dots_by_rejection(sf::RenderWindow &window, std::random_device &gen,
                         std::uniform_real_distribution<double> &rejection_dist,
                         down_curve &db, upper_curve &ub,
                         sf::CircleShape &point, int n) {
  for (int i = 0; i < n; i++) {
    double x = rejection_dist(gen);
    double y = rejection_dist(gen);
    if (x > db.inverse_function(y) && x < ub.inverse_function(y) &&
        y > db.function(x) && y < ub.function(x))
      point.setFillColor(sf::Color::Green);
    else
      point.setFillColor(sf::Color::Red);
    point.setPosition({static_cast<float>(x) * 500 + 100.f,
                       1000.f - static_cast<float>(y) * 500 + 100.f});
    window.draw(point);
  }
}

int main() {
  upper_curve ub;
  down_curve db;

  double a = count_left_bound(ub, db);
  double b = count_right_bound(ub, db);

  db.cut_from_to(a, b);
  ub.cut_from_to(a, b);

  std::random_device gen;
  std::uniform_real_distribution<double> rejection_dist(0, 2);
  std::uniform_real_distribution<double> line_dist(0, db.length + ub.length);

  sf::RenderWindow window(sf::VideoMode({1200, 1200}), "rejection method");
  window.setFramerateLimit(60);

  sf::CircleShape point(1.f);
  bool draw_dots = false;

  std::vector<sf::CircleShape> dots;

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
        draw_dots = !draw_dots;
        window.clear();
        window.display();
        dots.clear();
      }
    }
    if (draw_dots) {
      draw_n_dots_by_rejection(window, gen, rejection_dist, db, ub, point,
                               3000);
      if (dots.size() < 10000) {
        auto tmp = generate_n_curve_dots(gen, line_dist, db, ub, 10);
        dots.insert(dots.end(), tmp.begin(), tmp.end());
      }
      for (const auto &dot : dots) {
        window.draw(dot);
      }
      window.display();
      window.clear();
    }
  }
}
