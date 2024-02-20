#include <cmath>
#include <limits>

#include "functions.h"

WinInstance::WinInstance(sf::RenderWindow& window_) : window(window_) {
  window.clear(sf::Color::White);
  image.create(window.getSize().x, window.getSize().y, sf::Color::White);
  texture.loadFromImage(image);
  sprite.setTexture(texture);
  sprite.setPosition(0, 0);
}

void WinInstance::drawImage(bool update) {
  if (update) {
    window.clear(sf::Color::White);
    texture.update(image);
    sprite.setTexture(texture);
    window.draw(sprite);
    window.display();
    image.create(window.getSize().x, window.getSize().y, sf::Color::White);
  } else {
    window.clear(sf::Color::White);
    window.draw(sprite);
    window.display();
  }
}

bool WinInstance::saveImage(const std::string& filename, bool update) {
  bool flag = image.saveToFile(filename);
  if (update) {
    window.clear(sf::Color::White);
    texture.update(image);
    sprite.setTexture(texture);
    window.draw(sprite);
    window.display();
    image.create(window.getSize().x, window.getSize().y, sf::Color::White);
  } else {
    window.clear(sf::Color::White);
    window.draw(sprite);
    window.display();
  }
  return flag;
}

void WinInstance::display() {
  window.display();
}

bool WinInstance::isOpen() const {
  return window.isOpen();
}

bool WinInstance::pollEvent(sf::Event& event) {
  return window.pollEvent(event);
}

void WinInstance::close() {
  window.close();
}

void WinInstance::clear(sf::Color color) {
  window.clear(color);
}

void WinInstance::lineBresenham(const Point<int>& s, const Point<int>& e, const sf::Color& color) {
  Point<int> start(s.x(), s.y());
  Point<int> end(e.x(), e.y());
  if (start.x() > end.x())
    std::swap(start, end);
  int x, y;
  int ix, iy;
  int dx = abs(end.x() - start.x()), dy = abs(end.y() - start.y());
  bool swapped(false);

  if (dx >= dy) {
    x = start.x(), y = start.y();
    ix = sgn(end.x() - start.x()), iy = sgn(end.y() - start.y());
  } else {
    x = start.y(), y = start.x();
    ix = sgn(end.y() - start.y()), iy = sgn(end.x() - start.x());
    std::swap(dx, dy);
    swapped = true;
  }

  int error = 2 * dy - dx;
  for (int i = 0; i <= dx; i++) {
    if (!swapped)
      image.setPixel(x, y, color);
    else
      image.setPixel(y, x, color);
    if (error >= 0) {
      y += iy;
      error -= 2 * dx;
    }
    x += ix;
    error += 2 * dy;
  }
}

void WinInstance::polygon(const std::vector<Point<int>>& vertex, const sf::Color& color) {
  for (int i = 0; i < vertex.size() - 1; ++i) {
    lineBresenham(vertex[i], vertex[i + 1], color);
  }
}

bool WinInstance::checkConvex(const std::vector<Point<int>>& vertex) {
  pointType type = pointPositionToLineSegment(vertex[0], vertex[1], vertex[2]);
  for (int i = 0; i < vertex.size() - 2; i++) {
    for (int j = 0; j < vertex.size() - 1; j++) {
      if (j != i && j != i + 1) {
        if (pointPositionToLineSegment(vertex[j], vertex[i], vertex[i + 1]) != type)
          return false;
      }
    }
  }
  return true;
}

clockWiseType WinInstance::checkClockWise(const std::vector<Point<int>>& vertex) {
  if (!checkConvex(vertex))
    return clockWiseType::NONCONVEX;
  pointType type = pointPositionToLineSegment(vertex[0], vertex[1], vertex[2]);
  if (type == pointType::RIGHT)
    return clockWiseType::CW;
  else
    return clockWiseType::CCW;
}

bool WinInstance::checkPolygonSimplicity(const std::vector<Point<int>>& vertex, Point<double>& p) {
  for (int i = 0; i < vertex.size() - 2; i++) {
    for (int j = 0; j < vertex.size() - 1; j++) {
      if (j != i && j != i + 1 && (j != (i > 0 ? i - 1 : vertex.size() - 2))) {
        if (lineSegmentsInterception(vertex[i], vertex[i + 1], vertex[j], vertex[j + 1], p) == interceptionType::CROSS)
          return false;
      }
    }
  }
  return true;
}

