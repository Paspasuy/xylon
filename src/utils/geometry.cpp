#ifndef XYLON_GEOMETRY_CPP
#define XYLON_GEOMETRY_CPP

#include <algorithm>
#include <cmath>
#include <vector>

namespace geom {
    const float EPS = 1e-6;
    const int INF = 10000007;

    class Line;

    struct Point {
        float x;
        float y;

        float dot(const Point& pt) const { return x * pt.x + y * pt.y; }

        float cross(const Point& pt) const { return x * pt.y - y * pt.x; }

        Point rotate(const Point& center, float angle) const;
        Point reflect(const Point& center) const;
        Point reflect(const Line& axis) const;
        Point scale(const Point& center, float coefficient) const;

        float len() const { return sqrt(x * x + y * y); }

        float dist(const Point& pt) const;
        Point norm() const;

        Point(float x, float y) : x(x), y(y) {}
    };

    bool operator==(const Point& p1, const Point& p2) {
        return fabs(p1.x - p2.x) < EPS && fabs(p1.y - p2.y) < EPS;
    }

    bool operator!=(const Point& p1, const Point& p2) { return !(p1 == p2); }

    Point operator+(const Point& p1, const Point& p2) { return Point{p1.x + p2.x, p1.y + p2.y}; }

    Point& operator+=(Point& p1, const Point& p2) {
        p1.x += p2.x;
        p1.y += p2.y;
        return p1;
    }

    Point operator-(const Point& pt) { return Point{-pt.x, -pt.y}; }

    Point operator-(const Point& p1, const Point& p2) { return p1 + (-p2); }

    Point operator*(const Point& pt, float k) { return Point(pt.x * k, pt.y * k); }

    Point operator/(const Point& pt, float k) { return Point(pt.x / k, pt.y / k); }

    Point Point::norm() const {
        float l = len();
        return Point(x / l, y / l);
    };

    float Point::dist(const Point& pt) const { return (*this - pt).len(); }

    Point Point::rotate(const Point& center, float angle) const {
        angle *= M_PI / 180;
        float sn = sin(angle), cs = cos(angle);
        Point vec = *this - center;
        return center + Point(vec.x * cs - vec.y * sn, vec.y * cs + vec.x * sn);
    }

    Point Point::scale(const Point& center, float coefficient) const {
        Point vec = *this - center;
        return center + vec * coefficient;
    }

    Point Point::reflect(const Point& center) const { return center * 2 - *this; }

    bool collinear(const Point& p1, const Point& p2, const Point& p3) {
        return (p2 - p1).cross(p3 - p1) == 0;
    }

    class Line {
        float a, b, c;

    public:
        Line(const Point& p1, const Point& p2)
            : a(p1.y - p2.y), b(p2.x - p1.x), c(-p1.x * a - p1.y * b) {
            float len = sqrt(a * a + b * b);
            a /= len;
            b /= len;
            c /= len;
        }

        Line(float angle, float shift) : Line(Point{0, shift}, Point{1, shift + angle}) {}

        Line(const Point& pt, float angle) : Line(pt, Point{pt.x + 1, pt.y + angle}) {}

        float get_a() const { return a; }

        float get_b() const { return b; }

        float get_c() const { return c; }

        bool operator==(const Line& ln) const {
            return isParallel(ln) && fabs(ln.b * c - ln.c * b) < EPS &&
                   fabs(ln.a * c - ln.c * a) < EPS;
            // return collinear(p1, p2, ln.p1) && collinear(p1, p2, ln.p2);
        }

        bool isParallel(const Line& ln) const { return fabs(ln.a * b - ln.b * a) < EPS; }

        float getDistance(const Point& pt) const { return fabs(a * pt.x + b * pt.y + c); }

        Point intersect(const Line& ln) const {
            float zn = a * ln.b - b * ln.a;
            return Point(-(c * ln.b - b * ln.c) / zn, -(a * ln.c - c * ln.a) / zn);
        }
    };

    bool operator!=(const Line& p1, const Line& p2) { return !(p1 == p2); }

    Point Point::reflect(const Line& axis) const {
        float a = axis.get_a();
        float b = axis.get_b();
        float c = axis.get_c();
        float signed_dist = a * x + b * y + c;
        Point normal(a, b);
        return *this - normal * signed_dist * 2;
    }

    bool intersect1d(float a, float b, float c, float d) {
        if (a > b) std::swap(a, b);
        if (c > d) std::swap(c, d);
        return std::max(a, c) < std::min(b, d) + EPS;
    }

    bool insideLine(const Line& ln, const Point& pt) { return ln.getDistance(pt) < EPS; }

