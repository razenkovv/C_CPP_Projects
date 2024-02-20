#pragma once

#include <Eigen/Dense>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

template <typename T>
class Point;

template <typename T>
class Face;

class Hex;

template <typename T>
class Point3D;

enum class pointType { LEFT, RIGHT, BEHIND, BETWEEN, ORIGIN, DESTINATION };
enum class interceptionType { SAME, PARALLEL, CROSS, NO_CROSS };
enum class pointTypeToPolygonEdge { TOUCHING, CROSS_LEFT, CROSS_RIGHT, INESSENTIAL };
enum class pointToPolygonType { INSIDE, OUTSIDE };
enum class methods { EO, NZW, NONEXTERIOR };
enum class clockWiseType { CW, CCW, NONCONVEX };

class WinInstance {
 private:
  sf::RenderWindow& window;
  sf::Texture texture;
  sf::Sprite sprite;
  sf::Image image;

  template <typename T>
  interceptionType linesInterception(const Point<T>& a1, const Point<T>& a2, const Point<T>& b1, const Point<T>& b2, double& t);

  pointToPolygonType fillPointEvenOdd(const Point<int>& p, const std::vector<Point<int>>& polygon);

  pointToPolygonType fillPointNonZeroWinding(const Point<int>& p, const std::vector<Point<int>>& polygon);

 public:
  WinInstance(sf::RenderWindow&);
  void display();
  bool isOpen() const;
  bool pollEvent(sf::Event& event);
  void close();
  void drawImage(bool update = true);
  void clear(sf::Color color);

  void lineBresenham(const Point<int>& s, const Point<int>& e, const sf::Color& color);

  void polygon(const std::vector<Point<int>>& vertex, const sf::Color& color);

  template <typename T>
  pointType pointPositionToLineSegment(const Point<T>& p, const Point<T>& start, const Point<T>& end);

  bool checkConvex(const std::vector<Point<int>>& vertex);

  template <typename T>
  interceptionType linesInterceptionCoords(const Point<T>& a1, const Point<T>& a2, const Point<T>& b1, const Point<T>& b2, Point<double>& crossCoords);

  template <typename T>
  interceptionType lineSegmentsInterception(const Point<T>& a1, const Point<T>& a2, const Point<T>& b1, const Point<T>& b2, Point<double>& crossCoords);

  bool checkPolygonSimplicity(const std::vector<Point<int>>& vertex, Point<double>& p);

  void boundingBox(const std::vector<Point<int>>& polygon, std::vector<Point<int>>& res);

  pointTypeToPolygonEdge pttpe(const Point<int>& p, const Point<int>& p1, const Point<int>& p2);

  void fillPolygon(const std::vector<Point<int>>& polygon, const methods& method, const sf::Color& color);

  bool saveImage(const std::string& filename, bool update);

  void curveBezier3(const std::vector<Point<int>>& points, const sf::Color& color);

  void curveBspline3(std::vector<Point<int>>& points, const sf::Color& color);

  clockWiseType checkClockWise(const std::vector<Point<int>>& vertex);

  bool clipLineCyrusBeck(const std::vector<Point<int>>& polygon, const Point<int>& p1, const Point<int>& p2, Point<int>& p1_new, Point<int>& p2_new);

  void parallelProjection(Hex& hex, const sf::Color& color, bool onlyVisible);

  void perspectiveProjection(Hex& hex, double k, const sf::Color& color, bool onlyVisible);

  void rotate(Hex& hex, const Point3D<double>& vector, double phi);

  std::vector<bool> backFaceCulling(Hex& hex);
};

template <typename T>
int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}

template <typename T>
class Point {
 private:
  T x_coord;
  T y_coord;

