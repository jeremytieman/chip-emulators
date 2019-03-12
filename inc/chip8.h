#include <array>
#include <vector>

namespace CodexMachina
{
  class Chip8
  {
  public:
    void loadMemory(const std::vector<unsigned char> data, const unsigned short offset);
    void reset();
  private:
    constexpr static size_t MEMORY_SIZE{ 0x1000 };

    unsigned short _I;
    unsigned char _delayTimer;
    std::array<unsigned char, MEMORY_SIZE> _memory;
    unsigned char _soundTimer;
    unsigned short _pc;
    std::array<unsigned char, 16> _V;
  };
} // namespace CodexMachina