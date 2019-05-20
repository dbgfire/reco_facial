#include <opencv2/opencv.hpp>

int main(){

    try{
        cv::VideoCapture videoOpenCv;
        bool test = videoOpenCv.open(0);
        if(test==false)
        {
            throw -101;
        }
        //allocation memoire
        cv::Mat img;
        int count=0;
        //creation fenetre
        cv::namedWindow("window2",cv::WINDOW_AUTOSIZE);

        //lecture d'une image et enregistremet sur le dique dure
        videoOpenCv.read(img);
        cv::imwrite("test.png",img);

        while (true)
        {
            count++;
            videoOpenCv.read(img);
            cv::imshow("window2",img);
            int key = cv::waitKey(1000/25);
            if(key==(int)'q')
            {
                break;
            }
        }
        
    }
    catch(int e)
    {
        fprintf(stderr,"there war an error in camera opening\n");
        return e;
    }
    return 0;
}