#include "ui/WelcomePage.h"

#include <QApplication>

#include "ui/MainWizard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWizard wizard;
    return a.exec();
}
