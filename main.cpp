//------------------------------------------------------------------------------

// Project - AC Circuits
// Monty Kirner - 14/04/21

//change

//------------------------------------------------------------------------------

#include "base_component.hpp"
#include "circuit.hpp"
#include "resistors.hpp"
#include "capacitors.hpp"
#include "inductors.hpp"

//------------------------------------------------------------------------------
// Set up libraries:
//------------------------------------------------------------------------------

// To store components / circuits made by the user:
static std::vector<std::shared_ptr<component>> components_library;
static std::vector<std::unique_ptr<circuit>> circuits_library;

//------------------------------------------------------------------------------
// Useful functions:
//------------------------------------------------------------------------------

// Template function to check if user input if valid (int/double/char etc.):
template <class T> T valid_input(
  const std::string& prompt = "")
{
  T valid_input{};

  // Loops until user input is valid:
  bool is_valid = false;
  while (!is_valid) {
    try {
      std::cout << prompt;

      std::string user_input{};
      std::getline(std::cin, user_input);
      std::istringstream stream{user_input};

      // True if it can go into valid_input:
      if (stream >> valid_input && !(stream >> user_input) ) {
        is_valid = true;

      } else {
        throw std::invalid_argument{"Incorrect input type."};
      }
    }
    // Any incorrect input thrown here:
    catch (const std::invalid_argument& ia) {
      std::cout << std::endl;
      std::cerr << "Invalid argument error: ";
      std::cout << std::endl
      << ia.what() << std::endl;
      std::cin.clear();
    }
  }

  return valid_input;
}

//------------------------------------------------------------------------------

// Checks that int input is in required range:
int valid_int_range(const int &min, const int &max)
{
  int valid_int{};

  // Loops until user input is valid:
  bool is_valid = false;
  while (!is_valid) {
    try {
      std::string prompt = "Please enter a number between "
      + std::to_string(min) + "-" + std::to_string(max) + ": ";

      valid_int = valid_input<int>(prompt);

      // Returns true if it's within the range:
      if (valid_int >= min && valid_int <= max) {
        is_valid = true;
      
      } else {
        throw std::out_of_range{"Integer not within the range."};
      }
    }
    catch (const std::out_of_range& oor) {
      std::cout << std::endl;
      std::cerr << "Out of Range error: " << std::endl
      << oor.what() << std::endl; 
    }
  }

  std::cout << "Input " << valid_int << " accepted." << std::endl;
  return valid_int;
}

//------------------------------------------------------------------------------

// Used for yes / no questions:
bool yes_or_no(const std::string &question)
{
  char valid_choice{};

  bool is_valid = false;
  while (!is_valid) {
    try {
      std::string prompt = question + "\nPlease input y/n for yes or no: ";

      valid_choice = valid_input<char>(prompt);

      // Converts input to lower case (to accept Y/N):
      if (tolower(valid_choice) == 'y' || tolower(valid_choice) == 'n') {
        std::cout << "Input " << valid_choice << " accepted." << std::endl;
        is_valid = true;
      
      } else {
        throw std::invalid_argument{"Input char must be either y/n."};
      }
    }
    catch (const std::invalid_argument& ia) {
      std::cout << std::endl;
      std::cerr << "Invalid argument error: " << std::endl
      << ia.what() << std::endl;
    }
  }

  if (valid_choice == 'y') {
    return true;

  } else {
    return false;
  }
}
//------------------------------------------------------------------------------

