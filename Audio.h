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
	        //cout << sum.data[4] << "and " << rhs.data[4] << endl;
	        for (int i = 0; i < sum.data.size(); ++i) {
	            value = sum.data[i] + rhs.data[i];
	            sum.data[i] = (value > limit ? limit : value);
	        }

	        cout << "Done adding!" << endl;
	        //cout << "size of sum.data is " << sum.data.size() << endl;
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

	        Audio<T> result(*this);
	        int r = range.second - range.first;
			result.samples = result.data.size() - r;	//number of samples reduced
	        result.data.resize(result.samples);	
	        int count = 0;

	        //loop and copy the out-of-range items into result		
	        for (int i = 0; i < data.size(); i++) {		
	            if (i < range.first || i > range.second) {		
	                result.data[count++] = data[i];		
	            }		
	        }		

	        cout << "Cut operation done" << endl;
	        //cout << "New size of result is " << result.data.size() << endl;
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
			for (int i = 0; i < result.data.size(); i++) {
	            value = data[i] * F.first;		
	            result.data[i] = (value > limit) ? limit : value;		//saturation limits
	        }

	        cout << "Volume factor done" << endl;
	        return result;
	    }


	    /*----------------Other methods-----------------------------------------*/

	    //range add. adds two segments of two files together
	    Audio<T> radd(const Audio<T>& rhs, std::pair<float, float> range1, std::pair<float, float> range2) const {
	        Audio<T> result(*this);
	        result.data.resize(range1.second - range1.first);

	        std::pair<int, int> r1(int(range1.first), int(range1.second));
	        std::pair<int, int> r2(int(range2.first), int(range2.second));

	        //use the ^ operator to do the cuts for each
	        Audio<T> a1 = result^r1;
	        Audio<T> a2 = rhs^r2;

	        //return the sum of both
	        return a1 + a2;
    	}

    	//reverse operator using STL reverse
    	Audio<T> rev() const {
	        Audio<T> result(*this);
	        std::reverse(result.data.begin(), result.data.end());
	        return result;
	    }

		/*---------------Functor Operators-------------------------------------*/

	    //lambda
/*
	    static constexpr auto accumulate_function = [](double accumulator, const T& value) {
	         return accumulator + pow(value, 2);
	     };

	                                
	   double rms() const {
	        double result = std::accumulate(data.begin(), data.end(),
	                0, accumulate_function);
	        result /= samples;
	        result = pow(result, 0.5);
	        cout << " RMS = " << result << endl;
	        return result;
	    }

		//function object to normalize
	    template<typename U>
	    class normalize {
	    private:
	        float rootMeanSquare;
	        float d;
	    public:

	        normalize(float r, float _d) : rootMeanSquare(rms), d(_d) {
	        }

	        //overload () operator
	        double operator()(const U& a) const {
	            return a * (d / rootMeanSquare);
	        }
	    };

	    Audio<T> norm(float des)const {
	        Audio<T> result(*this);
	        std::transform(result.data.begin(), result.data.end(), result.data.begin(), normalize<T>(rms(), des));
	    } */
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
    //cout << "Number of samples is " << numSamples << endl;

    //allocate space of size numSamples in the vector then read into it
    data.resize(numSamples);
    ifs.read((char*)(intptr_t)&data[0], numSamples);
    ifs.close();

    //cout << "Size of vector in loadAudio() is " << data.size() << endl;
	
	cout << "Audio byte vector created" << endl;
	//return data;
}

template<typename T> bool saveAudio(vector<T>& data, string outputFile, int numSamples, int numChannels) {

		cout << "Beginning saveAudio" << endl;

	    std::ofstream out;
	   //cout << "Size of result data in saveAudio is " << data.size() << endl;
	   //cout << "Element 1 is " << data[4] << endl;
	   int fileSize = data.size()*sizeof(T)*numChannels;
        out.open(outputFile, ios::binary | ios::out);
        out.write(reinterpret_cast<char*>(&data[0]), fileSize);
        out.close();

        cout << "output written" << endl;
}



