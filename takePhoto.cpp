#include <iostream>
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // win32
#include <time.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <string>
#include <ctime>
#define BLACK CV_RGB(0,0,0)

#include <opencv2/opencv.hpp>
void sleepcp(int milliseconds) 
{
#ifdef WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif // win32
}

std::string datetime()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%d-%m-%Y %H-%M-%S",timeinfo);
    return std::string(buffer);
}

int calculationFPS(time_t *start, int count)
{
    int ret = count;
    if(count==100)
    {
        double seconds = 1;
        double fps = 0;
        time_t end;

        time(&end);
        seconds = difftime(end,*start);
        fps = count/seconds;
        std::cout << "\033[031;1mFrames per seconds : " << fps << "\033[0m" << std::endl;
        sleepcp(300);

        time(start);

        ret = 0;
    }
    return ret;
}

int main()
{
        try
        {
            //reconnaissance des visages

            cv::VideoCapture videoOpenCv;
            bool test = videoOpenCv.open(0);
            int counter=0;
            if(test==false)
            {
                throw -101;
            }

            cv::CascadeClassifier face_cascade;

            if(!face_cascade.load("/usr/share/opencv/haarcascades/haarcascade_frontalface_alt2.xml"))
            {
                throw -103;
            }
            cv::Mat img,bwsrc;
            cv::namedWindow("Window3", cv::WINDOW_AUTOSIZE);

            while(true)
            {
              videoOpenCv.read(img);

              std::vector<cv::Rect> faces;
              std::vector<cv::Scalar> colors;
              std::vector<std::string> names;

              cv::Size sizeRect(40, 40);

              face_cascade.detectMultiScale(img, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, sizeRect);
              for( size_t i = 0; i < faces.size(); i++ )
              {
                  colors.push_back(BLACK);
                  names.push_back("Human");
              }

              for( size_t i = 0; i < faces.size(); i++ )
              {
                  CvRect r = faces.at(i);
                  rectangle (img, cvPoint( r.x, r.y ), cvPoint( r.x + r.width, r.y + r.height ), colors.at(i), 1, 8, 0);
                  putText(img,names.at(i), cvPoint(r.x + r.width + 10, r.y + r.height + 10), cv::FONT_HERSHEY_PLAIN, 1.0, colors.at(i), 2.0);

              }
                cv::imshow("Window3", img);
                int key = cv::waitKey(1000/25);
                if(key==(int)'q')
                {
                    break;
                }
                if(key==(int)'c')
                {
                  std::string fname;
                  for( size_t i = 0; i < faces.size(); i++ ){
                    CvRect r = faces.at(i);
                    cv::Rect myROI(cvPoint( r.x, r.y ), cvPoint( r.x + r.width, r.y + r.height ));
                    cv::Mat croppedImage = img(myROI);
                    cv::Mat face_resized;
                    cv::resize(croppedImage, face_resized, cv::Size(92 , 92), 1.0, 1.0, cv::INTER_CUBIC);
                    counter++;
                    cv::cvtColor(face_resized, bwsrc, CV_BGR2GRAY);
                    fname="user/Photo"+std::to_string(counter)+".pgm";
                    cv::imwrite(fname,bwsrc);
                  }

                }
            }
        }
        catch(int e)
        {
                return e;
        }

        return 0;
}