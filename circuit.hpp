//------------------------------------------------------------------------------

// Project - AC Circuits
// Monty Kirner - 14/04/21

//------------------------------------------------------------------------------
// Derived class for circuit:
//------------------------------------------------------------------------------

#ifndef circuit_hpp
#define circuit_hpp

#include "base_component.hpp"

//------------------------------------------------------------------------------

namespace circuits
{
  class circuit : public component
  {
  private:
    double voltage;

    // Stores all components in a given circuit:
    std::vector<std::shared_ptr<component>> circuit_comps;

  public:
    // For cloning shared_ptr of circuit component:
    std::unique_ptr<component> clone() const;

    // Default constructor:
    circuit();

    // Parameterised constructor:
    circuit(const double &freq, const double &volt);

    // Copy constructor for deep copying:
    circuit(const circuit &circ);

    // Move constructor (note double &&):
    circuit(circuit &&circ);

    // Destructor:
    ~circuit();

//------------------------------------------------------------------------------

    // Calcs total impedance, need to calc each time a comp is added / removed:
    void set_impedance();

    std::complex<double> calc_series_impedance(
      const std::vector<std::shared_ptr<component>> &series_sub_circ) const;

    std::complex<double> calc_parallel_impedance(
      const std::vector<std::shared_ptr<component>> &parallel_sub_circ) const;

    // Sets / returns the frequency of the circuit:
    void set_value(const double &freq);
    double get_value() const;

    void set_frequency(const double &freq);
    double get_frequency() const;

    void set_voltage(const double &volt);
    double get_voltage() const;

    // Returns number of components in vector:
    double get_size() const;

//------------------------------------------------------------------------------

    // Prints total impedance, freq, volt details:
    void print_info() const;

    // Prints components in the circuit:
    void print_components() const;

    // Prints circuit diagram of the components:
    void print_diagram() const;

//------------------------------------------------------------------------------

    // Template to add components to circuit in series or parallel:
    template <class T> void add_component(
      std::shared_ptr<T> &comp, const char &conn, const bool &nest);

    // To remove a given component from circuit_comps:
    void remove_component(const size_t &index);
  };
}

//------------------------------------------------------------------------------
// Adding components to circuit (using templates):
//------------------------------------------------------------------------------

using namespace circuits;

// Add components to circuit in series or parallel:
template <class T> void circuit::add_component(
  std::shared_ptr<T> &comp,
  const char &conn, const bool &nest)
{
  circuit_comps.push_back(comp->clone() );

  // Set the member data for this component:
  circuit_comps.back()->set_connection_type(conn);
  circuit_comps.back()->set_nested_bool(nest);
  circuit_comps.back()->set_frequency(frequency);

  set_impedance();
}

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------