#pragma once

#include "cell.h"
#include <vector>

class Inventory {
private:
    int countCells;
    int mainCell;

    std::vector<Cell> cells;
public:
    Inventory();

    Cell At(int i);
    int GetCountCells();
    Cell GetMainCell();
    void SetMainCell(int i);
};


