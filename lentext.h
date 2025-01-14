// lentext.h
// Copyright 1997, Gregory A. Riccardi

#ifndef LENTEXT_H
#define LENTEXT_H

#include <iostream>

using namespace std;

#ifndef FALSE
#define FALSE (0)
#define TRUE (1)
#endif

class LengthTextBuffer 
// a buffer which holds length-based text fields.
{  public:
	LengthTextBuffer (int maxBytes = 1000);
		// construct with a maximum of maxBytes
	void Clear (); // clear fields from buffer
	int Read (istream &);
	int Write (ostream &) const;
	int Pack (const char *, short size = -1);
		// set the value of the next field of the buffer;
	int Unpack (char *);
		// extract the value of the next field of the buffer
	void Print (ostream &) const;
 	int Init (int maxBytes = 1000);

		//---------------------------------------------------------------------------------------------------------
		// Methods of reading CSV file.
	int ReadCSV(istream & stream);
    friend ostream& operator<<(std::ostream& out, const LengthTextBuffer& record); ///< Overloads the output stream operator.
    friend istream& operator>>(std::istream& in, LengthTextBuffer& record); ///< Overloads the input stream operator. (not been used here)
		//---------------------------------------------------------------------------------------------------------

   private:
	char * Buffer; // character array to hold field values
	int BufferSize; // size of packed fields
	int MaxBytes; // maximum number of characters in the buffer
	int NextByte; // packing/unpacking position in buffer
};

#endif
