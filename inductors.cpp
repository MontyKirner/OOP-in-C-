//------------------------------------------------------------------------------

// Project - AC Circuits
// Monty Kirner - 14/04/21

//------------------------------------------------------------------------------
// Derived classes for ideal/non-ideal inductors:
//------------------------------------------------------------------------------

#include "inductors.hpp"

//------------------------------------------------------------------------------
// Ideal Inductor Class:
//------------------------------------------------------------------------------

// For cloning unique_ptr of component:
std::unique_ptr<component> inductor::clone() const
{
  return std::make_unique<inductor>(*this);
}

// Default constructor:
inductor::inductor()
{
  type = "inductor";
  symbol = 'L';
  inductance = 0;
  set_impedance();
}

//------------------------------------------------------------------------------

// Parameterised constructor:
inductor::inductor(const double &ind)
{
  if (ind < 0.0) {
    throw std::out_of_range{"Cannot have negative inductance."};

  } else if (ind <= 100e-15) {
    throw std::out_of_range{"Inductance must be above 100 fH."};

  } else if (ind >= 10e3) {
    throw std::out_of_range{"Inductance must be below 10 kH."};
  }

  type = "inductor";
  symbol = 'L';
  inductance = ind;
  set_impedance();
}

//------------------------------------------------------------------------------

// Copy constructor:
inductor::inductor(const inductor &induc)
{
  type = induc.type;
  symbol = induc.symbol;
  impedance = induc.impedance;
  frequency = induc.frequency;
  inductance = induc.inductance;
}

//------------------------------------------------------------------------------

// Move constructor:
inductor::inductor(inductor &&induc)
{
  // Steal the data:
  type = induc.type;
  symbol = induc.symbol;
  impedance = induc.impedance;
  frequency = induc.frequency;
  inductance = induc.inductance;

  // Empty 'old' inductor data:
  induc.type = "empty";
  induc.symbol = 'N';
  induc.impedance = 0;
  induc.frequency = 0;
  induc.inductance = 0;
}

//------------------------------------------------------------------------------

// Destructor:
inductor::~inductor() {}

//------------------------------------------------------------------------------
// Access Functions:
//------------------------------------------------------------------------------

void inductor::set_impedance()
{
  double omega = (2 * M_PI * frequency);
  impedance = std::complex<double>{0.0, (omega * inductance)};
}

//------------------------------------------------------------------------------

void inductor::set_inductance(const double &ind)
{
  if (ind < 0.0) {
    throw std::out_of_range{"Cannot have negative inductance."};

  } else if (ind <= 100e-15) {
    throw std::out_of_range{"Inductance must be above 100 fH."};

  } else if (ind >= 10e3) {
    throw std::out_of_range{"Inductance must be below 10 kH."};
  }

  inductance = ind;

  // To ensure impedance is adjusted correctly:
  set_impedance();
}

double inductor::get_inductance() const
{
  return inductance;
}

void inductor::set_value(const double &ind)
{
  if (ind < 0.0) {
    throw std::out_of_range{"Cannot have negative inductance."};

  } else if (ind <= 100e-15) {
    throw std::out_of_range{"Inductance must be above 100 fH."};

  } else if (ind >= 10e3) {
    throw std::out_of_range{"Inductance must be below 10 kH."};
  }

  inductance = ind;
  set_impedance();
}

double inductor::get_value() const
{
  return inductance;
}

//------------------------------------------------------------------------------

void inductor::set_frequency(const double &freq)
{
  frequency = freq;
  set_impedance();
}

double inductor::get_frequency() const
{
  return frequency;
}

//------------------------------------------------------------------------------

void inductor::print_info() const
{
  if (inductance <= 1e-2 || inductance >= 1e3) {
    std::cout << std::scientific;
  } else {
    std::cout << std::fixed;
  }
  std::cout << "Inductor:" << std::endl
  << "    Inductance, L = " << inductance << " H," << std::endl;
}


//------------------------------------------------------------------------------
// Non-ideal inductor Class:
//------------------------------------------------------------------------------

// For cloning unique_ptr of component:
std::unique_ptr<component> real_inductor::clone() const
{
  return std::make_unique<real_inductor> (*this);
}

// Default constructor:
real_inductor::real_inductor()
{
  type = "real inductor";
  symbol = 'l';
  inductance = 0;
  resistance = 0;
  capacitance = 0;
  set_impedance();
}

//------------------------------------------------------------------------------

// Parameterised constructor:
real_inductor::real_inductor(
  const double &res, const double &ind, const double &cap)
{
  if (ind < 0.0) {
    throw std::out_of_range{"Cannot have negative inductance."};

  } else if (ind <= 100e-15) {
    throw std::out_of_range{"Inductance must be above 100 fH."};

  } else if (ind >= 10e3) {
    throw std::out_of_range{"Inductance must be below 10 kH."};
  }

//------------------------------------------------------------------------------

  if (res <= 0.0) {
    throw std::out_of_range{"Resistance must be above 0 Ohms."};

  } else if (res >= 1.0) {
    throw std::out_of_range{"Resistance must be below 1 Ohm."};
  }

//------------------------------------------------------------------------------

  if (cap <= 0.0) {
    throw std::out_of_range{"Capacitance must be above 0 F."};

  } else if (cap >= 10e-12) {
    throw std::out_of_range{"Capacitance must be below 10 pF."};
  }

//------------------------------------------------------------------------------

  type = "real inductor";
  symbol = 'l';
  inductance = ind;
  resistance = res;
  capacitance = cap;
  set_impedance();
}

//------------------------------------------------------------------------------

// Copy constructor:
real_inductor::real_inductor(const real_inductor &induc)
{
  type = induc.type;
  symbol = induc.symbol;
  impedance = induc.impedance;
  frequency = induc.frequency;
  inductance = induc.inductance;
  resistance = induc.resistance;
  capacitance = induc.capacitance;
}

//------------------------------------------------------------------------------

// Move constructor:
real_inductor::real_inductor(real_inductor &&induc)
{
  // Steal the data:
  type = induc.type;
  symbol = induc.symbol;
  impedance = induc.impedance;
  frequency = induc.frequency;
  inductance = induc.inductance;
  resistance = induc.resistance;
  capacitance = induc.capacitance;

  // Empty 'old' real_inductor data:
  induc.type = "empty";
  induc.symbol = 'N';
  induc.impedance = 0;
  induc.frequency = 0;
  induc.inductance = 0;
  induc.resistance = 0;
  induc.capacitance = 0;
}

//------------------------------------------------------------------------------

// Destructor:
real_inductor::~real_inductor() {}

//------------------------------------------------------------------------------
// Access Functions:
//------------------------------------------------------------------------------

void real_inductor::set_impedance()
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

void real_inductor::print_info() const
{
  std::cout << "Non-ideal inductor:" << std::endl;

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