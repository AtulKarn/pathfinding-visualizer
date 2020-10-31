#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Path Visualization for A* and Dijkstra's");
    ui->radioButton->setChecked(true);
    ui->radioButton_3->setChecked(true);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(aStar_loop()));
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initialize_table(){

    Row = ui->tableWidget->rowCount();
    Col = ui->tableWidget->columnCount();
    Src = qMakePair(0,0);
    Dest = qMakePair(Row-1, Col-1);

    for (int i=0; i <Row;i++)
    {
        for(int j=0; j < Col;j++){
            QTableWidgetItem *newItem = new QTableWidgetItem();
            ui->tableWidget->setItem(i, j, newItem);
            ui->tableWidget->item(i,j)->setBackground(QColor(255,255,255));
        }
    }

    ui->tableWidget->item(Src.first,Src.second)->setBackground(QColor(0,255,0));
    ui->tableWidget->item(Dest.first,Dest.second)->setBackground(QColor(255,0,0));



    closedSet = new bool*[Row];
    for(int i=0;i<Row;i++)
        closedSet[i] = new bool[Col];

    cellDetails = new cell*[Row];
    for(int i=0;i<Row;i++)
        cellDetails[i] = new cell[Col];

    grid = new int*[Row];
    for(int i=0;i<Row;i++)
        grid[i] = new int[Col];

}


void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    if (ui->tableWidget->item(row,column)->background() == QColor(10,10,10)
            && (ui->tableWidget->item(row,column)->background() != QColor(0,255,0)
            && ui->tableWidget->item(row,column)->background() != QColor(255,0,0))){
        ui->tableWidget->item(row,column)->setBackground(QColor(255,255,255));
    }

    else if (ui->tableWidget->item(row,column)->background() != QColor(0,255,0)
             && ui->tableWidget->item(row,column)->background() != QColor(255,0,0)){
        ui->tableWidget->item(row,column)->setBackground(QColor(10,10,10));
    }
}



void MainWindow::on_tableWidget_cellEntered(int row, int column)
{
    if ((ui->tableWidget->item(row,column)->background() == QColor(10,10,10))
            && (ui->tableWidget->item(row,column)->background() != QColor(0,255,0)
            && ui->tableWidget->item(row,column)->background() != QColor(255,0,0))){
        ui->tableWidget->item(row,column)->setBackground(QColor(255,255,255));
    }


    else if (ui->tableWidget->item(row,column)->background() != QColor(0,255,0)
             && ui->tableWidget->item(row,column)->background() != QColor(255,0,0)){
        ui->tableWidget->item(row,column)->setBackground(QColor(10,10,10));
    }
}


void MainWindow::makegrid()
{
    //0 is blocked and 1 is unblocked
    for (i=0; i < Row;i++)
    {
        for(j=0; j < Col;j++){
            if(ui->tableWidget->item(i,j)->background() == QColor(10,10,10))
                grid[i][j] = 0;


            else if (ui->tableWidget->item(i,j)->background() == QColor(0,0,255))
                    {ui->tableWidget->item(i,j)->setBackground(QColor(255,255,255));
                grid[i][j] = 1;
            }


            else if (ui->tableWidget->item(i,j)->background() == QColor(10,100,10))
                    {ui->tableWidget->item(i,j)->setBackground(QColor(255,255,255));
                grid[i][j] = 1;
            }


            else
                grid[i][j] = 1;


            cellDetails[i][j].f = FLT_MAX;
            cellDetails[i][j].g = FLT_MAX;
            cellDetails[i][j].h = FLT_MAX;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;

            closedSet[i][j] = false;
        }
    }
}




bool MainWindow::isValid(int row, int col)
{
    return (row>=0) && (row<Row) && (col>=0) && (col<Col);
}



bool MainWindow::isNotWall(int row, int col)
{
    if (grid[row][col] == 1)
            return (true);
        else
            return (false);
}



bool MainWindow::isDestination(int row, int col)
{
    if (row == Dest.first && col == Dest.second)
        return (true);
    else
        return (false);
}



double MainWindow::calculateHValue(int row, int col)
{
    if (mode==1)
    {
        if (heuristic == 1)
            return (double)(abs(row-Dest.first)+abs(col-Dest.second));
        else
            return ((double)sqrt ((row-Dest.first)*(row-Dest.first) + (col-Dest.second)*(col-Dest.second)));  //euclidean
    }
    else
        return 0.0;
}



void MainWindow::tracePath()
{
    row = Dest.first;
    col = Dest.second;


    while (!(cellDetails[row][col].parent_i == row
             && cellDetails[row][col].parent_j == col ))
    {
        path.push (qMakePair(row, col));
        temp_row = cellDetails[row][col].parent_i;
        temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }

    path.push (qMakePair(row, col));
    while (!path.empty())
    {
        p = path.top();
        path.pop();
        if (!(p.first==Src.first && p.second==Src.second) && !(p.first==Dest.first && p.second==Dest.second))
        ui->tableWidget->item(p.first,p.second)->setBackground(QColor(0,0,255));
    }
    return;
}




