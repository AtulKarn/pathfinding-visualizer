#include <QApplication>
#include<bits/stdc++.h>

#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.initialize_table();
    w.showMaximized();
    return a.exec();
}
