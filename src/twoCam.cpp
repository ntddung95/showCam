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

VideoCapture vid1,vid2,vid3;
Mat img1, img2,img3;
bool done1,done2,done3,start = true;

void cam1(){
	
	vid1.open("rtsp://admin:1111@10.11.11.119:554");
	if(vid1.isOpened())
		cout<<"Cam1: OK\n";
	else
		cout<<"Cam1: Fail\n";
	while(start){
		vid1 >> img1;
		done1 = true;
	}
}

void cam2(){
	
	vid2.open("rtsp://admin:1111@10.11.11.193:554");
	if(vid2.isOpened())
		cout<<"Cam2: OK\n";
	else
		cout<<"Cam2: Fail\n";
	while(start){
		vid2 >> img2;
		done2 = true;
	}
}




int main(int argc, char **argv){
	int gop = 10;
	done1 = false;
	done2 = false;
	done3 = false;
	
	//ipConf.setGOP(gop);
	
	thread cam1Thr1(cam1);
	thread cam2Thr1(cam2);
	if (!fs::is_directory(argv[1]) || !fs::exists(argv[1])) { 
    	fs::create_directory(argv[1]);
	}
	char dir[50];
	sprintf(dir,"%s/Left",argv[1]);
	if (!fs::is_directory(dir) || !fs::exists(dir)){ 
    	fs::create_directory(dir);

	}
	sprintf(dir,"%s/Right",argv[1]);
	if (!fs::is_directory(dir) || !fs::exists(dir)){ 
    	fs::create_directory(dir);
    }
	namedWindow("XXX",WINDOW_NORMAL);

	int count = 0;
	
	Mat imgX1,imgX2,imgMain1,imgMain2,imgMain3;
	usleep(1000000);
	while (true){
		/*if ((img1 == NULL) || (img2 == NULL)){
			usleep(200000);
			cout<<"isEmpty\n";
			continue;
		}*/
		if(done1 && done2){
			imgMain1 = img1.clone();
			imgMain2 = img2.clone();
			Rect roi(0,0,1920/2,1080/2);
			
			cout<<"count: "<<count<<endl;
			hconcat(imgMain1, imgMain2, imgX1);
			imshow("XXX",imgX1);
			
			
			//if (count == 100)
			int key = waitKey(200);
			if(key == 27){
				start = false;
				break;
			}
			if(key == 32){
				char imgName[100];
				sprintf(imgName,"%s/Left/%04d.png",argv[1],count);
				imwrite(imgName,imgMain1);
				sprintf(imgName,"%s/Right/%04d.png",argv[1],count);
				imwrite(imgName,imgMain2);
				count++;
			}
		}
		else
			usleep(100000);
	}
	cam1Thr1.join();
	cam2Thr1.join();
	return 0;
}