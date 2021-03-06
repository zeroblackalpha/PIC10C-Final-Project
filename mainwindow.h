#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QFileDialog>
#include "kmeans.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void valueChanged(int newValue);

// Custom slots for UI actions
private slots:
    void on_OpenFile_clicked();
    void on_SaveFile_clicked();
    void on_NextIteration_clicked();
    void on_Completion_clicked();
    void on_ClusterNum_valueChanged(int arg1);
    void on_IterationNum_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;

    // Image path for the image to be opened
    QString imagePath = "";

    // Needed to display the original image
    QPixmap imagePixmap;
    QImage  *imageObject;
    QGraphicsScene *scene;

    // Needed to display the new image
    QPixmap newImagePixmap;
    QImage  *newImageObject;
    QGraphicsScene *newScene;

    // Custom class object to run the k-means algorithm
    kmeans image;

    int clusterNum = 10;
    int iterationNum = 200;
};

#endif // MAINWINDOW_H