void add_valid_component(const std::string &type)
{
  bool is_valid = false;
  while (!is_valid) {

    try{
      double res{}, ind{}, cap{};

      if (type == "resistor") {
        res = valid_input<double>("Please input the resistance in Ohms: ");

        components_library.push_back(
          std::make_shared<resistor>(resistor{res}) );
        is_valid = true;
      }

      else if (type == "inductor"){
        ind = valid_input<double>("Please input the inductance in Henrys: ");

        components_library.push_back(
          std::make_shared<inductor>(inductor{ind}) );
        is_valid = true;
      }

      else if (type == "capacitor"){
        cap = valid_input<double>("Please input the capacitance in Farads: ");

        components_library.push_back(
          std::make_shared<capacitor>(capacitor{cap}) );
        is_valid = true;
      }

      else if (type == "non-ideal resistor") {
        res = valid_input<double>("Please input the resistance in Ohms: ");
        ind = valid_input<double>(
          "Please input the non-ideal inductance in Henrys: ");
        cap = valid_input<double>(
          "Please input the non-ideal capacitance in Farads: ");

        components_library.push_back(
          std::make_shared<real_resistor>(real_resistor{res, ind, cap}) );
        is_valid = true;
      }

      else if (type == "non-ideal inductor") {
        ind = valid_input<double>("Please input the inductance in Henrys: ");
        cap = valid_input<double>(
          "Please input the non-ideal capacitance in Farads: ");
        res = valid_input<double>(
          "Please input the non-ideal resistance in Ohms: ");

        components_library.push_back(
          std::make_shared<real_inductor>(real_inductor{res, ind, cap}) );
        is_valid = true;
      }

      else if (type == "non-ideal capacitor") {
        cap = valid_input<double>("Please input the capacitance in Farads: ");
        res = valid_input<double>(
          "Please input the non-ideal resistance in Ohms: ");
        ind = valid_input<double>(
          "Please input the non-ideal inductance in Henrys: ");

        components_library.push_back(
          std::make_shared<real_capacitor>(real_capacitor{res, ind, cap}) );
        is_valid = true;

      // Note to myself that I've called the function wrong (does nothing):
      } else {
        std::cout << "Invalid component type specified." << std::endl;
        is_valid = true;
      }
    }
    // Parametrised constructor throws (in each component class):
    catch (const std::out_of_range& oor) {
      std::cout << std::endl;
      std::cerr << "Out of Range error: " << std::endl
      << oor.what() << std::endl; 
    }
  }

  std::cout << "New " << type << " created." << std::endl;
}

//------------------------------------------------------------------------------
// Case 1 - Create new component:
//------------------------------------------------------------------------------

void create_component()
{
  bool add_component = true;
  while (add_component) {

    std::cout << std::endl
    << "----------------------------------------------------------" << std::endl
    << "                  Create New Component:                   " << std::endl
    << "----------------------------------------------------------" << std::endl
    << std::endl
    << "Please choose one of the options below:" << std::endl
    << std::endl
    << "[1] - Resistor" << std::endl
    << "[2] - Inductor" << std::endl
    << "[3] - Capacitor" << std::endl
    << "[4] - Non-ideal resistor" << std::endl
    << "[5] - Non-ideal inductor" << std::endl
    << "[6] - Non-ideal capacitor" << std::endl
    << "[7] - Return to main menu" << std::endl
    << std::endl
    << "----------------------------------------------------------" << std::endl
    << std::endl;

    int choice = valid_int_range(1, 7);
    switch (choice) {
      case 1: {
        add_valid_component("resistor");
        break;
      }
      case 2: {
        add_valid_component("inductor");
        break;
      }
      case 3: {
        add_valid_component("capacitor");
        break;
      }
      case 4: {
        add_valid_component("non-ideal resistor");
        break;
      }
      case 5: {
        add_valid_component("non-ideal inductor");
        break;
      }
      case 6: {
        add_valid_component("non-ideal capacitor");
        break;
      }
      case 7: {
        // Breaks while loop:
        add_component = false;
        break;
      }
    }
  }

  std::cout << std::endl;
  std::cout << "Returning to main menu." << std::endl;
}

//------------------------------------------------------------------------------
// Case 2 - Create new circuit:
//------------------------------------------------------------------------------

void create_circuit()
{
  bool add_circuit = true;
  while (add_circuit) {

    std::cout << std::endl
    << "----------------------------------------------------------" << std::endl
    << "                   Create new circuit:                    " << std::endl
    << "----------------------------------------------------------" << std::endl
    << std::endl
    << "Please choose one of the options below:" << std::endl
    << std::endl
    << "[1] - Create new circuit" << std::endl
    << "[2] - Return to main menu" << std::endl
    << std::endl
    << "----------------------------------------------------------" << std::endl
    << std::endl;

    int choice = valid_int_range(1, 2);
    switch (choice) {

      case 1: {
        bool is_valid = false;
        while (!is_valid) {
          try {
            double circuit_freq = valid_input<double>(
              "Enter the frequency of the circuit (Hz): ");

            double circuit_volt = valid_input<double>(
              "Now enter the voltage of the circuit (V): ");

            circuits_library.push_back(
              std::make_unique<circuit>(circuit{circuit_freq, circuit_volt}) );

            std::cout << "Circuit created." << std::endl;
            is_valid = true;
          }
          // Frequency and voltage throws (in circuit.cpp):
          catch (const std::out_of_range& oor) {
            std::cout <<  std::endl;
            std::cerr << "Out of Range error: " << std::endl
            << oor.what() << std::endl;
          }
        }
        break;
      }

//------------------------------------------------------------------------------

      case 2: {
        // Breaks while loop:
        add_circuit = false;
        break;
      }
    }
  }

  std::cout << std::endl;
  std::cout << "Returning to main menu." << std::endl;
}

