#pragma once

#include <array>
#include <stdexcept>
#include <vector>

namespace CodexMachina
{
  class Chip8
  {
  public:
    constexpr static auto getTiming() { return TIMING; }
    constexpr static auto getX() { return X; }
    constexpr static auto getY() { return Y; }

    Chip8() { reset(); }
    void emulateCycle();
    auto getDisplay() const { return _display; }
    auto getPixel(const size_t x, const size_t y) const
    {
      if (x > X) { throw std::invalid_argument{ "x is too large" }; }
      if (y > Y) { throw std::invalid_argument{ "y is too large" }; }
      return _display[(y * X) + x];
    }
    auto getSound() const { return _makeSound; }
    void loadFont(const unsigned short offset = 0);
    void loadMemory(const std::vector<unsigned char> data, const unsigned short offset);
    void reset();
  private:
    constexpr static size_t MEMORY_SIZE{ 0x1000 };
    constexpr static double TIMING{ 1.0 / 60.0 }; // 60 Hertz
    constexpr static size_t X{ 64 };
    constexpr static size_t Y{ 32 };

    bool _blocked = false;
    unsigned char _delayTimer;
    std::array<unsigned char, X * Y> _display;
    unsigned short _I;
    std::array<unsigned char, 16> _keys;
    bool _makeSound;
    std::array<unsigned char, MEMORY_SIZE> _memory;
    unsigned short _pc;
    unsigned char _soundTimer;
    unsigned char _sp;
    std::array<unsigned short, 16> _spriteAddrs;
    std::array<unsigned short, 16> _stack;
    std::array<unsigned char, 16> _V;
  };
} // namespace CodexMachina