#include "cpp-23-24-geometry.hpp"
// Конструкторы
Vector::Vector() : x_(0), y_(0) {}
Vector::Vector(int64_t co_rx, int64_t co_ry) : x_(co_rx), y_(co_ry) {}

// Методы получения координат
int64_t Vector::GetX() const { return x_; }
int64_t Vector::GetY() const { return y_; }

// Операторы для скалярного и векторного произведения
int64_t Vector::operator*(const Vector& other) const {
  return x_ * other.x_ + y_ * other.y_;
}

int64_t Vector::operator^(const Vector& other) const {
  return x_ * other.y_ - y_ * other.x_;
}

// Операторы для сложения и вычитания векторов
Vector& Vector::operator+=(const Vector& other) {
  x_ += other.x_;
  y_ += other.y_;
  return *this;
}

Vector& Vector::operator-=(const Vector& other) {
  x_ -= other.x_;
  y_ -= other.y_;
  return *this;
}

// Операторы умножения на число
Vector& Vector::operator*=(int64_t scalar) {
  x_ *= scalar;
  y_ *= scalar;
  return *this;
}

Vector operator*(int64_t scalar, const Vector& vec) {
  return Vector(vec.x_ * scalar, vec.y_ * scalar);
}

// Унарный оператор минус
Vector Vector::operator-() const { return Vector(-x_, -y_); }
// Операторы сложения и вычитания векторов
Vector operator+(Vector lhs, const Vector& rhs) {
  Vector res(lhs.GetX(), lhs.GetY());
  return res += rhs;
  ;
}

Vector operator-(Vector lhs, const Vector& rhs) {
  Vector res(lhs.GetX(), lhs.GetY());
  res -= rhs;
  return res;
}

Point::Point() {}
Point::Point(int64_t corx, int64_t cory) : Vector(corx, cory) {}
void Point::Move(const Vector& veca) {
  x_ += veca.GetX();
  y_ += veca.GetY();
}
bool Point::ContainsPoint(const Point& pointa) const {
  return x_ == pointa.x_ && y_ == pointa.y_;
}
bool Point::CrossSegment(const Segment& segmentab) const {
  return segmentab.ContainsPoint(*this);
}
IShape* Point::Clone() const { return new Point(x_, y_); }
int64_t Point::GetX() const { return x_; }
int64_t Point::GetY() const { return y_; }

Segment::Segment(Point pointa, Point pointb) : a_(pointa), b_(pointb) {}
void Segment::Move(const Vector& veca) {
  a_.Move(veca);
  b_.Move(veca);
}
bool Segment::ContainsPoint(const Point& pointa) const {
  return (((pointa.GetX() - a_.GetX()) * (b_.GetY() - a_.GetY()) ==
           (pointa.GetY() - a_.GetY()) * (b_.GetX() - a_.GetX())) &&
          (std::min(a_.GetX(), b_.GetX()) <= pointa.GetX() &&
           pointa.GetX() <= std::max(a_.GetX(), b_.GetX())) &&
          (std::min(a_.GetY(), b_.GetY()) <= pointa.GetY() &&
           pointa.GetY() <= std::max(a_.GetY(), b_.GetY())));
}
// Согласно вашему комментарию, я не стал менять логику и переименовал функцию
// VectorWorks, оставив комментарий о её назначении. Я не хотел использовать
// методы других классов, чтобы они были независимы друг от друга, за
// исключением случаев, когда мне явно передают переменные этого
// класса.
bool Segment::CrossSegment(const Segment& segmentab) const {
  return Intersection(a_.GetX(), b_.GetX(), segmentab.GetA().GetX(),
                      segmentab.GetB().GetX()) &&
         Intersection(a_.GetY(), b_.GetY(), segmentab.GetA().GetY(),
                      segmentab.GetB().GetY()) &&
         DoubleVectorProduct(a_, b_, segmentab.GetA()) *
                 DoubleVectorProduct(a_, b_, segmentab.GetB()) <=
             0 &&
         DoubleVectorProduct(segmentab.GetA(), segmentab.GetB(), a_) *
                 DoubleVectorProduct(segmentab.GetA(), segmentab.GetB(), b_) <=
             0;
}
IShape* Segment::Clone() const { return new Segment(a_, b_); }
Point Segment::GetA() const { return a_; }
Point Segment::GetB() const { return b_; }

Line::Line(Point pointa, Point pointb) : a_(pointa), b_(pointb) {}
void Line::Move(const Vector& veca) {
  a_.Move(veca);
  b_.Move(veca);
}
bool Line::ContainsPoint(const Point& pointa) const {
  return EvaluatePointInLineEquation(this->GetA(), this->GetB(), this->GetC(),
                                     pointa) == 0;
  // изночально для проверки я использовал свойство коллинеарности точек на
  // плоскости, но поменял логику что бы впихнуть функцию подстановки точки в
  // уравнение прямой. Согласно вашему коментарию.
  // return ((pointa.GetX() - a_.GetX()) * (b_.GetY() - a_.GetY()) ==
  //         (pointa.GetY() - a_.GetY()) * (b_.GetX() - a_.GetX()));
}
bool Line::CrossSegment(const Segment& segmentab) const {
  return ((EvaluatePointInLineEquation(this->GetA(), this->GetB(), this->GetC(),
                                       segmentab.GetA())) *
          (EvaluatePointInLineEquation(this->GetA(), this->GetB(), this->GetC(),
                                       segmentab.GetB()))) <= 0;
}