/*--------------------------THE SPECIALIZED TEMPLATE CLASS---------------------------------------*/
/*
  The only difference in this class is that the data vector is now a vector of std::pairs<T, T>.
  So its just replacing everything with pair and numChannels = 2
*/
template <typename T>
class Audio<std::pair<T, T>>{
	//private variables
	private:
	    int sampling_rate;
	    long samples;
	    int bits;
	    int channels;
		int limit;

	//public methods. Methods are defined in this .h file as well since its a templated class
	public:
		std::vector< std::pair<T, T> > data;	//to hold the audio bytes

		/* --------------The Six Special Member Constructors------------------*/

		//Constructor
		Audio(std::vector< std::pair<T, T> > audio, int sr, int num_bits) : data(audio), sampling_rate(sr), bits(num_bits), channels(2) {
   			 std::cout << "Constructor called." << std::endl;
		}

		//Destructor
		~Audio() {};

		//Copy Constructor
		Audio(const Audio & rhs) : sampling_rate(rhs.sampling_rate), bits(rhs.bits), channels(2), data(rhs.data) {
			std::cout << "Copy constructor called " << std::endl;

			//return *this;
		}

		//Copy Assignment
		Audio& operator=(const Audio & rhs){
			std::cout << "Copy assignment called " << std::endl;
			this->sampling_rate = rhs.sampling_rate;
			this->bits = rhs.bits;
			this->channels = 2;

			//copy vector
			for (int i=0; i<rhs.data.size(); i++){
				this->data.push_back(rhs.data[i]);
			}
			
			return *this;
		}

