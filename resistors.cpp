//------------------------------------------------------------------------------

// Project - AC Circuits
// Monty Kirner - 14/04/21

//------------------------------------------------------------------------------
// Derived classes for ideal/non-ideal resistors:
//------------------------------------------------------------------------------

#include "resistors.hpp"

//------------------------------------------------------------------------------
// Ideal Resistor Class:
//------------------------------------------------------------------------------

// For cloning unique_ptr of component:
std::unique_ptr<component> resistor::clone() const
{
  return std::make_unique<resistor>(*this);
}

// Default constructor:
resistor::resistor() : component{}
{
  type = "resistor";
  symbol = 'R';
  resistance = 0;
  set_impedance();
}

//------------------------------------------------------------------------------

// Parameterised constructor:
resistor::resistor(const double &res)
{
  if (res < 0.0) {
    throw std::out_of_range{"Cannot have negative resistance."};

  } else if (res <= 1e-9) {
    throw std::out_of_range{"Resistance must be above 1 nOhm."};

  } else if (res >= 10e9) {
    throw std::out_of_range{"Resistance must be below 10 GOhms."};
  }

  type = "resistor";
  symbol = 'R';
  resistance = res;
  set_impedance();
}

//------------------------------------------------------------------------------

// Copy constructor:
resistor::resistor(const resistor &resis)
{
  type = resis.type;
  symbol = resis.symbol;
  impedance = resis.impedance;
  frequency = resis.frequency;
  resistance = resis.resistance;
}

//------------------------------------------------------------------------------

// Move constructor:
resistor::resistor(resistor &&resis)
{
  // Steal the data:
  type = resis.type;
  symbol = resis.symbol;
  impedance = resis.impedance;
  frequency = resis.frequency;
  resistance = resis.resistance;

  // Empty 'old' resistor data:
  resis.type = "empty";
  resis.symbol = 'N';
  resis.impedance = 0;
  resis.frequency = 0;
  resis.resistance = 0;
}

//------------------------------------------------------------------------------

// Destructor:
resistor::~resistor() {}

//------------------------------------------------------------------------------
// Access Functions:
//------------------------------------------------------------------------------

void resistor::set_impedance()
{
  impedance = resistance;
}

//------------------------------------------------------------------------------

void resistor::set_resistance(const double &res)
{
  if (res < 0.0) {
    throw std::out_of_range{"Cannot have negative resistance."};

  } else if (res <= 1e-9) {
    throw std::out_of_range{"Resistance must be above 1 nOhm."};

  } else if (res >= 10e9) {
    throw std::out_of_range{"Resistance must be below 10 GOhms."};
  }

  resistance = res;
  // To ensure impedance is adjusted correctly:
  set_impedance();
}

double resistor::get_resistance() const
{
  return resistance;
}

void resistor::set_value(const double &res)
{
  if (res < 0.0) {
    throw std::out_of_range{"Cannot have negative resistance."};

  } else if (res <= 1e-9) {
    throw std::out_of_range{"Resistance must be above 1 nOhm."};

  } else if (res >= 10e9) {
    throw std::out_of_range{"Resistance must be below 10 GOhms."};
  }

  resistance = res;
  set_impedance();
}

double resistor::get_value() const
{
  return resistance;
}

//------------------------------------------------------------------------------

void resistor::set_frequency(const double &freq)
{
  frequency = freq;
  set_impedance();
}

double resistor::get_frequency() const
{
  return frequency;
}

//------------------------------------------------------------------------------

void resistor::print_info() const
{
  if (resistance <= 1e-2 || resistance >= 1e3) {
    std::cout << std::scientific;
  } else {
    std::cout << std::fixed;
  }
  std::cout << "Resistor:" << std::endl
  << "    Resistance, R = " << resistance << " Ohms." << std::endl;
}


//------------------------------------------------------------------------------
// Non-ideal Resistor Class:
//------------------------------------------------------------------------------

