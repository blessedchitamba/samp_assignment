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
	int numSamples;
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
				loadAudio(data1, soundFile1, numSamples, numChannels);
				loadAudio(data2, soundFile2, numSamples, numChannels);
				cout << "Vectors created and loaded" << endl;

				//create the two Audio objects and the result object
				Audio<i16> a1(data1, sampleRate, numBits);
				Audio<i16> a2(data2, sampleRate, numBits);
				cout << "Audio objects created" << endl;
				cout << "size of a1.data is " << a1.data.size() << endl;

				Audio<i16> result = a1 + a2;
				cout << "Result object created" << endl;
				//result.setFile(out_file_name);
				//result.saveFile();
			}
			else
			{
				//create the two vectors to be passed into the Audio constructors
				vector<i8> data1, data2;
				loadAudio(data1, soundFile1, numSamples, numChannels);
				loadAudio(data2, soundFile2, numSamples, numChannels);
				cout << "Vectors created" << endl;

				//create the two Audio objects and the result object
				Audio<i8> a1(data1, sampleRate, numBits);
				Audio<i8> a2(data2, sampleRate, numBits);
				cout << "Audio objects created" << endl;

				Audio<i8> result = a1 + a2;
				cout << "Result object created" << endl;
				//result.setFile(out_file_name);
				//result.saveFile()
			}

	    }
	}
}
