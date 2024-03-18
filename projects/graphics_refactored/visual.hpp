#pragma once

#include <filesystem>
#include <SFML/Graphics.hpp>

#include "primitives2D.hpp"
#include "primitives3D.hpp"

namespace utils {

template <typename T>
int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}

}  // namespace utils

namespace win {

class WinInstance {
 private:
  sf::RenderWindow& window_;
  sf::Texture texture_;
  sf::Sprite sprite_;
  sf::Image image_;

 public:
  WinInstance(sf::RenderWindow& w);
  sf::Image& get_image() & { return image_; }
  void display();
  bool isOpen() const;
  bool pollEvent(sf::Event& event);
  void close();
  void drawImage();
  void saveToFile(const std::filesystem::path& path);
  void clear(sf::Color color);
  virtual ~WinInstance() {}
};

class Drawer final : public WinInstance {
 public:
  Drawer(sf::RenderWindow& window) : WinInstance(window) {}

  template <typename T>
  void lineBresenham(const geom2D::Point<T>& s, const geom2D::Point<T>& e, const sf::Color& color = sf::Color::Black);

  template <typename T>
  void lineBresenham(const geom2D::LineSegment<T>& l, const sf::Color& color = sf::Color::Black);

  template <typename T>
  void drawPolygon(const std::vector<geom2D::Point<T>>& points, const sf::Color& color = sf::Color::Black);

  template <typename T>
  void drawPolygon(const std::vector<geom2D::LineSegment<T>>& segments, const sf::Color& color = sf::Color::Black);

  template <typename T>
  void drawPolygon(const geom2D::Polygon<T>& polygon, const sf::Color& color = sf::Color::Black);

  template <typename T>
  void drawBezier3Curve(const std::vector<geom2D::Point<T>>& points, const sf::Color& color = sf::Color::Black);

  template <typename T>
  void drawCircle(const geom2D::Circle<T>& circle, const sf::Color& color = sf::Color::Black);