//------------------------------------------------------------------------------
// Case 5 - View existing components (used in Case 3/4):
//------------------------------------------------------------------------------

void components_info(const bool &print_title)
{
  if (components_library.size() == 0) {
    std::cout << std::endl;
    std::cout << "You haven't added any components yet!" << std::endl;

  } else {

    // Don't print when modifying components:
    if (print_title == true) {
      std::cout << std::endl
      << "----------------------------------------------------------" << std::endl
      << "                View existing components:                 " << std::endl
      << "----------------------------------------------------------" << std::endl;
    }

    std::cout << std::endl
    << "You've created " << components_library.size()
    << " components." << std::endl
    << "Here are ALL the existing components data:" << std::endl;
    std::cout << std::endl;

    for (int i{}; i < components_library.size(); ++i) {
      std::cout << "Component " << (i + 1) << " - ";
      components_library[i]->print_info();
    }
  }

  if (print_title == true) {
    std::cout << std::endl;
    std::cout << "Returning to main menu." << std::endl;
  }
}

//------------------------------------------------------------------------------
// Case 3 - Change existing components:
//------------------------------------------------------------------------------

void change_component()
{
  if (components_library.size() == 0) {
    std::cout << std::endl;
    std::cout << "You haven't added any components yet!" << std::endl;

  } else {

    bool modify_components = true;
    while(modify_components) {

      std::cout << std::endl
      << "----------------------------------------------------------" << std::endl
      << "               Change existing components:                " << std::endl
      << "----------------------------------------------------------" << std::endl
      << std::endl
      << "Please choose one of the options below:" << std::endl
      << std::endl
      << "[1] - Modify a component" << std::endl
      << "[2] - Return to main menu" << std::endl
      << std::endl
      << "----------------------------------------------------------" << std::endl
      << std::endl;

      int choice = valid_int_range(1, 2);
      switch (choice) {

        case 1: {
          components_info(false);
          
          std::cout << std::endl
          << "Choose a component to modify." << std::endl;

          int comp_choice = valid_int_range(1, components_library.size() );
          
          // To get correct vector index again (starts from 0):
          --comp_choice;

          bool is_valid = false;
          while (!is_valid) {

            try {
              std::string prompt = "Please enter the new value for the "
              + components_library[comp_choice]->get_type() + ": ";

              double new_value = valid_input<double>(prompt);

              components_library[comp_choice]->set_value(new_value);

              std::cout << std::endl;
              std::cout << "Component data modified." << std::endl;
              is_valid = true;
            }
            // New value must be in range throws:
            catch (const std::out_of_range& oor) {
              std::cout << std::endl;
              std::cerr << "Out of Range error: " << std::endl
              << oor.what() << std::endl;
            }
          }
          break;
        }

//------------------------------------------------------------------------------

        case 2: {
          // Breaks while loop:
          modify_components = false;
          break;
        }
      }
    }
  }

  std::cout << std::endl;
  std::cout << "Returning to main menu." << std::endl;
}

//------------------------------------------------------------------------------
// Case 4 - Change existing circuits:
//------------------------------------------------------------------------------

