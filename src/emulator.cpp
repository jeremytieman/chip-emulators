#include <boost/program_options.hpp>
#include <chip8.h>
#include <iostream>
#include <string>

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
  // Handle command line parameters
  std::string machineName;
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "produce help message")
    ("machine,m", po::value<std::string>(&machineName)->required(), "machine name")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);

  if (vm.count("help") != 0) {
    std::cout << desc << "\n";
    return 0;
  }

  po::notify(vm);

  return 0;
}