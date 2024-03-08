// lentext.cc
// Copyright 1997, Gregory A. Riccardi

#include "lentext.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string.h>

LengthTextBuffer :: LengthTextBuffer (int maxBytes)
 // construct with a maximum of maxBytes
{
	Init (maxBytes);
}

void LengthTextBuffer :: Clear ()
// clear fields from buffer
{
	NextByte = 0;
	BufferSize = 0;
}

int LengthTextBuffer :: Read (istream & stream)
{
	Clear ();
	stream . read((char *)&BufferSize, sizeof(BufferSize));
	
	if (stream.fail()) return FALSE;
	if (BufferSize > MaxBytes) return FALSE; // buffer overflow
	stream . read (Buffer, BufferSize);
	return stream . good ();
}


int LengthTextBuffer :: Write (ostream & stream) const
{
	stream . write ((char*)&BufferSize, sizeof(BufferSize));
	stream . write (Buffer, BufferSize);
	return stream . good ();
}


int LengthTextBuffer :: Pack (const char * str, short size)
// set the value of the next field of the buffer;
// if size = -1 (default) use strlen(str) as length of field
{
	short len; // length of string to be packed
	if (size >= 0) len = size;
	else len = strlen (str);
	if (len > strlen(str)) // str is too short!
		return FALSE;
	int start = NextByte; // first character to be packed
	NextByte += (len + sizeof(len));
	if (NextByte > MaxBytes) return FALSE;
	memcpy (&Buffer[start], &len, sizeof(len));
	strncpy (&Buffer[start+sizeof(len)], str, len);
	BufferSize = NextByte;
	return TRUE;
}

int LengthTextBuffer :: Unpack (char * str)
// extract the value of the next field of the buffer
{
	short len; // length of packed string
	if (NextByte >= BufferSize) return FALSE; // no more fields
	int start = NextByte; // first character to be unpacked
	memcpy (&len, &Buffer[start], sizeof(short));
	NextByte += len + sizeof(short);
	if (NextByte > BufferSize) return FALSE;
	strncpy (str, &Buffer[start+sizeof(short)], len);
	str [len] = 0; // zero termination for string 
	return TRUE;
}

void LengthTextBuffer :: Print (ostream & stream) const
{
	stream << "Buffer has characters "<<MaxBytes<<" and Buffer Size "
		<<BufferSize<<endl;
}

int LengthTextBuffer :: Init (int maxBytes)
 // construct with a maximum of maxBytes
{
	if (maxBytes < 0) maxBytes = 0;
	MaxBytes = maxBytes;
	Buffer = new char[MaxBytes];
	Clear ();
	return 1;
}

int LengthTextBuffer :: ReadCSV(istream& stream)
{
	string line;
	if(!(getline(stream, line))){ // Check the file.
		cout << "Fail to read the CSV file." << endl;
		return FALSE;
	}
	Clear(); // Clear the stream
	stringstream instream(line);
	string field;
	while(getline(instream, field, ',')){ // Loading the content into field, stop at ','
		Pack(field.c_str(), field.length()); // Packing the content into buffer
	}
	return TRUE;
}

// Same method of unpack
ostream &operator<<(ostream& out, const LengthTextBuffer& buffer) {
    int pos = 0; // Start position in the buffer
    short fieldLength; // length of packed string
    while (pos < buffer.BufferSize) { // check if the position less than size
        // Unpack the length of the next field
        memcpy(&fieldLength, buffer.Buffer + pos, sizeof(short)); // Copy the length of string into fieldLength, representing the length of next string
        pos += sizeof(short);// Move to string
        // Unpack and output the field
        out.write(buffer.Buffer + pos, fieldLength); // Output the string
        out << ","; // Separate fields with a space (It could be modified)
        pos += fieldLength; // Move to next length
	}
    return out;
	}