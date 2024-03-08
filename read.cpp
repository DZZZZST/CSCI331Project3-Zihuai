#include "lentext.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(){

    int count = 0; // Start at 0
    int const maxcount = 100; // Stop at 100

    ifstream csvFile("us_postal_codes.csv");
    ofstream output("test_outcome.txt");
    if (!csvFile){
        cout << "Can't open the file" << endl;
        return 1;
    }

    LengthTextBuffer buffer(1000); // Setting a suitable buffer, it depents on data.
    while (buffer.ReadCSV(csvFile) && count < maxcount){ // Condition
        cout << buffer << endl;
        output << buffer << endl;
        count++;
    }

    return 0;
}