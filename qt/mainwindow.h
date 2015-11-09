#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QAbstractButton>

#include "src/cvip.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_selectImgBtn_clicked();
    void on_processBtn_clicked();

private:
    Ui::MainWindow *ui;

    cv::Mat_<cv::Vec3b> inputImage;
};

#endif // MAINWINDOW_H
