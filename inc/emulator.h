#pragma once

#include <initializer_list>
#include <unordered_map>
#include <vector>

namespace CodexMachina
{
  constexpr static auto emulatorList = { "CHIP8" };
  static auto emulators() { return emulatorList; }

  class ChipEmulator
  {
    public:
      virtual void emulateCycle() = 0;

      virtual double getDefaultTimingHertz() const = 0;
      virtual std::vector<unsigned char> getDisplay() const = 0;
      virtual std::vector<unsigned char> getMemory() const = 0;
      virtual std::string getNextOpcode() const = 0;
      virtual std::string getNextOpcodeDesc() const = 0;
      virtual size_t getPC() const = 0;
      virtual std::initializer_list<const char*> getRegisterNames() const = 0;
      virtual std::unordered_map<const char *, unsigned long long> getRegisterValues() const = 0;
      virtual double getTimingHertz() const = 0;

      virtual bool hasRegisters() const = 0;
      virtual bool hasStack() const = 0;

      virtual void loadMemory(const std::vector<unsigned char> data, const size_t offset) = 0;

      virtual void setTimingHertz(const double timing) = 0;
      //virtual void setMemory(const unsigned char byte, const size_t location) = 0;
  };
} // namespace CodexMachine
