/*1.先用mat储存原图 
2.分离通道 
3.加权合成为灰度图 
4. 进行二值化处理 
5.进行开运算 
6.进行轮廓检索，对轮廓图近似为矩形，存储四个角的指针 
7.在mat原图上对四个角连线,最后输出图像*/
#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>
#include<sstream>
#include<string>
#include<iomanip>

int main()
{
    cv::VideoCapture vc;
    vc.open("../video/video.mp4");
    if (!vc.isOpened())    //如果读取到空
    {
        std::cout << "Error loading video" << std::endl;    
        return -1;
    }

    cv::Mat frame;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<cv::Mat> channels;
    while(true)
    {
        vc.read(frame);
        if (!vc.read(frame)) 
        {
            std::cout << "Cannot read frame from video or frame is empty" << std::endl;
            break; 
        }
        cv::Mat hsv;
        cv::Mat mask;
        cv::Scalar lower(84,45,48);
        cv::Scalar upper(110,255,255);
        double start = cv::getTickCount();
        cv::cvtColor(frame,hsv,cv::COLOR_BGR2HSV);
        GaussianBlur(hsv, hsv, cv::Size(3, 3), 0, 0);
        inRange(hsv,lower,upper,mask);
        cv::morphologyEx(mask, mask, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
        cv::findContours(mask,contours,hierarchy,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
        for (int i = 0; i < (int)contours.size(); i++)
	    {
		double g_dConArea = contourArea(contours[i], true);
	    }
        std::vector <std::vector<cv::Point>>::iterator iter = contours.begin();
	    for (; iter != contours.end();)
	    {
		    double g_dConArea = contourArea(*iter);
		    if ((g_dConArea < 2000)||(g_dConArea > 5000))
		    {
			iter = contours.erase(iter);
		    }
		    else
		    {
			++iter;
		    }
	    }

        for(int i=0;i<contours.size();i++)
        {
            cv::RotatedRect rext=cv::minAreaRect(contours[i]);
            cv::Point2f vertices[4];
            rext.points(vertices);
            for(int j=0;j<4;j++)
            {
                cv::line(frame,vertices[j],vertices[(j+1)%4],cv::Scalar(0,0,255),5);
            }

            double end = cv::getTickCount();
            double time = (end - start)/ cv::getTickFrequency();
	        cv::Scalar textColor(0, 0, 255);  
	        int fontFace = cv::FONT_HERSHEY_SIMPLEX;    
	        double fontScale = 1.0;               
	        int thickness = 2; 
            std::stringstream ss;
            ss.setf(std::ios::fixed);
            ss<<std::setprecision(2);
            ss<<time;                   
	        cv::putText(frame, ss.str(), vertices[0], fontFace, fontScale, textColor, thickness);
        }

        cv::imshow("frame",frame);
        cv::waitKey(25);
    }
    vc.release();

}