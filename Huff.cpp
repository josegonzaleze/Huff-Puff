// Authors: Jose Gonzales and Nathan Burner

//Note: This program only gets the count of the characters. counts how many 'a', 'b' etc
#include<fstream>
#include<iostream>
#include<iomanip>


using namespace std;
int main() {

	char fileName[80];

	cout << "Please enter the full file name: ";
	cin.getline(fileName, 20);
	cout << endl << "Name of file: " << fileName << endl;

	//Look for '.' on name given
	int pointLocation;
	char *temp = strchr(fileName, '.');
	string outputFile = "";

	//If there is one '.'
	if (temp != NULL) {
		// temp - filename will return the index value of '.'
		pointLocation = temp - fileName;
		for (int i = 0; i < pointLocation; i++) {
			outputFile += fileName[i];
		}
	}
	else {
		//If not '.' in file, then we want the whole text
		temp = strchr(fileName, '\0');
		pointLocation = temp - fileName;
		for (int i = 0; i < pointLocation; i++) {
			outputFile += fileName[i];
		}
	}

	//Adding the desired extension to the file
	outputFile += ".huff";

	//--Creating input file:
	ifstream fin(fileName, ios::binary);
	if (!fin) {
		//If file was not found, exit the program
		cout << "File does not exist, exiting..." << endl;
		system("PAUSE");
		return 0;
	}
	ofstream fout(outputFile);

	unsigned char buffer[16];
	int hexCount = 0;

	//Display of title:
	cout << "HEXDUMP FOR FILE: " << fileName << endl << endl;

	//Array that contains all the characters.
	/******************************************* 
		Ask Jose about this variable, crashed at the end of the program when set to 255,
		works fine with 256. Error was "Stack around the variable charCount was corrupted" 
	********************************************/
	int charCount[256];
	//Initializing to 0 since list is empty
	for (int i = 0; i <= 255; i++) {
		charCount[i] = 0;
	}
	//int number;
	while (!fin.eof())
	{

		fin.read((char*)buffer, 2);

		//Check how any bytes were read
		streamsize size = fin.gcount();
		if (size != 0) {


			
			//Display of Bynaries
			for (int i = 0; i < size; i++) {

				//cout << setfill('0') << setw(2) << uppercase << int(buffer[i]) << " ";
				charCount[(int)buffer[i]]++;

			}
		}
	}

	//Display of all the repetitions
	for (int i = 0; i <= 255; i++) {

		if (charCount[i] > 0)
		{
			cout << "Index " << i << " = " << charCount[i] << endl;
		}
	}

	fin.close();
	fout.close();
	return 0;
}