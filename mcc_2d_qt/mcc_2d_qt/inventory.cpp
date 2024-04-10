#include "inventory.h"

Inventory::Inventory() {
    countCells = 7;
    mainCell = 0;

    for (int i = 0; i < 7; ++i) {
        cells.emplace_back(320 + i*Cell::GetSize(), 710, static_cast<BlockType>(i), 0, i == mainCell);
    }
}

Cell Inventory::At(int i) {
    return cells[i];
}

int Inventory::GetCountCells() {
    return countCells;
}

Cell Inventory::GetMainCell() {
    return cells[mainCell];
}

void Inventory::SetMainCell(int i) {
    cells[mainCell].SetMain(false);
    cells[i].SetMain(true);

    mainCell = i;
}
