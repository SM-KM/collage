#include <iostream>
#include <string>

class Shape {
protected:
  int x;
  int y;
  std::string type;

public:
  Shape(int X, int Y, std::string t) {
    x = X;
    y = Y;
    type = t;
  };

  virtual std::string draw() = 0;
};

class Polygon : public Shape {
private:
  int n_sides;

public:
  Polygon(int x, int y, int n, std::string t) : Shape(x, y, t) { n_sides = n; };
  std::string draw() override { return "Im a: " + type; };
  Polygon();
  int get_sides() { return n_sides; };
};

class Rectangle : public Shape {
private:
  int height;
  int width;

public:
  Rectangle(int X, int Y, int h, int a, std::string t) : Shape(X, Y, t) {
    height = h;
    width = a;
  };
  std::string draw() override { return "I'm a: " + type; };
  Rectangle();
  int get_height() { return height; };
  int get_width() { return width; };
};

class Circle : public Shape {
private:
  int radius;

public:
  Circle(int X, int Y, int r, std::string t) : Shape(X, Y, t) { radius = r; };
  std::string draw() override { return "I'm a: " + type; };
  Circle();

  int get_radius() { return radius; };
};

int main() {
  Circle C1(2, 3, 18, "Circle");
  std::cout << C1.draw() << std::endl;
  std::cout << "The radius of my circle is: " << C1.get_radius() << std::endl;

  Rectangle R1(1, 2, 7, 22, "Rectangle");
  std::cout << R1.draw() << std::endl;
  std::cout << "The height is: " << R1.get_height() << std::endl;
  std::cout << "The width is: " << R1.get_width() << std::endl;

  Polygon P1(1, 2, 14, "Polygon");
  std::cout << P1.draw() << std::endl;
  std::cout << "The polygon has " << P1.get_sides() << " sides" << std::endl;

  return 0;
}
