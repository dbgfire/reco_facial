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
        //Ici on affiche le framerate et on met de la couleur sous Unix (bleu et gras)
        std::cout << "\033[034;1mFrames per seconds : " << fps << "\033[0m" << std::endl;

        //On fait une petite pause pour avoir le temps de lire le framerate dans la console
        sleepcp(300);

        time(start);

        ret = 0;
    }
    return ret;
}

int main(int argc, const char *argv[]) {
    char names[7][10] = {"none", "Alan", "Nicolas", "Vito", "Manu", "Aubin","Billon"};
    // char names[20][10] = {"None", "Alexis", "Jules", "Romain","Vito","aubin","Alan","Louis"};
    if (argc != 2) {
        cout << "usage: " << argv[0] << " </path/to/haar_cascade>" << endl;
        cout << "\t </path/to/haar_cascade> -- Path to the Haar Cascade for face detection." << endl;
        exit(1);
    }
   
    string fn_haar = string(argv[1]);//string("/usr/share/opencv/haarcascades/haarcascade_frontalface_alt2.xml");
  
    int im_width = 640;
    int im_height = 480;
    Ptr<LBPHFaceRecognizer> model = cv::face::createLBPHFaceRecognizer();
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
    cv::Size sizeRect(40, 40);
    for(;;) {
        count++;
        cap >> frame;
        Mat original = frame.clone();
        Mat gray;
        cvtColor(original, gray, CV_BGR2GRAY);
        vector< Rect_<int> > faces;
        haar_cascade.detectMultiScale(gray, faces);
        // haar_cascade.detectMultiScale(gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, sizeRect);
        for(int i = 0; i < faces.size(); i++) {
            // Process face by face:
            Rect face_i = faces[i];
            Mat face = gray(face_i);
            Mat face_resized;
            cv::resize(face, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
            int predicted_label = -1;
            double predicted_confidence = 0.0;
            model->predict(face_resized, predicted_label, predicted_confidence);
            rectangle(original, face_i, CV_RGB(255, 0,15), 1); 
            string box_text;
            if (predicted_confidence < 100){
                box_text = format("%s a %f", names[predicted_label], round(100 -predicted_confidence));
            } 
            else{
                box_text = format("unknown ");
            }

            cout<<box_text<<endl;
            // sleepcp(300);
            int pos_x = std::max(face_i.tl().x - 10, 0);
            int pos_y = std::max(face_i.tl().y - 10, 0);
            putText(original, box_text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(255,0,15), 1.0);
        }
        count = calculationFPS(&start, count);
        imshow("face_recognizer", original);
        int key = cv::waitKey(1000/25);
                if(key==(int)'q')
                {
                    break;
                }
    }
    return 0;
}
