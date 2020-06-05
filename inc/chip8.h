#pragma once

#include <algorithm>
#include <array>
#include "emulator.h"
#include <iterator>
#include <stdexcept>
#include <vector>

namespace CodexMachina
{
  class Chip8 : public ChipEmulator
  {
  public:
    virtual void emulateCycle() override;

    virtual double getDefaultTiming() const override { return DEFAULT_TIMING; }

    virtual std::vector<unsigned char> getDisplay() const override
    {
      std::vector<unsigned char> out(_display.size());
      std::copy(_display.begin(), _display.end(), std::back_inserter(out));
      return out;
    }

    virtual std::vector<unsigned char> getMemory() const override
    {
      std::vector<unsigned char> out(_memory.size());
      std::copy(_memory.begin(), _memory.end(), std::back_inserter(out));
      return out;
    }

    virtual size_t getPC() const override { return _pc; }
    virtual std::initializer_list<const char*> getRegisterNames() const override { return REGISTER_NAMES; }

    virtual std::unordered_map<const char*, unsigned long long> getRegisterValues() const override
    {
      std::unordered_map<const char*, unsigned long long> registers;
      registers["V0"] = _V[0];
      registers["V1"] = _V[1];
      registers["V2"] = _V[2];
      registers["V3"] = _V[3];
      registers["V4"] = _V[4];
      registers["V5"] = _V[5];
      registers["V6"] = _V[6];
      registers["V7"] = _V[7];
      registers["V8"] = _V[8];
      registers["V9"] = _V[9];
      registers["VA"] = _V[10];
      registers["VB"] = _V[11];
      registers["VC"] = _V[12];
      registers["VD"] = _V[13];
      registers["VE"] = _V[14];
      registers["VF"] = _V[15];
      return registers;
    }

    virtual double getTiming() const override { return _timing; }
    virtual bool hasRegisters() const override { return true; }
    virtual bool hasStack() const override { return true; }
    virtual void loadMemory(const std::vector<unsigned char> data, const size_t offset) override;
    virtual void setTiming(const double timing) override { _timing = timing; }
    
    constexpr static auto getX() { return X; }
    constexpr static auto getY() { return Y; }

    Chip8() { reset(); }

    auto getPixel(const size_t x, const size_t y) const
    {
      if (x > X) { throw std::invalid_argument{ "x is too large" }; }
      if (y > Y) { throw std::invalid_argument{ "y is too large" }; }
      return _display[(y * X) + x];
    }

    auto getSound() const { return _makeSound; }
    void loadFont(const unsigned short offset = 0);
    void reset();

  private:
    constexpr static double DEFAULT_TIMING{ 1.0 / 60.0 }; // 60 Hertz
    constexpr static size_t MEMORY_SIZE{ 0x1000 };
    constexpr static auto REGISTER_NAMES = { "V0",
      "V1", "V2", "V3", "V4", "V5", "V6", "V7",
      "V8", "V9", "VA", "VB", "VC", "VD", "VE",
      "VF" };
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
    double _timing;
    std::array<unsigned char, 16> _V;
  };
} // namespace CodexMachina