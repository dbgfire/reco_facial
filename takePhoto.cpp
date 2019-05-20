#include <opencv2/opencv.hpp>

using namespace std;
int main(){

    try{
        cv::VideoCapture videoOpenCv;
        bool test = videoOpenCv.open(0);
        if(test==false)
        {
            throw -101;
        }
        char folder[5]="img/";  
        char folderCreateCommand[20];
        try
        {
            sprintf(folderCreateCommand,  "mkdir %s", folder);
            system(folderCreateCommand);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
       

        //allocation memoire
        cv::Mat img;
        int count=0;
        //creation fenetre
        cv::namedWindow("window2",cv::WINDOW_AUTOSIZE);
        cout << count<< '\n';
        //lecture d'une image et enregistremet sur le dique dure
    
        while (true)
        {
            
            videoOpenCv.read(img);
            cv::imshow("window2",img);
            int key = cv::waitKey(1000/25);
            if(key==(int)'q')
            {
                break;
            }
            if(key==(int)'c'){
                count++;
                char message[100];                
                sprintf(message, "%stest%d.png", folder,count);
                cout << message << '\n';
                videoOpenCv.read(img);  
                cv::imwrite(message,img);

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