#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QButtonGroup>
#include <QImage>
#include <QStatusBar>
#include <QMouseEvent>
#include <QPixmap>
#include <QPainter>

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
    void resize_image();
    void flip_image();
    void select_image();

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    Ui::MainWindow *ui;
    cv::Mat_<cv::Vec3b> src_image;
    cv::Mat_<cv::Vec3b> dst_image;
    QButtonGroup *button_group;
    QString file_name;
    QPixmap pixmap;

    bool validate();
    void save_image();
};

#endif // MAINWINDOW_H
