/*
  This is the templated Audio class that is meant to hold a single audio file
  instance extracted from a .raw file, and will be stored in the form of a 
  std::vector<T> where T is either int8_t or int16_t.
*/

#ifndef AUDIO_H
#define AUDIO_H

#include <vector>
#include <ostream>
#include <istream>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <numeric>
#include <cmath>


using namespace std;

template <typename T>
class Audio{
	//private variables
	private:
	    std::vector<T> data;	//to hold the audio bytes
	    int sampling_rate;
	    long samples;
	    int bits;
	    int channels;
		int limit;

	//public methods. Methods are defined in this .h file as well since its a templated class
	public:

		/* --------------The Six Special Member Constructors------------------*/

		//Constructor
		Audio(std::vector<T> audio, int sr, int num_bits) : data(audio), sampling_rate(sr), bits(num_bits), channels(1) {
   			 std::cout << "Constructor called." << std::endl;
		}

		//Destructor
		~Audio();

		//Copy Constructor
		Audio(const Audio & rhs) : sampling_rate(rhs.sampling_rate), bits(rhs.bits), channels(1), data(rhs.data) {
			std::cout << "Copy constructor called " << std::endl;

			//return *this;
		}

		//Copy Assignment
		Audio& operator=(const Audio & rhs) : sampling_rate(rhs.sampling_rate), bits(rhs.bits), channels(1), data(rhs.data){
			std::cout << "Copy assignment called " << std::endl;
			
			return *this;
		}

		//Move Constructor
		Audio(Audio && rhs) : sampling_rate(move(rhs.sampling_rate)), bits(move(rhs.bits)), data(move(rhs.data), channels(1)) {
			std::cout << "Move constructor called " << std::endl;
			rhs.data.clear();
		}

		//Move Assignment
		Audio& operator=(Audio && rhs){
			std::cout << "Move assignment called " << std::endl;
			this->sampling_rate = std::move(rhs.sampling_rate);
			this->bits = std::move(rhs.bits);
			this->data = std::move(rhs.data);
			rhs.data.clear();
			return *this;
		}



		/* --------------Overloaded Operators----------------------------------*/

		//

		/*---------------Functor Operators-------------------------------------*/

};

#endif