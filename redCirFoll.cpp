
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
#define PTH 75
using namespace std;
using namespace cv;



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




Gpio LenA,Lin1,Lin2,LenB,Lin3,Lin4,RenA,Rin1,Rin2,RenB,Rin3,Rin4;


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
        RenA.set_value("0");
        RenB.set_value("0");
       Lin1.set_value("1");
        Lin2.set_value("0");

        Lin3.set_value("1");
        Lin4.set_value("0");


		

}

void left(){
	    LenA.set_value("0");
        LenB.set_value("0");
        RenA.set_value("1");
        RenB.set_value("1");
        
          Rin1.set_value("0");
        Rin2.set_value("1");
        Rin3.set_value("0");
        Rin4.set_value("1");

}

int main(int argc, char** argv)
{
  Mat src, src_gray;

  float area;
  /// Read the image
  VideoCapture cap(1); //capture the video from web cam
     Mat hsv_image,bgr_image;

     if ( !cap.isOpened() )  // if not success, exit program
     {
          cout << "Cannot open the web cam" << endl;
          return -1;
     }
     cap.set(CV_CAP_PROP_FRAME_HEIGHT,240);
     cap.set(CV_CAP_PROP_FRAME_WIDTH,320);
      Point center;
     int radius;
     
      int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH); 
	int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT); 
   VideoWriter video("outcpp.avi",CV_FOURCC('M','J','P','G'),10, Size(frame_width,frame_height));
   //  namedWindow( "Hough Circle Transform Demo" );

  while(1){

	  cap>>src;
	  /// Convert it to gray
	  cvtColor( src, src_gray, CV_BGR2GRAY );

	  /// Reduce the noise so we avoid false circle detection
	  GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

	  vector<Vec3f> circles;

	  /// Apply the Hough Transform to find the circles
	  HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/2, 100, 30, 0, 0 );

	  /*
	  if(circles.size()==0){
		  printf("PRSOFDFO \n");
	  }
	  */
	  /// Draw the circles detected
	  for( size_t i = 0; i < circles.size(); i++ )
	  {
		  Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		   radius = cvRound(circles[i][2]);
		  area=3.141*radius*radius;
		  // circle center
		  circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
		  // circle outline
		  circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
	   }


	  if(circles.size()!=0){
		if(radius>80){
		  stop();
		 // printf("STOP \n");

		}
	  else{
		  if(center.x>160+PTH){
			  right();//printf("RIGHT \n");
		  }
		  else if(center.x<160-PTH){
			  left();//printf("LEFT \n");
		  }
		  else
			  forward();//printf("forward \n");
	  }

	}	
	 else{
		stop();//printf("STOP no circle \n");
		}
		

	  /// Show your results

	 // imshow( "Hough Circle Transform Demo", src );

	  waitKey(5);
	 }
  return 0;
}
