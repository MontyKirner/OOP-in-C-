//------------------------------------------------------------------------------

// Project - AC Circuits
// Monty Kirner - 14/04/21

//------------------------------------------------------------------------------
// Derived class for circuit:
//------------------------------------------------------------------------------

#include "circuit.hpp"
#include "resistors.hpp"
#include "capacitors.hpp"
#include "inductors.hpp"

//------------------------------------------------------------------------------
// Constructors and destructors:
//------------------------------------------------------------------------------

// For cloning shared_ptr of component:
std::unique_ptr<component> circuit::clone() const
{
  return std::make_unique<circuit>(*this);
}

// Default constructor:
circuit::circuit()
{
  type = "circuit";
  symbol = '~';
  frequency = 0;
  voltage = 0;
}

//------------------------------------------------------------------------------

// Parameterised constructor implementation:
circuit::circuit(const double &freq, const double &volt)
{
  if (freq < 0.0) {
    throw std::out_of_range{"Cannot have negative frequency."};
  }

  if (volt < 0.0) {
    throw std::out_of_range{"Cannot have negative voltage."};
  }

  type = "circuit";
  symbol = '~';
  frequency = freq;
  voltage = volt;
}

//------------------------------------------------------------------------------

// Copy constructor:
circuit::circuit(const circuit &circ)
{
  type = circ.type;
  symbol = circ.symbol;
  impedance = circ.impedance;
  frequency = circ.frequency;
  voltage = circ.voltage;
  circuit_comps = circ.circuit_comps;
}

//------------------------------------------------------------------------------

// Move constructor:
circuit::circuit(circuit &&circ)
{
  // Steal the data:
  type = circ.type;
  symbol = circ.symbol;
  impedance = circ.impedance;
  frequency = circ.frequency;
  voltage = circ.voltage;
  circuit_comps = circ.circuit_comps;

  // Empty 'old' capacitor data:
  circ.type = "empty";
  circ.symbol = 'N';
  circ.impedance = 0;
  circ.frequency = 0;
  circ.voltage = 0;

  for (auto &comp : circ.circuit_comps) {
    comp.reset();
  }

}

//------------------------------------------------------------------------------

// Destructor:
circuit::~circuit(){}

//------------------------------------------------------------------------------
// Calculate Total Impedance of Circuit:
//------------------------------------------------------------------------------

// Returns reduced impedance value for a section of series components:
std::complex<double> circuit::calc_series_impedance(
  const std::vector<std::shared_ptr<component>> &series_sub_circ) const
{
  std::complex<double> impedance_sum{};

  // Total Z = z1 + z2 +z3 + ...:
  for (const auto &comp : series_sub_circ) {
    impedance_sum += (comp->get_impedance() );
  }

  return impedance_sum;
}

//------------------------------------------------------------------------------

// Returns reduced impedance value for a parallel of series components:
std::complex<double> circuit::calc_parallel_impedance(
  const std::vector<std::shared_ptr<component>> &parallel_sub_circ) const
{
  std::complex<double> reciprocal_sum{};

  // (1 / Total Z) = 1/z1 + 1/z2 + 1/z3 + ...:
  for (const auto &comp : parallel_sub_circ) {
    reciprocal_sum += (1.0 / (comp->get_impedance()) );
  }

  return (1.0 / reciprocal_sum);
}

//------------------------------------------------------------------------------

// Function calculates the total impedance of the circuit:
void circuit::set_impedance()
{
  // Will contain the sub circuit impedances - i.e. series components:
  std::vector<std::shared_ptr<std::complex<double>>> circuit_impedance;

  // Will hold the temporary series / parallel sub circuits:
  std::vector<std::shared_ptr<component>> series_sub_circ;
  std::vector<std::shared_ptr<component>> parallel_sub_circ;

  for (const auto &comp : circuit_comps) {

    if (comp->get_connection_type() == 's') {

      // Keeps adding until it reaches a parallel component:
      series_sub_circ.push_back(comp);

      // Previous chain consisted of a parallel components:
      if (parallel_sub_circ.size() != 0) {

        // End of a parallel loop, so add up this section and append:
        std::complex<double> para_sub_impedance
        = calc_parallel_impedance(parallel_sub_circ);

        circuit_impedance.push_back(
          std::make_shared<std::complex<double>> (para_sub_impedance) );
        parallel_sub_circ.clear();
      }

    } else if (comp->get_connection_type() == 'p') {

      // Add to parallel sub circuit vector:
      parallel_sub_circ.push_back(comp);

      // Previous chain consisted of a series components:
      if (series_sub_circ.size() != 0) {
      
        // End of a series chain, so add up this chain and append:
        std::complex<double> series_sub_impedance
        = calc_series_impedance(series_sub_circ);

        circuit_impedance.push_back(
          std::make_shared<std::complex<double>> (series_sub_impedance) );
        series_sub_circ.clear();
      }
    }
  }

  // Add remaining series component impedances up:
  if (series_sub_circ.size() != 0) {

    std::complex<double> series_sub_impedance
    = calc_series_impedance(series_sub_circ);
    circuit_impedance.push_back(
      std::make_shared<std::complex<double>> (series_sub_impedance) );
  
  // Add remaining parallel component impedances up:
  } else if (parallel_sub_circ.size() != 0) {
      std::complex<double> para_sub_impedance
      = calc_parallel_impedance(parallel_sub_circ);
      circuit_impedance.push_back(
        std::make_shared<std::complex<double>> (para_sub_impedance) );
  }

  std::complex<double> circ_impedance{};
  // Now have a vector full of "reduced series" impedances, so just sum these:
  for (const auto &impedance_value : circuit_impedance) {
    circ_impedance += *impedance_value;
  }

  impedance = circ_impedance;
}

