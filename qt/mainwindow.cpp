#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->sliderHeight, SIGNAL(valueChanged(int)), this, SLOT(process()));
    connect(ui->sliderWidth, SIGNAL(valueChanged(int)), this, SLOT(process()));

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
        ui->modifiedImg->setPixmap(QPixmap(file_name));
        src_image = cv::imread(file_name.toStdString());
    }
}

void MainWindow::process()
{
    double width = ui->sliderWidth->value();
    double height = ui->sliderHeight->value();

    if(validate()) {
        ui->modImgProperty->setText("Width: " + QString::number(width) + "% | Height: " + QString::number(height) + "%");

        cv::Mat_<cv::Vec3b> output;

        switch(myBtnGroup->checkedId()) {
            case 0:
                output = cvip::bilinear(src_image, width, height);
                cv::imwrite("../img/output.jpg", output);

                ui->modifiedImg->setPixmap(QPixmap("../img/output.jpg"));

                break;
           case 1:
                output = cvip::nearest_neighbor(src_image, width, height);
                cv::imwrite("../img/output.jpg", output);

                ui->modifiedImg->setPixmap(QPixmap("../img/output.jpg"));
                break;
        }
    }
}

bool MainWindow::validate() {

    if(file_name.isEmpty()) {
        QMessageBox::warning(this, "Alerta!", "Selecione uma imagem!");
        return false;
    }

    if(myBtnGroup->checkedId() == -1) {
        QMessageBox::warning(this, "Alerta!", "Selecione um método!");
        return false;
    }

    return true;
}
