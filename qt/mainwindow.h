#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QButtonGroup>
#include <QScrollArea>

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
    QButtonGroup *myBtnGroup;
    cv::Mat_<cv::Vec3b> src_image;
    QString file_name;

    bool validate();
};

#endif // MAINWINDOW_H