 public:
  Point() : x_coord(0), y_coord(0) {}
  Point(T x_, T y_) : x_coord(x_), y_coord(y_) {}
  Point(const Point<T>& p) : x_coord(p.x()), y_coord(p.y()) {}
  T x() const { return x_coord; }
  T y() const { return y_coord; }
  void setx(T x_) { x_coord = x_; }
  void sety(T y_) { y_coord = y_; }
  void print() const { std::cout << x_coord << " " << y_coord << std::endl; }
  Point<T>& operator=(const Point<T>& p) {
    x_coord = p.x();
    y_coord = p.y();
    return *this;
  }
  friend Point<T> operator+(const Point<T>& p1, const Point<T>& p2) { return Point(p1.x() + p2.x(), p1.y() + p2.y()); }
  friend Point<T> operator-(const Point<T>& p1, const Point<T>& p2) { return Point(p1.x() - p2.x(), p1.y() - p2.y()); }
};

template <typename T>
class Point3D {
 private:
  T x_coord;
  T y_coord;
  T z_coord;

 public:
  Point3D() : x_coord(0), y_coord(0), z_coord(0) {}
  Point3D(T x_, T y_, T z_) : x_coord(x_), y_coord(y_), z_coord(z_) {}
  Point3D(const Point3D<T>& p) : x_coord(p.x()), y_coord(p.y()), z_coord(p.z()) {}
  T x() const { return x_coord; }
  T y() const { return y_coord; }
  T z() const { return z_coord; }
  void setx(T x_) { x_coord = x_; }
  void sety(T y_) { y_coord = y_; }
  void setz(T z_) { z_coord = z_; }
  void print() const { std::cout << x_coord << " " << y_coord << " " << z_coord << std::endl; }
  Point3D<T>& operator=(const Point3D<T>& p) {
    x_coord = p.x();
    y_coord = p.y();
    z_coord = p.z();
    return *this;
  }
  friend Point<T> operator+(const Point3D<T>& p1, const Point3D<T>& p2) { return Point3D(p1.x() + p2.x(), p1.y() + p2.y(), p1.z() + p2.z()); }
  friend Point<T> operator-(const Point3D<T>& p1, const Point3D<T>& p2) { return Point3D(p1.x() - p2.x(), p1.y() - p2.y(), p1.z() - p2.z()); }
};

template <typename T>
class Face {
 private:
  std::vector<Point3D<T>> points;

 public:
  Face(std::vector<Point3D<T>>& points_) : points(points_) {}
  int size() const { return points.size(); }
  Point3D<T> get(int i) const { return points[i]; }
  void print() {
    for (auto p : points) {
      p.print();
    }
  }
};

class Hex {
 private:
  std::vector<Face<double>> faces;
  std::vector<Point3D<double>> normals;  // outer normals
  std::vector<Point3D<double>> points;
  int nFaces = 6;
  int nPoints = 8;

 public:
  Hex(std::vector<Point3D<double>>& _points) { setPoints(_points); }  // [0,1,2,3] - lower face; [4,5,6,7] - upper face

  void print() {
    faces[0].print();
    faces[1].print();
  }

  std::vector<Face<double>> getFaces() { return faces; }
  std::vector<Point3D<double>> getPoints() { return points; }
  std::vector<Point3D<double>> getNormals() { return normals; }

  int getnPoints() { return nPoints; }
  int getnFaces() { return nFaces; }

  void setPoints(std::vector<Point3D<double>> new_points) {
    points.clear();
    faces.clear();
    normals.clear();
    points.insert(points.begin(), new_points.begin(), new_points.end());
    faces.reserve(nFaces);
    normals.reserve(nFaces);

    routine(points, faces, normals, 0, 3, 2, 1);
    routine(points, faces, normals, 5, 6, 7, 4);
    routine(points, faces, normals, 1, 2, 6, 5);
    routine(points, faces, normals, 0, 4, 7, 3);
    routine(points, faces, normals, 0, 1, 5, 4);
    routine(points, faces, normals, 3, 7, 6, 2);
  }

