#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "kmeans.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QObject::connect(this, SIGNAL(valueChanged(int)), ui->IterationCount, SLOT(setNum(int)));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_OpenFile_clicked() {
    imagePath = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "",
                tr("PNG (*.png)" )
                );

    // Check if a file is actually selected
    if (imagePath != "") {
        QByteArray temp = imagePath.toLatin1();
        const char *imagePathString = temp.data();
        image = kmeans(imagePathString, clusterNum);

        imagePixmap = QPixmap::fromImage(
                    QImage(image.getString(), image.getWidth(), image.getHeight(), QImage::Format_RGBA8888)
                        );

        scene = new QGraphicsScene(this);
        newScene = new QGraphicsScene(this);
        scene->addPixmap(imagePixmap);
        newScene->addPixmap(imagePixmap);
        scene->setSceneRect(imagePixmap.rect());
        newScene->setSceneRect(imagePixmap.rect());
        ui->OriginalImage->setScene(scene);
        ui->NewImage->setScene(scene);
    }
}

void MainWindow::on_SaveFile_clicked() {
    QString saveImagePath = QFileDialog::getSaveFileName(
                this,
                tr("Save File"),
                "",
                tr("PNG (*.png)" )
                );
    QByteArray temp = saveImagePath.toLatin1();
    const char *imagePathString = temp.data();

    image.writePNG(imagePathString);
}

void MainWindow::on_NextIteration_clicked() {
    if (!image.isConverged() && image.getIteration() < iterationNum) {
        emit valueChanged(image.nextIteration());
        newImagePixmap = QPixmap::fromImage(
                    QImage(image.getString(), image.getWidth(), image.getHeight(), QImage::Format_RGBA8888)
                        );
        newScene->clear();
        newScene->addPixmap(newImagePixmap);
        newScene->setSceneRect(newImagePixmap.rect());
        ui->NewImage->setScene(newScene);
    }
}

void MainWindow::on_Completion_clicked() {
    while (!image.isConverged() && image.getIteration() < iterationNum) {
        emit valueChanged(image.nextIteration());
        newImagePixmap = QPixmap::fromImage(
                    QImage(image.getString(), image.getWidth(), image.getHeight(), QImage::Format_RGBA8888)
                        );
        newScene->clear();
        newScene->addPixmap(newImagePixmap);
        newScene->setSceneRect(newImagePixmap.rect());
        ui->NewImage->setScene(newScene);
    }
}

void MainWindow::on_ClusterNum_valueChanged(int arg1) {
    clusterNum = arg1;

    // Only reset UI if an image is already being processed
    if (imagePath != "") {
        QByteArray temp = imagePath.toLatin1();
        const char *imagePathString = temp.data();
        kmeans newImage(imagePathString, clusterNum);
        image = newImage;
        newImagePixmap = QPixmap::fromImage(
                    QImage(image.getString(), image.getWidth(), image.getHeight(), QImage::Format_RGBA8888)
                        );
        newScene->clear();
        newScene->addPixmap(newImagePixmap);
        newScene->setSceneRect(newImagePixmap.rect());
        ui->NewImage->setScene(newScene);
    }
}

void MainWindow::on_IterationNum_valueChanged(int arg1) {
    iterationNum = arg1;
}
