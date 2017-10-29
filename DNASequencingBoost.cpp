/*
 * Team: #HPCSquadGoals
 * Team members: Anthony Enem, Ali Khalid
 * Project: DNA Squencing with Shortest Common Superstring
 * Code: Parallel Version
 *
 * Input file: HPCSquadGoalsInput1.txt
 * Compilation Instructions: use the make file to comile
 * Execution Instructions: mpirun -np [number of processes] [exe name] < in.txt
 *
 * **The number of processors used must be at least N*(N-1)/2, where N is the number
 * **of strings in the input file. For optimal results, the number of processors
 * **should be as close to N*(N-1)/2 as possible.
 *
 * Output is written to HPCSquadGoalsOutput.txt
 */

#include "OverlapString.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <utility>
#include <cstdio>
#include <time.h>
#include <mpi.h>
#include <utility>
#include <boost/mpi.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/utility.hpp>

using namespace std;
namespace mpi = boost::mpi;

//Functor used to sort strings by their lengths
struct SortByLength
{
  bool inline operator()(const string& a, const string& b) throw()
  {
    //If the lengths are equal, compare normally
    if(a.length() == b.length()){
      return a < b;
    }
    //else compare lengths
    return a.length() < b.length();
  }
};

//Set of strings to store possible results of shortest common superstrings
//Results are sorted by length of strings
set<string, SortByLength> Result;

//Function prototypes

/*************************************************************************
 * Recursive Function to combine a vector of strings into their shortest
 * common superstring
 * Parameters: A vector of strings
 */
void combineStrings(vector<string>&) throw();

/************************************************************************
 * Function to remove strings that are subtrings of other strings in the
 * same vector
 * Parameters: A vector of strings
 */
void removeSubstrings(vector<string>& data) throw();

int signal = 1;
int proc;


mpi::environment env;
mpi::communicator world;

//Main function
int main()
{

  if(world.rank() == 0)
  {
    //Routine for process 0

    struct timespec now, tmstart;

    int size;
    string str;
    vector<string> data;

    //read size of input
    cin >> size;

    //Read each input string and add to vector
    for(int i = 0; i < size; i++){
      cin >> str;
      data.push_back(str);
    }

    sort(data.begin(), data.end(), SortByLength()); //sort by length
    removeSubstrings(data); //remove substrings

    clock_gettime(CLOCK_REALTIME, &tmstart); //start timer

    //Find all possible combinations for shortest common superstring
    combineStrings(data);

    clock_gettime(CLOCK_REALTIME, &now); //end timer
    double seconds = (double)((now.tv_sec+now.tv_nsec*1e-9) - (double)(tmstart.tv_sec+tmstart.tv_nsec*1e-9));

    //Send signal for other processes to quit
    signal = -1;
    for(int i = 1; i < world.size(); i++){
      world.send(i, 0, signal);
    }

    //Remove strings that are longer that the first string
    while(Result.size() > 1)
    {
      //if the length of the last string is longer than the length of the first string
      if(prev(Result.end())->length() > Result.begin()->length()){
        //Erase the last string
        Result.erase(prev(Result.end()));
      }
      else{
        break;
      }
    }

    //Redirect output to file
    freopen("output.txt", "w", stdout);

    //Print Results
    cout << "There are " << Result.size() << " possible shortest common superstrings with length " << Result.begin()->length() << "." << endl << endl;

    for(const string& str : Result){
      cout << str << endl;
    }

    cout << endl << "C Time: " << seconds << " seconds." << endl;
  }
  else{
    //Other processes will get a signal from process 0
    //Signal 1 means to compare 2 strings, Signal -1 means to quit
    //
    pair<string, string> strPair;
    pair<int, int> overlap_data,
      overlap;
    signal = 0;

    //Do while loop so other processes can keep receiving and comparing strings from process 0 until process 0 is done with combining
    do{
      //Receive signal from process 0
      world.recv(0, 0, signal);

      if(signal == 1)
      {
        world.recv(0, 0, strPair); //revieve string pair
        overlap = overlapStrings(strPair); //get over lap in for ab and ba

        if(overlap.first > overlap.second){
          //string a should come before string b
          overlap_data = make_pair(overlap.first, 0);
        }
        else if(overlap.first < overlap.second){
          //string b should come before string a
          overlap_data = make_pair(overlap.second, 1);
        }
        else{
          //both strings have the same overlap, so use both combinations
          overlap_data = make_pair(overlap.first, 2);
        }

        world.send(0, 0, overlap_data); //send overlap data back

      }

    }while(signal != -1); //repeat until process 0 sends the quit signal (-1)

  }

  MPI_Finalize();

  return 0;
}

