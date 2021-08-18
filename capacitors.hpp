//------------------------------------------------------------------------------

// Project - AC Circuits
// Monty Kirner - 14/04/21

//------------------------------------------------------------------------------
// Derived classes for ideal/non-ideal capacitors:
//------------------------------------------------------------------------------

#ifndef capacitor_hpp
#define capacitor_hpp

#include "base_component.hpp"

//------------------------------------------------------------------------------
// Ideal Capacitor Class:
//------------------------------------------------------------------------------

class capacitor : public component
{
protected:
  double capacitance;

public:
  // For cloning unique_ptr of capacitor component:
  std::unique_ptr<component> clone() const;

  // Default constructor:
  capacitor();

  // Parameterised constructor:
  capacitor(const double &cap);

  // Copy constructor for deep copying:
  capacitor(const capacitor &capac);

  // Move constructor (note double &&):
  capacitor(capacitor &&capac);

  // Destructor:
  ~capacitor();

//------------------------------------------------------------------------------

  void set_impedance();

  void set_capacitance(const double &cap);
  double get_capacitance() const;

  void set_value(const double &cap);
  double get_value() const;

  void set_frequency(const double &freq);
  double get_frequency() const;

  void print_info() const;
};


//------------------------------------------------------------------------------
// Non-ideal Capacitor Class:
//------------------------------------------------------------------------------

class real_capacitor : public capacitor
{
private:
  // Non-ideal capacitor has very small res/ind:
  double resistance;
  double inductance;

public:
  // For cloning unique_ptr of real capacitor component:
  std::unique_ptr<component> clone() const;

  // Default constructor:
  real_capacitor();

  // Parameterised constructor:
  real_capacitor(const double &res, const double &ind, const double &cap);

  // Copy constructor for deep copying:
  real_capacitor(const real_capacitor &real_capac);

  // Move constructor (note double &&):
  real_capacitor(real_capacitor &&real_capac);

  // Destructor:
  ~real_capacitor();

//------------------------------------------------------------------------------

  // Different calculation for impedance:
  void set_impedance();

  void print_info() const;
};

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------