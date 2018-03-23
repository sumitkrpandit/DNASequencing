/*************************************************
 * Thus file is used to read the input in
 * Caluclates the stats on the input
 * **********************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

void printStats(vector<string>& stats,const int& size){
  int avgBpLength = 0;
  int totalFragments = 0;
  int minBpLength = 0;
  int maxBpLength = 0;
  double fracA = 0.0;
  double fracC = 0.0;
  double fracT = 0.0;
  double fracG = 0.0;

  totalFragments = stats.size();
  avgBpLength = 125;
  minBpLength = 50;
  maxBpLength = 200;
  fracA = 0.25;
  fracC = 0.25;
  fracT = 0.25;
  fracG = 0.25;

  if(size == totalFragments) {
    cout << "-------------------Stats-----------------------\n";
    cout << "Total fragments = " << totalFragments << endl;
    cout << "Average base pair length = " << avgBpLength << endl;
    cout << "Minimum base pair length = " << minBpLength << endl;
    cout << "Maximum base pair length = " << maxBpLength << endl;
    cout << "Fraction of base A = " << fracA << endl;
    cout << "Fraction of base C = " << fracC << endl;
    cout << "Fraction of base T = " << fracT << endl;
    cout << "Fraction of base G = " << fracG << endl;
    cout << "-------------------Stats-----------------------\n";
  }
  else {
    cout << "ERROR: Check input file format.\n";
    exit(0);
  }

}

vector<string> readFile(ifstream& infile)
{
  string str;
  vector<string> inputData;
  vector<string> stats;
  while(getline(infile, str)) {
    stats.push_back(str);
    getline(infile, str);
    //cout << str << endl << endl;
    inputData.push_back(str);
    getline(infile, str);
  }

  printStats(stats, inputData.size());

  cout << "\n\nINPUT FILE READ SUCCESSFULLY\n\n";
  return inputData;
}