void WinInstance::boundingBox(const std::vector<Point<int>>& polygon, std::vector<Point<int>>& res) {
  int maxx = 0.0, maxy = 0.0, minx = std::numeric_limits<int>::max(), miny = std::numeric_limits<int>::max();
  for (Point<int> vertex : polygon) {
    if (vertex.x() > maxx)
      maxx = vertex.x();
    if (vertex.x() < minx)
      minx = vertex.x();
    if (vertex.y() > maxy)
      maxy = vertex.y();
    if (vertex.y() < miny)
      miny = vertex.y();
  }
  res[0].setx(minx), res[0].sety(miny);
  res[1].setx(minx), res[1].sety(maxy);
  res[2].setx(maxx), res[2].sety(maxy);
  res[3].setx(maxx), res[3].sety(miny);
  res[4].setx(minx), res[4].sety(miny);
  res.resize(5);
  res.shrink_to_fit();
}

pointTypeToPolygonEdge WinInstance::pttpe(const Point<int>& p, const Point<int>& p1, const Point<int>& p2) {
  switch (pointPositionToLineSegment(p, p1, p2)) {
    case pointType::LEFT:
      if (p.y() > p1.y() && p.y() <= p2.y())
        return pointTypeToPolygonEdge::CROSS_LEFT;
      else
        return pointTypeToPolygonEdge::INESSENTIAL;
    case pointType::RIGHT:
      if (p.y() > p2.y() && p.y() <= p1.y())
        return pointTypeToPolygonEdge::CROSS_RIGHT;
      else
        return pointTypeToPolygonEdge::INESSENTIAL;
    case pointType::BETWEEN:
    case pointType::ORIGIN:
    case pointType::DESTINATION:
      return pointTypeToPolygonEdge::TOUCHING;
    default:
      return pointTypeToPolygonEdge::INESSENTIAL;
  }
}

pointToPolygonType WinInstance::fillPointEvenOdd(const Point<int>& p, const std::vector<Point<int>>& polygon) {
  int param(0);
  for (int i = 0; i < polygon.size() - 1; i++) {
    switch (pttpe(p, polygon[i], polygon[i + 1])) {
      case pointTypeToPolygonEdge::TOUCHING:
        return pointToPolygonType::INSIDE;
      case pointTypeToPolygonEdge::CROSS_LEFT:
      case pointTypeToPolygonEdge::CROSS_RIGHT:
        param = 1 - param;
        break;
    }
  }
  if (param)
    return pointToPolygonType::INSIDE;
  else
    return pointToPolygonType::OUTSIDE;
}

pointToPolygonType WinInstance::fillPointNonZeroWinding(const Point<int>& p, const std::vector<Point<int>>& polygon) {
  int param(0);
  for (int i = 0; i < polygon.size() - 1; i++) {
    switch (pttpe(p, polygon[i], polygon[i + 1])) {
      case pointTypeToPolygonEdge::TOUCHING:
        return pointToPolygonType::INSIDE;
      case pointTypeToPolygonEdge::CROSS_LEFT:
        param++;
        break;
      case pointTypeToPolygonEdge::CROSS_RIGHT:
        param--;
        break;
    }
  }
  if (param)
    return pointToPolygonType::INSIDE;
  else
    return pointToPolygonType::OUTSIDE;
}