void change_circuit()
{
  if (circuits_library.size() == 0) {
    std::cout << std::endl;
    std::cout << "You haven't added any circuits yet!" << std::endl;

  } else {

    bool modify_circuits = true;
    while (modify_circuits) {

      std::cout << std::endl
      << "----------------------------------------------------------" << std::endl
      << "                Change existing circuits:                 " << std::endl
      << "----------------------------------------------------------" << std::endl
      << std::endl
      << "Please choose one of the options below:" << std::endl
      << std::endl
      << "[1] - Add components to a circuit" << std::endl
      << "[2] - Remove components from a circuit" << std::endl
      << "[3] - Change frequency of a circuit" << std::endl
      << "[4] - Change voltage of a circuit" << std::endl
      << "[5] - Return to main menu" << std::endl
      << std::endl
      << "----------------------------------------------------------" << std::endl
      << std::endl;

      int choice = valid_int_range(1, 5);
      switch (choice) {

//------------------------------------------------------------------------------

        case 1: {

          if (components_library.size() == 0) {
            std::cout << std::endl;
            std::cout << "You haven't added any components yet!" << std::endl;

          } else {
            std::cout << std::endl
            << "Choose the circuit you want to add components to." << std::endl;
            int circ_choice = valid_int_range(1, circuits_library.size() );

            // To get correct vector index again (starts from 0):
            --circ_choice;

            // Overrides choice to ensure first component cannot be nested:
            bool first_comp = false;

            if (circuits_library[circ_choice]->get_size() == 0) {
              std::cout << std::endl
              << "This circuit has no components yet!"<< std::endl;
              //std::cout << "The first component cannot be nested.";
              std::cout << std::endl;

              first_comp = true;
            }

            bool comps_add = true;
            while (comps_add) {

              components_info(false);

              std::cout << std::endl
              << "Choose the component you want to add." << std::endl;
              int comp_choice = valid_int_range(1, components_library.size() );

              // To get correct vector index again (starts from 0):
              --comp_choice;

              try {
                std::string prompt = "Do you want the component to be added"
                " in series or parallel?\nPlease enter either 's' or 'p'"
                " for series or parallel: ";

                char conn_choice = valid_input<char>(prompt);

                if (conn_choice != 's' && conn_choice != 'p') {
                  throw std::invalid_argument{"Input char must be either s/p."};
                }

                // Prevents first component from being nested:
                if (first_comp == true) {
                  circuits_library[circ_choice]->add_component(
                    components_library[comp_choice], conn_choice, false);
                  first_comp = false;

                } else {

                  bool nest_choice = false;
                  // Functionality all possible, but unable to calc impedance:
                  //bool nest_choice = yes_or_no("Is this a nested component?");

                  circuits_library[circ_choice]->add_component(
                    components_library[comp_choice], conn_choice, nest_choice);
                }

                std::cout << std::endl
                << "Component added to circuit." << std::endl;

                std::cout << std::endl;
                comps_add = yes_or_no(
                  "Do you want to add another component to this circuit?");
              }
              catch (const std::invalid_argument& ia) {
                std::cout << std::endl;
                std::cerr << "Invalid argument error: " << std::endl
                << ia.what() << std::endl;
              }
            }
          }
          break;
        }

//------------------------------------------------------------------------------

        case 2: {

          std::cout << std::endl;
          std::cout << "Choose the circuit you want to remove components from.";
          std::cout << std::endl;
          int circ_choice = valid_int_range(1, circuits_library.size() );

          // To get correct vector index again (starts from 0):
          --circ_choice;

          if (circuits_library[circ_choice]->get_size() == 0) {
            std::cout << std::endl
            << "This circuit has no components yet!"<< std::endl
            << "Please add components first." << std::endl;

          } else {

            bool comps_remove = true;
            while (comps_remove) {

              circuits_library[circ_choice]->print_components();

              std::cout << std::endl
              << "Choose the component you want to remove." << std::endl;
              int comp_choice = valid_int_range(
                1, circuits_library[circ_choice]->get_size() );

              --comp_choice;

              circuits_library[circ_choice]->remove_component(comp_choice);

              std::cout << std::endl;
              std::cout << "Component removed." << std::endl;

              // For cases where all components have been removed:
              if (circuits_library[circ_choice]->get_size() == 0) {
                std::cout << std::endl
                << "This circuit is now empty." << std::endl
                << "You cannot remove any more components from it." << std::endl;
                comps_remove = false;

              } else {
                std::cout << std::endl;
                comps_remove = yes_or_no(
                  "Do you want to remove another component from this circuit?");
              }
            }
          }
          break;
        }

//------------------------------------------------------------------------------

        case 3: {
          std::cout << std::endl
          << "Choose the circuit you want to change frequency of." << std::endl;
          int circ_choice = valid_int_range(1, circuits_library.size() );

          // To get correct vector index again (starts from 0):
          --circ_choice;

          bool is_valid = false;
          while (!is_valid) {
            try {
              std::cout << std::endl;
              std::string prompt = "Please enter the new value"
              " for the frequency (Hz): ";

              double new_value = valid_input<double>(prompt);

              circuits_library[circ_choice]->set_frequency(new_value);

              std::cout << std::endl;
              std::cout << "Frequency has been changed." << std::endl;
              is_valid = true;
            }
            // For negative frequency throw in set_frequency():
            catch (const std::out_of_range& oor) {
              std::cout << std::endl;
              std::cerr << "Out of Range error: " << std::endl
              << oor.what() << std::endl;
            }
          }
          break;
        }

//------------------------------------------------------------------------------

        case 4: {
          std::cout << std::endl
          << "Choose the circuit you want to change voltage of." << std::endl;
          int circ_choice = valid_int_range(1, circuits_library.size() );

          // To get correct vector index again (starts from 0):
          --circ_choice;

          bool is_valid = false;
          while (!is_valid) {
            try {
              std::cout << std::endl;
              std::string prompt = "Please enter the new value"
              " for the voltage (V): ";

              double new_value = valid_input<double>(prompt);

              circuits_library[circ_choice]->set_voltage(new_value);

              std::cout << std::endl;
              std::cout << "Voltage has been changed." << std::endl;
              is_valid = true;
            }
            // For negative voltage throw in set_voltage():
            catch (const std::out_of_range& oor) {
              std::cout << std::endl;
              std::cerr << "Out of Range error: " << std::endl
              << oor.what() << std::endl;
            }
          }
          break;
        }

//------------------------------------------------------------------------------

        case 5: {
          modify_circuits = false;
          break;
        }
      }
    }
  }

  std::cout << std::endl;
  std::cout << "Returning to main menu." << std::endl;
}

