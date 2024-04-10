#pragma once

#include <QImage>
#include <QRect>

enum class BlockType {
    EMPTY,
    GRASS_BLOCK,
    DIRT,
    STONE,
    BEDROCK,
    COBBLESTONE,
    TREE
};

class Block {

private:
    BlockType type;
    QImage image;
    QRect rect;
    bool destroyed;
    static const int SIZE = 50;

public:
    Block();
    Block(int, int, BlockType);
    ~Block();

public:
    bool isDestroyed();
    QRect getRect();
    QImage & getImage();
    BlockType getType();
    void setDestroyed(bool);
    void setRect(QRect);
    void setType(BlockType);
    void move(int,int);
    static int getSize() {
        return SIZE;
    };


};