  void routine(std::vector<Point3D<double>>& points, std::vector<Face<double>>& faces, std::vector<Point3D<double>>& normals, int p0, int p1, int p2, int p3) {
    auto points0 = std::vector<Point3D<double>>{points[p0], points[p1], points[p2], points[p3]};
    auto face0 = Face(points0);
    auto v1 = Eigen::Vector3d(points[p1].x() - points[p0].x(), points[p1].y() - points[p0].y(), points[p1].z() - points[p0].z());
    auto v2 = Eigen::Vector3d(points[p3].x() - points[p0].x(), points[p3].y() - points[p0].y(), points[p3].z() - points[p0].z());
    auto res_v = v1.cross(v2);
    double normal0_len = sqrt(std::pow(res_v.x(), 2) + std::pow(res_v.y(), 2) + std::pow(res_v.z(), 2));
    auto normal0 = Point3D<double>(res_v.x() / normal0_len, res_v.y() / normal0_len, res_v.z() / normal0_len);
    faces.push_back(face0);
    normals.push_back(normal0);
  }
};

template <typename T>
class Edge {
 private:
  Point<T> a;
  Point<T> b;

 public:
  Edge() : a(), b() {}
  Edge(Point<T> a_, Point<T> b_) : a(a_.x(), a_.y()), b(b_.x(), b_.y()) {}
};

template <typename T>
pointType WinInstance::pointPositionToLineSegment(const Point<T>& p, const Point<T>& start, const Point<T>& end) {
  T ax = end.x() - start.x();
  T by = p.y() - start.y();
  T bx = p.x() - start.x();
  T ay = end.y() - start.y();
  double s = ax * by - bx * ay;
  if (s > 0)
    return pointType::RIGHT;
  if (s < 0)
    return pointType::LEFT;
  if ((ax * bx < 0) || (ay * by < 0))
    return pointType::BEHIND;
  if ((ax * ax + ay * ay) < (bx * bx + by * by))
    return pointType::BEHIND;
  if (start.x() == p.x() && start.y() == p.y())
    return pointType::ORIGIN;
  if (end.x() == p.x() && end.y() == p.y())
    return pointType::DESTINATION;
  return pointType::BETWEEN;
}

template <typename T>
interceptionType WinInstance::linesInterception(const Point<T>& a1, const Point<T>& a2, const Point<T>& b1, const Point<T>& b2, double& t) {
  double nx = b2.y() - b1.y();
  double ny = b1.x() - b2.x();
  double denom = nx * (a2.x() - a1.x()) + ny * (a2.y() - a1.y());
  if (denom == 0) {
    pointType type = pointPositionToLineSegment(b1, b2, a1);
    if (type == pointType::LEFT || type == pointType::RIGHT)
      return interceptionType::PARALLEL;
    else
      return interceptionType::SAME;
  }
  double num = nx * (a1.x() - b1.x()) + ny * (a1.y() - b1.y());
  t = -num / denom;
  return interceptionType::CROSS;
}

template <typename T>
interceptionType WinInstance::linesInterceptionCoords(const Point<T>& a1, const Point<T>& a2, const Point<T>& b1, const Point<T>& b2, Point<double>& crossCoords) {
  double t(0.0);
  interceptionType type = linesInterception(a1, a2, b1, b2, t);
  if (type == interceptionType::PARALLEL || type == interceptionType::SAME) {
    return type;
  }
  crossCoords.setx(a1.x() + t * (a2.x() - a1.x()));
  crossCoords.sety(a1.y() + t * (a2.y() - a1.y()));
}

template <typename T>
interceptionType WinInstance::lineSegmentsInterception(const Point<T>& a1, const Point<T>& a2, const Point<T>& b1, const Point<T>& b2, Point<double>& crossCoords) {
  double t(0.0);
  interceptionType type = linesInterception(b1, b2, a1, a2, t);
  if (type == interceptionType::SAME || type == interceptionType::PARALLEL)
    return type;
  if (t < 0.0 || t > 1.0)
    return interceptionType::NO_CROSS;
  linesInterception(a1, a2, b1, b2, t);
  if (t < 0.0 || t > 1.0)
    return interceptionType::NO_CROSS;
  crossCoords.setx(a1.x() + t * (a2.x() - a1.x()));
  crossCoords.sety(a1.y() + t * (a2.y() - a1.y()));
  return interceptionType::CROSS;
}
