//------------------------------------------------------------------------------

// Project - AC Circuits
// Monty Kirner - 14/04/21

//------------------------------------------------------------------------------
// Abstract base class for components (used as interface only):
//------------------------------------------------------------------------------

#include "base_component.hpp"

//------------------------------------------------------------------------------

std::string component::get_type() const
{
  return type;
}

// For each type of component:
char component::get_symbol() const
{
  return symbol;
}

void component::set_connection_type(const char &conn)
{
  connection_type = conn;
}

char component::get_connection_type() const
{
  return connection_type;
}

void component::set_nested_bool(const bool &nest)
{
  is_nested = nest;
}

bool component::get_nested_bool() const
{
  return is_nested;
}

//------------------------------------------------------------------------------

std::complex<double> component::get_impedance() const
{
  return impedance;
}

double component::get_magnitude() const
{
  return std::abs(impedance);
}

double component::get_phase() const
{
  return std::arg(impedance);
}

//------------------------------------------------------------------------------