void removeSubstrings(vector<string>& data) throw()
{
  for(int i = 0; i < data.size(); i++){
    for(int j = i+1; j < data.size(); j++){
      //If string i is a substring of string j, remove string i from the set of strings
      if(data[j].find(data[i]) != string::npos){
        data.erase(data.begin()+i);
        i--;
        break;
      }
    }
  }
}

void combineStrings(vector<string>& data) throw()
{
  //Base case: If input has one string, then it is a superstring
  if(data.size() == 1){
    Result.insert(data.front());
    return;
  }

  int maxOverlap, overlap;

  vector<pair<int, int>> possiblePairs, stringPairs;

  proc = 1;
  //Get the pairs of strings with the most overlap
  for(int i = 0; i < data.size(); i++){
    for(int j = i+1; j < data.size(); j++){

      //send signal to proc for checking overlap
      world.send(proc, 0, signal);
      //Send pair of strings to be compared
      world.send(proc, 0, make_pair(data[i], data[j]));

      stringPairs.push_back(make_pair(i, j));

      ++proc;
    }
  }

  //Get maximum possible pairs
  maxOverlap = -1;
  pair<int, int> overlap_data;

  for(int i = 1; i < proc; ++i) {
    //Receive overlap data from process i
    world.recv(i, 0, overlap_data);

    //If current overlap is the maximum
    if(overlap_data.first > maxOverlap) {
      //remove previous possible pairs
      possiblePairs.clear();

      if(overlap_data.second == 0) {
        //add new pair in same order
        possiblePairs.push_back(stringPairs[i-1]);
      }
      else if(overlap_data.second == 1) {
        //add new pair in reverse order
        possiblePairs.push_back(make_pair(stringPairs[i-1].second, stringPairs[i-1].first));
      }
      else{
        //add both possible pairs
        possiblePairs.push_back(stringPairs[i-1]);
        possiblePairs.push_back(make_pair(stringPairs[i-1].second, stringPairs[i-1].first));
      }
      maxOverlap = overlap_data.first;

    }
    else if(overlap_data.first == maxOverlap) {
      //current overlap is the same as the maximum, so add possible pair(s) without removing the previous pairs

      if(overlap_data.second == 0) {
        possiblePairs.push_back(stringPairs[i-1]);
      }
      else if(overlap_data.second == 1) {
        possiblePairs.push_back(make_pair(stringPairs[i-1].second, stringPairs[i-1].first));
      }
      else{
        possiblePairs.push_back(stringPairs[i-1]);
        possiblePairs.push_back(make_pair(stringPairs[i-1].second, stringPairs[i-1].first));
      }
    }

  }

  stringPairs.clear();
  //Combine data for each possible pair of maximum overlap
  string jointString;
  vector<string> possibleData;
  for(auto p : possiblePairs){
    //join the two strings into a new string
    jointString = data[p.first] + data[p.second].substr(maxOverlap);
    //make duplicate of data
    possibleData = data;
    //erase possible string pairs from possible data
    possibleData.erase(possibleData.begin() + min(p.first, p.second));
    possibleData.erase(possibleData.begin() + max(p.first, p.second)-1);
    //add new joint string to possible data
    possibleData.push_back(jointString);

    //Recursively combine this possible set of data to find the shortest common superstrings
    combineStrings(possibleData);
  }

}
