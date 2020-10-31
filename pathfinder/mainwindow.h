#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include<QMainWindow>
#include<math.h>
#include<QStack>
#include<bits/stdc++.h>
#include<QMap>
#include<QTimer>
#include<QMultiMap>
#include<QElapsedTimer>
#include<QDebug>
#include<qrandom.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:




    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int Row, Col, i, j;

    typedef QPair<int,int> Pair ;
    typedef QPair<double, QPair<int,int>> Pair2 ;

    Pair Src, p;
    Pair Dest;
    double gNew, hNew, fNew;


    struct cell
    {
        int parent_i, parent_j;
        double f, g, h;
    };

    int **grid;
    bool **closedSet;
    cell **cellDetails;

    QStack<Pair> path;
    QMultiMap<double, Pair> openSet;
    bool foundDest;
    int row,col,temp_row,temp_col;
    int mode;
    QElapsedTimer time_elapsed;
    int heuristic;
    QRandomGenerator randomwall;
    int randomdensity=6;

    void initialize_table();
    void makegrid();
     bool isValid(int row, int col);
     bool isNotWall(int row, int col);
     bool isDestination(int row, int col);
     double calculateHValue(int row, int col);
     void tracePath();
     void aStar();

private slots:
    void on_tableWidget_cellClicked(int row, int column);

    void on_tableWidget_cellEntered(int row, int column);

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

public slots:
    void aStar_loop();

private:
    Ui::MainWindow *ui;
    QTimer *timer;

};
#endif // MAINWINDOW_H
