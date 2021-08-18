//------------------------------------------------------------------------------

// Project - AC Circuits
// Monty Kirner - 14/04/21

//------------------------------------------------------------------------------
// Derived classes for ideal/non-ideal capacitors:
//------------------------------------------------------------------------------

#include "capacitors.hpp"

//------------------------------------------------------------------------------
// Ideal Capacitor Class:
//------------------------------------------------------------------------------

// For cloning unique_ptr of component:
std::unique_ptr<component> capacitor::clone() const
{
  return std::make_unique<capacitor>(*this);
}

// Default constructor:
capacitor::capacitor()
{
  type = "capacitor";
  symbol = 'C';
  capacitance = 0;
  set_impedance();
}

//------------------------------------------------------------------------------

// Parameterised constructor:
capacitor::capacitor(const double &cap)
{
  if (cap < 0.0) {
    throw std::out_of_range{"Cannot have negative capacitance."};

  } else if (cap <= 1e-12) {
    throw std::out_of_range{"Capacitance must be above 1 pF."};

  } else if (cap >= 1e3) {
    throw std::out_of_range{"Capacitance must be below 1 kF."};
  }

  type = "capacitor";
  symbol = 'C';
  capacitance = cap;
  set_impedance();
}

//------------------------------------------------------------------------------

// Copy constructor:
capacitor::capacitor(const capacitor &capac)
{
  type = capac.type;
  symbol = capac.symbol;
  impedance = capac.impedance;
  frequency = capac.frequency;
  capacitance = capac.capacitance;
}

//------------------------------------------------------------------------------

// Move constructor:
capacitor::capacitor(capacitor &&capac)
{
  // Steal the data:
  type = capac.type;
  symbol = capac.symbol;
  impedance = capac.impedance;
  frequency = capac.frequency;
  capacitance = capac.capacitance;

  // Empty 'old' capacitor data:
  capac.type = "empty";
  capac.symbol = 'N';
  capac.impedance = 0;
  capac.frequency = 0;
  capac.capacitance = 0;
}

//------------------------------------------------------------------------------

// Destructor:
capacitor::~capacitor() {}

//------------------------------------------------------------------------------
// Access Functions:
//------------------------------------------------------------------------------

void capacitor::set_impedance()
{
  double omega = (2 * M_PI * frequency);
  impedance = std::complex<double>{0.0, (-1.0 / (omega * capacitance))};
}

//------------------------------------------------------------------------------

void capacitor::set_capacitance(const double &cap)
{
  if (cap < 0.0) {
    throw std::out_of_range{"Cannot have negative capacitance."};

  } else if (cap <= 1e-12) {
    throw std::out_of_range{"Capacitance must be above 1 pF."};

  } else if (cap >= 1e3) {
    throw std::out_of_range{"Capacitance must be below 1 kF."};
  }

  capacitance = cap;
  // To ensure impedance is adjusted correctly:
  set_impedance();
}

double capacitor::get_capacitance() const
{
  return capacitance;
}

void capacitor::set_value(const double &cap)
{
  if (cap < 0.0) {
    throw std::out_of_range{"Cannot have negative capacitance."};

  } else if (cap <= 1e-12) {
    throw std::out_of_range{"Capacitance must be above 1 pF."};

  } else if (cap >= 1e3) {
    throw std::out_of_range{"Capacitance must be below 1 kF."};
  }

  capacitance = cap;
  set_impedance();
}

double capacitor::get_value() const
{
  return capacitance;
}

//------------------------------------------------------------------------------

void capacitor::set_frequency(const double &freq)
{
  frequency = freq;
  set_impedance();
}

double capacitor::get_frequency() const
{
  return frequency;
}

//------------------------------------------------------------------------------

void capacitor::print_info() const
{
  if (capacitance <= 1e-2 || capacitance >= 1e3) {
    std::cout << std::scientific;

  } else {
    std::cout << std::fixed;
  }
  std::cout << "Capacitor:" << std::endl
  << "    Capacitance, C = " << capacitance << " F." << std::endl;
}


