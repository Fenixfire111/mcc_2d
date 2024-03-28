#pragma once

#include <QImage>
#include <QRect>

class Human {

public:
    Human();
    ~Human();

public:
    void resetState();
    void move();
    void setDx(int);
    void setDy(int);

    int getDx() const;
    int getDy() const;
    QRect getRect();
    QImage & getImage();

    int jump;
    bool stand;

private:
    QImage image;
    QRect rect;
    int dx;
    int dy;
    static const int INITIAL_X = 500;
    static const int INITIAL_Y = 500;

};
