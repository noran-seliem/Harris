#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->openImage1, SIGNAL(triggered()), this, SLOT(openImage()));
    connect(ui->openImage2, SIGNAL(triggered()), this, SLOT(openTemplateImage()));
    connect(ui->harrisImage1 ,SIGNAL(triggered()),this,SLOT(calcHarris1()));
    connect(ui->harrisImage2 ,SIGNAL(triggered()),this,SLOT(calcHarris2()));
    connect(ui->actionNCC ,SIGNAL(triggered()),this,SLOT(MatchNCC()));
    connect(ui->actionSSD ,SIGNAL(triggered()),this,SLOT(MatchSSD()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openImage(){
    auto fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "C://", tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));
    this->image = cv::imread(fileName.toStdString(),0);
    setView1(image);

}
void MainWindow::openTemplateImage(){
    auto fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "C://", tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));
    this->matchingImage = cv::imread(fileName.toStdString(),0);
    setView2(matchingImage);

}
void MainWindow::setView1(cv::Mat image){
    QPixmap im = ASM::cvMatToQPixmap(image);
    QGraphicsScene* scene = new QGraphicsScene;
    scene->addPixmap(im);
    ui->view1->setScene(scene);
    ui->view1->fitInView(scene->sceneRect(),Qt::IgnoreAspectRatio);
}
void MainWindow::setView2(cv::Mat image){
    QPixmap im = ASM::cvMatToQPixmap(image);
    QGraphicsScene* scene = new QGraphicsScene;
    scene->addPixmap(im);
    ui->view2->setScene(scene);
    ui->view2->fitInView(scene->sceneRect(),Qt::IgnoreAspectRatio);
}
void MainWindow::calcHarris1(){
        auto start = high_resolution_clock::now();
        Mat ResultImage = DetectCorners(this->image, 35, 3, this->ResultImage);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        MainWindow::setView1(ResultImage);
        QMessageBox msgBox;
        std::string txt = "Time to excute Harris is "+ std::to_string(duration.count())+ " milli second";
        msgBox.setText(txt.c_str());
        msgBox.exec();

}
void MainWindow::calcHarris2(){
    auto start = high_resolution_clock::now();
    Mat ResultImage = DetectCorners(this->matchingImage, 35, 3, this->ResultMatchingImage);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    MainWindow::setView2(ResultImage);
    QMessageBox msgBox;
    std::string txt = "Time to excute Harris is "+ std::to_string(duration.count())+" milli second";
    msgBox.setText(txt.c_str());
    msgBox.exec();
}
void MainWindow::MatchNCC(){
    auto start = high_resolution_clock::now();
    std::vector<cv::Mat>Target;
    std::vector<cv::Mat>ROI;
    for(int i =0;i<ResultImage.points.size();i++){
        if((ResultImage.points[i].y-(ResultImage.points[i].r/2))<0 ||(ResultImage.points[i].x-(ResultImage.points[i].r/2))<0 ||
                (ResultImage.points[i].y+(ResultImage.points[i].r/2))>=image.rows || (ResultImage.points[i].x+(ResultImage.points[i].r/2)) >= image.cols)
        {
        cv::Rect myROI(ResultImage.points[i].y-(ResultImage.points[i].r/2), ResultImage.points[i].x-(ResultImage.points[i].r/2)
                       ,ResultImage.points[i].y+(ResultImage.points[i].r/2), ResultImage.points[i].x+(ResultImage.points[i].r/2));
        ROI.push_back(image(myROI));
        }
        }
    for(int i =0;i<ResultMatchingImage.points.size();i++){
        if((ResultMatchingImage.points[i].y-(ResultMatchingImage.points[i].r/2))<0 ||(ResultMatchingImage.points[i].x-(ResultMatchingImage.points[i].r/2))<0 ||
                (ResultMatchingImage.points[i].y+(ResultMatchingImage.points[i].r/2))>=matchingImage.rows || (ResultMatchingImage.points[i].x+(ResultMatchingImage.points[i].r/2)) >= matchingImage.cols)
        {
        cv::Rect myROI(ResultMatchingImage.points[i].y-(ResultMatchingImage.points[i].r/2), ResultMatchingImage.points[i].x-(ResultMatchingImage.points[i].r/2)
                       ,ResultMatchingImage.points[i].y+(ResultMatchingImage.points[i].r/2), ResultMatchingImage.points[i].x+(ResultMatchingImage.points[i].r/2));
        Target.push_back(matchingImage(myROI));
        }
        }
    std::vector<std::pair<int,float>> NCCResult;
    for(int i=0;i<ROI.size();i++){
        NCCResult.push_back(MainWindow::ncc(ROI[i],Target));
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    QMessageBox msgBox;
    std::string txt = "Time to excute NCC matching is "+ std::to_string(duration.count())+" milli second";
    msgBox.setText(txt.c_str());
    msgBox.exec();
}

void MainWindow::MatchSSD(){
    auto start = high_resolution_clock::now();
    std::vector<cv::Mat>Target;
    std::vector<cv::Mat>ROI;
    for(int i =0;i<ResultImage.points.size();i++){
        if((ResultImage.points[i].y-(ResultImage.points[i].r/2))<0 ||(ResultImage.points[i].x-(ResultImage.points[i].r/2))<0 ||
                (ResultImage.points[i].y+(ResultImage.points[i].r/2))>=image.rows || (ResultImage.points[i].x+(ResultImage.points[i].r/2)) >= image.cols)
        {
            cv::Rect myROI(ResultImage.points[i].y-(ResultImage.points[i].r/2), ResultImage.points[i].x-(ResultImage.points[i].r/2)
                       ,ResultImage.points[i].y+(ResultImage.points[i].r/2), ResultImage.points[i].x+(ResultImage.points[i].r/2));
            ROI.push_back(image(myROI));
        }
    }
    for(int i =0;i<ResultMatchingImage.points.size();i++){
        cv::Rect myROI(ResultMatchingImage.points[i].y-(ResultMatchingImage.points[i].r/2), ResultMatchingImage.points[i].x-(ResultMatchingImage.points[i].r/2)
                       ,ResultMatchingImage.points[i].y+(ResultMatchingImage.points[i].r/2), ResultMatchingImage.points[i].x+(ResultMatchingImage.points[i].r/2));
        Target.push_back(matchingImage(myROI));
    }
    std::vector<std::pair<int,float>> SSDResult;
    for(int i=0;i<ROI.size();i++){
        SSDResult.push_back(MainWindow::ssd(ROI[i],Target));
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    QMessageBox msgBox;
    std::string txt = "Time to excute SSD matching is "+ std::to_string(duration.count())+" milli second";
    msgBox.setText(txt.c_str());
    msgBox.exec();
}
std::pair<int,float> MainWindow::ssd (cv::Mat ROI , std::vector<cv::Mat>& Target){
    int ssd_id =0;
    float ssd_val =2147483647;
    for(int k=0;k<Target.size();k++){
        if(ROI.size() != Target[k].size())
            cv::resize(Target[k],Target[k],ROI.size(),0,0,INTER_CUBIC);
        int sum_df =0;
        for(int i =0;i<ROI.rows;i++)
            for(int j=0;j<ROI.cols;j++){
                sum_df += ROI.at<uchar>(i,j)-Target[k].at<uchar>(i,j);
                sum_df = pow(sum_df,2);
            }
        if(sum_df <= ssd_val){
            ssd_val = sum_df;
            ssd_id =k;
        }

    }
    return (std::make_pair(ssd_id,ssd_val));
}
std::pair<int,float> MainWindow::ncc (cv::Mat ROI , std::vector<cv::Mat>& Target){
    int corr_id =0;
    float corr_val =0;
    for(int k=0;k<Target.size();k++){
        if(ROI.size() != Target[k].size())
            cv::resize(Target[k],Target[k],ROI.size(),0,0,INTER_CUBIC);
        int mul_sum =0 , sum_roi=0,sum_target=0;
        for(int i =0;i<ROI.rows;i++)
            for(int j=0;j<ROI.cols;j++){
                mul_sum += ROI.at<uchar>(i,j)*Target[k].at<uchar>(i,j);
                sum_roi += pow(ROI.at<uchar>(i,j),2);
                sum_target += pow(Target[k].at<uchar>(i,j),2);
            }
        sum_roi = sqrt(sum_roi);
        sum_target = sqrt(sum_target);
        float corr = float(mul_sum)/(sum_roi*sum_target);
        if(corr > corr_val){
            corr_val = corr;
            corr_id = k;
        }
    }
    return (std::make_pair(corr_id,corr_val));
}
