#include <QCoreApplication>

#include <QApplication>
#include <QWidget>

#include "game.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    Game window;

    window.resize(1000, 750);
    window.setWindowTitle("MCC_2D");
    window.show();

    return app.exec();
}