// For cloning unique_ptr of component:
std::unique_ptr<component> real_resistor::clone() const
{
  return std::make_unique<real_resistor>(*this);
}

// Default constructor:
real_resistor::real_resistor()
{
  type = "real resistor";
  symbol = 'r';
  resistance = 0;
  inductance = 0;
  capacitance = 0;
  set_impedance();
}

//------------------------------------------------------------------------------

// Parameterised constructor:
real_resistor::real_resistor(
  const double &res, const double &ind, const double &cap)
{
  if (res < 0.0) {
    throw std::out_of_range{"Cannot have negative resistance."};

  } else if (res <= 1e-9) {
    throw std::out_of_range{"Resistance must be above 1 nOhm."};

  } else if (res >= 10e9) {
    throw std::out_of_range{"Resistance must be below 10 GOhms."};
  }

//------------------------------------------------------------------------------

  if (ind <= 0.0) {
    throw std::out_of_range{"Non-ideal inductance must be above 0 H."};

  } else if (ind >= 10e-9) {
    throw std::out_of_range{"Non-ideal inductance must be below 10 nH."};
  }

//------------------------------------------------------------------------------

  if (cap <= 0.0) {
    throw std::out_of_range{"Non-ideal capacitance must be above 0 F."};

  } else if (cap >= 10e-12) {
    throw std::out_of_range{"Non-ideal capacitance must be below 10 pF."};
  }

//------------------------------------------------------------------------------

  type = "real resistor";
  symbol = 'r';
  resistance = res;
  inductance = ind;
  capacitance = cap;
  set_impedance();
}

//------------------------------------------------------------------------------

// Copy constructor:
real_resistor::real_resistor(const real_resistor &resis)
{
  type = resis.type;
  symbol = resis.symbol;
  impedance = resis.impedance;
  frequency = resis.frequency;
  resistance = resis.resistance;
  inductance = resis.inductance;
  capacitance = resis.capacitance;
}

//------------------------------------------------------------------------------

// Move constructor:
real_resistor::real_resistor(real_resistor &&resis)
{
  // Steal the data:
  type = resis.type;
  symbol = resis.symbol;
  impedance = resis.impedance;
  frequency = resis.frequency;
  resistance = resis.resistance;
  inductance = resis.inductance;
  capacitance = resis.capacitance;

  // Empty 'old' real_resistor data:
  resis.type = "empty";
  resis.symbol = 'N';
  resis.impedance = 0;
  resis.frequency = 0;
  resis.resistance = 0;
  resis.inductance = 0;
  resis.capacitance = 0;
}

//------------------------------------------------------------------------------

// Destructor:
real_resistor::~real_resistor() {}

//------------------------------------------------------------------------------
// Access Functions:
//------------------------------------------------------------------------------

void real_resistor::set_impedance()
{
  // Break up into smaller calcs:
  double omega = (2 * M_PI * frequency);

  double real_a = (1 - (pow(omega, 2) * capacitance * inductance));
  double real_b = (omega * resistance * capacitance);

  double real_part = resistance / ((pow(real_a, 2) + pow(real_b, 2)));

//------------------------------------------------------------------------------

  double omega_cubed = pow(omega, 3);
  double ind_squared = pow(inductance, 2);
  double res_squared = pow(resistance, 2);

  // Separate numerator terms:
  double imag_a = (omega * inductance);
  double imag_b = (omega_cubed * capacitance * ind_squared);
  double imag_c = (omega * capacitance * res_squared);

  double imag_numerator = (imag_a + imag_b - imag_c);

//------------------------------------------------------------------------------

  double imag_d = pow(real_a, 2);
  double imag_e = pow(real_b, 2);

  double imag_denominator = (imag_d + imag_e);

  double imag_part = imag_numerator / imag_denominator;

  impedance = std::complex<double>{real_part, imag_part};
}

//------------------------------------------------------------------------------

void real_resistor::print_info() const
{
  std::cout << "Non-ideal resistor:" << std::endl;

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