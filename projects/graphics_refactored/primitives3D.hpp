#pragma once

#include <Eigen/Dense>

namespace geom3D {

template <typename T>
struct Point;
template <typename T>
class LineSegment;
template <typename T>
class Face;
template <typename T>
class Figure;
template <typename T>
class Box;

template <typename T>
struct Point {
  T x_, y_, z_;

  Point() : x_{}, y_{}, z_{} {}
  Point(T x, T y, T z) : x_{x}, y_{y}, z_{z} {}

  Point& operator+=(const Point& rhs) {
    x_ += rhs.x_;
    y_ += rhs.y_;
    z_ += rhs.z_;
    return *this;
  }

  Point operator+(const Point& rhs) const {
    Point tmp{*this};
    tmp += rhs;
    return tmp;
  }
};

template <typename T>
class LineSegment final {
 public:
  Point<T> a_;
  Point<T> b_;
  double length_;

  LineSegment(const Point<T>& a, const Point<T>& b) : a_(a), b_(b), length_(std::sqrt(std::pow(a_.x_ - b_.x_, 2) + std::pow(a_.y_ - b_.y_, 2) + std::pow(a_.z_ - b_.z_, 2))) {}
};

template <typename T>
class Face final {
 private:
  std::vector<Point<T>> points_;
  std::vector<LineSegment<T>> segments_;

  void fill_segments() {
    segments_.reserve(points_.size());
    for (int i = 0; i < points_.size() - 1; ++i) {
      segments_.emplace_back(points_[i], points_[i + 1]);
    }
    segments_.emplace_back(points_[points_.size() - 1], points_[0]);
  }

 public:
  Face(const std::vector<Point<T>>& points) : points_(points) { fill_segments(); }
  Face(std::vector<Point<T>>&& points) : points_(points) { fill_segments(); }
  int get_n_points() { return points_.size(); }
  const std::vector<Point<T>>& get_points() const& { return points_; }
  const std::vector<LineSegment<T>>& get_segments() const& { return segments_; }
};

template <typename T>
class Figure {
 public:
  virtual double volume() const = 0;
  virtual ~Figure() {}
};

template <typename T>
class Box final : public Figure<T> {
 private:
  std::vector<Point<T>> points_;  // 0, 1, 2, 3 - lower face; 4, 5, 6, 7 - upper face
  std::vector<Face<T>> faces_;
  Eigen::Vector3<T> v1;
  Eigen::Vector3<T> v2;
  Eigen::Vector3<T> v3;
  int n_points_ = 8;
  int n_faces_ = 6;

 public:
  const std::vector<Point<T>>& get_points() const& { return points_; }
  const std::vector<Face<T>>& get_faces() const& { return faces_; }
  int get_n_faces() const { return n_faces_; }

  Box(const std::vector<Point<T>>& points)
      : points_{points},
        v1{points_[3].x_ - points_[0].x_, points_[3].y_ - points_[0].y_, points_[3].z_ - points_[0].z_},
        v2{points_[1].x_ - points_[0].x_, points_[1].y_ - points_[0].y_, points_[1].z_ - points_[0].z_},
        v3{points_[4].x_ - points_[0].x_, points_[4].y_ - points_[0].y_, points_[4].z_ - points_[0].z_} {
    faces_.reserve(n_faces_);
    faces_.push_back(std::vector{points_[0], points_[3], points_[2], points_[1]});
    faces_.push_back(std::vector{points_[5], points_[6], points_[7], points_[4]});
    faces_.push_back(std::vector{points_[1], points_[2], points_[6], points_[5]});
    faces_.push_back(std::vector{points_[0], points_[4], points_[7], points_[3]});
    faces_.push_back(std::vector{points_[0], points_[1], points_[5], points_[4]});
    faces_.push_back(std::vector{points_[3], points_[7], points_[6], points_[2]});
  }

  Box<double> create_rotated(const Point<double>& vector, double phi) {
    std::vector<Point<double>> res_points;
    res_points.reserve(n_points_);

    double vector_len = std::sqrt(std::pow(vector.x_, 2) + std::pow(vector.y_, 2) + std::pow(vector.z_, 2));
    Point n(vector.x_ / vector_len, vector.y_ / vector_len, vector.z_ / vector_len);

    Eigen::Matrix3d rotateMatrix;
    rotateMatrix(0, 0) = cos(phi) + n.x_ * n.x_ * (1 - cos(phi));
    rotateMatrix(0, 1) = n.x_ * n.y_ * (1 - cos(phi)) + n.z_ * sin(phi);
    rotateMatrix(0, 2) = n.x_ * n.z_ * (1 - cos(phi)) - n.y_ * sin(phi);
    rotateMatrix(1, 0) = n.x_ * n.y_ * (1 - cos(phi)) - n.z_ * sin(phi);
    rotateMatrix(1, 1) = cos(phi) + n.y_ * n.y_ * (1 - cos(phi));
    rotateMatrix(1, 2) = n.y_ * n.z_ * (1 - cos(phi)) + n.x_ * sin(phi);
    rotateMatrix(2, 0) = n.x_ * n.z_ * (1 - cos(phi)) + n.y_ * sin(phi);
    rotateMatrix(2, 1) = n.y_ * n.z_ * (1 - cos(phi)) - n.x_ * sin(phi);
    rotateMatrix(2, 2) = cos(phi) + n.z_ * n.z_ * (1 - cos(phi));

    for (auto &p : points_) {
      Eigen::Vector3d v(p.x_, p.y_, p.z_);
      auto res_v = v.transpose() * rotateMatrix;
      Point<double> res_p(res_v.x(), res_v.y(), res_v.z());
      res_points.push_back(res_p);
    }
    return Box<double>(res_points);
  }

  double volume() const override { return std::abs(v1.cross(v2).dot(v3)); }
};

template <typename T>
std::ostream& operator<<(std::ostream& s, const Point<T>& p) {
  return s << "(" << p.x_ << " ; " << p.y_ << " ; " << p.z_ << ")";
}

}  // namespace geom3D