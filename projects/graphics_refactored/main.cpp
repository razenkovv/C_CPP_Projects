#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "primitives2D.hpp"
#include "primitives3D.hpp"
#include "visual.hpp"

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "Window");

  win::Drawer drawer(window);

  geom2D::Polygon<int> p({{50, 100}, {100, 100}, {150, 200}, {100, 200}});
  geom2D::Triangle<double> t{{500.5, 100.43}, {700.45, 150}, {755, 380}};
  geom2D::Circle<double> c{{400.1, 400.25}, 55.4};

  std::vector<geom3D::Point<int>> points{geom3D::Point(300, 200, 100), geom3D::Point(400, 200, 100), geom3D::Point(400, 300, 100), geom3D::Point(300, 300, 100),
                                         geom3D::Point(300, 200, 200), geom3D::Point(400, 200, 200), geom3D::Point(400, 300, 200), geom3D::Point(300, 300, 200)};
  geom3D::Box<int> box{points};

  drawer.drawPolygon(p, sf::Color::Red);
  drawer.drawPolygon(t, sf::Color::Blue);
  drawer.drawCircle(c, sf::Color::Green);
  drawer.drawBezier3Curve<int>({{100, 500}, {780, 110}, {20, 100}, {650, 400}}, sf::Color::Magenta);
  drawer.drawBox(box, 500);
  auto box2 = box.create_rotated(geom3D::Point{3., 3.1, 2.5}, 1.6);
  drawer.drawBox(box2, 500, sf::Color::Blue);
  drawer.drawImage();
  drawer.saveToFile("result.png");

  while (drawer.isOpen()) {
    sf::Event event;
    while (drawer.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        drawer.close();
    }
  }

  return 0;
}