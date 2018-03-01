/*************************************************
 * Thus file is used to read the input in
 * Caluclates the stats on the input
 * **********************************************/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> readFile()
{
  string str;
  vector<string> inputData;

  while(getline(cin, str)) {
    getline(cin, str);
    //cout << str << endl << endl;
    inputData.push_back(str);
    getline(cin, str);
  }

  return inputData;
}
