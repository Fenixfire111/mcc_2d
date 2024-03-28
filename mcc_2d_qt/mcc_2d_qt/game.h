#pragma once

#include <QWidget>
#include <QKeyEvent>
#include "block.h"
#include "human.h"
#include <vector>
#include <memory>

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
    std::pair<int, int> convertPixelsToCoordinates(int xp, int yp, bool enableCorrections = true);
    void moveWorld();

private:
    int worldDx;
    int timerId;
    int correctionX;
    static const int N_OF_BLOCKS = 1500;
    static const int ROWS_OF_BLOCKS = 15;
    static const int COLUMNS_OF_BLOCKS = 100;
    static const int DELAY = 10;

    // std::vector<Block> blocks;
    std::vector<std::vector<Block>> blocks;
    std::unique_ptr<Human> human;
};
