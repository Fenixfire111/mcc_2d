#pragma once

#include <QWidget>
#include <QKeyEvent>
#include "block.h"
#include "human.h"
#include "inventory.h"
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
    void StandardWorldGeneration();

private:
    int worldDx;
    int worldDy;
    int timerId;
    int correctionX;
    int correctionY;

    Inventory inventory;

    static const int ROWS_OF_BLOCKS = 100;
    static const int COLUMNS_OF_BLOCKS = 200;
    static const int NORMALIZATION_X = 5000;
    static const int NORMALIZATION_Y = 2000;
    static const int GROUND_LINE = 50;
    static const int DELAY = 10;

    std::vector<std::vector<Block>> blocks;
    std::unique_ptr<Human> human;
};

// 1)сдвиг экрана вверх
// 2)залипания клавиш при прыжке
// 3)полет в космос на верху + ласт слой блоков
