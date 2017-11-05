#include<fstream>
#include<iostream>
#include<iomanip>
#include<string>
#include<vector>
#include<queue>
//#include<functional>


using namespace std;

struct Huffman
{
	int glyph;
	double count;
	int left;
	int right;

};

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

	//Array that contains all the characters count.
	double glyphCount[257];
	//Creating array of glyphs
	int glyph[257];


	//Initializing to 0 on glyphcount since list is empty
	for (int i = 0; i < 257; i++) {
		glyphCount[i] = 0;
	}

	int number;
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
	//CREATION OF THE HUFMANN ARRAY//
	struct Huffman huffmanArray[513];
	int huffmanGlyphCount = 0;
	for (int i = 0; i < glyphNumber; i++) {
		huffmanArray[i].glyph = glyph[i];
		huffmanArray[i].count = glyphCount[(int)glyph[i]];
		huffmanArray[i].left = -1;
		huffmanArray[i].right = -1;
		huffmanGlyphCount++;
	}
	//Addding the eof
	huffmanArray[huffmanGlyphCount].glyph = 257;
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
	for (int i = 0; i < huffmanGlyphCount * 2 - 1; i++) {
		cout << huffmanArray[i].glyph << " " << huffmanArray[i].count << " " << huffmanArray[i].left << " " << huffmanArray[i].right << endl;
	}
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
	for (int i = 0; i < huffmanGlyphCount*2 -1; i++) {
		cout << huffmanArray[i].glyph << " " << huffmanArray[i].count << " " << huffmanArray[i].left << " " << huffmanArray[i].right << endl;
	}
	//***AT THIS POINT WE HAVE THE ARRAY READY TO BE WRITTEN TO FILE**//

	//Write:
	//-- Size of Name of File
	//-- Name of file
	//-- Huffman table
	//-- Huffman encoding
	fin.close();
	fout.close();
	system("PAUSE");
	return 0;
}