void WinInstance::fillPolygon(const std::vector<Point<int>>& polygon, const methods& method, const sf::Color& color) {
  std::vector<Point<int>> boundbox(5);
  boundingBox(polygon, boundbox);
  switch (method) {
    case methods::EO:
      for (int i = boundbox[0].x(); i <= boundbox[2].x(); i++) {
        for (int j = boundbox[0].y(); j <= boundbox[2].y(); j++) {
          if (fillPointEvenOdd(Point(i, j), polygon) == pointToPolygonType::INSIDE)
            image.setPixel(i, j, color);
        }
      }
      break;

    case methods::NZW:
      for (int i = boundbox[0].x(); i <= boundbox[2].x(); i++) {
        for (int j = boundbox[0].y(); j <= boundbox[2].y(); j++) {
          if (fillPointNonZeroWinding(Point(i, j), polygon) == pointToPolygonType::INSIDE)
            image.setPixel(i, j, color);
        }
      }
      break;

    case methods::NONEXTERIOR:
      std::vector<Point<int>> interceptPolygon;
      interceptPolygon.reserve(2 * polygon.size());
      std::vector<bool> interceptFlags;
      interceptFlags.reserve(2 * polygon.size());
      std::vector<std::vector<Point<int>>> nextAfterInterception;
      std::vector<Point<int>> tmp{Point<int>(), Point<int>(), Point<int>(), Point<int>()};
      nextAfterInterception.reserve(2 * polygon.size());

      for (int i = 0; i < polygon.size() - 1; ++i) {
        Point<int> a = polygon[i];
        Point<int> b = polygon[i + 1];
        interceptPolygon.push_back(a);
        interceptFlags.push_back(false);
        nextAfterInterception.push_back(tmp);
        for (int j = 0; j < polygon.size() - 1; ++j) {
          Point<double> interceptPoint;
          if ((j == i) || (j == (i + 1) % (polygon.size() - 1)) || ((j + 1) % (polygon.size() - 1) == i) || (j + 1 == i + 1))
            continue;
          if (lineSegmentsInterception(a, b, polygon[j], polygon[j + 1], interceptPoint) == interceptionType::CROSS) {
            int x = static_cast<int>(interceptPoint.x());
            int y = static_cast<int>(interceptPoint.y());
            interceptPolygon.push_back(Point<int>(x, y));
            interceptFlags.push_back(true);
            std::vector<Point<int>> points{a, b, polygon[j], polygon[j + 1]};
            nextAfterInterception.push_back(points);
          }
        }
      }
      interceptPolygon.push_back(polygon[0]);
      interceptFlags.push_back(false);
      nextAfterInterception.push_back(tmp);
      interceptPolygon.shrink_to_fit();
      interceptFlags.shrink_to_fit();
      nextAfterInterception.shrink_to_fit();

      std::vector<Point<int>> resultPolygon;
      resultPolygon.reserve(interceptPolygon.size());
      for (int i = 0; i < interceptPolygon.size(); ++i) {
        resultPolygon.push_back(interceptPolygon[i]);
        if (interceptFlags[i]) {
          if (pointPositionToLineSegment(nextAfterInterception[i][2], nextAfterInterception[i][0], nextAfterInterception[i][1]) == pointType::LEFT) {
            resultPolygon.push_back(nextAfterInterception[i][2]);
            int j = i;
            while (interceptPolygon[j].x() != nextAfterInterception[i][2].x() || interceptPolygon[j].y() != nextAfterInterception[i][2].y()) {
              ++j;
            }
            i = j - 1;
          } else {
            int j = i;
            resultPolygon.push_back(nextAfterInterception[i][3]);
            while (interceptPolygon[j].x() != nextAfterInterception[i][3].x() || interceptPolygon[j].y() != nextAfterInterception[i][3].y()) {
              ++j;
            }
            i = j - 1;
          }
        }
      }
      resultPolygon.shrink_to_fit();
      std::vector<Point<int>> boundbox(5);
      boundingBox(resultPolygon, boundbox);
      for (int i = boundbox[0].x(); i <= boundbox[2].x(); i++) {
        for (int j = boundbox[0].y(); j <= boundbox[2].y(); j++) {
          if (fillPointEvenOdd(Point(i, j), resultPolygon) == pointToPolygonType::INSIDE)
            image.setPixel(i, j, color);
        }
      }
      break;
  }
}

void WinInstance::curveBezier3(const std::vector<Point<int>>& points, const sf::Color& color) {
  if (points.size() != 4) {
    throw std::runtime_error("\n(curveBezier3) Cubic Bézier curves are plotted for 4 points.");
  }
  int H = std::max(abs(points[0].x() - 2 * points[1].x() + points[2].x()) + abs(points[0].y() - 2 * points[1].y() + points[2].y()),
                   abs(points[1].x() - 2 * points[2].x() + points[3].x()) + abs(points[1].y() - 2 * points[2].y() + points[3].y()));
  int N = std::ceil(1 + std::sqrt(3 * H));
  double tau = 1.0 / N;
  double t = tau;
  Point<int> p1, p2(points[0]);
  for (int i = 0; i < N - 1; ++i) {
    p1 = p2;
    p2.setx(std::round(std::pow(1 - t, 3) * points[0].x() + 3 * t * std::pow(1 - t, 2) * points[1].x() + 3 * std::pow(t, 2) * (1 - t) * points[2].x() +
                       std::pow(t, 3) * points[3].x()));
    p2.sety(std::round(std::pow(1 - t, 3) * points[0].y() + 3 * t * std::pow(1 - t, 2) * points[1].y() + 3 * std::pow(t, 2) * (1 - t) * points[2].y() +
                       std::pow(t, 3) * points[3].y()));
    t += tau;
    lineBresenham(p1, p2, color);
  }
  lineBresenham(p2, points[3], color);
}

