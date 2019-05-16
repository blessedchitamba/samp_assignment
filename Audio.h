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
	    //std::vector<T> data;	//to hold the audio bytes
	    int sampling_rate;
	    long samples;
	    int bits;
	    int channels;
		int limit;

	//public methods. Methods are defined in this .h file as well since its a templated class
	public:
		std::vector<T> data;	//to hold the audio bytes

		/* --------------The Six Special Member Constructors------------------*/

		//Constructor
		Audio(std::vector<T> audio, int sr, int num_bits) : data(audio), sampling_rate(sr), bits(num_bits), channels(1) {
   			 std::cout << "Constructor called." << std::endl;
		}

		//Destructor
		~Audio() {};

		//Copy Constructor
		Audio(const Audio & rhs) : sampling_rate(rhs.sampling_rate), bits(rhs.bits), channels(1), data(rhs.data) {
			std::cout << "Copy constructor called " << std::endl;

			//return *this;
		}

		//Copy Assignment
		Audio& operator=(const Audio & rhs){
			std::cout << "Copy assignment called " << std::endl;
			this->sampling_rate = rhs.sampling_rate;
			this->bits = rhs.bits;
			this->channels = 1;

			//copy vector
			for (int i=0; i<rhs.data.size(); i++){
				this->data.push_back(rhs.data[i]);
			}
			
			return *this;
		}

		//Move Constructor
		Audio(Audio && rhs) : sampling_rate(move(rhs.sampling_rate)), bits(move(rhs.bits)), data(move(rhs.data)), channels(1) {
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

		//Add operator +.
		Audio<T> operator+(const Audio<T>& rhs)const {
			//Add two sound files of same sample rate and length etc. Clip to prevent saturation
			cout << "Adding the two together" << endl;
			int limit = (int(std::numeric_limits<T>::max()));

			//new object using copy constructor. copy this object and do modifications on the copy and return the copy
	        Audio<T> sum(*this);
	        int value;
	        cout << sum.data[4] << "and " << rhs.data[4] << endl;
	        for (int i = 0; i < sum.samples; ++i) {
	            value = sum.data[i] + rhs.data[i];
	            sum.data[i] = (value > limit ? limit : value);
	        }

	        cout << "Done adding!" << endl;
	        cout << "size of sum.data is " << sum.data.size() << endl;
	        return sum;
		}

		// | Concatenate operator.
		Audio<T> operator|(const Audio<T>& rhs)const {
			//Concatenate two audio files with same sampling, sample size, and mono/stereo settings
			cout << "Concatenating the two together" << endl;

			//new object using copy constructor. copy this object and do modifications on the copy and return the copy
	        Audio<T> concat(*this);

	        //resize the length of concat data vector to be able to accommodate the sum of the two operands
	        concat.data.resize(data.size() + rhs.data.size());

	        //number of samples too
	        //concat.samples = samples + rhs.samples;

	        //add rhs data array to the other half of concat.data
	        for (int i = data.size(); i < concat.data.size(); i++) {
	            concat.data[i] = rhs.data[i - data.size()];
	        }

	        cout << "Done concatenating" << endl;
	        return concat;
	    }

	    // ^ Cut operation.
	    Audio<T> operator^(std::pair<int, int> range)const {
	    	//use the provided pair to cut the audio from the start and end points indicated
	    	cout << "Cutting audio from start to end points" << endl;

	        Audio<T> result;
	        int r = range.second - range.first;
			result.samples = samples - r;	//number of samples reduced
	        result.data.resize(result.samples);	
	        int count = 0;

	        //loop and copy the out-of-range items into result		
	        for (int i = 0; i < samples; i++) {		
	            if (i < range.first || i > range.second) {		
	                result.data[count++] = data[i];		
	            }		
	        }		

	        cout << "Cut operation done" << endl;
	        return result;
	    }

	    // * Volume factor 
	    Audio<T> operator*(std::pair<float, float> F)const {
	    	//For mono channel, use the first value in the pair to perform the operation. Multiply each 
	    	//sample value by the pair value. Take note of limits too.
	    	cout << "Volume factor operation starting" << endl;

	        Audio<T> result(*this);

	        //loop through and multiply
	        int value;
			for (int i = 0; i < result.samples; i++) {
	            value = data[i] * F.first;		
	            result.data[i] = (value > limit) ? limit : value;		//saturation limits
	        }

	        cout << "Volume factor done" << endl;
	        return result;
	    }


		/*---------------Functor Operators-------------------------------------*/

};


//loadAudio and saveAudio template functions
template<typename T> bool loadAudio(vector<T>& data, string filename, int numSamples, int numChannels) {
	//implement here
	cout << "Beginning loadAudio.." << endl;
	//vector<T> data;

    std::ifstream ifs;
    ifs.open(filename, ios::binary);

    //determine the size of the file using seekg and tellg
    ifs.seekg(0, ifs.end);
    int fileSize = ifs.tellg();
    ifs.seekg(0, ifs.beg);
    numSamples = fileSize / (sizeof (T) * numChannels);
    cout << "Number of samples is " << numSamples << endl;

    //allocate space of size numSamples in the vector then read into it
    data.resize(numSamples);
    ifs.read((char*)(intptr_t)&data[0], numSamples);

    cout << "Size of vector in loadAudio() is " << data.size() << endl;
	
	cout << "Audio byte vector created" << endl;
	//return data;
}

template<typename T> bool saveAudio(vector<T>& data, string outputFile, int numSamples, int numChannels) {

		cout << "Beginning saveAudio" << endl;

	    std::ofstream out;
	   cout << "Size of result data in saveAudio is " << data.size() << endl;
	   //cout << "Element 1 is " << data[4] << endl;
	   int fileSize = data.size()*sizeof(T)*numChannels;
        out.open(outputFile, ios::binary | ios::out);
        out.write(reinterpret_cast<char*>(&data[0]), fileSize);
        out.close();

        cout << "output written" << endl;
}


/*------------END--------------*/
#endif
