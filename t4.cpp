// Santiago Vargas Rugeles / A01647287
// Andres Ramirez Ruiz / A01647072

#include <stdexcept>
#include <string>

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

int main(int argc, char *argv[]) { return 0; }
