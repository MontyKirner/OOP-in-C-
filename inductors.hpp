//------------------------------------------------------------------------------

// Project - AC Circuits
// Monty Kirner - 14/04/21

//------------------------------------------------------------------------------
// Derived classes for ideal/non-ideal inductors:
//------------------------------------------------------------------------------

#ifndef inductor_hpp
#define inductor_hpp

#include "base_component.hpp"

//------------------------------------------------------------------------------
// Ideal Inductor Class:
//------------------------------------------------------------------------------

class inductor : public component
{
protected:
  double inductance;

public:
  // For cloning unique_ptr of inductor component:
  std::unique_ptr<component> clone() const;

  // Default constructor:
  inductor();

  // Parameterised constructor:
  inductor(const double &ind);

  // Copy constructor for deep copying:
  inductor(const inductor &induc);

  // Move constructor (note double &&):
  inductor(inductor &&induc);

  // Destructor:
  ~inductor();

//------------------------------------------------------------------------------

  void set_impedance();

  void set_inductance(const double &ind);
  double get_inductance() const;

  void set_value(const double &ind);
  double get_value() const;

  void set_frequency(const double &freq);
  double get_frequency() const;

  void print_info() const;
};


//------------------------------------------------------------------------------
// Non-ideal Inductor Class:
//------------------------------------------------------------------------------

class real_inductor : public inductor
{
private:
  // Non-ideal inductor has very small res/cap:
  double resistance;
  double capacitance;

public:
  // For cloning unique_ptr of real inductor component:
  std::unique_ptr<component> clone() const;

  // Default constructor:
  real_inductor();

  // Parameterised constructor:
  real_inductor(const double &res, const double &ind, const double &cap);

  // Copy constructor for deep copying:
  real_inductor(const real_inductor &real_induc);

  // Move constructor (note double &&):
  real_inductor(real_inductor &&real_induc);

  // Destructor:
  ~real_inductor();

//------------------------------------------------------------------------------

  // Different calculation for impedance:
  void set_impedance();

  void print_info() const;
};

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------