void MainWindow::aStar()
{
//Initializing source details
    i = Src.first;
    j = Src.second;
        cellDetails[i][j].f = 0.0;
        cellDetails[i][j].g = 0.0;
        cellDetails[i][j].h = 0.0;
        cellDetails[i][j].parent_i = i;
        cellDetails[i][j].parent_j = j;
    openSet.clear();
    openSet.insert(0.0, qMakePair (i, j));
    foundDest = false;
        timer->start(100-1*ui->horizontalSlider->value());


}




void MainWindow::on_pushButton_2_clicked()
{
    if (ui->radioButton->isChecked())
    {
        mode=1;
    }
    if (ui->radioButton_2->isChecked())
    {
        mode=2;
    }
    if (ui->radioButton_3->isChecked())
    {
        heuristic=1;
    }
    if (ui->radioButton_4->isChecked())
    {
        heuristic=2;
    }
    makegrid();
    ui->label_4->setStyleSheet("QLabel{color: black;}");
    ui->label_5->setStyleSheet("QLabel{color: black;}");
    ui->label_4->setText("Searching for path");
    ui->label_5->setText("...");
    time_elapsed.start();
    aStar();


}


void MainWindow::on_pushButton_clicked()
{
    timer->stop();

    for (int i=0; i <Row;i++)
    {
        for(int j=0; j < Col;j++){
            ui->tableWidget->item(i, j)->setBackground(QColor(255,255,255));
        }
    }

    ui->tableWidget->item(Src.first,Src.second)->setBackground(QColor(0,255,0));
    ui->tableWidget->item(Dest.first,Dest.second)->setBackground(QColor(255,0,0));
    ui->label_4->setStyleSheet("QLabel{color: black;}");
    ui->label_5->setStyleSheet("QLabel{color: black;}");
    ui->label_4->setText("Welcome");
    ui->label_5->setText("...");
    ui->radioButton->setChecked(true);
    ui->radioButton_3->setChecked(true);
}




