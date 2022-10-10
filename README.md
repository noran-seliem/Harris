# Harris Corner Detection
# UI
![Image](./Images/mainwindow.png)
The main window consists of:
-  2 graphics view which are used to view the images and the out put
- open menu which has 2 options image 1 and image 2 to open both of the images
- Harris which like wise has an option for each image
- Distance which also has 2 options one for the normalized cross correlation (NCC) and the sum square diffrence (SSD) for the matching
- pls note that the correct order of using the program is 
1. open both images 
2. run harris for both of the images 
3. run NCC and SSD
4. after using Harris or the matching operations a QmessageBox will appear with the combutaion time


#  Harris Corner Detection
 
 
|                       **Original**                        |              **Result**               |
| :---------------------------------------------------------------: | :------------------------------------------------------: |
| ![Image](./Images/Chess.jpg) | ![Image](./Images/Output.png) |



## Gaussian filter 
```cpp
Mat padding(Mat img, int k_width, int k_height)
{
    Mat scr;
    img.convertTo(scr, CV_64FC1);

    int p_rows, p_cols;
    p_rows = (k_height - 1) / 2;
    p_cols = (k_width - 1) / 2;
    // zero padding
    Mat p_image(Size(scr.cols + 2 * p_cols, scr.rows + 2 * p_rows), CV_64FC1, Scalar(0));
    scr.copyTo(p_image(Rect(p_cols, p_rows, scr.cols, scr.rows)));

    return p_image;
}

// func to implement gaussian filter
Mat  Gaussian(Mat scr, int k_w, int k_h, double sigma)
{
    // gaussian kernel
    //w(i,j) = k*(exp((i^2+j^2)/(2*sig))
    int p_rows = (k_h - 1) / 2;
    int p_cols = (k_w - 1) / 2;
    Mat kernel(k_h, k_w, CV_64FC1);
    for (int i = -p_rows; i <= p_rows; i++)
    {
        for (int j = -p_cols; j <= p_cols; j++)
        {
            kernel.at<double>(i + p_rows, j + p_cols) = exp(-(i * i + j * j) / (2.0 * sigma));
        }
    }
    kernel = kernel / sum(kernel);

    Mat p_image = padding(scr, k_w, k_h);

    Mat output = Mat::zeros(scr.size(), CV_64FC1);

    for (int i = 0; i < scr.rows; i++)
    {
        for (int j = 0; j < scr.cols; j++)
        {
            output.at<double>(i, j) = sum(kernel.mul(p_image(Rect(j, i, k_w, k_h)))).val[0];
        }
    }
    //output.convertTo(output, CV_8UC1);
    return output;
}
```
 First we build the Gaussian pyramid of the image in each vector it is applied to Gaussian filter with 
 
 alpha =  segma * k^i	
 
 K= \sqrt{2} 
 
 Segma =1.6 
 
 From vector to vector image is resized by half

```cpp
vector< vector<Mat>> octavepyramid(4);
    double k = sqrt(2);
    double segma = 1.6;
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 5; i++) {
            double alpha = pow(k, i) * segma;
            Mat imagenew = Gaussian(img, 5, 5, alpha);
            octavepyramid[j].push_back(imagenew);
        }

        resize(octavepyramid[j][3], img, Size(img.cols / 2, img.rows / 2), INTER_CUBIC);
    }
    
```
 Then we calculate difference of Gaussian DOG from each successive images

```cpp

    vector<vector <Mat>>DOG(4);
        Mat dogImage;
        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 4; i++) {
                cv::Mat mask;
                cv::subtract(octavepyramid[j][i], octavepyramid[j][i + 1], dogImage, mask, -1);
                //        dogImage = octavepyramid[j][i] - octavepyramid[j][i+1] ;
                DOG[j].push_back(dogImage);
            }
        }
    
```

 Calculate extremas through 3D kernel, if pixel extrema it is written recorded into a bt matrix


Then we remove non significant points of low threshold

### Team members


| Name                | Sec | BN  |
| ------------------- | --- | --- |
| ezzeldeen Esmail    | 1   | 50  |
| Noran ElShahat      | 2   | 40  |
| Moamen Gamal        | 2   | 11  |
| omar sayed          | 2   | 2   |
| Abdelrahman Almahdy | 1   | 45  |