void WinInstance::curveBspline3(std::vector<Point<int>>& points, const sf::Color& color) {
  double tau = 0.01;
  double t = tau;
  points.insert(points.begin(), Point(2 * points[0].x() - points[1].x(), 2 * points[0].y() - points[1].y()));
  points.insert(points.end(), Point(2 * points[points.size() - 1].x() - points[points.size() - 2].x(), 2 * points[points.size() - 1].y() - points[points.size() - 2].y()));
  int s = points.size();
  for (int j = 0; j < s - 3; ++j) {
    Point<int> p1, p2;
    p2.setx(std::round((1.0 / 6.0) * (points[j].x() + 4 * points[j + 1].x() + points[j + 2].x())));
    p2.sety(std::round((1.0 / 6.0) * (points[j].y() + 4 * points[j + 1].y() + points[j + 2].y())));
    t = tau;
    while (t < 1) {
      p1 = p2;
      p2.setx(std::round((1.0 / 6.0) * (std::pow(1 - t, 3) * points[j].x() + (3 * std::pow(t, 3) - 6 * std::pow(t, 2) + 4) * points[j + 1].x() +
                                        (-3 * std::pow(t, 3) + 3 * std::pow(t, 2) + 3 * t + 1) * points[j + 2].x() + std::pow(t, 3) * points[j + 3].x())));
      p2.sety(std::round((1.0 / 6.0) * (std::pow(1 - t, 3) * points[j].y() + (3 * std::pow(t, 3) - 6 * std::pow(t, 2) + 4) * points[j + 1].y() +
                                        (-3 * std::pow(t, 3) + 3 * std::pow(t, 2) + 3 * t + 1) * points[j + 2].y() + std::pow(t, 3) * points[j + 3].y())));
      t += tau;
      lineBresenham(p1, p2, color);
    }
    Point<int> tmp(std::round((1.0 / 6.0) * (points[j + 1].x() + 4 * points[j + 2].x() + points[j + 3].x())),
                   std::round((1.0 / 6.0) * (points[j + 1].y() + 4 * points[j + 2].y() + points[j + 3].y())));
    lineBresenham(p2, tmp, color);
    tmp.print();
  }
}

bool WinInstance::clipLineCyrusBeck(const std::vector<Point<int>>& polygon, const Point<int>& p1, const Point<int>& p2, Point<int>& p1_new, Point<int>& p2_new) {
  Point<double> __tmp__;
  std::vector<Point<int>> _polygon(polygon.begin(), polygon.end());
  if (!checkPolygonSimplicity(polygon, __tmp__)) {
    throw std::runtime_error("\n(clipLineCyrusBeck) Polygon is not simple.");
  }
  switch (checkClockWise(polygon)) {
    case clockWiseType::NONCONVEX:
      throw std::runtime_error("\n(clipLineCyrusBeck) Polygon is not convex.");
    case clockWiseType::CCW:
      std::reverse(_polygon.begin(), _polygon.end());
      break;
    case clockWiseType::CW:
      break;
  }

  Point<int> s = p2 - p1;
  double t(0.0), t1(0.0), t2(1.0);
  for (int i = 0; i < _polygon.size() - 1; i++) {
    switch (linesInterception(p1, p2, _polygon[i], _polygon[i + 1], t)) {
      case interceptionType::SAME:
        return false;
      case interceptionType::PARALLEL:
        if (pointPositionToLineSegment(p1, _polygon[i], _polygon[i + 1]) == pointType::LEFT)
          return false;
        break;
      case interceptionType::CROSS:
        int nx = _polygon[i].y() - _polygon[i + 1].y();
        int ny = _polygon[i + 1].x() - _polygon[i].x();
        if (nx * s.x() + ny * s.y() > 0) {
          t1 = std::max(t, t1);
        } else {
          t2 = std::min(t, t2);
        }
        break;
    }
  }
  if (t1 <= t2) {
    p1_new.setx(std::round(p1.x() + t1 * (p2.x() - p1.x())));
    p1_new.sety(std::round(p1.y() + t1 * (p2.y() - p1.y())));
    p2_new.setx(std::round(p1.x() + t2 * (p2.x() - p1.x())));
    p2_new.sety(std::round(p1.y() + t2 * (p2.y() - p1.y())));
    return true;
  }
  return false;
}

void WinInstance::parallelProjection(Hex& hex, const sf::Color& color, bool onlyVisible) {
  auto faces = hex.getFaces();
  std::vector<bool> vis_faces;
  vis_faces.resize(hex.getnFaces(), true);
  if (onlyVisible)
    vis_faces = backFaceCulling(hex);

  for (int j = 0; j < faces.size(); ++j) {
    if (vis_faces[j]) {
      auto f = faces[j];
      for (int i = 0; i < f.size() - 1; ++i) {
        auto p0 = f.get(i);
        auto p1 = f.get(i + 1);
        auto p0_z = Point<int>(std::round(p0.x()), std::round(p0.y()));
        auto p1_z = Point<int>(std::round(p1.x()), std::round(p1.y()));
        lineBresenham(p0_z, p1_z, color);
      }
      auto p0 = f.get(f.size() - 1);
      auto p1 = f.get(0);
      auto p0_z = Point<int>(std::round(p0.x()), std::round(p0.y()));
      auto p1_z = Point<int>(std::round(p1.x()), std::round(p1.y()));
      lineBresenham(p0_z, p1_z, color);
    }
  }
}

