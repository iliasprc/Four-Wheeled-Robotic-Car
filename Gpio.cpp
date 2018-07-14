/*

 * Gpio.cpp
 *
 *  Created on: Apr 1, 2017
 *      Author: iliaspap
 */

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "Gpio.h"

using namespace std;
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

    string setdir_str ="/sys/class/gpio/gpio" + this->gpioname + "/direction";
    ofstream setdirgpio(setdir_str.c_str()); // open direction file for gpio
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

	     exportgpio << this->gpioname ; //write GPIO number to export
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


