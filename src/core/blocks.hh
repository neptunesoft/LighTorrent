#include <iostream>
#include <stdint.h>
#include <vector>

namespace Core
{
  class Blocks
  {
    public:
      // Ctor & Dtor
      Blocks(int pieceLength);
      ~Blocks();

      int getBlockOffset();
      void setBlockData(uint32_t block, const std::string& data);

      bool isFull() const;
      void setFull(bool full);
      bool isWaiting() const;
      long long int getSize() const;

    private:
      bool isFull_;
      bool isWaiting_; // waiting for return of peer messages (requested is full)
      std::vector<bool> blocks_;
      std::vector<bool> requested_;
      std::vector<std::string> data_;
  };
}