		//Move Constructor
		Audio(Audio && rhs) : sampling_rate(move(rhs.sampling_rate)), bits(move(rhs.bits)), data(move(rhs.data)), channels(2) {
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
		Audio< std::pair<T, T>> operator+(const Audio<std::pair<T, T>>& rhs)const {
			//Add two sound files of same sample rate and length etc. Clip to prevent saturation
			cout << "Adding the two together" << endl;
			int limit = (int(std::numeric_limits<T>::max()));

			//new object using copy constructor. copy this object and do modifications on the copy and return the copy
	        Audio<std::pair<T, T>> sum(*this);
	        int left, right;
	        //cout << sum.data[4] << "and " << rhs.data[4] << endl;

	        for (int i = 0; i < sum.data.size(); ++i) {
	            left = sum.data[i].first + rhs.data[i].first;
	            right = sum.data[i].second + rhs.data[i].second;
	            left = (left > limit ? limit : left);
	            right = (right > limit ? limit : right);
	            sum.data[i] = std::make_pair(left, right);
	        }

	        cout << "Done adding!" << endl;
	        //cout << "size of sum.data is " << sum.data.size() << endl;
	        return sum;
		}

		// | Concatenate operator.
		Audio<std::pair<T, T>> operator|(const Audio<std::pair<T, T>>& rhs)const {
			//Concatenate two audio files with same sampling, sample size, and mono/stereo settings
			cout << "Concatenating the two together" << endl;

			//new object using copy constructor. copy this object and do modifications on the copy and return the copy
	        Audio<std::pair<T, T>> concat(*this);

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
	    Audio<std::pair<T, T>> operator^(std::pair<int, int> range)const {
	    	//use the provided pair to cut the audio from the start and end points indicated
	    	cout << "Cutting audio from start to end points" << endl;

	        Audio<std::pair<T, T>> result(*this);
	        int r = range.second - range.first;
			result.samples = result.data.size() - r;	//number of samples reduced
	        result.data.resize(result.samples);	
	        int count = 0;

	        //loop and copy the out-of-range items into result		
	        for (int i = 0; i < data.size(); i++) {		
	            if (i < range.first || i > range.second) {		
	                result.data[count++] = data[i];		
	            }		
	        }		

	        cout << "Cut operation done" << endl;
	        //cout << "New size of result is " << result.data.size() << endl;
	        return result;
	    }

	    // * Volume factor 
	    Audio<std::pair<T, T>> operator*(std::pair<float, float> F)const {
	    	//For mono channel, use the first value in the pair to perform the operation. Multiply each 
	    	//sample value by the pair value. Take note of limits too.
	    	cout << "Volume factor operation starting" << endl;

	        Audio<std::pair<T, T>> result(*this);

	        //loop through and multiply
	        int left, right;
			for (int i = 0; i < result.data.size(); i++) {
	            left = data[i].first * F.first;	
	            right = data[i].second * F.second;

	            left = (left > limit ? limit : left);
	            right = (right > limit ? limit : right);
	            result.data[i] = std::make_pair(left, right);
	        }

	        cout << "Volume factor done" << endl;
	        return result;
	    }

	    /*----------------Other methods-------------------------------------*/

	    //reverse method with STL reverse
	    Audio< std::pair<T, T > > rev() {
	        Audio< std::pair<T, T > > result(*this);
	        std::reverse(result.data.begin(), result.data.end());
	        return result;
	    }

	    //range add
	    Audio<std::pair<T, T >> radd(const Audio<std::pair<T, T >>& rhs, std::pair<float, float> range1, std::pair<float, float> range2) const {
	        Audio<std::pair<T, T >> result(*this);
	        result.data.resize(range1.second - range1.first);

	        std::pair<int, int> r1(int(range1.first), int(range1.second));
	        std::pair<int, int> r2(int(range2.first), int(range2.second));

	        //use the ^ operator to do the cuts for each
	        Audio<std::pair<T, T >> a1 = result^r1;
	        Audio<std::pair<T, T >> a2 = rhs^r2;

	        //return the sum of both
	        return a1 + a2;
    	}

		/*---------------Functor Operators-------------------------------------*/


}; 


//loadAudio and saveAudio template functions
template<typename T> bool loadAudio(vector< std::pair<T, T> >& data, string filename, int numSamples, int numChannels) {
	//implement here
	cout << "Beginning loadAudio.." << endl;
	//vector<T> data;

    std::ifstream ifs;
    ifs.open(filename, ios::binary);

    //determine the size of the file using seekg and tellg
    ifs.seekg(0, ifs.end);
    int fileSize = ifs.tellg();
    ifs.seekg(0, ifs.beg);
    //cout << "filesize is "<< fileSize << ", size of T is " << sizeof(T) <<", numChannels is " << numChannels << endl;
    numSamples = fileSize / (sizeof (T) * numChannels);
    //cout << "Number of samples is " << numSamples << endl;

    //allocate space of size numSamples in the vector then read into it
    data.resize(numSamples);

    //do the reading in a loop
    T left, right;
    for (int i = 0; i < numSamples; i++) {
        ifs.read((char*)(intptr_t) &left, sizeof (T));
        ifs.read((char*)(intptr_t) &right, sizeof (T));
        data.push_back(std::make_pair(left, right));
    }
    ifs.close();
    //ifs.read((char*)(intptr_t)&data[0], numSamples);

    //cout << "Size of vector in loadAudio() is " << data.size() << endl;
	
	cout << "Audio byte vector created" << endl;
	//return data;
}

template<typename T> bool saveAudio(vector< std::pair<T, T> >& data, string outputFile, int numSamples, int numChannels) {

		cout << "Beginning saveAudio" << endl;

	    std::ofstream out;
	   //cout << "Size of result data in saveAudio is " << data.size() << endl;
	   //cout << "Element 1 is " << data[4] << endl;
	   int fileSize = data.size()*sizeof(T)*numChannels;
        out.open(outputFile, ios::binary | ios::out);

        for (int i = 0; i < numSamples; ++i) {
            out.write(reinterpret_cast<char*>(&data[i].first), sizeof (T));
            out.write(reinterpret_cast<char*>(&data[i].second), sizeof (T));
        }

        //out.write(reinterpret_cast<char*>(&data[0]), fileSize);
        out.close();

        cout << "output written" << endl;
}



/*------------END--------------*/
#endif
