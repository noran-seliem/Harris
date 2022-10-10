#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include<QFileDialog>
#include <qmessagebox.h>
#include<QString>
#include <string.h>
#include <memory>
#include <stdexcept>
#include<cvQTconvert.h>
#include<Harris.h>

namespace Ui { class MainWindow; }
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void openImage();
    void openTemplateImage();
    void setView1(cv::Mat);
    void setView2(cv::Mat);
    void calcHarris1();
    void calcHarris2();
    void MatchNCC();
    void MatchSSD();
public:
    cv::Mat image;
    cv::Mat matchingImage;
    Harris ResultImage;
    Harris ResultMatchingImage;
    std::pair<int,float> ssd (cv::Mat ROI , std::vector<cv::Mat>& Target);
    std::pair<int,float> ncc (cv::Mat ROI , std::vector<cv::Mat>& Target);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
