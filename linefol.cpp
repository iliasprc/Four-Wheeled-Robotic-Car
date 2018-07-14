

#include "opencv2/opencv.hpp"
#include <unistd.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <fstream>

#include <iostream>
#include <sstream>
#include "Gpio.h"

#include <cstdio>
#include <ctime>
#include <string.h>
#include <pthread.h>

#define PTH 50
#define THRESH 80
using namespace std;
using namespace cv;

VideoCapture vcap(0);
Mat frame,buffer;
Mat currentFrame;
Rect roi(0,0,320,200);
Point center;

int BUFF=0;
int fcount=0;
int i,j;

int size=1;   
char lastmove='f';

double minVal; double maxVal;
Point minLoc; Point maxLoc;

pthread_cond_t condProc, condRead;
pthread_mutex_t the_mutex;

Gpio LenA,Lin1,Lin2,LenB,Lin3,Lin4,RenA,Rin1,Rin2,RenB,Rin3,Rin4;


Gpio::Gpio(){

}
 Gpio::Gpio(string name){
	this->gpioname=name;
	//value="0";
	export_gpio();


}
 Gpio::Gpio(string name,string dir){
 	this->gpioname=name;
 	//value="0";
 	export_gpio();
 	set_direction(dir);
 	set_value("0");

 }


     
int Gpio::set_direction(string dir){

    string setdir_str ="/sys/class/gpio/gpio" + this->gpioname + "/direction";   ofstream setdirgpio(setdir_str.c_str()); // open direction file for gpio
        if (setdirgpio < 0){
            cout << " OPERATION FAILED: Unable to set direction of GPIO"<< this->gpioname <<" ."<< endl;
            return -1;
        }

        setdirgpio << dir; //write direction to direction file
        setdirgpio.close(); // close direction file
        return 0;

}

int Gpio::set_value(string val){

    string setval_str = "/sys/class/gpio/gpio" + this->gpioname + "/value";
    ofstream setvalgpio(setval_str.c_str()); // open value file for gpio
        if (setvalgpio < 0){
            cout << " OPERATION FAILED: Unable to set the value of GPIO"<< this->gpioname <<" ."<< endl;
            return -1;
        }

        setvalgpio << val ;//write value to value file
        setvalgpio.close();// close value file
        return 0;



}


int Gpio::export_gpio(){
	 string export_str = "/sys/class/gpio/export";
	  ofstream exportgpio(export_str.c_str());
	  if (exportgpio < 0){
	         cout << " OPERATION FAILED: Unable to export GPIO"<< this->gpioname <<" ."<< endl;
	         return -1;
	     }

	     exportgpio << this->gpioname ; //writ
	     exportgpio.close(); //close export file
	     return 0;
}


int Gpio::unexport_gpio(){
		string unexport_str = "/sys/class/gpio/unexport";
	    ofstream unexportgpio(unexport_str.c_str()); //Open unexport file
	    if (unexportgpio < 0){
	        cout << " OPERATION FAILED: Unable to unexport GPIO"<< this->gpioname <<" ."<< endl;
	        return -1;
	    }

	    unexportgpio << this->gpioname ; //write GPIO number to unexport
	    unexportgpio.close(); //close unexport file
	    return 0;
}






void init(void){
        //left back
        LenA=Gpio("48","out");
		Lin1=Gpio("49","out");
        Lin2=Gpio("50","out");
        // left front
        LenB=Gpio("51","out");
        Lin3=Gpio("58","out");
        Lin4=Gpio("59","out");
        //right front
        RenA=Gpio("21","out");
        Rin1=Gpio("22","out");
        Rin2=Gpio("23","out");

		RenB=Gpio("24","out");
        Rin3=Gpio("25","out");
        Rin4=Gpio("26","out");

}

void forward(){
        LenA.set_value("1");
        LenB.set_value("1");
        RenA.set_value("1");
        RenB.set_value("1");

        Rin1.set_value("0");
        Rin2.set_value("1");

        Rin3.set_value("0");
        Rin4.set_value("1");

        Lin1.set_value("1");
        Lin2.set_value("0");
        Lin3.set_value("1");
        Lin4.set_value("0");



}
void backward(){
        LenA.set_value("1");
        LenB.set_value("1");
        RenA.set_value("1");
        RenB.set_value("1");

        Rin1.set_value("1");
        Rin2.set_value("0");

        Rin3.set_value("1");
        Rin4.set_value("0");

        Lin1.set_value("0");
        Lin2.set_value("1");

        Lin3.set_value("0");
        Lin4.set_value("1");


}
void stop(){

        LenA.set_value("0");
        LenB.set_value("0");
        RenA.set_value("0");
        RenB.set_value("0");

}
void rotate_right(){
        LenA.set_value("1");
        LenB.set_value("1");
        RenA.set_value("1");
        RenB.set_value("1");
        //left forward
        Lin1.set_value("1");
        Lin2.set_value("0");

        Lin3.set_value("1");
        Lin4.set_value("0");


        Rin1.set_value("1");
        Rin2.set_value("0");

        Rin3.set_value("1");
        Rin4.set_value("0");


}

