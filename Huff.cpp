// Authors: Jose Gonzales and Nathan Burner
#include<fstream>
#include<iostream>
#include<iomanip>
#include<string>
#include<vector>
#include<queue>
#include<algorithm>
//#include<functional>


using namespace std;

struct Huffman
{
	int glyph;
	double count;
	int left;
	int right;

};

//CREATION OF THE HUFMANN ARRAY//
struct Huffman huffmanArray[513];
int huffmanentries;
string huffmanCodes[258];
int codesCount = 0;

void printHuffman(int index, int huffmanGlyphCount, string code) {
	if (index > huffmanGlyphCount || index == -1) {
		return;
	}
	if (huffmanArray[index].glyph != -1) {
		//cout << huffmanArray[index].glyph << ": " << code << "\n";
		//Save the code here
		huffmanCodes[huffmanArray[index].glyph] = code;
		codesCount++;
		
	}

	printHuffman(huffmanArray[index].left, huffmanGlyphCount, code + "0");
	printHuffman(huffmanArray[index].right, huffmanGlyphCount, code + "1");
}
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
	//Getting a string of the original name
	string inputFile = "";
	for (int i = 0; i < sizeof(fileName); i++) {
		if (fileName[i] != '\0') 
			inputFile = inputFile + fileName[i];
		else
			i = sizeof(fileName) - 1;
		
	}
	//Adding the desired extension to the file
	outputFile += ".huf";

	//--Creating input file:
	ifstream fin(fileName, ios::binary);
	if (!fin) {
		//If file was not found, exit the program
		cout << "File does not exist, exiting..." << endl;
		system("PAUSE");
		return 0;
	}
	//--Creating output file
	ofstream fout(outputFile, ios::binary);

	unsigned char buffer[16];
	int hexCount = 0;

	//Array that contains all the characters count.
	double glyphCount[257];
	//Creating array of glyphs
	int glyph[257];


	//Initializing to 0 on glyphcount since list is empty
	for (int i = 0; i < 257; i++) {
		glyphCount[i] = 0;
	}
	//Initializing to -1 on the huffman codes.
	for (int i = 0; i < 258; i++) {
		huffmanCodes[i] = "2";
	}
	int number;

	/*Get the size of the file.
	streamoff sizeOfFile = 0;
	fin.seekg(0,fin.end);
	sizeOfFile = fin.tellg();
	fin.seekg(0);
	int const x = (int)sizeOfFile;
	int FileChar[x]*/;

	while (!fin.eof())
	{
		
		fin.read((char*)buffer, 16);

		//Check how any bytes were read
		streamsize size = fin.gcount();
		if (size != 0) {

			//Display of Bynaries
			for (int i = 0; i < size; i++) {

				//cout << setfill('0') << setw(2) << uppercase << int(buffer[i]) << " ";
				glyphCount[(int)buffer[i]]++;
			}
		}
	}
	//Build the Glyph array based on the Hex code found.
	int glyphNumber = 0;
	for (int i = 0; i < 257; i++) {
		if (glyphCount[i] != 0) {
			glyph[glyphNumber] = i;
			glyphNumber++;
		}
	}

	//Display of all the repetitions
	/*glyphNumber = 0;
	for(int i = 0; i < 257; i++) {

	if (glyphCount[i] != 0) {
	cout << glyph[glyphNumber] << " = " << glyphCount[i] << endl;
	glyphNumber++;
	}
	}
	*/
	//Computing the size


	//***********************************************//

	int huffmanGlyphCount = 0;
	for (int i = 0; i < glyphNumber; i++) {
		huffmanArray[i].glyph = glyph[i];
		huffmanArray[i].count = glyphCount[(int)glyph[i]];
		huffmanArray[i].left = -1;
		huffmanArray[i].right = -1;
		huffmanGlyphCount++;
	}
	//Addding the eof
	huffmanArray[huffmanGlyphCount].glyph = 256;
	huffmanArray[huffmanGlyphCount].count = 1;
	huffmanArray[huffmanGlyphCount].left = -1;
	huffmanArray[huffmanGlyphCount].right = -1;
	huffmanGlyphCount++;
	

	//*****AT THIS POINT WE HAVE THE HUFFMAN INITIAL ARRAY*****//

	//Creating a mean heap from the array
	int leastspot = 0;
	double leastFreq = 0;
	int leastAdd = 0;
	int indexsearch = 0;
	bool change = false;
	//----
	int tempGlyph;
	double tempCount;
	int tempLeft;
	int tempRight;

	for (int i = 0; i < huffmanGlyphCount; i++) {
		leastspot = i;
		leastFreq = huffmanArray[i].count;
		leastAdd = i;
		indexsearch = leastspot;
		for (indexsearch; indexsearch < huffmanGlyphCount; indexsearch++) {

			if (huffmanArray[indexsearch].count < leastFreq) {
				leastFreq = huffmanArray[indexsearch].count;
				leastAdd = indexsearch;
				change = true;
			}
		}
		if (change) {
			tempGlyph = huffmanArray[leastspot].glyph;
			tempCount = huffmanArray[leastspot].count;
			tempLeft = huffmanArray[leastspot].left;
			tempRight = huffmanArray[leastspot].right;

			huffmanArray[leastspot].glyph = huffmanArray[leastAdd].glyph;
			huffmanArray[leastspot].count = huffmanArray[leastAdd].count;
			huffmanArray[leastspot].left = huffmanArray[leastAdd].left;
			huffmanArray[leastspot].right = huffmanArray[leastAdd].right;

			huffmanArray[leastAdd].glyph = tempGlyph;
			huffmanArray[leastAdd].count = tempCount;
			huffmanArray[leastAdd].left = tempLeft;
			huffmanArray[leastAdd].right = tempRight;
		}
	}


	//*****AT THIS POINT WE HAVE OUR MIN HEAP***//
	

	//******HUFFMAN ALGORITHM!!*****//
	int m = 0;
	int h = huffmanGlyphCount - 1;
	int f = huffmanGlyphCount;
	for (int i = 0; i < huffmanGlyphCount - 1; i++) {
		//Mark m
		if (huffmanArray[2].count < huffmanArray[1].count && h > 1) {
			m = 2;
		}
		else {
			m = 1;
		}
		//Move m -> f
		huffmanArray[f].glyph = huffmanArray[m].glyph;
		huffmanArray[f].count = huffmanArray[m].count;
		huffmanArray[f].left = huffmanArray[m].left;
		huffmanArray[f].right = huffmanArray[m].right;

		//Move h -> m
		huffmanArray[m].glyph = huffmanArray[h].glyph;
		huffmanArray[m].count = huffmanArray[h].count;
		huffmanArray[m].left = huffmanArray[h].left;
		huffmanArray[m].right = huffmanArray[h].right;

		//Reheap?
		//Creating a mean heap from the array
		leastspot = 0;
		leastFreq = 0;
		leastAdd = 0;
		indexsearch = 0;
		change = false;
		//----
		tempGlyph;
		tempCount;
		tempLeft;
		tempRight;

		for (int i = 0; i < h; i++) {
			leastspot = i;
			leastFreq = huffmanArray[i].count;
			leastAdd = i;
			indexsearch = leastspot;
			for (indexsearch; indexsearch < h; indexsearch++) {

				if (huffmanArray[indexsearch].count < leastFreq) {
					leastFreq = huffmanArray[indexsearch].count;
					leastAdd = indexsearch;
					change = true;
				}
			}
			if (change) {
				tempGlyph = huffmanArray[leastspot].glyph;
				tempCount = huffmanArray[leastspot].count;
				tempLeft = huffmanArray[leastspot].left;
				tempRight = huffmanArray[leastspot].right;

				huffmanArray[leastspot].glyph = huffmanArray[leastAdd].glyph;
				huffmanArray[leastspot].count = huffmanArray[leastAdd].count;
				huffmanArray[leastspot].left = huffmanArray[leastAdd].left;
				huffmanArray[leastspot].right = huffmanArray[leastAdd].right;

				huffmanArray[leastAdd].glyph = tempGlyph;
				huffmanArray[leastAdd].count = tempCount;
				huffmanArray[leastAdd].left = tempLeft;
				huffmanArray[leastAdd].right = tempRight;
			}
		}
		//Move 0 -> h
		huffmanArray[h].glyph = huffmanArray[0].glyph;
		huffmanArray[h].count = huffmanArray[0].count;
		huffmanArray[h].left = huffmanArray[0].left;
		huffmanArray[h].right = huffmanArray[0].right;

		//Create a merge/freq node at 0
		huffmanArray[0].glyph = -1;
		huffmanArray[0].count = huffmanArray[h].count + huffmanArray[f].count;
		huffmanArray[0].left = h;
		huffmanArray[0].right = f;

		//Reheap??
		//Creating a mean heap from the array
		leastspot = 0;
		leastFreq = 0;
		leastAdd = 0;
		indexsearch = 0;
		change = false;
		//----
		tempGlyph;
		tempCount;
		tempLeft;
		tempRight;

		for (int i = 0; i < h; i++) {
			leastspot = i;
			leastFreq = huffmanArray[i].count;
			leastAdd = i;
			indexsearch = leastspot;
			for (indexsearch; indexsearch < h; indexsearch++) {

				if (huffmanArray[indexsearch].count < leastFreq) {
					leastFreq = huffmanArray[indexsearch].count;
					leastAdd = indexsearch;
					change = true;
				}
			}
			if (change) {
				tempGlyph = huffmanArray[leastspot].glyph;
				tempCount = huffmanArray[leastspot].count;
				tempLeft = huffmanArray[leastspot].left;
				tempRight = huffmanArray[leastspot].right;

				huffmanArray[leastspot].glyph = huffmanArray[leastAdd].glyph;
				huffmanArray[leastspot].count = huffmanArray[leastAdd].count;
				huffmanArray[leastspot].left = huffmanArray[leastAdd].left;
				huffmanArray[leastspot].right = huffmanArray[leastAdd].right;

				huffmanArray[leastAdd].glyph = tempGlyph;
				huffmanArray[leastAdd].count = tempCount;
				huffmanArray[leastAdd].left = tempLeft;
				huffmanArray[leastAdd].right = tempRight;
			}
		}

		//Move h(to the left) and f (to the right)
		h--;
		f++;
	}

	//***AT THIS POINT WE HAVE THE ARRAY READY TO BE WRITTEN TO FILE**//


	//Building the huffman codes:
	string code;
	int index = 0;
	printHuffman(index, huffmanGlyphCount*2 -1, code);


	//****AT THIS POINT WE HAVE THE HUFFMAN CODES IN AN ARRAY WHICH INDEX IS THE GLYPH FOR SUCH CODE****//

	//Writing to file.

	//Writing the size of name
	int nameSize = inputFile.size();
	fout.write((char*)&nameSize,sizeof nameSize);
	fout.flush();

	//Writing name of file
	fout.write(fileName, nameSize);
	fout.flush();

	//Writing name of Huffman Table entries.
	int tableEntries = huffmanGlyphCount * 2 - 1;
	fout.write((char*)&tableEntries, sizeof tableEntries);
	fout.flush();

	//Writing the Huffman table.
	for (int i = 0; i < huffmanGlyphCount * 2 - 1; i++) {
		fout.write((char*)&huffmanArray[i].glyph, sizeof huffmanArray[i].glyph);
		fout.write((char*)&huffmanArray[i].left, sizeof huffmanArray[i].left);
		fout.write((char*)&huffmanArray[i].right, sizeof huffmanArray[i].right);
		fout.flush();
	}

	cout << "Working... This may take a long time... be patient" << endl;
	//Write the encoded glyphs
	string binaryCodeRead = "";
	fin.clear();
	fin.seekg(0, ios::beg);
	string listOfCodes = "";
	string byteLenghtCode = "";
	while (fin.read((char*)buffer, 1)) {

		//Check how many bytes were read
		streamsize size = fin.gcount();
		if (size != 0) {

			//create the string code
			for (int i = 0; i < size; i++) {

				binaryCodeRead = binaryCodeRead + huffmanCodes[(int)buffer[i]];
			}

			//Adding the eof
			if (fin.eof()) {
				binaryCodeRead = binaryCodeRead + huffmanCodes[256];
			}

			listOfCodes = listOfCodes + binaryCodeRead;

			if (listOfCodes.size() < 8) {
				//Keep looping until we get 8

				//but if it is the eof we need to work with what we got
				if (fin.eof()) {
					//Get the bits to write to file
					byteLenghtCode = listOfCodes.substr(0, listOfCodes.size());
					//reverse(byteLenghtCode.begin(),byteLenghtCode.end());

					//Time to write to file
					//get one byte
					char binaryCode_group[9] = "00000000";
					bool notLast = true;

					for (int i = 0; i < byteLenghtCode.size(); i++) {

						binaryCode_group[i] = byteLenghtCode[i];
					}

					//byte to be encoded
					unsigned char byte1 = '\0';
					//length of huffman code
					int bitstringLength = strlen(binaryCode_group);

					//building an encoded byte from right to left
					int cnt = 0;
					for (int i = 0; i < bitstringLength; i++)
					{
						//is the bit "on"?
						if (binaryCode_group[i] == '1')
							//turn the bit on using the OR bitwise operator
							byte1 = byte1 | (int)pow(2.0, cnt);
						cnt++;
					}

					fout.write((char*)&byte1, sizeof byte1);
					listOfCodes.erase(0, listOfCodes.size());
				}
			}
			else {
				//Get the first 8 to write to file
				byteLenghtCode = listOfCodes.substr(0,listOfCodes.size());
				//reverse(byteLenghtCode.begin(),byteLenghtCode.end());

				//Time to write to file
				//get one byte
				char binaryCode_group[9] = "00000000";
				bool notLast = true;

						for (int i = 0; i < 8; i++) {

							binaryCode_group[i] = byteLenghtCode[i];
						}

						//byte to be encoded
						unsigned char byte1 = '\0';
						//length of huffman code
						int bitstringLength = strlen(binaryCode_group);

						//building an encoded byte from right to left
						int cnt = 0;
						for (int i = 0; i < 8; i++)
						{
							//is the bit "on"?
							if (binaryCode_group[i] == '1')
								//turn the bit on using the OR bitwise operator
								byte1 = byte1 | (int)pow(2.0, cnt);
							cnt++;
						}

						fout.write((char*)&byte1, sizeof byte1);
						listOfCodes.erase(0, 8);
			}
		}
		binaryCodeRead = "";
	}
	//We finish reading...
	listOfCodes = listOfCodes + huffmanCodes[256];

	while(listOfCodes.size() != 0) {

		if (listOfCodes.size() < 8) {
			
				//Get the first 8 to write to file
				byteLenghtCode = listOfCodes.substr(0, listOfCodes.size());
				//reverse(byteLenghtCode.begin(),byteLenghtCode.end());

				//Time to write to file
				//get one byte
				char binaryCode_group[9] = "00000000";
				bool notLast = true;

				for (int i = 0; i < byteLenghtCode.size(); i++) {

					binaryCode_group[i] = byteLenghtCode[i];
				}

				//byte to be encoded
				unsigned char byte1 = '\0';
				//length of huffman code
				int bitstringLength = strlen(binaryCode_group);

				//building an encoded byte from right to left
				int cnt = 0;
				for (int i = 0; i < 9; i++)
				{
					//is the bit "on"?
					if (binaryCode_group[i] == '1')
						//turn the bit on using the OR bitwise operator
						byte1 = byte1 | (int)pow(2.0, cnt);
					cnt++;
				}

				fout.write((char*)&byte1, sizeof byte1);
				listOfCodes.erase(0, listOfCodes.size());
		}
		else {
			//Get the first 8 to write to file
			byteLenghtCode = listOfCodes.substr(0, 8);
			//reverse(byteLenghtCode.begin(),byteLenghtCode.end());

			//Time to write to file
			//get one byte
			char binaryCode_group[9] = "00000000";
			bool notLast = true;

			for (int i = 0; i < 8; i++) {

				binaryCode_group[i] = byteLenghtCode[i];
			}

			//byte to be encoded
			unsigned char byte1 = '\0';
			//length of huffman code
			int bitstringLength = strlen(binaryCode_group);

			//building an encoded byte from right to left
			int cnt = 0;
			for (int i = 0; i < 8; i++)
			{
				//is the bit "on"?
				if (binaryCode_group[i] == '1')
					//turn the bit on using the OR bitwise operator
					byte1 = byte1 | (int)pow(2.0, cnt);
				cnt++;
			}

			fout.write((char*)&byte1, sizeof byte1);
			listOfCodes.erase(0, 8);
		}
	}
	
	fin.close();
	fout.close();
	system("PAUSE");
	return 0;
}