void WinInstance::perspectiveProjection(Hex& hex, double k, const sf::Color& color, bool onlyVisible) {  // center in (0, 0, k)
  double r = 1 / k;
  auto points = hex.getPoints();
  std::vector<Point3D<double>> new_points;
  new_points.reserve(hex.getnFaces());
  for (auto p : points) {
      double denom = r * p.z() + 1;
      new_points.push_back(Point3D<double>(p.x() / denom, p.y() / denom, p.z() / denom));
  }

  Hex new_hex(new_points);
  auto faces = new_hex.getFaces();
  std::vector<bool> vis_faces;
  vis_faces.resize(new_hex.getnFaces(), true);
  if (onlyVisible)
    vis_faces = backFaceCulling(new_hex);

  for (int j = 0; j < faces.size(); ++j) {
    if (vis_faces[j]) {
      auto f = faces[j];
      for (int i = 0; i < f.size() - 1; ++i) {
        auto p0 = f.get(i);
        auto p1 = f.get(i + 1);
        auto p0_proj = Point<int>(std::round(p0.x()), std::round(p0.y()));
        auto p1_proj = Point<int>(std::round(p1.x()), std::round(p1.y()));
        lineBresenham(p0_proj, p1_proj, color);
      }
      auto p0 = f.get(f.size() - 1);
      auto p1 = f.get(0);
      auto p0_proj = Point<int>(std::round(p0.x()), std::round(p0.y()));
      auto p1_proj = Point<int>(std::round(p1.x()), std::round(p1.y()));
      lineBresenham(p0_proj, p1_proj, color);
    }
  }
}

void WinInstance::rotate(Hex& hex, const Point3D<double>& vector, double phi) {
  auto points = hex.getPoints();
  std::vector<Point3D<double>> res_points;
  res_points.reserve(hex.getnPoints());

  double vector_len = sqrt(std::pow(vector.x(), 2) + std::pow(vector.y(), 2) + std::pow(vector.z(), 2));
  Point3D n(vector.x() / vector_len, vector.y() / vector_len, vector.z() / vector_len);

  Eigen::Matrix3d rotateMatrix;
  rotateMatrix(0, 0) = cos(phi) + n.x() * n.x() * (1 - cos(phi));
  rotateMatrix(0, 1) = n.x() * n.y() * (1 - cos(phi)) + n.z() * sin(phi);
  rotateMatrix(0, 2) = n.x() * n.z() * (1 - cos(phi)) - n.y() * sin(phi);
  rotateMatrix(1, 0) = n.x() * n.y() * (1 - cos(phi)) - n.z() * sin(phi);
  rotateMatrix(1, 1) = cos(phi) + n.y() * n.y() * (1 - cos(phi));
  rotateMatrix(1, 2) = n.y() * n.z() * (1 - cos(phi)) + n.x() * sin(phi);
  rotateMatrix(2, 0) = n.x() * n.z() * (1 - cos(phi)) + n.y() * sin(phi);
  rotateMatrix(2, 1) = n.y() * n.z() * (1 - cos(phi)) - n.x() * sin(phi);
  rotateMatrix(2, 2) = cos(phi) + n.z() * n.z() * (1 - cos(phi));

  for (int i = 0; i < hex.getnPoints(); ++i) {
    Eigen::Vector3d v(points[i].x(), points[i].y(), points[i].z());
    auto res_v = v.transpose() * rotateMatrix;
    Point3D<double> res_p(res_v.x(), res_v.y(), res_v.z());
    res_points.push_back(res_p);
  }
  hex.setPoints(res_points);
}

std::vector<bool> WinInstance::backFaceCulling(Hex& hex) {
  std::vector<bool> vis_faces;
  vis_faces.resize(hex.getnFaces());
  auto normals = hex.getNormals();
  auto view_vector = Eigen::Vector3d(0., 0., -1.);
  for (int i = 0; i < normals.size(); ++i) {
    auto v = Eigen::Vector3d(normals[i].x(), normals[i].y(), normals[i].z());
    if (v.dot(view_vector) < 0)
      vis_faces[i] = true;
    else
      vis_faces[i] = false;
  }
  return vis_faces;
}
