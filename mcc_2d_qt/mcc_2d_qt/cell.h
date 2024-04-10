#pragma once

#include <QImage>
#include <QRect>

#include "block.h"

class Cell
{
private:
    BlockType type_;
    int count_;
    bool main_;

    QImage image_;
    QRect rect_;

    static const int SIZE_ = 40;
public:
    Cell(int x, int y, BlockType blockType, int countBlocks, bool main);

    QRect GetRect();
    QImage & GetImage();
    BlockType GetBlockType();
    bool GetMain();
    void SetRect(QRect);
    void SetType(BlockType);
    void SetMain(bool);

    static int GetSize() {
        return SIZE_;
    };
};
