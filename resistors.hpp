//------------------------------------------------------------------------------

// Project - AC Circuits
// Monty Kirner - 14/04/21

//------------------------------------------------------------------------------
// Derived classes for ideal/non-ideal resistors:
//------------------------------------------------------------------------------

#ifndef resistor_hpp
#define resistor_hpp

#include "base_component.hpp"

//------------------------------------------------------------------------------
// Ideal Resistor Class:
//------------------------------------------------------------------------------

class resistor : public component
{
protected:
  double resistance;

public:
  // For cloning unique_ptr of resistor component:
  std::unique_ptr<component> clone() const;

  // Default constructor:
  resistor();

  // Parameterised constructor:
  resistor(const double &res);

  // Copy constructor for deep copying:
  resistor(const resistor &resis);

  // Move constructor (note double &&):
  resistor(resistor &&resis);

  // Destructor:
  ~resistor();

//------------------------------------------------------------------------------

  void set_impedance();

  void set_resistance(const double &res);
  double get_resistance() const;

  void set_value(const double &res);
  double get_value() const;

  void set_frequency(const double &freq);
  double get_frequency() const;

  void print_info() const;
};


//------------------------------------------------------------------------------
// Non-ideal Resistor Class:
//------------------------------------------------------------------------------

class real_resistor : public resistor
{
private:
  // Non-ideal resistor has very small ind/cap:
  double inductance;
  double capacitance;

public:
  // For cloning unique_ptr of real resistor component:
  std::unique_ptr<component> clone() const;

  // Default constructor:
  real_resistor();

  // Parameterised constructor:
  real_resistor(const double &res, const double &ind, const double &cap);

  // Copy constructor for deep copying:
  real_resistor(const real_resistor &real_resis);

  // Move constructor (note double &&):
  real_resistor(real_resistor &&real_resis);

  // Destructor:
  ~real_resistor();

//------------------------------------------------------------------------------

  // Different calculation for impedance:
  void set_impedance();

  void print_info() const;
};

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------