void rotate_left(){
        LenA.set_value("1");
        LenB.set_value("1");
        RenA.set_value("1");
        RenB.set_value("1");
        //left backward
        Lin1.set_value("0");
        Lin2.set_value("1");

        Lin3.set_value("0");
        Lin4.set_value("1");

        Rin1.set_value("0");
        Rin2.set_value("1");
        Rin3.set_value("0");
        Rin4.set_value("1");


}



void right(){
	    LenA.set_value("1");
        LenB.set_value("1");
        RenA.set_value("1");
        RenB.set_value("1");
        
        Lin1.set_value("1");
        Lin2.set_value("0");
        Lin3.set_value("1");
        Lin4.set_value("0");
        Rin1.set_value("0");
        Rin2.set_value("0");
        Rin3.set_value("0");
        Rin4.set_value("0");


}




void left(){
	    LenA.set_value("1");
        LenB.set_value("1");
        RenA.set_value("1");
        RenB.set_value("1");
       
		Rin1.set_value("0");
        Rin2.set_value("1");
        Rin3.set_value("0");
        Rin4.set_value("1");

		Lin1.set_value("0");
        Lin2.set_value("0");

        Lin3.set_value("0");
        Lin4.set_value("0");


}









void calcDir(Mat &fra){
	size=1;
	minMaxLoc(currentFrame,&minVal,&maxVal,&minLoc,&maxLoc);
	threshold(currentFrame,currentFrame,THRESH,255,THRESH_BINARY_INV);

	
    Mat erodeElmt = getStructuringElement(MORPH_RECT, Size(5, 5));
    Mat dilateElmt = getStructuringElement(MORPH_RECT, Size(5, 5));
    erode(currentFrame, currentFrame, erodeElmt);
    dilate(currentFrame, currentFrame, dilateElmt);
   for(j=0;j<roi.height;j++){
	   for(i=0;i<roi.width;i++){
		//std::cout<<val<<std::endl;
				if(currentFrame.at<uchar>(j,i)>100){
					center.x+=i;
					center.y+=j;
					size++;
				   }


				}
		   }


   center.x=center.x/size;
   center.y=center.y/size;
   circle(currentFrame, center, 5, Scalar(0, 255,0), -1, 8, 0);


	if(minVal<THRESH){
		if(center.x>(160+PTH)){
			 circle(frame, center, 5, Scalar(0, 255,0), -1, 8, 0);
			lastmove='R';
			right();
            }

		else if(center.x<(160-PTH)){
			 circle(frame, center, 5, Scalar(255,0 ,0), -1, 8, 0);
           	left();
			lastmove='L';
            }
        else
			{
			circle(frame, center, 5, Scalar(0,0 ,255), -1, 8, 0);
        	forward();
			}
		}
    else{
		if(lastmove=='R'){
			rotate_right();
		}
		else if(lastmove=='L'){
			rotate_left();
		}
	}

//   prev.x=minLoc.x;
//   prev.y=minLoc.y;





}

void *UpdateFrame(void *arg)
{
    for(;;)
    {

    	 pthread_mutex_lock(&the_mutex);
        while(BUFF!=0)
        	pthread_cond_wait(&condRead, &the_mutex);
        vcap >> frame;
         cvtColor(frame,currentFrame,CV_BGR2GRAY);
        fcount++;

        BUFF=1;
        if(fcount%1000==0){
        	cout<<"FRAMERATE"<<endl;
        }
        pthread_cond_signal(&condProc);	/* wake up process thread */
        pthread_mutex_unlock(&the_mutex);	/* release the buffer */
    }
    pthread_exit(0);
}

void *ProcFrame(void *arg){
    for(;;)
    {

        pthread_mutex_lock(&the_mutex);
       if (BUFF == 0)
        	pthread_cond_wait(&condProc, &the_mutex);

       
        currentFrame=currentFrame(roi);
         GaussianBlur(currentFrame,currentFrame,Size(9,9),1,1);
        calcDir(currentFrame);

        BUFF= 0;
        pthread_cond_signal(&condRead);	/* wake up consumer */
        pthread_mutex_unlock(&the_mutex);	/* release the buffer */

       

     //   imshow("f",currentFrame);
        waitKey(1);

    }
    pthread_exit(0);
}

int main(int, char**) {
	if(!vcap.isOpened()){
		cout<<"cant open camera"<<endl;
	}

    vcap.set(CV_CAP_PROP_FRAME_HEIGHT,240);
    vcap.set(CV_CAP_PROP_FRAME_WIDTH,320);

	init();
	
    pthread_t upFr, proFr;

    // Initialize the mutex and condition variables

    pthread_mutex_init(&the_mutex, NULL);
    pthread_cond_init(&condProc, NULL);		/* Initialize process thread condition variable */
    pthread_cond_init(&condRead, NULL);		/* Initialize updateframe thread condition variable */

    // Create the threads
    pthread_create(&proFr, NULL, UpdateFrame, NULL);
    pthread_create(&upFr, NULL, ProcFrame, NULL);

    // Wait for the threads to finish
    // Otherwise main might run to the end
    // and kill the entire process when it exits.
    pthread_join(proFr, NULL);
    pthread_join(upFr, NULL);

    // Cleanup -- would happen automatically at end of program
    pthread_mutex_destroy(&the_mutex);  //	 Free up the_mutex
    pthread_cond_destroy(&condProc);		// Free up consumer condition variable
    pthread_cond_destroy(&condRead);	//	 Free up producer condition variable */
}
