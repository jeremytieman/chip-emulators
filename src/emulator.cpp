#include <boost/program_options.hpp>
#include <chip8.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

namespace po = boost::program_options;

auto loadBinaryFile(const std::string& filename)
{
  if (!std::filesystem::exists(filename))
  {
    std::string error{ filename };
    error.append(" does not exist");
    throw std::invalid_argument(error);
  }

  std::ifstream input("C:\\Final.gif", std::ios::binary);
  std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
  return buffer;

}

int main(int argc, char* argv[])
{
  // Handle command line parameters
  std::string computerName;
  std::string programFileName;
  po::options_description desc{ "Allowed options" };
  std::string chipDescr{
    "computer name\n\n"
    "Values:\n"
    "  chip8: Chip-8 emulator\n"
    "  schip8: SChip-8 emulator\n"
    "  i8080: Intel 8080 emulator\n"
  };
  desc.add_options()
    ("help,h", "produce help message")
    ("computer,c", po::value<std::string>(&computerName)->required(), chipDescr.c_str())
    ("program,p", po::value<std::string>(&programFileName)->required(), "program file name")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);

  if (vm.count("help") != 0) {
    std::cout << desc << "\n";
    return 0;
  }

  po::notify(vm);

  if ("chip8" == computerName)
  {
    CodexMachina::Chip8 chip8;
    auto buffer = loadBinaryFile(programFileName);
    chip8.loadMemory(buffer, 0x200);
    std::string windowTitle("CHIP8 - ");
    windowTitle.append(programFileName);
    sf::RenderWindow window(sf::VideoMode(chip8.getX(), chip8.getY()), windowTitle);
  }
  else if ("schip8" == computerName)
  {

  }
  else if ("i8080" == computerName)
  {

  }
  else
  {
    std::cout << desc << "\n";
    return 0;
  }

  return 0;
}