  template <typename T>
  void drawBox(const geom3D::Box<T>& box, double k = 1, const sf::Color& color = sf::Color::Black);  // perspective projection, center in (0, 0, k)
};

template <typename T>
void win::Drawer::lineBresenham(const geom2D::Point<T>& s, const geom2D::Point<T>& e, const sf::Color& color) {
  geom2D::Point<int> start{static_cast<int>(std::round(s.x_)), static_cast<int>(std::round(s.y_))};
  geom2D::Point<int> end{static_cast<int>(std::round(e.x_)), static_cast<int>(std::round(e.y_))};
  if (start.x_ > end.x_)
    std::swap(start, end);
  int x, y;
  int ix, iy;
  int dx = std::abs(end.x_ - start.x_), dy = std::abs(end.y_ - start.y_);
  bool swapped(false);

  if (dx >= dy) {
    x = start.x_, y = start.y_;
    ix = utils::sgn(end.x_ - start.x_), iy = utils::sgn(end.y_ - start.y_);
  } else {
    x = start.y_, y = start.x_;
    ix = utils::sgn(end.y_ - start.y_), iy = utils::sgn(end.x_ - start.x_);
    std::swap(dx, dy);
    swapped = true;
  }

  int error = 2 * dy - dx;
  for (int i = 0; i <= dx; i++) {
    if (!swapped)
      get_image().setPixel(x, y, color);
    else
      get_image().setPixel(y, x, color);
    if (error >= 0) {
      y += iy;
      error -= 2 * dx;
    }
    x += ix;
    error += 2 * dy;
  }
}

template <typename T>
void win::Drawer::lineBresenham(const geom2D::LineSegment<T>& l, const sf::Color& color) {
  lineBresenham(l.a_, l.b_, color);
}

template <typename T>
void win::Drawer::drawPolygon(const std::vector<geom2D::Point<T>>& polygon, const sf::Color& color) {
  for (int i = 0; i < polygon.size() - 1; ++i) {
    lineBresenham(polygon[i], polygon[i + 1], color);
  }
  lineBresenham(polygon[polygon.size() - 1], polygon[0], color);
}

template <typename T>
void win::Drawer::drawPolygon(const std::vector<geom2D::LineSegment<T>>& polygon, const sf::Color& color) {
  for (auto& l : polygon) {
    lineBresenham(l, color);
  }
}

template <typename T>
void win::Drawer::drawPolygon(const geom2D::Polygon<T>& polygon, const sf::Color& color) {
  for (const auto& l : polygon.get_segments()) {
    lineBresenham(l, color);
  }
}

template <typename T>
void win::Drawer::drawBezier3Curve(const std::vector<geom2D::Point<T>>& points, const sf::Color& color) {
  if (points.size() != 4) {
    throw std::runtime_error("\n(drawBezier3Curve) Cubic Bezier curves are plotted for 4 points.");
  }
  double H = std::max(std::abs(points[0].x_ - 2 * points[1].x_ + points[2].x_) + std::abs(points[0].y_ - 2 * points[1].y_ + points[2].y_),
                      std::abs(points[1].x_ - 2 * points[2].x_ + points[3].x_) + std::abs(points[1].y_ - 2 * points[2].y_ + points[3].y_));
  int N = std::ceil(1 + std::sqrt(3 * H));
  double tau = 1.0 / N;
  double t = tau;
  geom2D::Point<T> p1, p2{points[0]};

  for (int i = 0; i < N - 1; ++i) {
    p1 = p2;
    p2.x_ = std::pow(1 - t, 3) * points[0].x_ + 3 * t * std::pow(1 - t, 2) * points[1].x_ + 3 * std::pow(t, 2) * (1 - t) * points[2].x_ + std::pow(t, 3) * points[3].x_;
    p2.y_ = std::pow(1 - t, 3) * points[0].y_ + 3 * t * std::pow(1 - t, 2) * points[1].y_ + 3 * std::pow(t, 2) * (1 - t) * points[2].y_ + std::pow(t, 3) * points[3].y_;
    t += tau;
    lineBresenham(p1, p2, color);
  }
  lineBresenham(p2, points[3], color);
}

template <typename T>
void win::Drawer::drawCircle(const geom2D::Circle<T>& circle, const sf::Color& color) {
  double coef = (4.0 / 3.0) * (1.0 / (1.0 + std::sqrt(2)));
  std::vector<geom2D::Point<T>> ps{
      {circle.c_.x_ + circle.r_, circle.c_.y_}, {circle.c_.x_, circle.c_.y_ + circle.r_}, {circle.c_.x_ - circle.r_, circle.c_.y_}, {circle.c_.x_, circle.c_.y_ - circle.r_}};
  std::vector<geom2D::Point<T>> pt{{circle.c_.x_ + circle.r_, circle.c_.y_ + circle.r_},
                                   {circle.c_.x_ - circle.r_, circle.c_.y_ + circle.r_},
                                   {circle.c_.x_ - circle.r_, circle.c_.y_ - circle.r_},
                                   {circle.c_.x_ + circle.r_, circle.c_.y_ - circle.r_}};
  geom2D::Polygon<T> pss{ps};
  geom2D::Polygon<T> pts{pt};
  geom2D::Point<double> p0{}, p1{}, p2{}, p3{};

  for (auto pss_it = pss.get_segments().begin(), pts_it = pts.get_segments().begin(), pss_end = pss.get_segments().end(); pss_it != pss_end; ++pss_it, ++pts_it) {
    p0.x_ = static_cast<double>(pss_it->a_.x_);
    p0.y_ = static_cast<double>(pss_it->a_.y_);
    p1.x_ = pss_it->a_.x_ + coef * (pts_it->a_.x_ - pss_it->a_.x_);
    p1.y_ = pss_it->a_.y_ + coef * (pts_it->a_.y_ - pss_it->a_.y_);
    p2.x_ = pss_it->b_.x_ + coef * (pts_it->a_.x_ - pss_it->b_.x_);
    p2.y_ = pss_it->b_.y_ + coef * (pts_it->a_.y_ - pss_it->b_.y_);
    p3.x_ = static_cast<double>(pss_it->b_.x_);
    p3.y_ = static_cast<double>(pss_it->b_.y_);
    drawBezier3Curve(std::vector<geom2D::Point<double>>{p0, p1, p2, p3}, color);
  }
}

template <typename T>
void win::Drawer::drawBox(const geom3D::Box<T>& box, double k, const sf::Color& color) {
  double r = 1 / k;
  auto points = box.get_points();
  std::vector<geom3D::Point<double>> new_points;
  new_points.reserve(box.get_n_faces());
  for (auto& p : points) {
    double denom = r * p.z_ + 1;
    new_points.push_back(geom3D::Point<double>(p.x_ / denom, p.y_ / denom, p.z_ / denom));
  }

  geom3D::Box new_box(new_points);
  auto faces = new_box.get_faces();
  for (auto& f : faces) {
    for (auto& s : f.get_segments()) {
      auto p0_proj = geom2D::Point(s.a_.x_, s.a_.y_);
      auto p1_proj = geom2D::Point(s.b_.x_, s.b_.y_);
      lineBresenham(p0_proj, p1_proj, color);
    }
  }
}

}  // namespace win