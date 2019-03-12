#include "chip8.h"

namespace CodexMachina
{
  void Chip8::loadMemory(const std::vector<unsigned char> data, unsigned short offset)
  {
    for (size_t i = 0; i < data.size(); ++i)
    {
      _memory[offset + i] = data[i];
    }
  }

  void Chip8::reset()
  {
    _pc = 0x200;
  }
} // namespace CodexMachina