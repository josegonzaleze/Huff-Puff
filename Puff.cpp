// Authors: Jose Gonzales and Nathan Burner

#include<fstream>
#include<iostream>
#include<iomanip>
#include<string>
#include<algorithm>

using namespace std;

struct Huffman
{
	int glyph;
	int left;
	int right;
};

const int NUM_GLYPHS = 257;
//CREATION OF THE HUFMANN ARRAY//
struct Huffman huffmanArray[513];
string huffmanCodes[NUM_GLYPHS];
int codesCount = 0;

void translateBitString(string &bitString, int numEntries)
{
	int count = 0;
	string temp = "";
	bool isFound = false;
	int stringLength = bitString.size();
	
	while (!isFound)
	{
		temp = temp + bitString[count];

		for (int i = 0; i < NUM_GLYPHS; i++)
		{
			if (huffmanCodes[i] == temp)
			{
				if (i != 256)
				{
					// Write to file
					cout << i << endl;
					isFound = true;
					bitString.erase(0, count + 1);
					break;
				}
			}
		}

		//
		if (count == stringLength - 1)
		{
			break;
		}

		if (!isFound)
		{
			count++;
		}
	}
}

string addPadding(int stringLength)
{
	string padding = "";
	for (int i = 0; i < 8 - stringLength; i++)
	{
		padding += "0";
	}
	return padding;
}

string binary(int num)
{
	string temp = "";
	int rem;

	if (num <= 1)
	{
		temp += to_string(num);
		return temp;
	}
	rem = num % 2;
	temp = binary(num / 2);
	temp = temp + to_string(rem);

	return temp;
}

void decodeHuffFile(ifstream &file, int numEntries)  
{
	int stringLength = 0;
	string bitString = "";
	string tempString = "";
	int byteToRead = 0;

	while (!file.eof())
	{
		file.read((char*)&byteToRead, 1);
		tempString = binary(byteToRead);
		tempString = addPadding(tempString.size()) + tempString;
		bitString += tempString;
		translateBitString(bitString, numEntries);
	}
}

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

void populateHuffmanArray(ifstream &file, int numEntries)
{
	for (int i = 0; i < numEntries; i++)
	{
		file.read((char*)&huffmanArray[i].glyph, sizeof(huffmanArray[i].glyph));
		file.read((char*)&huffmanArray[i].left, sizeof(huffmanArray[i].left));
		file.read((char*)&huffmanArray[i].right, sizeof(huffmanArray[i].right));
	}
}

int readFileHeader(ifstream &file)
{
	int fileNameLength = 0;
	int numEntries = 0;

	file.read((char*)&fileNameLength, 4);

	char *fileName = new char[fileNameLength + 1];
	file.read((char*)fileName, fileNameLength);
	fileName[fileNameLength] = '\0';

	file.read((char*)&numEntries, 4);

	return numEntries;
}

void main()
{
	string inFileName = "";
	cout << "Enter the name of the file to decompress: ";
	cin >> inFileName;

	ifstream hufFile(inFileName, ios::in | ios::binary);
	int numEntries = readFileHeader(hufFile);
	populateHuffmanArray(hufFile, numEntries);
	printHuffman(0, numEntries, "");
	decodeHuffFile(hufFile, numEntries);
}