IShape* Line::Clone() const { return new Line(a_, b_); }
int64_t Line::GetA() const { return (b_.GetY() - a_.GetY()); }
int64_t Line::GetB() const { return (a_.GetX() - b_.GetX()); }
int64_t Line::GetC() const {
  return (a_.GetY() * b_.GetX() - a_.GetX() * b_.GetY());
}

Ray::Ray(Point pointa, Point pointb) : a_(pointa), b_(pointb) {}
void Ray::Move(const Vector& veca) {
  a_.Move(veca);
  b_.Move(veca);
}
bool Ray::ContainsPoint(const Point& pointa) const {
  Vector napr(this->b_.GetX() - this->a_.GetX(),
              this->b_.GetY() - this->a_.GetY());
  return (pointa.GetX() - this->a_.GetX()) * napr.GetY() ==
             (pointa.GetY() - this->a_.GetY()) * napr.GetX() &&
         (pointa.GetX() - this->a_.GetX()) * napr.GetX() >= 0 &&
         (pointa.GetY() - this->a_.GetY()) * napr.GetY() >= 0;
}

bool Ray::CrossSegment(const Segment& segmentab) const {
  Vector r_a1(segmentab.GetA().GetX() - this->a_.GetX(),
              segmentab.GetA().GetY() - this->a_.GetY());
  Vector r_a2(segmentab.GetB().GetX() - this->a_.GetX(),
              segmentab.GetB().GetY() - this->a_.GetY());

  Vector napr(this->b_.GetX() - this->a_.GetX(),
              this->b_.GetY() - this->a_.GetY());

  if ((segmentab.GetA().GetX() - this->a_.GetX()) * napr.GetX() < 0) {
    return false;
  }

  return ((r_a1 ^ napr) * (r_a1 ^ r_a2) >= 0 &&
          (napr ^ r_a1) * (napr ^ r_a2) <= 0);
}

IShape* Ray::Clone() const { return new Ray(a_, b_); }
Point Ray::GetA() { return a_; }
Vector Ray::GetVector() {
  return Vector(b_.GetX() - a_.GetX(), b_.GetY() - a_.GetY());
}

Circle::Circle(Point pointa, size_t radius) : a_(pointa), r_(radius) {}
void Circle::Move(const Vector& veca) { a_.Move(veca); }
bool Circle::ContainsPoint(const Point& pointa) const {
  return (((pointa.GetX() - a_.GetX()) * (pointa.GetX() - a_.GetX()) +
           (pointa.GetY() - a_.GetY()) * (pointa.GetY() - a_.GetY())) <=
          (int64_t)(r_ * r_));
}

bool Circle::CrossSegment(const Segment& segmentab) const {
  // Вектор от центра окружности до начальной точки отрезка
  Vector vec_ao(this->a_.GetX() - segmentab.GetA().GetX(),
                this->a_.GetY() - segmentab.GetA().GetY());

  // Вектор, представляющий отрезок
  Vector vec_ab(segmentab.GetB().GetX() - segmentab.GetA().GetX(),
                segmentab.GetB().GetY() - segmentab.GetA().GetY());

  // Проверка, лежит ли начальная и конечная точки отрезка внутри окружности
  if (SquareDistanceBetweenPoints(this->a_, segmentab.GetA()) < pow(r_, 2) &&
      SquareDistanceBetweenPoints(this->a_, segmentab.GetB()) < pow(r_, 2)) {
    return false;
  }

  // Проверка на пересечение окружности и отрезка
  return pow((vec_ao ^ vec_ab), 2) <=
         pow(r_, 2) * (pow(vec_ab.GetX(), 2) + pow(vec_ab.GetY(), 2));
}

IShape* Circle::Clone() const { return new Circle(a_, r_); }
Point Circle::GetCentre() { return a_; }
size_t Circle::GetRadius() const { return r_; }

/*Двойное векторное произведение используется в геометрии для определения
ориентации трёх точек относительно друг друга. VectorWorks- не помню по какой
причине дал такое имя.*/
int64_t DoubleVectorProduct(Point first, Point second, Point thirth) {
  return ((second.GetX() - first.GetX()) * (thirth.GetY() - first.GetY()) -
          (second.GetY() - first.GetY()) * (thirth.GetX() - first.GetX()));
}
bool Intersection(int64_t point_a, int64_t point_b, int64_t point_c,
                  int64_t point_d) {
  if (point_a > point_b) {
    std::swap(point_a, point_b);
  }
  if (point_c > point_d) {
    std::swap(point_c, point_d);
  }
  return std::max(point_a, point_c) <= std::min(point_b, point_d);
}
int64_t EvaluatePointInLineEquation(int64_t&& coefficients_line_a,
                                    int64_t&& coefficients_line_b,
                                    int64_t&& coefficients_line_c,
                                    const Point& point) {
  return coefficients_line_a * point.GetX() +
         coefficients_line_b * point.GetY() + coefficients_line_c;
}

int64_t SquareDistanceBetweenPoints(Point point_a, Point point_b) {
  return pow((point_a.GetX() - point_b.GetX()), 2) +
         pow((point_a.GetY() - point_b.GetY()), 2);
}
