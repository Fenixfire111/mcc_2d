#include "cell.h"

Cell::Cell(int x, int y, BlockType blockType, int countBlocks, bool main) {
    type_ = blockType;
    count_ = 0;
    main_ = main;

    if (main_) {
        image_.load("images/mainCell_1.png");
    } else {
        image_.load("images/cell.png");
    }
    rect_ = image_.rect();
    rect_.translate(x, y);
}

QRect Cell::GetRect() {

    return rect_;
}

void Cell::SetRect(QRect rect) {

    rect_ = rect;
}

QImage & Cell::GetImage() {

    return image_;
}

BlockType Cell::GetBlockType() {
    return type_;
}

bool Cell::GetMain() {
    return main_;
}

void Cell::SetType(BlockType type) {
    type_ = type;
}

void Cell::SetMain(bool main) {
    main_ = main;
    if (main_) {
        image_.load("images/mainCell_1.png");
    } else {
        image_.load("images/cell.png");
    }
}