//------------------------------------------------------------------------------
// Case 6 - View existing circuits:
//------------------------------------------------------------------------------

void circuits_info()
{
  if (circuits_library.size() == 0) {
    std::cout << std::endl;
    std::cout << "You haven't added any circuits yet!" << std::endl;

  } else {

    bool view_circuits = true;
    while (view_circuits) {

      std::cout << std::endl
      << "----------------------------------------------------------" << std::endl
      << "                 View existing circuits:                  " << std::endl
      << "----------------------------------------------------------" << std::endl
      << std::endl
      << "Please choose one of the options below:" << std::endl
      << std::endl
      << "[1] - View circuit information (e.g. total impedance)" << std::endl
      << "[2] - View list of components in a circuit" << std::endl
      << "[3] - View circuit diagram" << std::endl
      << "[4] - Return to main menu" << std::endl
      << std::endl
      << "----------------------------------------------------------" << std::endl
      << std::endl;

      int choice = valid_int_range(1, 4);
      switch (choice) {

        case 1: {
          bool view_info = true;
          while (view_info) {
            std::cout << std::endl
            << "Which circuit do you want information about?" << std::endl;
            int circ_choice = valid_int_range(1, circuits_library.size() );

            // To get correct vector index again (starts from 0):
            --circ_choice;

            circuits_library[circ_choice]->print_info();

            if (circuits_library.size() == 1) {
              view_info = false;

            } else {
              std::cout << std::endl;
              view_info = yes_or_no("Do you want to look at another circuit?");
            }
          }
          break;
        }

//------------------------------------------------------------------------------

        case 2: {
          bool view_list = true;
          while (view_list) {
            std::cout << std::endl
            << "Which circuit do you want the list of components for?";
            std::cout << std::endl;
            int circ_choice = valid_int_range(1, circuits_library.size() );

            // To get correct vector index again (starts from 0):
            --circ_choice;

            if (circuits_library[circ_choice]->get_size() == 0) {
            std::cout << std::endl
            << "This circuit has no components yet!"<< std::endl
            << "Please add components first." << std::endl;

            } else {
              circuits_library[circ_choice]->print_components();
            }

            if (circuits_library.size() == 1) {
              view_list = false;
            
            } else {
              std::cout << std::endl;
              view_list = yes_or_no(
                "Do you want to look at another circuits list of components?");
            }
          }
          break;
        }

//------------------------------------------------------------------------------

        case 3: {
          bool view_diagram = true;
          while (view_diagram) {
            std::cout << std::endl
            << "Which circuit do you want the diagram of?";
            std::cout << std::endl;
            int circ_choice = valid_int_range(1, circuits_library.size() );

            // To get correct vector index again (starts from 0):
            --circ_choice;

            if (circuits_library[circ_choice]->get_size() == 0) {
            std::cout << std::endl
            << "This circuit has no components yet!"<< std::endl
            << "Please add components first." << std::endl;

            } else {
              circuits_library[circ_choice]->print_diagram();
            }

            if (circuits_library.size() == 1) {
              view_diagram = false;

            } else {
              std::cout << std::endl;
              view_diagram = yes_or_no(
                "Do you want to look at another circuits diagram?");
            }
          }
          break;
        }

//------------------------------------------------------------------------------

        case 4: {
          view_circuits = false;
          break;
        }
      }
    }
  }

  std::cout << std::endl;
  std::cout << "Returning to main menu." << std::endl;
}

