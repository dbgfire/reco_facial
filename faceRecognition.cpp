#include "opencv2/core.hpp"
#include "opencv2/face.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // win32.
#include <time.h>
void sleepcp(int milliseconds) // cross-platform sleep function
{
#ifdef WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif // win32
}
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // win32

using namespace cv;
using namespace cv::face;
using namespace std;

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
        std::cout << "\033[034;1mFrames per seconds : " << fps << "\033[0m" << std::endl;
        sleepcp(300);

        time(start);

        ret = 0;
    }
    return ret;
}

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        cout << "usage: " << argv[0] << " </path/to/haar_cascade>" << endl;
        cout << "\t </path/to/haar_cascade> -- Path to the Haar Cascade for face detection." << endl;
        exit(1);
    }
    string fn_haar = string(argv[1]);
    Ptr<BasicFaceRecognizer> model = createEigenFaceRecognizer();
    try
    {
        model->load("trainer.yml");
    }
    catch(const std::exception& e)
    {
        std::cerr << "error : "<<e.what() << '\n';
        exit(0);
    }
    CascadeClassifier haar_cascade;
    haar_cascade.load(fn_haar);
    VideoCapture cap(0);
    if(!cap.isOpened()) {
        cerr << "Capture Device ID " << 0 << "cannot be opened." << endl;
        return -1;
    }
    time_t start;
    time(&start);
    int count = 0;
    Mat frame;
    for(;;) {
        count++;
        cap >> frame;
        Mat original = frame.clone();
        Mat gray;
        cvtColor(original, gray, CV_BGR2GRAY);
        vector< Rect_<int> > faces;
        haar_cascade.detectMultiScale( gray, faces, 1.1, 2, 0|CASCADE_FIND_BIGGEST_OBJECT, Size(92 , 92) );
        for(int i = 0; i < faces.size(); i++) {
            Mat face = gray(faces[i]);
            Mat face_resized;
            cv::resize(face, face_resized, Size(92, 92), 1.0, 1.0, INTER_CUBIC);
            int pred=model->predict(face_resized);
            cout << "id "<<pred<<endl;
        }
        count = calculationFPS(&start, count);
    }
    return 0;
}
