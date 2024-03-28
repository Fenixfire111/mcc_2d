#pragma once

#include <QImage>
#include <QRect>

class Block {

public:
    Block(int, int);
    ~Block();

public:
    bool isDestroyed();
    void setDestroyed(bool);
    QRect getRect();
    void setRect(QRect);
    QImage & getImage();
    void move(int);

private:
    QImage image;
    QRect rect;
    bool destroyed;
};
