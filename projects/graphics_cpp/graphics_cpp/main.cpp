#include <SFML/Graphics.hpp>
#include <iostream>

#include "functions.h"

#define M_PI 3.14159265358979323846

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "Window");

  WinInstance w(window);
  // simple polygon
  // std::vector<Point<int>> vertex{Point(100, 100), Point(200, 50), Point(600, 200), Point(780, 500), Point(300, 570), Point(123, 300), Point(100, 100)};

  // nonconvex simple polygon
  //  std::vector<Point<int>> vertex{Point(100, 100), Point(400, 400), Point(600, 200), Point(780, 500), Point(300, 570), Point(123, 300), Point(100, 100)};

  // nonsimple polygon
  // std::vector<Point<int>> vertex1{Point(50, 50), Point(500, 50), Point(250, 400), Point(150, 250), Point(600, 500), Point(50, 550), Point(50, 50)};
  // std::vector<Point<int>> vertex{Point(100, 100), Point(200, 200), Point(100, 100)};

  // Bresenham algorithm
  // w.lineBresenham(Point(0, 0), Point(3, 8), sf::Color::Black);
  // w.lineBresenham(Point(3, 8), Point(0, 0), sf::Color::Red);
  // w.lineBresenham(Point(0, 0), Point(8, 3), sf::Color::Black);
  // w.lineBresenham(Point(8, 3), Point(0, 0), sf::Color::Red);
  // w.lineBresenham(Point(9, 0), Point(3, 8), sf::Color::Black);
  // w.lineBresenham(Point(3, 8), Point(9, 0), sf::Color::Red);
  //  w.lineBresenham(Point(678, 523), Point(100, 111), sf::Color::Red);
  //  w.lineBresenham(Point(345, 123), Point(742, 1), sf::Color::Green);

  // polygon draw
  // w.polygon(vertex1, sf::Color::Black);
  // w.polygon(vertex2, sf::Color::Red);

  // check convex polygon or not
  // std::vector<Point<int>> vertex{Point(319, 289), Point(306, 926), Point(795, 880), Point(1216, 786), Point(1163, 171), Point(319, 289)};
  // if (w.checkConvex(vertex))
  //   std::cout << "convex" << std::endl;
  // else
  //   std::cout << "nonconvex" << std::endl;
  // w.polygon(vertex, sf::Color::Black);

  // check lineSegmentsInterception method
  // Point<double> p(0.0, 0.0);
  // interceptionType type = w.lineSegmentsInterception(vertex[0], vertex[1], vertex[2], vertex[3], p);
  // switch (type) {
  //   case interceptionType::SAME:
  //     std::cout << "SAME\n";
  //     break;
  //   case interceptionType::PARALLEL:
  //     std::cout << "PARALLEL\n";
  //     break;
  //   case interceptionType::CROSS:
  //     std::cout << "CROSS\n";
  //     break;
  //   case interceptionType::NO_CROSS:
  //     std::cout << "NO_CROSS\n";
  //     break;
  // }
  // std::cout << p.x() << " " << p.y() << std::endl;

  // check simple polygon or not
  //  Point<double> p1(0.0, 0.0);
  //  if (w.checkPolygonSimplicity(vertex, p1))
  //    std::cout << "simple" << std::endl;
  //  else {
  //    std::cout << "nonsimple" << std::endl;
  //    std::cout << p1.x() << " " << p1.y() << std::endl;
  //  }

  // get bounding box
  // std::vector<Point<int>> boundbox(5);
  // w.boundingBox(vertex, boundbox);
  // w.polygon(boundbox, sf::Color::Red);

  // methods::EO or methods::NZW or methods::NONEXTERIOR
  // w.fillPolygon(vertex1, methods::NONEXTERIOR, sf::Color::Green);
  // w.polygon(vertex1, sf::Color::Black);

  // // Bezier curve
  // std::vector<Point<int>> points1{Point(100, 500), Point(200, 100), Point(525, 110), Point(650, 400)};
  // std::vector<Point<int>> points2{Point(100, 500), Point(780, 110), Point(20, 100), Point(650, 400)}; // with loop
  // std::vector<Point<int>> vertex{points1};
  // vertex.insert(vertex.end(), points1[0]);
  // w.polygon(vertex, sf::Color::Red);
  // w.curveBezier3(points1, sf::Color::Black);
  // w.curveBezier3(points2, sf::Color::Magenta);

  // //Cyrus-Beck clipping
  // std::vector<Point<int>> polygon{Point(100, 500), Point(200, 100), Point(525, 100), Point(650, 400), Point(100, 500)};
  // //std::vector<Point<int>> polygon{Point(100, 500), Point(650, 400), Point(525, 110), Point(200, 100), Point(100, 500)};
  // Point<int> p11(25, 500), p21(200, 300); // usual
  // Point<int> p12(50, 500), p22(700, 300); // usual
  // Point<int> p13(300, 400), p23(200, 325); // inside
  // Point<int> p14(600, 450), p24(700, 500);  // outside
  // Point<int> p15(650, 400), p25(700, 450); //parallel
  // //Point<int> p16(650, 400), p26(700, 450);
  // std::vector<Point<int>> points{p11, p21, p12, p22, p13, p23, p14, p24, p15, p25};// p16, p26};
  // w.polygon(polygon, sf::Color::Green);
  // for (int i = 0; i < points.size(); i+=2) {
  //   w.lineBresenham(points[i], points[i+1], sf::Color::Black);
  //   Point<int> p1_new, p2_new;
  //   if (w.clipLineCyrusBeck(polygon, points[i], points[i+1], p1_new, p2_new)) {
  //     w.lineBresenham(p1_new, p2_new, sf::Color::Red);
  //   } else {
  //     std::cout << "segment "<< i/2 << " is outside the polygon" << std::endl;
  //   }
  // }

  // Bspline3 curve
  // std::vector<Point<int>> points1{Point(250, 600), Point(450, 100), Point(900, 100), Point(1100,550), Point(1300, 400), Point(1500, 800)};
  // w.polygon(points1, sf::Color::Green);
  // w.curveBspline3(points1, sf::Color::Red);

  // 3D Parallelepiped
  auto points = std::vector<Point3D<double>>{Point3D(400., 300., 100.), Point3D(500., 300., 100.), Point3D(500., 400., 100.), Point3D(400., 400., 100.),
                                             Point3D(400., 300., 200.), Point3D(500., 300., 200.), Point3D(500., 400., 200.), Point3D(400., 400., 200.)};
  // Hex hex(points);
  // // w.parallelProjection(hex, sf::Color::Black, true);
  // w.rotate(hex, Point3D(3.,1.,1.), M_PI/6);
  // w.parallelProjection(hex, sf::Color::Red, true);
  // // w.perspectiveProjection(hex, 500., sf::Color::Blue, true);

  // w.saveImage("image.png", false);
  // w.drawImage();

  // while (w.isOpen()) {
  //   sf::Event event;
  //   while (w.pollEvent(event)) {
  //     if (event.type == sf::Event::Closed)
  //       w.close();
  //   }
  // }

  Hex hex(points);
  for (int i = 0; i < 200; ++i) {
    w.perspectiveProjection(hex, 500., sf ::Color::Red, true);
    w.rotate(hex, Point3D(3., 3.1, 2.5), M_PI / 100);
    w.saveImage("images/image" + std::to_string(i) + ".png", true);
    w.clear(sf::Color::White);
  }

  return 0;
}