//------------------------------------------------------------------------------
// Access Functions:
//------------------------------------------------------------------------------

void circuit::set_value(const double &freq)
{
  if (freq < 0.0) {
    throw std::out_of_range{"Cannot have negative frequency."};
  }

  // Sets all components to frequency of circuit:
  for (const auto &comp : circuit_comps) {
    comp->set_frequency(frequency);
  }

  frequency = freq;
  set_impedance();
}

double circuit::get_value() const
{
  return frequency;
}

void circuit::set_frequency(const double &freq)
{
  if (freq < 0.0) {
    throw std::out_of_range{"Cannot have negative frequency."};
  }

  // Sets all components to frequency of circuit:
  for (const auto &comp : circuit_comps) {
    comp->set_frequency(frequency);
  }

  frequency = freq;
  set_impedance();
}

double circuit::get_frequency() const
{
  return frequency;
}

void circuit::set_voltage(const double &volt)
{
  if (volt < 0.0) {
    throw std::out_of_range{"Cannot have negative voltage."};
  }

  voltage = volt;
}

double circuit::get_voltage() const
{
  return voltage;
}

double circuit::get_size() const
{
  return circuit_comps.size();
}

//------------------------------------------------------------------------------
// Printing info:
//------------------------------------------------------------------------------

void circuit::print_info() const
{
  std::cout << std::endl
  << "Circuit information: " << std::endl;

  if (frequency <= 1e-2 || frequency >= 1e3) {
    std::cout << std::scientific;
  } else {
    std::cout << std::fixed;
  }
  std::cout << "    Frequency, f = " << frequency << " Hz," << std::endl;

  if (voltage <= 1e-2 || voltage >= 1e3) {
    std::cout << std::scientific;
  } else {
    std::cout << std::fixed;
  }
  std::cout << "    Voltage, V = " << voltage << " V."  << std::endl;

  // For cases of circuits containing no components yet:
  if (get_size() == 0) {
    std::cout << std::endl
    << "You need to add components to this circuit to view " << std::endl
    << "data such as the total impedance and current." << std::endl;

  } else {

    std::cout << std::endl
    << "Total impedance of circuit:" << std::endl;

    if (get_magnitude() <= 1e-2 || get_magnitude() >= 1e3) {
      std::cout << std::scientific;
    } else {
      std::cout << std::fixed;
    }
    std::cout << "    Magnitude = " 
    << get_magnitude() << " Ohms," << std::endl;

    if (get_phase() <= 1e-2 || get_phase() >= 1e3) {
      std::cout << std::scientific;

    } else {
        std::cout << std::fixed;
    }

    if (get_phase() == 0) {
      std::cout << "    and phase = 0 radians." << std::endl;

    } else {
    std::cout << "    and phase = " 
    << get_phase() << " radians." << std::endl;
    }

    std::cout << std::endl;

    // Current, I = V / Z:
    double current = (voltage / get_magnitude() );

    if (current <= 1e-2 || current >= 1e3) {
      std::cout << std::scientific;
    } else {
      std::cout << std::fixed;
    }
    std::cout << "Total current, I = " << current << " A, ";
    
    if (get_phase() != 0) {
      std::cout << "with phase difference" << std::endl;
      if (get_phase() <= 1e-2 || get_phase() >= 1e3) {
        std::cout << std::scientific;
      } else {
          std::cout << std::fixed;
      }

      std::cout << "between the voltage of " 
      << get_phase() << " radians." << std::endl;

    } else {
      std::cout << std::endl;
      std::cout << "in phase with the voltage." << std::endl;
    }
  }
}