    bool insideSegment(const Point& p1, const Point& p2, const Point& pt) {
        return (p1 - pt).dot(p2 - pt) < EPS;
    }

    bool checkSegmentIntersect(const Point& p1, const Point& p2, const Point& q1, const Point& q2) {
        if (!intersect1d(p1.x, p2.x, q1.x, q2.x) || !intersect1d(p1.y, p2.y, q1.y, q2.y))
            return false;
        Line l1 = Line(p1, p2);
        Line l2 = Line(q1, q2);
        if (l1.isParallel(l2)) return l1 == l2;
        Point pt = l1.intersect(l2);
        return insideSegment(p1, p2, pt) && insideSegment(q1, q2, pt);
    }

    Line orthoThroughPoint(std::pair<Point, Point> ln, const Point& pt) {
        Point vec = (ln.first - ln.second).rotate(Point(0, 0), 90);
        return Line(pt, pt + vec);
    }

    class Shape {
    public:
        virtual float perimeter() const = 0;
        virtual float area() const = 0;
        virtual bool operator==(const Shape& another) const = 0;
        virtual bool isCongruentTo(const Shape& another) const = 0;
        virtual bool isSimilarTo(const Shape& another) const = 0;
        virtual bool containsPoint(const Point& pt) const = 0;
        virtual void rotate(const Point& center, float angle) = 0;
        virtual void reflect(const Point& center) = 0;
        virtual void reflect(const Line& axis) = 0;
        virtual void scale(const Point& center, float coefficient) = 0;

        virtual ~Shape() {}
    };

    class Polygon : public Shape {
    protected:
        std::vector<Point> vertices;

