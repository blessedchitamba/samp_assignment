/*the driver file for the samp application
  It will do the reading of audio file, loading of the byte array,
  as well as taking user input for audio processing options
*/

#include <fstream>  // Needed for file stream objects
#include <iostream> // Needed for console stream objects
#include <sstream>  // Needed for string stream objects
#include <string>   // Needed for strings
#include <vector>   // Needed for the vector container
#include "Audio.h"  // the file containing the functions

using namespace std;
//using namespace chtble001;

int main(int argc, char * argv[])
{
	//process user options
	typedef int8_t i8;
	typedef int16_t i16;
	typedef Audio <pair<int8_t,int8_t>> i8_pair;
	typedef Audio <pair<int16_t,int16_t>> i16_pair;

	//parse command line arguments
	string soundFile1, soundFile2;
	int numSamples1, numSamples2;
	int sampleRate = stoi(argv[2]);
	int numBits = stoi(argv[4]);
	int numChannels = stoi(argv[6]);
	int index = 6;	//current index of the numChannels in argv array
	string outputFile = "out.raw";	//default file name
	string temp = "-o";

	//if argv[7]== -o then argv[8] is output file name
	if(argv[7]==temp){

		++index;	//prefix. index now 7
		outputFile = argv[++index];	//prefix. argv[8] is referenced
	}

	//even if the above is not true, the next index are the operations
	string option = argv[++index];	//argv[9] if above is true, 7 if not

	//now process the individual options

	//IF OPTION IS ADD...
	if (option == "-add") {
		//implement add
		cout << "Add option selected.." << endl;
		soundFile1 = argv[++index];
		soundFile2 = argv[++index];
		
		cout<<soundFile1<<endl;
		cout<<soundFile2<<endl;

		//do the logic of creating the appropriate template object depending on the command line options
		if(numChannels==1)
		{	
			if(numBits==16)
			{
				//create the two vectors to be passed into the Audio constructors
				vector<i16> data1, data2;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				loadAudio(data2, soundFile2, numSamples2, numChannels);
				//cout << "Size of data1 is " << data1.size() << endl;
				cout << "Vectors created and loaded" << endl;

				//create the two Audio objects and the result object
				Audio<i16> a1(data1, sampleRate, numBits);
				Audio<i16> a2(data2, sampleRate, numBits);
				cout << "Audio objects created" << endl;
				//cout << "size of a1.data is " << a1.data.size() << endl;

				Audio<i16> result = a1 + a2;
				cout << "Result object created" << endl;
				//cout << "Size of result.data is " << result.data.size() << endl;
				saveAudio(result.data, outputFile, numSamples1, numChannels);
			}
			else
			{
				//create the two vectors to be passed into the Audio constructors
				vector<i8> data1, data2;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				loadAudio(data2, soundFile2, numSamples2, numChannels);
				cout << "Vectors created" << endl;

				//create the two Audio objects and the result object
				Audio<i8> a1(data1, sampleRate, numBits);
				Audio<i8> a2(data2, sampleRate, numBits);
				cout << "Audio objects created" << endl;

				Audio<i8> result = a1 + a2;
				cout << "Result object created" << endl;
				//cout << "Size of result.data is " << result.data.size() << endl;
				//result.setFile(out_file_name);
				saveAudio(result.data, outputFile, numSamples1, numChannels);
			}

	    }

	    else
	    {
			if(numBits==16)
			{
				//create the two vectors to be passed into the Audio constructors
				vector<std::pair<i16, i16> > data1, data2;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				loadAudio(data2, soundFile2, numSamples2, numChannels);
				//cout << "Size of data1 is " << data1.size() << endl;
				cout << "Vectors created and loaded" << endl;

				i16_pair a1(data1,sampleRate,numBits);
				i16_pair a2(data2,sampleRate,numBits);
				cout << "Audio objects created" << endl;

				i16_pair result=a1 + a2;
				cout << "Result object created" << endl;
				//cout << "Size of result.data is " << result.data.size() << endl;
				saveAudio(result.data, outputFile, numSamples1, numChannels);
			}
			else
			{
				//create the two vectors to be passed into the Audio constructors
				vector<std::pair<i8, i8> > data1, data2;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				loadAudio(data2, soundFile2, numSamples2, numChannels);
				//cout << "Size of data1 is " << data1.size() << endl;
				cout << "Vectors created and loaded" << endl;

				i8_pair a1(data1,sampleRate,numBits);
				i8_pair a2(data2,sampleRate,numBits);
				cout << "Audio objects created" << endl;
				
				i8_pair result=a1 + a2;
				cout << "Result object created" << endl;
				//cout << "Size of result.data is " << result.data.size() << endl;
				//result.setFile(out_file_name);
				saveAudio(result.data, outputFile, numSamples1, numChannels);
			}	    	
	    }
	}


	//IF OPTION IS CONCAT...
	else if (option == "-cat") {
		//implement concat
		cout << "Concat option selected.." << endl;
		soundFile1 = argv[++index];
		soundFile2 = argv[++index];

		//do the logic of creating the appropriate template object depending on the command line options
		if(numChannels==1)
		{	
			if(numBits==16)
			{
				//create the two vectors to be passed into the Audio constructors
				vector<i16> data1, data2;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				loadAudio(data2, soundFile2, numSamples2, numChannels);
				cout << "Size of data1 is " << data1.size() << endl;
				cout << "Vectors created and loaded" << endl;

				//create the two Audio objects and the result object
				Audio<i16> a1(data1, sampleRate, numBits);
				Audio<i16> a2(data2, sampleRate, numBits);
				cout << "Audio objects created" << endl;
				//cout << "size of a1.data is " << a1.data.size() << endl;

				Audio<i16> result = a1|a2;
				cout << "Result object created" << endl;
				//cout << "Size of result.data is " << result.data.size() << endl;
				saveAudio(result.data, outputFile, numSamples1+numSamples2, numChannels);
			}
			else
			{
				//create the two vectors to be passed into the Audio constructors
				vector<i8> data1, data2;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				loadAudio(data2, soundFile2, numSamples2, numChannels);
				cout << "Vectors created" << endl;

				//create the two Audio objects and the result object
				Audio<i8> a1(data1, sampleRate, numBits);
				Audio<i8> a2(data2, sampleRate, numBits);
				cout << "Audio objects created" << endl;

				Audio<i8> result = a1|a2;
				cout << "Result object created" << endl;
				//cout << "Size of result.data is " << result.data.size() << endl;
				//result.setFile(out_file_name);
				saveAudio(result.data, outputFile, numSamples1+numSamples2, numChannels);
			}

	    }

	    else
	    {
			if(numBits==16)
			{
				//create the two vectors to be passed into the Audio constructors
				vector<std::pair<i16, i16> > data1, data2;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				loadAudio(data2, soundFile2, numSamples2, numChannels);
				//cout << "Size of data1 is " << data1.size() << endl;
				cout << "Vectors created and loaded" << endl;

				i16_pair a1(data1,sampleRate,numBits);
				i16_pair a2(data2,sampleRate,numBits);
				cout << "Audio objects created" << endl;

				i16_pair result=a1|a2;
				cout << "Result object created" << endl;
				//cout << "Size of result.data is " << result.data.size() << endl;
				//result.setFile(out_file_name);
				saveAudio(result.data, outputFile, numSamples1, numChannels);
			}
			else
			{
				//create the two vectors to be passed into the Audio constructors
				vector<std::pair<i8, i8> > data1, data2;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				loadAudio(data2, soundFile2, numSamples2, numChannels);
				//cout << "Size of data1 is " << data1.size() << endl;
				cout << "Vectors created and loaded" << endl;

				i8_pair a1(data1,sampleRate,numBits);
				i8_pair a2(data2,sampleRate,numBits);
				cout << "Audio objects created" << endl;
				
				i8_pair result=a1|a2;
				cout << "Result object created" << endl;
				//cout << "Size of result.data is " << result.data.size() << endl;
				//result.setFile(out_file_name);
				saveAudio(result.data, outputFile, numSamples1, numChannels);
			}	    	
	    }

	}


	//IF OPTION IS CUT...
	else if (option == "-cut") {
		//implement cut
		cout << "Cut option selected.." << endl;

		//take the range end limits and put them in a std::pair<>
		int start = stoi(argv[++index]);
		int end = stoi(argv[++index]);
		std::pair<int, int> F(start, end);

		soundFile1 = argv[++index];

		//do the logic of creating the appropriate template object depending on the command line options
		if(numChannels==1)
		{	
			if(numBits==16)
			{
				//create the vector to be passed into the Audio constructors
				vector<i16> data1;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				cout << "Size of data1 is " << data1.size() << endl;
				cout << "Vector created and loaded" << endl;

				//create the Audio object and the result object
				Audio<i16> a1(data1, sampleRate, numBits);
				cout << "Audio object created" << endl;
				//cout << "size of a1.data is " << a1.data.size() << endl;

				Audio<i16> result = a1^F;
				cout << "Result object created" << endl;
				//cout << "Size of result.data is " << result.data.size() << endl;
				saveAudio(result.data, outputFile, numSamples1, numChannels);
			}
			else
			{
				//create the vector to be passed into the Audio constructors
				vector<i8> data1;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				cout << "Size of data1 is " << data1.size() << endl;
				cout << "Vector created and loaded" << endl;

				//create the Audio object and the result object
				Audio<i8> a1(data1, sampleRate, numBits);
				cout << "Audio object created" << endl;
				//cout << "size of a1.data is " << a1.data.size() << endl;

				Audio<i8> result = a1^F;
				cout << "Result object created" << endl;
				//cout << "Size of result.data is " << result.data.size() << endl;
				saveAudio(result.data, outputFile, numSamples1, numChannels);
			}

	    }

	    //else if two channels
	    else
	    {
			if(numBits==16)
			{
				//create the two vectors to be passed into the Audio constructors
				vector<std::pair<i16, i16> > data1;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				//cout << "Size of data1 is " << data1.size() << endl;
				cout << "Vector created and loaded" << endl;

				i16_pair a1(data1,sampleRate,numBits);
				cout << "Audio object created" << endl;

				i16_pair result=a1^F;
				cout << "Result object created" << endl;
				//cout << "Size of result.data is " << result.data.size() << endl;
				//result.setFile(out_file_name);
				saveAudio(result.data, outputFile, numSamples1, numChannels);
			}
			else
			{
				//create the two vectors to be passed into the Audio constructors
				vector<std::pair<i8, i8> > data1;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				//cout << "Size of data1 is " << data1.size() << endl;
				cout << "Vector created and loaded" << endl;

				i8_pair a1(data1,sampleRate,numBits);
				cout << "Audio object created" << endl;
				
				i8_pair result=a1^F;
				cout << "Result object created" << endl;
				//cout << "Size of result.data is " << result.data.size() << endl;
				//result.setFile(out_file_name);
				saveAudio(result.data, outputFile, numSamples1, numChannels);
			}	    	
	    }
	}


	//IF OPTION IS VOLUME FACTOR...
	else if (option == "-v") {
		//implement cut
		cout << "Volume Factor option selected.." << endl;

		//take the volume factor floats and put them in a std::pair<>
		float v1 = stof(argv[++index]);
		float v2 = stof(argv[++index]);
		std::pair<float, float> F(v1, v2);

		soundFile1 = argv[++index];

		//do the logic of creating the appropriate template object depending on the command line options
		if(numChannels==1)
		{	
			if(numBits==16)
			{
				//create the vector to be passed into the Audio constructors
				vector<i16> data1;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				//cout << "Size of data1 is " << data1.size() << endl;
				cout << "Vector created and loaded" << endl;

				//create the Audio object and the result object
				Audio<i16> a1(data1, sampleRate, numBits);
				cout << "Audio object created" << endl;
				//cout << "size of a1.data is " << a1.data.size() << endl;

				Audio<i16> result = a1*F;
				cout << "Result object created" << endl;
				//cout << "Size of result.data is " << result.data.size() << endl;
				saveAudio(result.data, outputFile, numSamples1, numChannels);
			}
			else
			{
				//create the vector to be passed into the Audio constructors
				vector<i8> data1;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				//cout << "Size of data1 is " << data1.size() << endl;
				cout << "Vector created and loaded" << endl;

				//create the Audio object and the result object
				Audio<i8> a1(data1, sampleRate, numBits);
				cout << "Audio object created" << endl;
				//cout << "size of a1.data is " << a1.data.size() << endl;

				Audio<i8> result = a1*F;
				cout << "Result object created" << endl;
				//cout << "Size of result.data is " << result.data.size() << endl;
				saveAudio(result.data, outputFile, numSamples1, numChannels);
			}

	    }

	    //else if two channels
	    else
	    {
			if(numBits==16)
			{
				//create the two vectors to be passed into the Audio constructors
				vector<std::pair<i16, i16> > data1;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				//cout << "Size of data1 is " << data1.size() << endl;
				cout << "Vector created and loaded" << endl;

				i16_pair a1(data1,sampleRate,numBits);
				cout << "Audio object created" << endl;

				i16_pair result=a1*F;
				cout << "Result object created" << endl;
				//cout << "Size of result.data is " << result.data.size() << endl;
				//result.setFile(out_file_name);
				saveAudio(result.data, outputFile, numSamples1, numChannels);
			}
			else
			{
				//create the two vectors to be passed into the Audio constructors
				vector<std::pair<i8, i8> > data1;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				//cout << "Size of data1 is " << data1.size() << endl;
				cout << "Vector created and loaded" << endl;

				i8_pair a1(data1,sampleRate,numBits);
				cout << "Audio objects created" << endl;
				
				i8_pair result=a1*F;
				cout << "Result object created" << endl;
				//cout << "Size of result.data is " << result.data.size() << endl;
				//result.setFile(out_file_name);
				saveAudio(result.data, outputFile, numSamples1, numChannels);
			}	    	
	    }
	}


	//ELSE IF OPTION IS RANGE ADD
	if (option == "-radd") {
		//implement add
		cout << "Range Add option selected.." << endl;

		//take the range end limits and put them in a std::pair<>
		int r1 = stoi(argv[++index]);
		int r2 = stoi(argv[++index]);
		std::pair<int, int> range1(r1, r2);

		int s1 = stoi(argv[++index]);
		int s2 = stoi(argv[++index]);
		std::pair<int, int> range2(s1, s2);

		soundFile1 = argv[++index];
		soundFile2 = argv[++index];

		//do the logic of creating the appropriate template object depending on the command line options
		if(numChannels==1)
		{	
			if(numBits==16)
			{
				//create the two vectors to be passed into the Audio constructors
				vector<i16> data1, data2;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				loadAudio(data2, soundFile2, numSamples2, numChannels);
				//cout << "Size of data1 is " << data1.size() << endl;
				cout << "Vectors created and loaded" << endl;

				//create the two Audio objects and the result object
				Audio<i16> a1(data1, sampleRate, numBits);
				Audio<i16> a2(data2, sampleRate, numBits);
				cout << "Audio objects created" << endl;
				//cout << "size of a1.data is " << a1.data.size() << endl;

				Audio<i16> result = a1.radd(a2, range1, range2);
				cout << "Result object created" << endl;
				//cout << "Size of result.data is " << result.data.size() << endl;
				saveAudio(result.data, outputFile, numSamples1, numChannels);
			}
			else
			{
				//create the two vectors to be passed into the Audio constructors
				vector<i8> data1, data2;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				loadAudio(data2, soundFile2, numSamples2, numChannels);
				cout << "Vectors created" << endl;

				//create the two Audio objects and the result object
				Audio<i8> a1(data1, sampleRate, numBits);
				Audio<i8> a2(data2, sampleRate, numBits);
				cout << "Audio objects created" << endl;

				Audio<i8> result = a1.radd(a2, range1, range2);
				cout << "Result object created" << endl;
				//cout << "Size of result.data is " << result.data.size() << endl;
				//result.setFile(out_file_name);
				saveAudio(result.data, outputFile, numSamples1, numChannels);
			}

	    }

	    else
	    {
			if(numBits==16)
			{
				//create the two vectors to be passed into the Audio constructors
				vector<std::pair<i16, i16> > data1, data2;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				loadAudio(data2, soundFile2, numSamples2, numChannels);
				//cout << "Size of data1 is " << data1.size() << endl;
				cout << "Vectors created and loaded" << endl;

				i16_pair a1(data1,sampleRate,numBits);
				i16_pair a2(data2,sampleRate,numBits);
				cout << "Audio objects created" << endl;

				i16_pair result=a1 + a2;
				cout << "Result object created" << endl;
				//cout << "Size of result.data is " << result.data.size() << endl;
				//result.setFile(out_file_name);
				saveAudio(result.data, outputFile, numSamples1, numChannels);
			}
			else
			{
				//create the two vectors to be passed into the Audio constructors
				vector<std::pair<i8, i8> > data1, data2;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				loadAudio(data2, soundFile2, numSamples2, numChannels);
				//cout << "Size of data1 is " << data1.size() << endl;
				cout << "Vectors created and loaded" << endl;

				i8_pair a1(data1,sampleRate,numBits);
				i8_pair a2(data2,sampleRate,numBits);
				cout << "Audio objects created" << endl;
				
				i8_pair result=a1 + a2;
				cout << "Result object created" << endl;
				//cout << "Size of result.data is " << result.data.size() << endl;
				//result.setFile(out_file_name);
				saveAudio(result.data, outputFile, numSamples1, numChannels);
			}	    	
	    }
	}

	//ELSE IF OPTION IS REVERSE
	else if (option == "-rev") {
		//implement cut
		cout << "Reverse option selected.." << endl;

		soundFile1 = argv[++index];

		//do the logic of creating the appropriate template object depending on the command line options
		if(numChannels==1)
		{	
			if(numBits==16)
			{
				//create the vector to be passed into the Audio constructors
				vector<i16> data1;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				//cout << "Size of data1 is " << data1.size() << endl;
				cout << "Vector created and loaded" << endl;

				//create the Audio object and the result object
				Audio<i16> a1(data1, sampleRate, numBits);
				cout << "Audio object created" << endl;
				//cout << "size of a1.data is " << a1.data.size() << endl;

				Audio<i16> result = a1.rev();
				cout << "Result object created" << endl;
				//cout << "Size of result.data is " << result.data.size() << endl;
				saveAudio(result.data, outputFile, numSamples1, numChannels);
			}
			else
			{
				//create the vector to be passed into the Audio constructors
				vector<i8> data1;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				//cout << "Size of data1 is " << data1.size() << endl;
				cout << "Vector created and loaded" << endl;

				//create the Audio object and the result object
				Audio<i8> a1(data1, sampleRate, numBits);
				cout << "Audio object created" << endl;
				//cout << "size of a1.data is " << a1.data.size() << endl;

				Audio<i8> result = a1.rev();
				cout << "Result object created" << endl;
				//cout << "Size of result.data is " << result.data.size() << endl;
				saveAudio(result.data, outputFile, numSamples1, numChannels);
			}

	    }

	    //else if two channels
	    else
	    {
			if(numBits==16)
			{
				//create the two vectors to be passed into the Audio constructors
				vector<std::pair<i16, i16> > data1;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				//cout << "Size of data1 is " << data1.size() << endl;
				cout << "Vector created and loaded" << endl;

				i16_pair a1(data1,sampleRate,numBits);
				cout << "Audio object created" << endl;
				
				i16_pair result = a1.rev();
				cout << "Result object created" << endl;
				//cout << "Size of result.data is " << result.data.size() << endl;
				//result.setFile(out_file_name);
				saveAudio(result.data, outputFile, numSamples1, numChannels);
			}
			else
			{
				//create the two vectors to be passed into the Audio constructors
				vector<std::pair<i8, i8> > data1;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				//cout << "Size of data1 is " << data1.size() << endl;
				cout << "Vector created and loaded" << endl;

				i8_pair a1(data1,sampleRate,numBits);
				cout << "Audio objects created" << endl;
				
				i8_pair result = a1.rev();
				cout << "Result object created" << endl;
				//cout << "Size of result.data is " << result.data.size() << endl;
				//result.setFile(out_file_name);
				saveAudio(result.data, outputFile, numSamples1, numChannels);
			}	    	
	    }
	}


	/*//ELSE IF OPTION IS RMS
	else if (option == "-rms") {
		//implement cut
		cout << "RMS option selected.." << endl;

		soundFile1 = argv[++index];

		//do the logic of creating the appropriate template object depending on the command line options
		if(numChannels==1)
		{	
			if(numBits==16)
			{
				//create the vector to be passed into the Audio constructors
				vector<i16> data1;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				cout << "Size of data1 is " << data1.size() << endl;
				cout << "Vector created and loaded" << endl;

				//create the Audio object and the result object
				Audio<i16> a1(data1, sampleRate, numBits);
				cout << "Audio object created" << endl;
				//cout << "size of a1.data is " << a1.data.size() << endl;

				a1.rms();
				cout << "Result object created" << endl;
				cout << "Size of result.data is " << a1.data.size() << endl;
				saveAudio(a1.data, outputFile, numSamples1, numChannels);
			}
			else
			{
				//create the vector to be passed into the Audio constructors
				vector<i8> data1;
				loadAudio(data1, soundFile1, numSamples1, numChannels);
				cout << "Size of data1 is " << data1.size() << endl;
				cout << "Vector created and loaded" << endl;

				//create the Audio object and the result object
				Audio<i8> a1(data1, sampleRate, numBits);
				cout << "Audio object created" << endl;
				//cout << "size of a1.data is " << a1.data.size() << endl;

				a1.rms();
				cout << "Result object created" << endl;
				cout << "Size of result.data is " << a1.data.size() << endl;
				saveAudio(a1.data, outputFile, numSamples1, numChannels);
			}

	    }
	 }*/
}
