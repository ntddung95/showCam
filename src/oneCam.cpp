#include <iostream>
#include "config.h"
#include <opencv2/opencv.hpp>
#include <thread>
#include <pthread.h>
#include <experimental/filesystem>
#include <unistd.h>
using namespace std;   
using namespace cv;
namespace fs = std::experimental::filesystem;

VideoCapture vid1;
Mat img1;
bool done1,start = true;

void cam1(){
	
	vid1.open("rtsp://admin:1111@10.11.11.198:554");
	if(vid1.isOpened())
		cout<<"Cam1: OK\n";
	else
		cout<<"Cam1: Fail\n";
	while(start){
		vid1 >> img1;
		done1 = true;
	}
}



int main(int argc, char **argv){
	
	done1 = false;	
	//ipConf.setGOP(gop);
	
	thread cam1Thr1(cam1);
	
	if (!fs::is_directory(argv[1]) || !fs::exists(argv[1])) { 
    	fs::create_directory(argv[1]);
	}
	
	namedWindow("XXX",WINDOW_NORMAL);

	int count = 0;
	
	Mat imgMain1;
	usleep(1000000);
	while (true){
		/*if ((img1 == NULL) || (img2 == NULL)){
			usleep(200000);
			cout<<"isEmpty\n";
			continue;
		}*/
		if(done1){
			imgMain1 = img1.clone();
			Rect roi(0,0,1920/2,1080/2);
			if(argc == 3)
				imgMain1 = imgMain1(roi);
			cout<<"count: "<<count<<endl;
			
			imshow("XXX",imgMain1);
			
			
			//if (count == 100)
			int key = waitKey(200);
			if(key == 27){
				start = false;
				break;
			}
			if(key == 32){
				char imgName[100];
				sprintf(imgName,"%s/%04d.bmp",argv[1],count);
				imwrite(imgName,imgMain1);
				count++;
			}
		}
		else
			usleep(100000);
	}
	cam1Thr1.join();
	return 0;
}




