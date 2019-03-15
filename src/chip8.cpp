#include "chip8.h"
#include <cstdlib>
#include <stdexcept>
#include <sstream>

namespace CodexMachina
{
  void Chip8::emulateCycle()
  {
    unsigned short opcode = _memory[_pc++] << 8;
    opcode |= _memory[_pc++];

    if ((opcode & 0xF000) == 0x0000)
    {
      // Calls RCA 1802 program at address (opcode & 0x0FFF)
    }
    else if (opcode == 0x00E0)
    {
      // Clear the screen
    }
    else if (opcode == 0x00EE)
    {
      if (0 == _sp)
      {
        throw std::logic_error("The stack is empty.");
      }

      _pc = _stack[_sp--];
    }
    else if ((opcode & 0xF000) == 0x1000)
    {
      _pc = opcode & 0x0FFF;
    }
    else if ((opcode & 0xF000) == 0x2000)
    {
      _stack[_sp++] = _pc;
      _pc = opcode & 0x0FFF;
    }
    else if ((opcode & 0xF000) == 0x3000)
    {
      if (_V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) { _pc += 2; }
    }
    else if ((opcode & 0xF000) == 0x4000)
    {
      if (_V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) { _pc += 2; }
    }
    else if ((opcode & 0xF00F) == 0x5000)
    {
      if (_V[(opcode & 0x0F00) >> 8] == _V[(opcode & 0x00F0) >> 4]) { _pc += 2; }
    }
    else if ((opcode & 0xF000) == 0x6000)
    {
      _V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
    }
    else if ((opcode & 0xF000) == 0x7000)
    {
      _V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
    }
    else if ((opcode & 0xF00F) == 0x8000)
    {
      _V[(opcode & 0x0F00) >> 8] = _V[(opcode & 0x00F0) >> 4];
    }
    else if ((opcode & 0xF00F) == 0x8001)
    {
      _V[(opcode & 0x0F00) >> 8] |= _V[(opcode & 0x00F0) >> 4];
    }
    else if ((opcode & 0xF00F) == 0x8002)
    {
      _V[(opcode & 0x0F00) >> 8] &= _V[(opcode & 0x00F0) >> 4];
    }
    else if ((opcode & 0xF00F) == 0x8003)
    {
      _V[(opcode & 0x0F00) >> 8] ^= _V[(opcode & 0x00F0) >> 4];
    }
    else if ((opcode & 0xF00F) == 0x8004)
    {
      auto vx = static_cast<unsigned short>(_V[(opcode & 0x0F00) >> 8]);
      auto vy = static_cast<unsigned short>(_V[(opcode & 0x00F0) >> 4]);
      ((vx + vy) > 0xFF) ? _V[0xF] = 1 : _V[0xF] = 0;
      _V[(opcode & 0x0F00) >> 8] += vy;
    }
    else if ((opcode & 0xF00F) == 0x8005)
    {
      auto vx = _V[(opcode & 0x0F00) >> 8];
      auto vy = _V[(opcode & 0x00F0) >> 4];
      (vy > vx) ? _V[0xF] = 0 : _V[0xF] = 1;
      _V[(opcode & 0x0F00) >> 8] -= vy;
    }
    else if ((opcode & 0xF00F) == 0x8006)
    {
      auto vx = _V[(opcode & 0x0F00) >> 8];
      _V[0xF] = (vx & 0x01);
      _V[(opcode & 0x0F00) >> 8] = vx >> 1;
    }
    else if ((opcode & 0xF00F) == 0x8007)
    {
      auto vx = _V[(opcode & 0x0F00) >> 8];
      auto vy = _V[(opcode & 0x00F0) >> 4];
      (vx > vy) ? _V[0xF] = 0 : _V[0xF] = 1;
      _V[(opcode & 0x0F00) >> 8] = vy - vx;
    }
    else if ((opcode & 0xF00F) == 0x800E)
    {
      auto vx = _V[(opcode & 0x0F00) >> 8];
      _V[0xF] = (vx & 0x80) >> 7;
      _V[(opcode & 0x0F00) >> 8] = vx << 1;
    }
    else if ((opcode & 0xF00F) == 0x9000)
    {
      if (_V[(opcode & 0x0F00) >> 8] != _V[(opcode & 0x00F0) >> 4]) { _pc += 2; }
    }
    else if ((opcode & 0xF000) == 0xA000)
    {
      _I = (opcode & 0x0FFF);
    }
    else if ((opcode & 0xF000) == 0xB000)
    {
      _pc = _V[0] + (opcode & 0x0FFF);
    }
    else if ((opcode & 0xF000) == 0xC000)
    {
      _V[(opcode & 0x0F00) >> 8] = (std::rand() % 256) & (opcode & 0x00FF);
    }
    else if ((opcode & 0xF000) == 0xD000)
    {
      unsigned char newVF = 0x0;
      const auto vx = _V[(opcode & 0x0F00) >> 8];
      const auto vy = _V[(opcode & 0x00F0) >> 4];
      const unsigned char n = (opcode & 0x000F);

      for (unsigned char i = 0; i < n; ++i)
      {
        const auto byte = _memory[_I + i];

        for (int j = 0; j < 8; ++j)
        {
          const auto bit = static_cast<unsigned char>((byte >> (7 - j)) & 0x01);
          const auto displayXIndex = (vx + j) % X;
          const auto displayYIndex = ((vy + n) % Y) * X;
          const auto currentValue = _display[displayXIndex + displayYIndex];
          const auto newValue = currentValue % bit;
          if (currentValue != newValue) { newVF = 0x01; }
          _display[displayXIndex + displayYIndex] = newValue;
        }
      }
    }
    else if ((opcode & 0xF0FF) == 0xE09E)
    {
      const auto keyIndex = _V[(opcode & 0x0F00) >> 8];
      if (1 == _keys[keyIndex]) { _pc += 2; }
    }
    else if ((opcode & 0xF0FF) == 0xE0A1)
    {
      const auto keyIndex = _V[(opcode & 0x0F00) >> 8];
      if (0 == _keys[keyIndex]) { _pc += 2; }
    }
  }

  void Chip8::loadMemory(const std::vector<unsigned char> data, unsigned short offset)
  {
    if (offset > MEMORY_SIZE)
    {
      std::stringstream ss{ std::ios::out };
      ss << std::hex << std::showbase << offset << " is greater than " <<
        std::hex << std::showbase << MEMORY_SIZE;
      throw std::invalid_argument(ss.str());
    }

    for (size_t i = 0; i < data.size(); ++i)
    {
      _memory[offset + i] = data[i];
    }
  }

  void Chip8::reset()
  {
    _delayTimer = 0;
    _display.fill(0);
    _keys.fill(0);
    _I = 0;
    _memory.fill(0);
    _pc = 0x200;
    _soundTimer = 0;
    _sp = 0;
    _stack.fill(0);
    _V.fill(0);
  }
} // namespace CodexMachina