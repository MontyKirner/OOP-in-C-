//------------------------------------------------------------------------------

// Project - AC Circuits
// Monty Kirner - 14/04/21

//------------------------------------------------------------------------------
// Abstract base class for components (used as interface only):
//------------------------------------------------------------------------------

#ifndef base_component_hpp
#define base_component_hpp

#include <iostream>
#include <iomanip>
#include <complex>
#include<vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <exception>
#include <memory>

//------------------------------------------------------------------------------

class component
{
protected:
  std::string type;
  std::complex<double> impedance;
  double frequency;
  // Series (s) or parallel (p):
  char connection_type;

  // Is the component nested:
  bool is_nested = false;
  char symbol;

public:
  // Returns a unique pointer to the component itself:
  virtual std::unique_ptr<component> clone() const = 0;

  // Virtual destructor:
  virtual ~component() {}

//------------------------------------------------------------------------------

  // PVFs to set / get values:

  // For cases where other data members will affect impedance value:
  virtual void set_impedance() = 0;

  // value refers to either the resis / induc / capac (and for non-ideal):
  virtual void set_value(const double &value) = 0;

  // Returns either resistance, capacitance or inductance:
  virtual double get_value() const = 0;

  virtual void set_frequency(const double &freq) = 0;
  virtual double get_frequency() const = 0;

  // PVF to print info of given component:
  virtual void print_info() const = 0;

//------------------------------------------------------------------------------

  // Returns name of component:
  std::string get_type() const;

  // Returns symbol of component (diagram):
  char get_symbol() const;

  void set_connection_type(const char &conn);

  // Returns type of connection e.g. s or p:
  char get_connection_type() const;

  void set_nested_bool(const bool &nest);

  // Returns true if component is nested:
  bool get_nested_bool() const;

//------------------------------------------------------------------------------

  // Returns impedance of component:
  std::complex<double> get_impedance() const;

  // Returns magnitude or phase difference of component:
  double get_magnitude() const;
  double get_phase() const;
};

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------