        bool internalCongruent(Polygon& another) const {
            size_t sz = vertices.size();
            for (size_t sh = 0; sh < sz; ++sh) {
                bool ok = true;
                for (size_t it = 0; it < sz; ++it) {
                    const Point& p1 = another.vertices[(sh + it) % sz];
                    const Point& p2 = another.vertices[(sh + it + 1) % sz];
                    const Point& q1 = vertices[it];
                    const Point& q2 = vertices[(it + 1) % sz];
                    if (fabs((p2 - p1).len() - (q2 - q1).len()) >= EPS) {
                        ok = false;
                        break;
                    }
                    const Point& p3 = another.vertices[(sh + it + 2) % sz];
                    const Point& q3 = vertices[(it + 2) % sz];
                    if (!(fabs((p2 - p1).dot(p3 - p1) - (q2 - q1).dot(q3 - q1)) < EPS &&
                          fabs((p2 - p1).cross(p3 - p1) - (q2 - q1).cross(q3 - q1)) < EPS)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) return true;
            }
            return false;
        }

        void transpose(const Point& vec) {
            for (size_t it = 0; it < vertices.size(); ++it) {
                vertices[it] += vec;
            }
        }

        float maxSide() const {
            float res = 0;
            for (size_t it = 0; it < vertices.size(); ++it) {
                res = std::max(res, (vertices[it] - vertices[(it + 1) % vertices.size()]).len());
            }
            return res;
        }

        size_t intersectCount(const Point& p1, const Point& p2) const {
            size_t res = 0;
            size_t sz = vertices.size();
            for (size_t it = 0; it < sz; ++it) {
                const Point& q1 = vertices[it];
                const Point& q2 = vertices[(it + 1) % sz];
                if (checkSegmentIntersect(p1, p2, q1, q2)) ++res;
            }
            return res;
        }

    public:
        Polygon(std::vector<Point> vertices) : vertices(vertices) {}

        Polygon() {}

        template <typename... Point>
        Polygon(const Point&... points) : vertices{points...} {}

        Polygon(std::initializer_list<Point> vertices) : vertices(vertices) {}

        float perimeter() const override {
            float res = 0;
            for (size_t it = 0; it < vertices.size(); ++it) {
                res += (vertices[it] - vertices[(it + 1) % vertices.size()]).len();
            }
            return res;
        }

        float area() const override {
            float res = 0;
            Point beg = Point(0, 0);
            for (size_t it = 0; it < vertices.size(); ++it) {
                res += (vertices[it] - beg).cross(vertices[(it + 1) % vertices.size()] - beg);
            }
            return fabs(res) / 2;
        }

        size_t verticesCount() const { return vertices.size(); }

        std::vector<Point> getVertices() const { return vertices; }

        bool isConvex() const {
            bool negative = false;
            bool positive = false;
            for (size_t it = 0; it < vertices.size(); ++it) {
                const Point& p1 = vertices[it];
                const Point& p2 = vertices[(it + 1) % vertices.size()];
                const Point& p3 = vertices[(it + 2) % vertices.size()];
                if ((p2 - p1).cross(p3 - p1) > 0) {
                    positive = true;
                } else {
                    negative = true;
                }
            }
            return !(positive && negative);
        }

        void rotate(const Point& center, float angle) override {
            for (size_t it = 0; it < vertices.size(); ++it) {
                vertices[it] = vertices[it].rotate(center, angle);
            }
        }

        void reflect(const Point& center) override { rotate(center, 180); }

        void reflect(const Line& axis) override {
            for (size_t it = 0; it < vertices.size(); ++it) {
                vertices[it] = vertices[it].reflect(axis);
            }
        }

        void scale(const Point& center, float coefficient) override {
            for (size_t it = 0; it < vertices.size(); ++it) {
                vertices[it] = vertices[it].scale(center, coefficient);
            }
        }

        bool isCongruentTo(const Shape& another) const override {
            const Polygon* polyptr = dynamic_cast<const Polygon*>(&another);
            if (polyptr == nullptr) return false;
            Polygon poly = *polyptr;
            size_t sz = vertices.size();
            if (poly.vertices.size() != sz) return false;
            if (internalCongruent(poly)) return true;
            reverse(poly.vertices.begin(), poly.vertices.end());
            if (internalCongruent(poly)) return true;
            reverse(poly.vertices.begin(), poly.vertices.end());
            poly.reflect(Line(Point(0, 0), Point(0, 1)));
            if (internalCongruent(poly)) return true;
            reverse(poly.vertices.begin(), poly.vertices.end());
            if (internalCongruent(poly)) return true;
            return false;
        }

        bool isSimilarTo(const Shape& another) const override {
            const Polygon* polyptr = dynamic_cast<const Polygon*>(&another);
            if (polyptr == nullptr) return false;
            Polygon poly = *polyptr;
            size_t sz = vertices.size();
            if (poly.vertices.size() != sz) return false;
            float mx1 = maxSide();
            float mx2 = poly.maxSide();
            poly.scale(Point(0, 0), mx1 / mx2);
            return isCongruentTo(poly);
        }

        bool containsPoint(const Point& pt) const override {
            for (size_t it = 0; it < vertices.size(); ++it) {
                const Point& p1 = vertices[it];
                const Point& p2 = vertices[(it + 1) % vertices.size()];
                if (insideLine(Line(p1, p2), pt) && insideSegment(p1, p2, pt)) return true;
            }
            Point remote(INF, 3);
            return (intersectCount(pt, remote) % 2 == 1);
        }

        bool operator==(const Polygon& another) const {
            size_t sz = vertices.size();
            if (another.vertices.size() != sz) return false;
            for (size_t sh = 0; sh < sz; ++sh) {
                bool ok = true, okr = true;
                for (size_t it = 0; it < sz; ++it) {
                    if (another.vertices[(it + sh) % sz] != vertices[it]) {
                        ok = false;
                    }
                    if (another.vertices[(it + sh) % sz] != vertices[sz - it - 1]) {
                        okr = false;
                    }
                }
                if (ok || okr) return true;
            }
            return false;
        }

        bool operator==(const Shape& another) const override {
            const Polygon* poly = dynamic_cast<const Polygon*>(&another);
            if (poly == nullptr) return false;
            return *poly == *this;
        }

        Point& operator[](size_t index) { return vertices[index]; }

        const Point& operator[](size_t index) const { return vertices[index]; }

        ~Polygon() {}
    };

    class Rectangle : public Polygon {
    public:
        Rectangle(Point p1, Point p2, Point p3, Point p4) : Polygon(p1, p2, p3, p4) {}

        float area() const override {
            float res = vertices[0].dist(vertices[1]) * vertices[1].dist(vertices[2]);
            return res;
        }

        Point center() const { return (vertices[0] + vertices[2]) / 2; }

        std::pair<Line, Line> diagonlas() const {
            return {Line(vertices[0], vertices[2]), Line(vertices[1], vertices[3])};
        }

        Rectangle(Point p1, Point p2, float k) {
            if (k > 1) k = 1 / k;
            vertices.push_back(p1);
            vertices.push_back(p1 + ((p2 - p1) * k * sqrt(1 / (k * k + 1)))
                                        .rotate(Point(0, 0), 90 - 180 / M_PI * atan(k)));
            vertices.push_back(p2);
            vertices.push_back(center() * 2 - vertices[1]);
        }
    };
}  // namespace geom
#endif  // XYLON_GEOMETRY_CPP
