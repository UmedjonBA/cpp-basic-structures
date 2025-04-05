#include <cmath>
#include <iostream>

#define toch 1e-10

class Vector {
 protected:
  int64_t x_;
  int64_t y_;

 public:
  // Конструкторы
  Vector();
  Vector(int64_t co_rx, int64_t co_ry);
  ~Vector() = default;
  // Методы получения координат
  int64_t GetX() const;
  int64_t GetY() const;

  // Операторы для скалярного и векторного произведения
  int64_t operator*(const Vector& other) const;

  int64_t operator^(const Vector& other) const;

  // Операторы для сложения и вычитания векторов
  Vector& operator+=(const Vector& other);

  Vector& operator-=(const Vector& other);

  // Операторы умножения на число
  Vector& operator*=(int64_t scalar);

  friend Vector operator*(int64_t scalar, const Vector& vec);

  // Унарный оператор минус
  Vector operator-() const;
};
class IShape;
class Point;
class Segment;
class Line;
class Ray;
class Circle;

class IShape {
 public:
  virtual void Move(const Vector& veca) = 0;
  virtual bool ContainsPoint(const Point& pointa) const = 0;
  virtual bool CrossSegment(const Segment& segmentab) const = 0;
  virtual IShape* Clone() const = 0;
  virtual ~IShape() = default;
};
class Point : public IShape, public Vector {
 public:
  Point();
  Point(int64_t corx, int64_t cory);
  void Move(const Vector& veca) override;
  bool ContainsPoint(const Point& pointa) const override;
  bool CrossSegment(const Segment& segmentab) const override;
  IShape* Clone() const override;
  int64_t GetX() const;
  int64_t GetY() const;
};
class Segment : public IShape {
 public:
  Segment(Point pointa, Point pointb);
  void Move(const Vector& veca) override;
  bool ContainsPoint(const Point& pointa) const override;
  bool CrossSegment(const Segment& segmentab) const override;
  IShape* Clone() const override;
  Point GetA() const;
  Point GetB() const;

 private:
  Point a_;
  Point b_;
};
class Line : public IShape {
 public:
  Line(Point pointa, Point pointb);
  void Move(const Vector& veca) override;
  bool ContainsPoint(const Point& pointa) const override;
  bool CrossSegment(const Segment& segmentab) const override;
  IShape* Clone() const override;
  int64_t GetA() const;
  int64_t GetB() const;
  int64_t GetC() const;

 private:
  Point a_;
  Point b_;
};
class Ray : public IShape {
 public:
  Ray(Point pointa, Point pointb);
  void Move(const Vector& veca) override;
  bool ContainsPoint(const Point& pointa) const override;
  bool CrossSegment(const Segment& segmentab) const override;
  IShape* Clone() const override;
  Point GetA();
  Vector GetVector();
  friend bool DoIntersect(Ray ray1, Ray ray2);

 private:
  Point a_;
  Point b_;
};
class Circle : public IShape {
 public:
  Circle(Point pointa, size_t radius);
  void Move(const Vector& veca) override;
  bool ContainsPoint(const Point& pointa) const override;
  bool CrossSegment(const Segment& segmentab) const override;
  IShape* Clone() const override;
  Point GetCentre();
  size_t GetRadius() const;

 private:
  Point a_;
  size_t r_;
};

// Операторы сложения и вычитания векторов
Vector operator+(Vector lhs, const Vector& rhs);

Vector operator-(Vector lhs, const Vector& rhs);

int64_t DoubleVectorProduct(Point first, Point second, Point thirth);
bool Intersection(int64_t point_a, int64_t point_b, int64_t point_c,
                  int64_t point_d);
int64_t EvaluatePointInLineEquation(int64_t&& coefficients_line_a,
                                    int64_t&& coefficients_line_b,
                                    int64_t&& coefficients_line_c,
                                    const Point& point);
int64_t SquareDistanceBetweenPoints(Point point_a, Point point_b);