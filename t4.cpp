// Santiago Vargas Rugeles / A01647287
// Andres Ramirez Ruiz / A01647072

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

// 1

struct Address {
  std::string roadName;
};

struct ShippingInfo {
  std::string name;
  std::string city;
  std::string state;
  int postalCode;
  Address Address;
};

class Shipping {
protected:
  ShippingInfo Sender;
  ShippingInfo Recipient;
  double ShipmentCost;

public:
  Shipping(ShippingInfo sender, ShippingInfo recipient, double shipmentCost)
      : Sender(sender), Recipient(recipient), ShipmentCost(shipmentCost) {}

  virtual double stimateCost() const { return ShipmentCost; }
  virtual ~Shipping() {};
};

class Envelope : Shipping {
private:
  double large, wide;
  double extraCost;

public:
  Envelope(double large, double wide, double extraCost, ShippingInfo sender,
           ShippingInfo recipient, double shipmentCost)
      : large(large), wide(wide), extraCost(extraCost),
        Shipping(sender, recipient, shipmentCost) {};

  double stimateCost() const override {
    return (large > 25.0 || wide > 30.0) ? ShipmentCost + extraCost
                                         : ShipmentCost;
  };
};

class Package : Shipping {
private:
  double large, wide, deep;
  double weightKg;
  double costKg;

public:
  Package(double large, double wide, double deep, double weightKg,
          double costKg, ShippingInfo sender, ShippingInfo recipient,
          double shipmentCost)
      : large(large), wide(wide), deep(deep), weightKg(weightKg),
        costKg(costKg), Shipping(sender, recipient, shipmentCost) {

    // Verify the weight and the costKg as positive
    if (weightKg <= 0.0 || costKg <= 0.0) {
      throw std::invalid_argument(
          "Hey the weightKg or the costKg is less or equal to 0.0");
    }
  }

  double stimateCost() const override {
    return (weightKg * costKg) + ShipmentCost;
  }
};

// 2
class Employee {
protected:
  std::string Name;
  int Age;

public:
  Employee(std::string name, int age) : Name(name), Age(age) {};
  virtual double calcSalary() const = 0;

  bool operator<(const Employee &other) { return this->Age < other.Age; }
  bool operator>(const Employee &other) { return this->Age > other.Age; }
  bool operator==(const Employee &other) { return this->Age == other.Age; }

  bool salaryGreaterThan(const Employee &other) {
    return this->calcSalary() > other.calcSalary();
  };
  bool salaryLessThan(const Employee &other) {
    return this->calcSalary() < other.calcSalary();
  };

  bool salaryEqual(const Employee &other) {
    return this->calcSalary() == other.calcSalary();
  }
};

class BaseEmployee : public Employee {
private:
  double MontlySalary;

public:
  BaseEmployee(std::string name, int age, double salary)
      : MontlySalary(salary), Employee(name, age) {};

  double calcSalary() const override { return MontlySalary; }
};

class ByHourEmployee : public Employee {
private:
  int workedHours;
  double payPerHour;

public:
  ByHourEmployee(std::string name, int age, int workedHours, double payPerHour)
      : workedHours(workedHours), payPerHour(payPerHour),
        Employee(name, age) {};

  double calcSalary() const override { return payPerHour * workedHours; };
};

class ByCommisionEmployee : public Employee {
private:
  double baseSalary;
  double sales;
  double commission;

public:
  ByCommisionEmployee(std::string name, int age, double baseSalary,
                      double sales, double commission)
      : baseSalary(baseSalary), sales(sales), commission(commission),
        Employee(name, age) {};

  double calcSalary() const override {
    return baseSalary + sales * commission;
  };
};

// 3

class Point2D {
protected:
  double x, y;

public:
  Point2D(double x, double y) : x(x), y(y) {}
  double distance(const Point2D &other) const {
    double dx = x - other.x;
    double dy = y - other.y;

    return std::sqrt(dx * dx + dy * dy);
  }

  Point2D operator+(const Point2D &other) const {
    return Point2D(x + other.x, y + other.y);
  }

  void show() { std::cout << x << " " << y << std::endl; }
};

class Point3D : Point2D {

private:
  double z;

public:
  Point3D(double x, double y, double z) : z(z), Point2D(x, y) {};
  double distance(const Point3D &other) const {
    double dx = x - other.x;
    double dy = y - other.y;
    double dz = z - other.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
  }

  Point3D operator+(const Point3D &other) {
    return Point3D(x + other.x, y + other.y, z + other.z);
  };

  void show() { std::cout << x << " " << y << " " << z << std::endl; }
};

int main(int argc, char *argv[]) {

  std::cout << "Testing secction 1 --------------------------------"
            << std::endl;
  std::cout << "\n";

  ShippingInfo sender = {"Alice", "New York", "NY", 10001, {"Main St"}};
  ShippingInfo recipient = {"Bob", "Los Angeles", "CA", 90001, {"Second St"}};

  Envelope env1(20.0, 25.0, 5.0, sender, recipient, 10.0);
  Envelope env2(26.0, 31.0, 5.0, sender, recipient, 10.0);

  std::cout << "Envelope 1 cost (no extra): " << env1.stimateCost()
            << std::endl;
  std::cout << "Envelope 2 cost (with extra): " << env2.stimateCost()
            << std::endl;

  try {
    Package pkg1(10.0, 10.0, 10.0, 2.0, 3.0, sender, recipient, 20.0);
    std::cout << "Package cost: " << pkg1.stimateCost() << std::endl;
  } catch (const std::invalid_argument &e) {
    std::cout << "Package creation failed: " << e.what() << std::endl;
  }

  try {
    Package pkg2(10.0, 10.0, 10.0, 0.0, 3.0, sender, recipient, 20.0);
  } catch (const std::invalid_argument &e) {
    std::cout << "Expected error for invalid package weight: " << e.what()
              << std::endl;
  }

  std::cout << "\n";
  std::cout << "Testing secction 2 --------------------------------"
            << std::endl;
  std::cout << "\n";

  BaseEmployee e1("John", 30, 3000);
  ByHourEmployee e2("Jane", 25, 160, 20);
  ByCommisionEmployee e3("Doe", 40, 1500, 5000, 0.1);

  std::cout << "John's salary: " << e1.calcSalary() << std::endl;
  std::cout << "Jane's salary: " << e2.calcSalary() << std::endl;
  std::cout << "Doe's salary: " << e3.calcSalary() << std::endl;

  std::cout << "Is John older than Jane? " << (e1 > e2) << std::endl;
  std::cout << "Does Jane earn more than Doe? " << e2.salaryGreaterThan(e3)
            << std::endl;

  std::cout << "\n";
  std::cout << "Testing secction 3 --------------------------------"
            << std::endl;
  std::cout << "\n";

  Point2D p1(1.0, 2.0);
  Point2D p2(4.0, 6.0);
  Point2D p3 = p1 + p2;

  std::cout << "Distance p1-p2: " << p1.distance(p2) << std::endl;
  std::cout << "p3 coordinates: ";
  p3.show();
  Point3D q1(1.0, 2.0, 3.0);
  Point3D q2(4.0, 6.0, 3.0);
  Point3D q3 = q1 + q2;

  std::cout << "Distance q1-q2: " << q1.distance(q2) << std::endl;
  std::cout << "q3 coordinates: ";
  q3.show();
  std::cout << "\n";
}