//------------------------------------------------------------------------------

// Prints out the components stored in the circuit:
void circuit::print_components() const
{
  std::cout << std::endl
  << "Here is this list of components in this circuit in order:" << std::endl;
  std::cout << std::endl;

  for (int i{}; i < circuit_comps.size(); ++i) {

    // Used to determine print statement:
    std::string connection{};
    std::string nested{};

    if (circuit_comps[i]->get_connection_type() == 's') {
      connection = "series";

    } else {
      connection = "parallel";
    }

    if (circuit_comps[i]->get_nested_bool() == true) {
      nested = "nested ";

    } else {
      nested = "";
    }

    std::cout << "Component " << (i + 1) << " is in " 
    << nested << connection << " - ";
    circuit_comps[i]->print_info();

    // Prints mag and phase for each comp:
    double magnitude =  circuit_comps[i]->get_magnitude();
    double phase =  circuit_comps[i]->get_phase();

    if (magnitude <= 1e-2 || magnitude >= 1e3) {
      std::cout << std::scientific;
    } else {
      std::cout << std::fixed;
    }
    std::cout << "    Magnitude = " 
    << magnitude << " Ohms," << std::endl;

    if (phase <= 1e-2 || phase >= 1e3) {
      std::cout << std::scientific;

    } else {
        std::cout << std::fixed;
    }

    if (phase == 0) {
      std::cout << "    and phase = 0 radians." << std::endl;

    } else {
    std::cout << "    and phase = " 
    << phase << " radians." << std::endl;
    }
  }
}

//------------------------------------------------------------------------------

// Prints out a tree diagram of the circuit:
// (uses similar method to set_impedance function)
void circuit::print_diagram() const
{
  // Info for smybols:
  std::cout << std::endl;
  std::cout << "Note: UPPERCASE = ideal / lowercase = non-ideal." << std::endl;

  // Initial section of circuit:
  std::cout << std::endl
  << " O" << std::endl
  << " |" << std::endl
  << "(~)" << std::endl;

  // Will hold the temporary parallel sub circuits:
  std::vector<std::shared_ptr<component>> parallel_sub_circ;

  for (const auto &comp : circuit_comps) {

    if (comp->get_connection_type() == 's') {

      // Previous chain consisted of a parallel components:
      if (parallel_sub_circ.size() != 0) {
        parallel_sub_circ.clear();
        // Ensures correct indent:
        std::cout << std::endl;
      }

      std::cout
      << " |" << std::endl
      << "[" << comp->get_symbol() << "]" << std::endl;

    } else if (comp->get_connection_type() == 'p') {

      // First component in a parallel chain:
      if (parallel_sub_circ.size() == 0) {
      
      // End of a series chain, so indent for first para comp:
      //std::cout << std::endl;
      std::cout
      << " |" << std::endl
      << " o--[" << comp->get_symbol() << "]";

      } else {
        // Continue parallel section:
        std::cout << "--[" << comp->get_symbol() << "]";
      }

      // Also add to parallel sub circuit vector:
      parallel_sub_circ.push_back(comp);
    }
  }

  // Final chain consisted of a parallel components:
  if (parallel_sub_circ.size() != 0) {
    parallel_sub_circ.clear();
    // Ensures correct indent:
    std::cout << std::endl;
  }

  // End of circuit:
  std::cout
  << " |" << std::endl
  << " O" << std::endl;
}

//------------------------------------------------------------------------------
// Adding / removing components to circuit:
//------------------------------------------------------------------------------

// Forward declarations for this template function (for ALL 6 types):
template void circuit::add_component<resistor>(
  std::shared_ptr<resistor> &resis, const char &conn, const bool &nest);

template void circuit::add_component<capacitor>(
  std::shared_ptr<capacitor> &capac, const char &conn, const bool &nest);

template void circuit::add_component<inductor>(
  std::shared_ptr<inductor> &induc, const char &conn, const bool &nest);

template void circuit::add_component<real_resistor>(
  std::shared_ptr<real_resistor> &real_resis, const char &conn, const bool &nest);

template void circuit::add_component<real_capacitor>(
  std::shared_ptr<real_capacitor> &real_capac, const char &conn, const bool &nest);

template void circuit::add_component<real_inductor>(
  std::shared_ptr<real_inductor> &real_induc, const char &conn, const bool &nest);

//------------------------------------------------------------------------------

// Removes a component from the circuit based on its index:
void circuit::remove_component(const size_t &index)
{
  circuit_comps.erase(circuit_comps.begin() + index);

  set_impedance();
}

//------------------------------------------------------------------------------