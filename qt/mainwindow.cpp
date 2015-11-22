#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->selectImgBtn, SIGNAL(clicked(bool)), this, SLOT(select_image()));
    connect(ui->sliderHeight, SIGNAL(valueChanged(int)), this, SLOT(resize_image()));
    connect(ui->sliderWidth, SIGNAL(valueChanged(int)), this, SLOT(resize_image()));
    connect(ui->rbHFlip, SIGNAL(clicked(bool)), this, SLOT(flip_image()));
    connect(ui->rbVFlip, SIGNAL(clicked(bool)), this, SLOT(flip_image()));

    button_group = new QButtonGroup();
    button_group->addButton(ui->rbBilinear, 0);
    button_group->addButton(ui->rbNeighbor, 1);

    ui->modifiedImg->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::select_image()
{
    file_name = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Images (*.png *.xpm *.jpg)"));

    if(!file_name.isEmpty()) {
        ui->imgLineEdit->setText(file_name);
        pixmap.load(file_name);
        ui->modifiedImg->setPixmap(pixmap);
        dst_image = src_image = cv::imread(file_name.toStdString());
    }
}

void MainWindow::resize_image()
{
    double width = ui->sliderWidth->value();
    double height = ui->sliderHeight->value();

    statusBar()->showMessage("Largura: " + QString::number(width)
                             + "% | Altura: " + QString::number(height) + "%");

    if(validate()) {
        switch(button_group->checkedId()) {
            case 0:
                dst_image = cvip::bilinear(src_image, width, height);
                statusBar()->showMessage("Interpolação bilinear | Largura: "
                                         + QString::number(width) + "% | Altura: " + QString::number(height) + "%");
                break;
           case 1:
                dst_image = cvip::nearest_neighbor(src_image, width, height);
                statusBar()->showMessage("Interpolação por vizinho mais próximo | Largura: "
                                         + QString::number(width) + "% | Altura: " + QString::number(height) + "%");
                break;
        }
        this->save_image();
    }
}

void MainWindow::flip_image() {

    if(ui->rbHFlip->isChecked()) {
        dst_image = cvip::flip(dst_image, cvip::FLIP_HORIZONTAL);
        statusBar()->showMessage("Flip horizontal");
    } else if(ui->rbVFlip->isChecked()) {
        dst_image = cvip::flip(dst_image, cvip::FLIP_VERTICAL);
        statusBar()->showMessage("Flip vertical");
    }

    this->save_image();
}

bool MainWindow::validate() {

    if(file_name.isEmpty()) {
        QMessageBox::warning(this, "Alerta!", "Selecione uma imagem!");
        return false;
    }

    if(button_group->checkedId() == -1) {
        QMessageBox::warning(this, "Alerta!", "Selecione um método!");
        return false;
    }

    return true;
}

void MainWindow::save_image() {
    cv::imwrite("../img/dst_image.jpg", dst_image);
    pixmap.load("../img/dst_image.jpg");
    ui->modifiedImg->setPixmap(pixmap);
}

bool MainWindow::eventFilter(QObject *object, QEvent *event) {
    QPoint pos;

    if (object == ui->modifiedImg) {
        if(event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

            if(mouseEvent->button() == Qt::LeftButton) {
                QPainter painter(&pixmap);
                painter.setPen(Qt::blue);
                painter.drawLine(mouseEvent->pos().x(), mouseEvent->pos().y(), 500, 500);
                ui->modifiedImg->setPixmap(pixmap);
                std::cout << mouseEvent->pos().x() << " " << mouseEvent->pos().y() << std::endl;
                return true;
            }
            return false;
        }
        if(event->type() == QEvent::MouseMove) {
            //case QEvent::MouseMove:
            return true;
        }
    }
    return false;
}
