#include "chip8.h"
#include <cstdlib>
#include <sstream>

namespace CodexMachina
{
  void Chip8::emulateCycle()
  {
    if (_blocked) return;
    _makeSound = false;
    unsigned short opcode = _memory[_pc++] << 8;
    opcode |= _memory[_pc++];

    if ((opcode & 0xF000) == 0x0000)
    {
      // Calls RCA 1802 program at address (opcode & 0x0FFF)
      // Intentionally ignored
    }
    else if (opcode == 0x00E0)
    {
      _display.fill(0);
    }
    else if (opcode == 0x00EE)
    {
      if (0 == _sp) { throw std::logic_error{ "The stack is empty." }; }
      _pc = _stack[_sp--];
    }
    else if ((opcode & 0xF000) == 0x1000)
    {
      _pc = opcode & 0x0FFF;
    }
    else if ((opcode & 0xF000) == 0x2000)
    {
      if (_stack.size() == _sp) { throw std::logic_error{ "The stack is full." }; }
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
      const auto vx = static_cast<unsigned short>(_V[(opcode & 0x0F00) >> 8]);
      const auto vy = static_cast<unsigned short>(_V[(opcode & 0x00F0) >> 4]);
      ((vx + vy) > 0xFF) ? _V[0xF] = 1 : _V[0xF] = 0;
      _V[(opcode & 0x0F00) >> 8] += vy;
    }
    else if ((opcode & 0xF00F) == 0x8005)
    {
      const auto vx = _V[(opcode & 0x0F00) >> 8];
      const auto vy = _V[(opcode & 0x00F0) >> 4];
      (vy > vx) ? _V[0xF] = 0 : _V[0xF] = 1;
      _V[(opcode & 0x0F00) >> 8] -= vy;
    }
    else if ((opcode & 0xF00F) == 0x8006)
    {
      const auto vx = _V[(opcode & 0x0F00) >> 8];
      _V[0xF] = (vx & 0x01);
      _V[(opcode & 0x0F00) >> 8] = vx >> 1;
    }
    else if ((opcode & 0xF00F) == 0x8007)
    {
      const auto vx = _V[(opcode & 0x0F00) >> 8];
      const auto vy = _V[(opcode & 0x00F0) >> 4];
      (vx > vy) ? _V[0xF] = 0 : _V[0xF] = 1;
      _V[(opcode & 0x0F00) >> 8] = vy - vx;
    }
    else if ((opcode & 0xF00F) == 0x800E)
    {
      const auto vx = _V[(opcode & 0x0F00) >> 8];
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
    else if ((opcode & 0xF0FF) == 0xF007)
    {
      const auto vIndex = (opcode & 0x0F00) >> 8;
      _V[vIndex] = _delayTimer;
    }
    else if ((opcode & 0xF0FF) == 0xF00A)
    {
      _blocked = true;
      const auto vIndex = (opcode & 0x0F00) >> 8;

      for (unsigned char keyIndex = 0; keyIndex < _keys.size(); ++keyIndex)
      {
        if (_keys[keyIndex] == 0) continue;
        _blocked = false;
        _V[vIndex] = keyIndex;
      }
    }
    else if ((opcode & 0xF0FF) == 0xF015)
    {
      const auto vIndex = (opcode & 0x0F00) >> 8;
      _delayTimer = _V[vIndex];
    }
    else if ((opcode & 0xF0FF) == 0xF018)
    {
      const auto vIndex = (opcode & 0x0F00) >> 8;
      _soundTimer = _V[vIndex];
    }
    else if ((opcode & 0xF0FF) == 0xF01E)
    {
      const auto vIndex = (opcode & 0x0F00) >> 8;
      _I += _V[vIndex];
    }
    else if ((opcode & 0xF0FF) == 0xF029)
    {
      const auto spriteIndex = (opcode & 0x0F00) >> 8;
      _I = _spriteAddrs[spriteIndex];
    }
    else if ((opcode & 0xF0FF) == 0xF033)
    {
      const auto vIndex = (opcode & 0x0F00) >> 8;
      const auto vx = _V[vIndex];
      _memory[_I] = vx / 100;
      _memory[_I + 1] = (vx / 10) - (_memory[_I] * 10);
      _memory[_I + 2] = vx - (_memory[_I] * 100) - (_memory[_I + 1] * 10);
    }
    else if ((opcode & 0xF0FF) == 0xF055)
    {
      const auto vIndex = (opcode & 0x0F00) >> 8;
      for (size_t i = 0; i <= vIndex; ++i) { _memory[_I + i] = _V[i]; }
    }
    else if ((opcode & 0xF0FF) == 0xF065)
    {
      const auto vIndex = (opcode & 0x0F00) >> 8;
      for (size_t i = 0; i <= vIndex; ++i) { _V[i] = _memory[_I + i]; }
    }
    else
    {
      std::stringstream ss{ std::ios::out };
      ss << "Invalid opcode: " << std::hex << std::showbase << opcode;
      throw std::logic_error{ ss.str() };
    }

    if (_delayTimer > 0) { --_delayTimer; }

    if (_soundTimer > 0)
    {
      --_soundTimer;
      if (0 == _soundTimer) { _makeSound = true; }
    }
  }

  /*
  unsigned char chip8Fontset[80] =
  {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };
  */

  void Chip8::loadFont(const unsigned short offset)
  {
    _spriteAddrs[0] = offset + 0;
    _memory[offset + 0] = 0xF0;
    _memory[offset + 1] = 0x90;
    _memory[offset + 2] = 0x90;
    _memory[offset + 3] = 0x90;
    _memory[offset + 4] = 0xF0;
    _spriteAddrs[1] = offset + 5;
    _memory[offset + 5] = 0x20;
    _memory[offset + 6] = 0x60;
    _memory[offset + 7] = 0x20;
    _memory[offset + 8] = 0x20;
    _memory[offset + 9] = 0x70;
    _spriteAddrs[2] = offset + 10;
    _memory[offset + 10] = 0xF0;
    _memory[offset + 11] = 0x10;
    _memory[offset + 12] = 0xF0;
    _memory[offset + 13] = 0x80;
    _memory[offset + 14] = 0xF0;
    _spriteAddrs[3] = offset + 15;
    _memory[offset + 15] = 0xF0;
    _memory[offset + 16] = 0x10;
    _memory[offset + 17] = 0xF0;
    _memory[offset + 18] = 0x10;
    _memory[offset + 19] = 0xF0;
    _spriteAddrs[4] = offset + 20;
    _memory[offset + 20] = 0x90;
    _memory[offset + 21] = 0x90;
    _memory[offset + 22] = 0xF0;
    _memory[offset + 23] = 0x10;
    _memory[offset + 24] = 0xF0;
    _spriteAddrs[5] = offset + 25;
    _memory[offset + 25] = 0xF0;
    _memory[offset + 26] = 0x80;
    _memory[offset + 27] = 0xF0;
    _memory[offset + 28] = 0x10;
    _memory[offset + 29] = 0xF0;
    _spriteAddrs[6] = offset + 30;
    _memory[offset + 30] = 0xF0;
    _memory[offset + 31] = 0x80;
    _memory[offset + 32] = 0xF0;
    _memory[offset + 33] = 0x90;
    _memory[offset + 34] = 0xF0;
    _spriteAddrs[7] = offset + 35;
    _memory[offset + 35] = 0xF0;
    _memory[offset + 36] = 0x10;
    _memory[offset + 37] = 0x20;
    _memory[offset + 38] = 0x40;
    _memory[offset + 39] = 0x40;
    _spriteAddrs[8] = offset + 40;
    _memory[offset + 40] = 0xF0;
    _memory[offset + 41] = 0x90;
    _memory[offset + 42] = 0xF0;
    _memory[offset + 43] = 0x90;
    _memory[offset + 44] = 0xF0;
    _spriteAddrs[9] = offset + 45;
    _memory[offset + 45] = 0xF0;
    _memory[offset + 46] = 0x90;
    _memory[offset + 47] = 0xF0;
    _memory[offset + 48] = 0x10;
    _memory[offset + 49] = 0xF0;
    _spriteAddrs[10] = offset + 50;
    _memory[offset + 50] = 0xF0;
    _memory[offset + 51] = 0x90;
    _memory[offset + 52] = 0xF0;
    _memory[offset + 53] = 0x90;
    _memory[offset + 54] = 0x90;
    _spriteAddrs[11] = offset + 55;
    _memory[offset + 55] = 0xE0;
    _memory[offset + 56] = 0x90;
    _memory[offset + 57] = 0xE0;
    _memory[offset + 58] = 0x90;
    _memory[offset + 59] = 0xE0;
    _spriteAddrs[12] = offset + 60;
    _memory[offset + 60] = 0xF0;
    _memory[offset + 61] = 0x80;
    _memory[offset + 62] = 0x80;
    _memory[offset + 63] = 0x80;
    _memory[offset + 64] = 0xF0;
    _spriteAddrs[13] = offset + 65;
    _memory[offset + 65] = 0xE0;
    _memory[offset + 66] = 0x90;
    _memory[offset + 67] = 0x90;
    _memory[offset + 68] = 0x90;
    _memory[offset + 69] = 0xE0;
    _spriteAddrs[14] = offset + 70;
    _memory[offset + 70] = 0xF0;
    _memory[offset + 71] = 0x80;
    _memory[offset + 72] = 0xF0;
    _memory[offset + 73] = 0x80;
    _memory[offset + 74] = 0xF0;
    _spriteAddrs[15] = offset + 75;
    _memory[offset + 75] = 0xF0;
    _memory[offset + 76] = 0x80;
    _memory[offset + 77] = 0xF0;
    _memory[offset + 78] = 0x80;
    _memory[offset + 79] = 0x80;
  }

  void Chip8::loadMemory(const std::vector<unsigned char> data, const size_t offset)
  {
    if (offset > MEMORY_SIZE)
    {
      std::stringstream ss{ std::ios::out };
      ss << std::hex << std::showbase << offset << " is greater than " <<
        std::hex << std::showbase << MEMORY_SIZE;
      throw std::invalid_argument{ ss.str() };
    }

    for (size_t i = 0; i < data.size(); ++i)
    {
      _memory[offset + i] = data[i];
    }
  }

  void Chip8::reset()
  {
    _blocked = false;
    _delayTimer = 0;
    _display.fill(0);
    _keys.fill(0);
    _I = 0;
    _makeSound = false;
    _memory.fill(0);
    _pc = 0x200;
    _soundTimer = 0;
    _sp = 0;
    _stack.fill(0);
    _V.fill(0);
    loadFont();
  }
} // namespace CodexMachina