#include "Geese.h"

Geese::Geese() : tile{-1} {}

void Geese::moveGeese(int tile) {
    this->tile = tile;
}

int Geese::whereIsit() {
    return tile;
}