//------------------------------------------------------------------------------
// Non-ideal Capacitor Class:
//------------------------------------------------------------------------------

// For cloning unique_ptr of component:
std::unique_ptr<component> real_capacitor::clone() const
{
  return std::make_unique<real_capacitor>(*this);
}

// Default constructor:
real_capacitor::real_capacitor()
{
  type = "real capacitor";
  symbol = 'c';
  capacitance = 0;
  resistance = 0;
  inductance = 0;
  set_impedance();
}

//------------------------------------------------------------------------------

// Parameterised constructor:
real_capacitor::real_capacitor(
  const double &res, const double &ind, const double &cap)
{
  if (cap < 0.0) {
    throw std::out_of_range{"Cannot have negative capacitance."};

  } else if (cap <= 1e-12) {
    throw std::out_of_range{"Capacitance must be above 1 pF."};

  } else if (cap >= 1e3) {
    throw std::out_of_range{"Capacitance must be below 1 kF."};
  }

//------------------------------------------------------------------------------

  if (res <= 0.0) {
    throw std::out_of_range{"Resistance must be above 0 Ohms."};

  } else if (res >= 1.0) {
    throw std::out_of_range{"Resistance must be below 1 Ohm."};
  }

//------------------------------------------------------------------------------

  if (ind <= 0.0) {
    throw std::out_of_range{"Inductance must be above 0 H."};

  } else if (ind >= 10e-9) {
    throw std::out_of_range{"Inductance must be below 10 nH."};
  }

//------------------------------------------------------------------------------

  type = "real capacitor";
  symbol = 'c';
  capacitance = cap;
  resistance = res;
  inductance = ind;
  set_impedance();
}

//------------------------------------------------------------------------------

// Copy constructor:
real_capacitor::real_capacitor(const real_capacitor &capac)
{
  type = capac.type;
  symbol = capac.symbol;
  impedance = capac.impedance;
  frequency = capac.frequency;
  capacitance = capac.capacitance;
  resistance = capac.resistance;
  inductance = capac.inductance;
}

//------------------------------------------------------------------------------

// Move constructor:
real_capacitor::real_capacitor(real_capacitor &&capac)
{
  // Steal the data:
  type = capac.type;
  symbol = capac.symbol;
  impedance = capac.impedance;
  frequency = capac.frequency;
  capacitance = capac.capacitance;
  resistance = capac.resistance;
  inductance = capac.inductance;

  // Empty 'old' real_capacitor data:
  capac.type = "empty";
  capac.symbol = 'N';
  capac.impedance = 0;
  capac.frequency = 0;
  capac.capacitance = 0;
  capac.resistance = 0;
  capac.inductance = 0;
}

//------------------------------------------------------------------------------

// Destructor:
real_capacitor::~real_capacitor() {}

//------------------------------------------------------------------------------
// Access Functions:
//------------------------------------------------------------------------------

void real_capacitor::set_impedance()
{
  // Break up into smaller calcs:
  double omega = (2 * M_PI * frequency);
  double fraction = (1 / (omega * capacitance));
  double imag_part = (omega * inductance) - fraction;

  impedance = std::complex<double>{resistance, imag_part};
}

//------------------------------------------------------------------------------

void real_capacitor::print_info() const
{
  std::cout << "Non-ideal capacitor:" << std::endl;

  // Converts to scientific notation:
  if (resistance <= 1e-2 || resistance >= 1e3) {
    std::cout << std::scientific;

  } else {
    std::cout << std::fixed;
  }

  std::cout << "    Resistance, R = " << resistance << " Ohms," << std::endl;

  if (inductance <= 1e-2 || inductance >= 1e3) {
    std::cout << std::scientific;

  } else {
    std::cout << std::fixed;
  }

  std::cout << "    Inductance, L = " << inductance << " H," << std::endl;

  if (capacitance <= 1e-2 || capacitance >= 1e3) {
    std::cout << std::scientific;

  } else {
    std::cout << std::fixed;
  }

  std::cout << "    Capacitance, C = " << capacitance << " F." << std::endl;
}

//------------------------------------------------------------------------------