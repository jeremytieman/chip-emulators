#include <array>
#include <vector>

namespace CodexMachina
{
  class Chip8
  {
  public:
    Chip8() { reset(); }
    void emulateCycle();
    void loadFont();
    void loadMemory(const std::vector<unsigned char> data, const unsigned short offset);
    void reset();
  private:
    constexpr static size_t MEMORY_SIZE{ 0x1000 };
    constexpr static size_t X{ 64 };
    constexpr static size_t Y{ 32 };

    bool _blocked = false;
    unsigned char _delayTimer;
    std::array<unsigned char, X * Y> _display;
    unsigned short _I;
    std::array<unsigned char, 16> _keys;
    std::array<unsigned char, MEMORY_SIZE> _memory;
    unsigned short _pc;
    unsigned char _soundTimer;
    unsigned char _sp;
    std::array<unsigned short, 16> _spriteAddrs;
    std::array<unsigned short, 16> _stack;
    std::array<unsigned char, 16> _V;
  };
} // namespace CodexMachina