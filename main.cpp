#include <QCoreApplication>


#include "app.h"


void startVoice();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    App* app = new App(&a);
    app->run();

    return a.exec();
}

