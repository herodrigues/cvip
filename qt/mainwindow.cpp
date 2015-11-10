#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myBtnGroup = new QButtonGroup();

    myBtnGroup->addButton(ui->rbBilinear, 0);
    myBtnGroup->addButton(ui->rbNeighbor, 1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selectImgBtn_clicked()
{
    file_name = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Images (*.png *.xpm *.jpg)"));

    if(!file_name.isEmpty()) {
        ui->imgLineEdit->setText(file_name);
        ui->sourceImg->setPixmap(QPixmap(file_name));

        src_image = cv::imread(file_name.toStdString());
        ui->srcImgProperty->setText("Width: " + QString::number(src_image.cols) + "px | Height: " + QString::number(src_image.rows) + "px");
    }
}

void MainWindow::on_processBtn_clicked()
{
    if(validate()) {
        double width = ui->axisX->text().toDouble();
        double height = ui->axisY->text().toDouble();
        ui->modImgProperty->setText("Width: " + QString::number(width) + "px | Height: " + QString::number(height) + "px");

        cv::Mat_<cv::Vec3b> output;

        switch(myBtnGroup->checkedId()) {
            case 0:
                output = cvip::interpolation::bilinear(src_image, width, height);
                cv::imwrite("../img/output.jpg", output);

                ui->modifiedImg->setPixmap(QPixmap("../img/output.jpg"));

                break;
           case 1:
                output = cvip::interpolation::nearest_neighbor(src_image, width, height);
                cv::imwrite("../img/output.jpg", output);

                ui->modifiedImg->setPixmap(QPixmap("../img/output.jpg"));
                break;
        }
    }
}

bool MainWindow::validate() {

    if(file_name.isEmpty()) {
        QMessageBox::warning(this, "Alert!", "Select an image!");
        return false;
    }

    if(myBtnGroup->checkedId() == -1) {
        QMessageBox::warning(this, "Alert!", "Select at least one method!");
        return false;
    }

    if(ui->axisX->text().isEmpty() || ui->axisY->text().isEmpty()) {
        QMessageBox::warning(this, "Alert!", "Width and height should have a value!");
        return false;
    }

    return true;
}
