#include "Spark/Sparky/AST.hpp"

namespace Spark::Sparky{

    Block::Block() = default;
    Block::~Block() = default;
    Block::Block(Block&&) noexcept = default;
    Block& Block::operator=(Block&&) noexcept = default;

}