//------------------------------------------------------------------------------
// Case 7 - Clear ALL library data:
//------------------------------------------------------------------------------

void clear_libraries()
{
  std::cout << std::endl
  << "----------------------------------------------------------" << std::endl
  << "                 Clear ALL library data:                  " << std::endl
  << "----------------------------------------------------------" << std::endl
  << std::endl;

  bool clear_choice = yes_or_no(
    "Are you sure you want to clear ALL library data?");
  
  if (clear_choice == true) {
    components_library.clear();
    circuits_library.clear();
    std::cout << "Library data has been deleted." << std::endl;

  } else {
    std::cout << "Library data will not be deleted." << std::endl;
  }

  std::cout << std::endl;
  std::cout << "Returning to main menu." << std::endl;
}

//------------------------------------------------------------------------------
// Case 8 - Quit program:
//------------------------------------------------------------------------------

bool quit_program()
{
  std::cout << std::endl
  << "----------------------------------------------------------" << std::endl
  << "                      Quit program:                       " << std::endl
  << "----------------------------------------------------------" << std::endl
  << std::endl;

  bool quit_choice = yes_or_no("Are you sure you want to quit?");

  if (quit_choice == true) {
    std::cout << std::endl;
    std::cout << "Ending program." << std::endl;
    std::cout << std::endl;

    // Remember to clear library data:
    components_library.clear();
    circuits_library.clear();

    // Return false so that run_program == false:
    return false;

  } else {
    std::cout << std::endl;
    std::cout << "Program continuing. Returning to main menu." << std::endl;

    // So that run_program == true:
    return true;
  }
}

//------------------------------------------------------------------------------
// Main - contains main menu interface:
//------------------------------------------------------------------------------

int main()
{
  bool run_program = true;
  while (run_program) {

    std::cout << std::setprecision(2) << std::fixed;

    std::cout << std::endl
    << "----------------------------------------------------------" << std::endl
    << "                        Main menu:                        " << std::endl
    << "----------------------------------------------------------" << std::endl
    << std::endl
    << "Please choose one of the options below:" << std::endl
    << std::endl
    << "[1] - Create new component" << std::endl
    << "[2] - Create new circuit" << std::endl
    << "[3] - Change existing components" << std::endl
    << "[4] - Change existing circuits" << std::endl
    << "[5] - View existing components" << std::endl
    << "[6] - View existing circuits" << std::endl
    << "[7] - Clear ALL library data" << std::endl
    << "[8] - Quit program" << std::endl
    << std::endl
    << "----------------------------------------------------------" << std::endl
    << std::endl;

    int choice = valid_int_range(1, 8);
    switch (choice) {

      case 1: {
        create_component();
        break;
      }
      case 2: {
        create_circuit();
        break;
      }
      case 3: {
        change_component();
        break;
      }
      case 4: {
        change_circuit();
        break;
      }
      case 5: {
        components_info(true);
        break;
      }
      case 6: {
        circuits_info();
        break;
      }
      case 7: {
        clear_libraries();
        break;
      }
      case 8: {
        run_program = quit_program();
        break;
      }
    }
  }

  return 0;
}
//------------------------------------------------------------------------------