#pragma once

#include <QWidget>
#include <QKeyEvent>
#include "block.h"
#include"human.h"
#include<vector>

class Game : public QWidget {

public:
    Game(QWidget *parent = 0);
    ~Game();

protected:
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
    void mousePressEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void drawObjects(QPainter *);

    void moveObjects();
    void checkCollision();
    bool checkСoordinates(int,int);
    std::pair<int, int> convertPixelsToCoordinates(int,int);

private:
    int timerId;
    static const int N_OF_BLOCKS = 300;
    static const int ROWS_OF_BLOCKS = 15;
    static const int COLUMNS_OF_BLOCKS = 20;
    static const int DELAY = 10;

    std::vector<Block> blocks;
    Human* human;
};
