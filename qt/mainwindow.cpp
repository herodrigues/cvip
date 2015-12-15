#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->sliderHeight, SIGNAL(valueChanged(int)), this, SLOT(resize_image()));
    connect(ui->sliderWidth, SIGNAL(valueChanged(int)), this, SLOT(resize_image()));
    connect(ui->rbHFlip, SIGNAL(clicked(bool)), this, SLOT(flip_image()));
    connect(ui->rbVFlip, SIGNAL(clicked(bool)), this, SLOT(flip_image()));
    connect(ui->actionOpenImage, SIGNAL(triggered(bool)),this, SLOT(select_image()));
    connect(ui->rbAdd, SIGNAL(clicked(bool)), this, SLOT(arithmetic()));
    connect(ui->rbSub, SIGNAL(clicked(bool)), this, SLOT(arithmetic()));
    connect(ui->actionOriginal, SIGNAL(triggered(bool)), this, SLOT(show_histogram()));
    connect(ui->actionEqualized, SIGNAL(triggered(bool)), this, SLOT(show_equalized_hist()));
    connect(ui->actionMedianFilter, SIGNAL(triggered(bool)), this, SLOT(median_filter()));
    connect(ui->actionMask1, SIGNAL(triggered(bool)), this, SLOT(laplace_filter1()));
    connect(ui->actionMask2, SIGNAL(triggered(bool)), this, SLOT(laplace_filter2()));
    connect(ui->actionMask3, SIGNAL(triggered(bool)), this, SLOT(laplace_filter3()));
    connect(ui->actionMask4, SIGNAL(triggered(bool)), this, SLOT(laplace_filter4()));
    connect(ui->actionSobelDetector, SIGNAL(triggered(bool)), this, SLOT(sobel_detector()));

    button_group = new QButtonGroup();
    button_group->addButton(ui->rbBilinear, 0);
    button_group->addButton(ui->rbNeighbor, 1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::select_image()
{
    file_name = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Images (*.png *.xpm *.jpg)"));

    if(!file_name.isEmpty()) {
        pixmap.load(file_name);
        ui->modifiedImg->setPixmap(pixmap);
        dst_image = src_image = cv::imread(file_name.toStdString());
    }
}

void MainWindow::show_histogram() {
    std::vector<int> hist = cvip::histogram(src_image);
    cvip::show_histogram(hist, "Histograma da imagem original");
}

void MainWindow::median_filter() {
    dst_image = cvip::median_filter(src_image);
    this->save_image();
}

void MainWindow::laplace_filter1() {
    int mask[9] = { 0, 1, 0, 1, -4, 1, 0, 1, 0 };

    dst_image = cvip::laplace_filter(src_image, mask);
    this->save_image();
}

void MainWindow::laplace_filter2() {
    int mask[9] = { 1, 1,  1 , 1, -8, 1, 1, 1, 1 };

    dst_image = cvip::laplace_filter(src_image, mask);
    this->save_image();
}

void MainWindow::laplace_filter3() {
    int mask[9] = { 0,  -1,  0, -1, 4, -1 , 0, -1, 0 };

    dst_image = cvip::laplace_filter(src_image, mask);
    this->save_image();
}

void MainWindow::laplace_filter4() {
    int mask[9] = { -1, -1, -1, -1,  8, -1, -1, -1, -1 };

    dst_image = cvip::laplace_filter(src_image, mask);
    this->save_image();
}

void MainWindow::sobel_detector() {
    dst_image = cvip::sobel_detector(src_image);
    this->save_image();
}

void MainWindow::show_equalized_hist() {
    auto hist = cvip::histogram(src_image);
    int size = src_image.rows * src_image.cols;
    float ratio = 255.0 / size;

    std::vector<float> pr_rk;
    auto freq = cvip::frequency(hist);
    std::vector<float> ps_sk(255, 0);
    std::vector<int> result;
    std::vector<int> norm;

    // Pr(rk)
    for(int i = 0; i < 256; i++)
        pr_rk.push_back((double) hist[i] / size);

    // Eq.
    for(int i = 0; i < 256; i++)
        norm.push_back(cvRound((double) freq[i] * ratio));

    // rk
    for(int i = 0; i < 256; i++)
        ps_sk[norm[i]] += pr_rk[i];

    for(int i = 0; i < 256; i++)
        result.push_back(cvRound(ps_sk[i] * 255));

    // normalizada
    for(int row = 0; row < src_image.rows; row++)
        for(int col = 0; col < src_image.cols; col++)
            for(int channel = 0; channel != cv::DataType<cv::Vec3b>::channels; channel++)
                dst_image(row, col)[channel] = norm[src_image(row, col)[channel]];

    cvip::show_histogram(result, "Histograma da imagem equalizada");
    this->save_image();
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

void MainWindow::on_btnCalcDistance_clicked()
{
    auto x1 = ui->lnX1->text().toFloat();
    auto y1 = ui->lnY1->text().toFloat();
    auto x2 = ui->lnX2->text().toFloat();
    auto y2 = ui->lnY2->text().toFloat();

    auto eucd = cvip::euclidean_distance(x1, y1, x2, y2);
    auto city_block = cvip::city_block_distance(x1, y1, x2, y2);
    auto chess = cvip::chessboard_distance(x1, y1, x2, y2);

    ui->lnEucd->setText(QString::number(eucd, 'f', 3));
    ui->lnD4->setText(QString::number(city_block));
    ui->lnD8->setText(QString::number(chess));
}

void MainWindow::arithmetic() {

    if(ui->rbAdd->isChecked())
        dst_image = cvip::addition(dst_image, tmp_image);

    if(ui->rbSub->isChecked())
        dst_image = cvip::subtraction(dst_image, tmp_image);

    this->save_image();
}

void MainWindow::on_btnTranslate_clicked()
{
    int x = ui->lnTransX->text().toInt();
    int y = ui->lnTransY->text().toInt();

    dst_image = cvip::translate(dst_image, x, y);
    this->save_image();
}

void MainWindow::on_btnLoadTmpImg_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Images (*.png *.xpm *.jpg)"));

    if(!file_name.isEmpty()) {
        ui->tmpImage->setPixmap(QPixmap(file_name).scaled(ui->tmpImage->width(), ui->tmpImage->height(), Qt::KeepAspectRatio));
        tmp_image = cv::imread(file_name.toStdString());
    }
}

void MainWindow::on_btnStretch_clicked()
{
    float r1 = ui->lnR1->text().toFloat();
    float r2 = ui->lnR2->text().toFloat();
    float s1 = ui->lnS1->text().toFloat();
    float s2 = ui->lnS2->text().toFloat();

    dst_image = cvip::contrast_stretch(src_image, r1, r2, s1, s2);

    this->save_image();
}