void MainWindow::aStar_loop()
{
    if(openSet.empty())
    {
        timer->stop();
        ui->label_4->setText("Destination not found. Time taken:");
        ui->label_5->setText(QVariant(time_elapsed.elapsed()).toString());
        ui->label_4->setStyleSheet("QLabel{color: red;}");
        ui->label_5->setStyleSheet("QLabel{color: red;}");
    }

    p = openSet.begin().value();
    openSet.erase(openSet.begin());

    i = p.first;
    j = p.second;
    closedSet[i][j] = true;


    if (isValid(i-1, j) == true)
            {
                if (isDestination(i-1, j) == true)
                {
                    timer->stop();
                    ui->label_4->setText("Destination found. Time taken:");
                    ui->label_5->setText(QVariant(time_elapsed.elapsed()).toString());
                    ui->label_4->setStyleSheet("QLabel{color: green;}");
                    ui->label_5->setStyleSheet("QLabel{color: green;}");


                    cellDetails[i-1][j].parent_i = i;
                    cellDetails[i-1][j].parent_j = j;
                    qDebug ("The destination cell is found\n");
                    tracePath ();
                    foundDest = true;
                    return;
                }
                else if (closedSet[i-1][j] == false &&
                         isNotWall(i-1, j) == true)
                {
                    gNew = cellDetails[i][j].g + 1.0;
                    hNew = calculateHValue (i-1, j);
                    fNew = gNew + hNew;

                    if (cellDetails[i-1][j].f == FLT_MAX ||
                            cellDetails[i-1][j].f > fNew)
                    {
                        openSet.insert(fNew, qMakePair(i-1, j));

                        cellDetails[i-1][j].f = fNew;
                        cellDetails[i-1][j].g = gNew;
                        cellDetails[i-1][j].h = hNew;
                        cellDetails[i-1][j].parent_i = i;
                        cellDetails[i-1][j].parent_j = j;
                        ui->tableWidget->item(i-1,j)->setBackground(QColor(10,100,10));

                    }
                }
            }

            if (isValid(i+1, j) == true)
            {
                if (isDestination(i+1, j) == true)
                {
                    timer->stop();
                    ui->label_4->setText("Destination found. Time taken:");
                    ui->label_5->setText(QVariant(time_elapsed.elapsed()).toString());
                    ui->label_4->setStyleSheet("QLabel{color: green;}");
                    ui->label_5->setStyleSheet("QLabel{color: green;}");


                    cellDetails[i+1][j].parent_i = i;
                    cellDetails[i+1][j].parent_j = j;
                    qDebug("The destination cell is found\n");
                    tracePath();
                    foundDest = true;
                    return;
                }
                else if (closedSet[i+1][j] == false &&
                         isNotWall(i+1, j) == true)
                {
                    gNew = cellDetails[i][j].g + 1.0;
                    hNew = calculateHValue(i+1, j);
                    fNew = gNew + hNew;

                    if (cellDetails[i+1][j].f == FLT_MAX ||
                            cellDetails[i+1][j].f > fNew)
                    {
                        openSet.insert(fNew, qMakePair (i+1, j));
                        // Update the details of this cell
                        cellDetails[i+1][j].f = fNew;
                        cellDetails[i+1][j].g = gNew;
                        cellDetails[i+1][j].h = hNew;
                        cellDetails[i+1][j].parent_i = i;
                        cellDetails[i+1][j].parent_j = j;
                        ui->tableWidget->item(i+1,j)->setBackground(QColor(10,100,10));

                    }
                }
            }


            if (isValid (i, j+1) == true)
            {
                if (isDestination(i, j+1) == true)
                {
                    timer->stop();
                    ui->label_4->setText("Destination found. Time taken:");
                    ui->label_5->setText(QVariant(time_elapsed.elapsed()).toString());
                    ui->label_4->setStyleSheet("QLabel{color: green;}");
                    ui->label_5->setStyleSheet("QLabel{color: green;}");


                    cellDetails[i][j+1].parent_i = i;
                    cellDetails[i][j+1].parent_j = j;
                    qDebug("The destination cell is found\n");
                    tracePath();
                    foundDest = true;
                    return;
                }

                else if (closedSet[i][j+1] == false &&
                         isNotWall (i, j+1) == true)
                {
                    gNew = cellDetails[i][j].g + 1.0;
                    hNew = calculateHValue (i, j+1);
                    fNew = gNew + hNew;

                    if (cellDetails[i][j+1].f == FLT_MAX ||
                            cellDetails[i][j+1].f > fNew)
                    {
                        openSet.insert(fNew,
                                            qMakePair (i, j+1));

                        cellDetails[i][j+1].f = fNew;
                        cellDetails[i][j+1].g = gNew;
                        cellDetails[i][j+1].h = hNew;
                        cellDetails[i][j+1].parent_i = i;
                        cellDetails[i][j+1].parent_j = j;
                        ui->tableWidget->item(i,j+1)->setBackground(QColor(10,100,10));

                    }
                }
            }

            if (isValid(i, j-1) == true)
            {

                if (isDestination(i, j-1) == true)
                {
                    timer->stop();
                    ui->label_4->setText("Destination found. Time taken:");
                    ui->label_5->setText(QVariant(time_elapsed.elapsed()).toString());
                    ui->label_4->setStyleSheet("QLabel{color: green;}");
                    ui->label_5->setStyleSheet("QLabel{color: green;}");


                    cellDetails[i][j-1].parent_i = i;
                    cellDetails[i][j-1].parent_j = j;
                    qDebug("The destination cell is found\n");
                    tracePath();
                    foundDest = true;
                    return;
                }

                else if (closedSet[i][j-1] == false &&
                         isNotWall(i, j-1) == true)
                {
                    gNew = cellDetails[i][j].g + 1.0;
                    hNew = calculateHValue(i, j-1);
                    fNew = gNew + hNew;

                    if (cellDetails[i][j-1].f == FLT_MAX ||
                            cellDetails[i][j-1].f > fNew)
                    {
                        openSet.insert(fNew,
                                              qMakePair (i, j-1));

                        cellDetails[i][j-1].f = fNew;
                        cellDetails[i][j-1].g = gNew;
                        cellDetails[i][j-1].h = hNew;
                        cellDetails[i][j-1].parent_i = i;
                        cellDetails[i][j-1].parent_j = j;
                        ui->tableWidget->item(i,j-1)->setBackground(QColor(10,100,10));

                    }
                }
            }
            if(openSet.empty())
            {
                timer->stop();
                ui->label_4->setText("Destination not found. Time taken:");
                ui->label_5->setText(QVariant(time_elapsed.elapsed()).toString());
                ui->label_4->setStyleSheet("QLabel{color: red;}");
                ui->label_5->setStyleSheet("QLabel{color: red;}");
            }
}

void MainWindow::on_pushButton_3_clicked()
{
    on_pushButton_clicked();
    randomdensity = 8 - ui->horizontalSlider_2->value();
    for (int i=0; i <Row;i++)
    {
        for(int j=0; j < Col;j++){
            if (!randomwall.bounded(0,randomdensity))
                    ui->tableWidget->item(i, j)->setBackground(QColor(10,10,10));
        }
    }

    ui->tableWidget->item(Src.first,Src.second)->setBackground(QColor(0,255,0));
    ui->tableWidget->item(Dest.first,Dest.second)->setBackground(QColor(255,0,0));
}
