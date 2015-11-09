#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selectImgBtn_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Images (*.png *.xpm *.jpg)"));
    ui->imgLineEdit->setText(file_name);

    QPixmap pixmap(file_name);
    ui->sourceImg->setPixmap(pixmap);

    inputImage = cv::imread(file_name.toStdString());
}

void MainWindow::on_processBtn_clicked()
{
    float axis_X = ui->axisX->text().toFloat();
    float axis_Y = ui->axisY->text().toFloat();

    if(ui->rbBilinear->isChecked()) {
        cv::Mat_<cv::Vec3b> bilinear = cvip::bilinear_interpolation(inputImage, cv::Size(), axis_X, axis_Y);
        cv::imwrite("../img/tux-mod.png", bilinear);
        //std::cout << bilinear;
    }
}

void MainWindow::TimerEvent()
{
    std::cout << "Timer event." << std::endl;
    int value = this->ui->progressBar->value();
    this->ui->progressBar->setValue(value+1);
}

