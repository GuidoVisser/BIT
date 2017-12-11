#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

int getMinimum(int a, int b, int c) {
    /*
     * Return the smallest values out of three integers.
     */
    int dummieArray[3] = {a, b, c};
    return *min_element(dummieArray, dummieArray + 3);
};

string getString() {
    /*
     * Ask user for a string. Check if all characters are alphabetical. If not, keep asking the user for an
     * alphabetical string until given.
     */
    string str;
    char c;

    // get string from user
    getline(cin, str);

    // check if all characters are alphabetical
    for (int i = 0; i < str.length(); i++) {
        c = str.at(i);
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z' ))) {
            cout << "string has illegal charachters. Please use only alphabetical characters." << endl;
            str = getString();
            break;
        };
    };

    // Ensure strings are lowercase
    transform(str.begin(), str.end(), str.begin(), ::tolower);

    return str;
};

int main() {

    const int asciiConst = 97;

    // ask user for two strings for comparison
    cout << "Please provide two strings for comparison:" << endl;
    string a = getString();
    string b = getString();

    // array that holds the last visited indices of each letter in stirng a. initialized as zeros
    int da[26] = { };

    // initialize Damerau-Levenshtein distance matrix, initialized as zeros
    vector <vector<int>> dldMatrix(a.size()+1, vector<int>(b.size()+1));

    // first row has incremental values
    for (int i = 1; i < a.size() + 1; i++) {
        dldMatrix[i][0] = i;
    };

    // first column has incremental values
    for (int i = 1; i < b.size() + 1; i++) {
        dldMatrix[0][i] = i;
    };

    // Calculate for each element of dldMatrix[i, j] the Damerau-Levenshtein distance between the first
    // i characters of string a and the first j characters of string b
    for(int i = 1; i <= a.size(); i++ ) {
        int db = 0;
        for(int j = 1; j <= b.size(); j++) {
            int i1 = da[(int)b[j-1] - asciiConst];
            int j1 = db;
            int substitutionCost = 0;

            // remember index of last character in b that is also in the first i charachters of a
            // and determine value of substitutionCost
            if(a[i-1] == b[j-1]) {
                db = j;
            } else {
                substitutionCost = 1;
            };

            // for this coordinate (i, j) calculate the substitution, deletion and insertion distance. Select the smallest
            int substitutionDistance = dldMatrix[i-1][j-1] + substitutionCost;
            int deletionDistance = dldMatrix[i-1][j] + 1;
            int insertionDistance = dldMatrix[i][j-1] + 1;
            dldMatrix[i][j] = getMinimum(substitutionDistance, deletionDistance, insertionDistance);

            // if a transposition is possible AND profitable change value to transpositionDistance
            if(i1 > 0 && j1 > 0) {
                int transpositionDistance = dldMatrix[i1-1][j1-1] + (i-i1-1) + (j-j1-1) + 1;
                if(dldMatrix[i][j] > transpositionDistance) {
                    dldMatrix[i][j] = transpositionDistance;
                };
            };
        };

        // remember last index of each character in a
        da[(int)a[i-1] - asciiConst] = i;
    };

    // provide user with the Damerau-Levenshtein distance of the two given strings
    cout << "the Damerau-Levenshtein distance between '" << a << "' and '" << b << "' is " << dldMatrix[a.size()][b.size()] << endl;

    return 0;

};
