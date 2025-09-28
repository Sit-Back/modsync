#include "ui/WelcomePage.h"

#include <QApplication>
#include <QStyleFactory>

#include "ui/MainWizard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));
    MainWizard wizard;
    return a.exec();
}
