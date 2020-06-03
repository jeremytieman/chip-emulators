#include <chip8.h>
#include <chrono>
#include <cstdlib>
#include <cxxopts.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

auto loadBinaryFile(const std::string& filename)
{
  if (!std::filesystem::exists(filename))
  {
    std::string error{ filename };
    error.append(" does not exist");
    throw std::invalid_argument(error);
  }

  std::ifstream input(filename, std::ios::binary);
  std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
  return buffer;

}

int main(int argc, char* argv[])
{
  // Handle command line parameters
  std::string chipName;
  std::string programFileName;
  cxxopts::Options options(argv[0], "Allowed options");
  std::string chipDescr{
    "Values:\n"
    "  chip8: Chip-8 emulator\n"
    "  schip8: SChip-8 emulator\n"
    "  i8080: Intel 8080 emulator\n"
  };
  options.add_options()
    ("h,help", "produce help message")
    ("c,chip", "chip name", cxxopts::value<std::string>(chipName), chipDescr.c_str())
    ("p,program",  "program file name", cxxopts::value<std::string>(programFileName))
    ;

  auto result = options.parse(argc, argv);

  if (result.count("help"))
  {
    std::cout << options.help() << "\n";
    return 0;
  }

  if (result.count("chip") == 0)
  {
    std::cerr << "Missing chip name\n";
    std::cerr << options.help() << "\n";
    return 0;
  }

  if (result.count("program") == 0)
  {
    std::cerr << "Missing program file name\n";
    std::cerr << options.help() << "\n";
    return 0;
  }

  if ("chip8" == chipName)
  {
    CodexMachina::Chip8 chip8;
    auto buffer = loadBinaryFile(programFileName);
    chip8.loadMemory(buffer, 0x200);
    std::string windowTitle("CHIP8 - ");
    windowTitle.append(programFileName);
    sf::RenderWindow window(sf::VideoMode(static_cast<unsigned int>(chip8.getX()), static_cast<unsigned int>(chip8.getY())), windowTitle);

    while (window.isOpen())
    {
      sf::Event event;

      while (window.pollEvent(event))
      {
        if (sf::Event::Closed == event.type) window.close();
      }

      window.clear();
      chip8.emulateCycle();
      sf::Image img;
      img.create(static_cast<unsigned int>(chip8.getX()), static_cast<unsigned int>(chip8.getY()));
      auto pixels = chip8.getDisplay();

      for (size_t i = 0; i < chip8.getY(); ++i)
      {
        for (size_t j = 0; j < chip8.getX(); ++j)
        {
          auto pixel = pixels[(i * chip8.getX()) + j];
          if (1 == pixel) img.setPixel(static_cast<unsigned int>(j), static_cast<unsigned int>(i), sf::Color::White);
        }
      }

      sf::Texture tex;
      tex.loadFromImage(img);
      sf::Sprite sprite{ tex };
      window.draw(sprite);
      window.display();
    }
  }
  else if ("schip8" == chipName)
  {

  }
  else if ("i8080" == chipName)
  {

  }
  else
  {
    std::cout << options.help() << "\n";
    return EXIT_SUCCESS;;
  }

  return EXIT_SUCCESS;
}