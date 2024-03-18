#pragma once

#include <cmath>
#include <numbers>

namespace geom2D {

template <typename T>
struct Point;
template <typename T>
class LineSegment;
template <typename T>
class Figure;
template <typename T>
class Circle;
template <typename T>
class Polygon;
template <typename T>
class Rectangle;
template <typename T>
class Triangle;

template <typename T>
struct Point {
  T x_, y_;

  Point() : x_{}, y_{} {}
  Point(T x, T y) : x_{x}, y_{y} {}

  Point& operator+=(const Point& rhs) {
    x_ += rhs.x_;
    y_ += rhs.y_;
    return *this;
  }

  Point& operator*=(const T& rhs) {
    x_ *= rhs;
    y_ *= rhs;
    return *this;
  }

  Point operator+(const Point& rhs) const {
    Point tmp{*this};
    tmp += rhs;
    return tmp;
  }

  Point operator*(const T& rhs) const {
    Point tmp{*this};
    tmp *= rhs;
    return tmp;
  }
};

template <typename T>
class Figure {
 public:
  virtual double square() const = 0;
  virtual ~Figure() {}
};

template <typename T>
class LineSegment final {
 public:
  Point<T> a_;
  Point<T> b_;
  double length_;

  LineSegment(const Point<T>& a, const Point<T>& b) : a_(a), b_(b), length_(std::sqrt(std::pow(a_.x_ - b_.x_, 2) + std::pow(a_.y_ - b_.y_, 2))) {}
};

template <typename T>
class Polygon : public Figure<T> {
 protected:
  std::vector<Point<T>> points_;
  std::vector<LineSegment<T>> segments_;

 private:
  void fill_points() {
    points_.reserve(segments_.size());
    for (auto& s : segments_) {
      points_.emplace_back(s.a_);
    }
  }

  void fill_segments() {
    segments_.reserve(points_.size());
    for (int i = 0; i < points_.size() - 1; ++i) {
      segments_.emplace_back(points_[i], points_[i + 1]);
    }
    segments_.emplace_back(points_[points_.size() - 1], points_[0]);
  }

 public:
  Polygon(const std::vector<Point<T>>& points) : points_(points) { fill_segments(); }
  Polygon(std::vector<Point<T>>&& points) : points_(points) { fill_segments(); }

  Polygon(const std::vector<LineSegment<T>>& segments) : segments_(segments) { fill_points(); }
  Polygon(std::vector<LineSegment<T>>&& segments) : segments_(segments) { fill_points(); }

  const std::vector<LineSegment<T>>& get_segments() const& { return segments_; }
  const std::vector<Point<T>>& get_points() const& { return points_; }

  double square() const override {
    double S{};
    for (auto& s : segments_) {
      S += (s.a_.x_ * s.b_.y_ - s.b_.x_ * s.a_.y_);
    }
    return 0.5 * S;
  }
};

template <typename T>
class Circle final : public Figure<T> {
 public:
  Point<T> c_;
  T r_;

  Circle(const Point<T>& c, T r) : c_(c), r_(r) {}
  double square() const override { return std::numbers::pi * r_ * r_; }
};

template <typename T>
class Triangle final : public Polygon<T> {
 public:
  Triangle(const Point<T>& a, const Point<T>& b, const Point<T>& c) : Polygon<T>{{a, b, c}} {}
};

template <typename T>
class Rectangle final : public Polygon<T> {
 public:
  Rectangle(const Point<T>& a, const Point<T>& b, const Point<T>& c, const Point<T>& d) : Polygon<T>{{a, b, c, d}} {}
};

template <typename T>
std::ostream& operator<<(std::ostream& s, const Point<T>& p) {
  return s << "(" << p.x_ << " ; " << p.y_ << ")";
}

}  // namespace geom2D