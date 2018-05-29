#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "kmeans.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_OpenFile_clicked() {
    QString imagePath = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "",
                tr("PNG (*.png)" )
                );
    QByteArray temp = imagePath.toLatin1();
    const char *imagePathString = temp.data();
    image = kmeans(imagePathString, clusterNum);

    imagePixmap = QPixmap::fromImage(
                QImage(image.getString(), image.getWidth(), image.getHeight(), QImage::Format_RGBA8888)
                    );

    scene = new QGraphicsScene(this);
    scene->addPixmap(imagePixmap);
    scene->setSceneRect(imagePixmap.rect());
    ui->OriginalImage->setScene(scene);
    image.initializeCentroids();
}

void MainWindow::on_SaveFile_clicked() {
    QString imagePath = QFileDialog::getSaveFileName(
                this,
                tr("Save File"),
                "",
                tr("PNG (*.png)" )
                );
    QByteArray temp = imagePath.toLatin1();
    const char *imagePathString = temp.data();

    image.writePNG(imagePathString);
}

void MainWindow::on_NextIteration_clicked() {
    image.nextIteration();
    imagePixmap = QPixmap::fromImage(
                QImage(image.getString(), image.getWidth(), image.getHeight(), QImage::Format_RGBA8888)
                    );
    scene->clear();
    scene->addPixmap(imagePixmap);
    //scene->setSceneRect(imagePixmap.rect());
    //ui->NewImage->setScene(scene);
}

void MainWindow::on_Completion_clicked() {
    while (!image.isConverged()) {
        image.nextIteration();
        {imagePixmap = QPixmap::fromImage(
                    QImage(image.getString(), image.getWidth(), image.getHeight(), QImage::Format_RGBA8888)
                        );
        scene->clear();
        scene->addPixmap(imagePixmap);
        scene->setSceneRect(imagePixmap.rect());
        ui->NewImage->setScene(scene);}
    }
}

void MainWindow::on_ClusterNum_valueChanged(int arg1) {
    clusterNum = arg1;
}

void MainWindow::on_IterationNum_valueChanged(int arg1) {
    iterationNum = arg1;
}
