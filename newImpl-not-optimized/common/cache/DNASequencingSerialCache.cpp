/*
 * Team: #HPCSquadGoals
 * Team members: Anthony Enem, Ali Khalid, Kenadi Campbell
 * Project: DNA Squencing with Shortest Common Superstring
 * Code: Sequential Version
 *
 * Two input files are provided: HPCSquadGoalsInput1.txt and HPCSquadGoalsInput2.txt
 * Compilation Instructions: g++ -std=c++11 [file name] -o [exe name]
 * Execution Instructions: ./[exe name] < [Input file name]
 * Output is written to HPCSquadGoalsOutput.txt
 */

#include<iostream>
#include<string>
#include<algorithm>
#include<vector>
#include<set>
#include<utility>
#include<cstdio>
#include<time.h>
#include "Cache.h"

using namespace std;

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

/**********************************************
 * Function to find the length of the maximum overlap between the end of
 * str1 and the start of str2
 * Parameters: 2 strings
 */
int overlapStrings(string& str1, string& str2) throw();

/*************************************************************************
 * Recursive Function to combine a vector of strings into their shortest
 * common superstring
 * Parameters: A vector of strings
 */
void combineStrings(vector<string>&);

/************************************************************************
 * Function to remove strings that are subtrings of other strings in the
 * same vector
 * Parameters: A vector of strings
 */
void removeSubstrings(vector<string>& data) throw();


//GLOBAL cache to store overlap of strings
Cache cache;


//Main function
int main(int argc, char** argv)
{
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


    //Sort input strings based on their lengths
    sort(data.begin(), data.end(), SortByLength());

    //Merge strings that are substrings of other strings
    removeSubstrings(data);

    struct timespec now, tmstart;
    clock_gettime(CLOCK_REALTIME, &tmstart);

    //Find all possible combinations for shortest common superstring
    combineStrings(data);

    clock_gettime(CLOCK_REALTIME, &now);
    double seconds = (double)((now.tv_sec+now.tv_nsec*1e-9) - (double)(tmstart.tv_sec+tmstart.tv_nsec*1e-9));

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

    //Print Results
    cout << "There are " << Result.size() << " possible shortest common superstrings with length " << Result.begin()->length() << "." << endl << endl;

    for(const string& str : Result){
        cout << str << endl;
    }

    cout << endl << "C Time: " << seconds << " seconds." << endl;

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

int overlapStrings(const string& str1, const string& str2) throw()
{
    int i;
    int minLength = min(str1.length(), str2.length());

    //str3 holds the last part of str1, str4 holds the first part of str2
    string str3 = str1.substr(str1.length()-minLength), str4 = str2.substr(0, minLength);

    for(i = 0; i < minLength; i++){
        //get the first end of str4
        str4.resize(minLength-i);

        //get the first end of str3
        reverse(str3.begin(), str3.end());
        str3.resize(minLength-i);
        reverse(str3.begin(), str3.end());

        //compare first end of str1 to last end of str2
        if(str3 == str4){
            //if strings are equal, then the largest overlap between str1 and str2 is found
            return str3.length();
        }
    }

    return 0;
}

int getOverlap(const string& first, const string& second) {
    int overlap = cache.get(first, second);
    if(overlap == -1){
        //check for overlap between start of first and end of second
        overlap = overlapStrings(first, second);
        cache.put(first, second, overlap);
    }
    return overlap;
}

void updatePossiblePairs(vector<pair<int, int>>& possiblePairs, int& overlap, int& maxOverlap, int& firstIdx, int& secondIdx) {
    if(overlap > maxOverlap){
        possiblePairs.clear(); //erase old possible pairs
        maxOverlap = overlap;
        possiblePairs.push_back(make_pair(firstIdx, secondIdx)); //add new possible pair
    }
    else if(overlap == maxOverlap){
        possiblePairs.push_back(make_pair(firstIdx, secondIdx)); //add new possible pair
    }
}

void combineStrings(vector<string>& data)
{
    //Base case: If input has one string, then it is a superstring
    if(data.size() == 1){
        Result.insert(data.front());
        return;
    }

    int maxOverlap, overlap;

    vector<pair<int, int>> possiblePairs;

    maxOverlap = -1;
    //Get the pairs of strings with the most overlap
    for(int i = 0; i < data.size(); i++){
        for(int j = i+1; j < data.size(); j++){

            //get overlap for end of data[i] and start of data[j]
            overlap = getOverlap(data[i], data[j]);
            updatePossiblePairs(possiblePairs, overlap, maxOverlap, i, j);

            //get overlap for end of data[j] and start of data[i]
            overlap = getOverlap(data[j], data[i]);
            updatePossiblePairs(possiblePairs, overlap, maxOverlap, j, i);
        }
    }

    //Combine data for each possible pair of maximum overlap
    string jointString;
    vector<string> possibleData;
    for(auto& p : possiblePairs){
        //join the two strings into a new string
        jointString = data[p.first] + data[p.second].substr(maxOverlap);
        //make duplicate of data
        possibleData = data;
        //erase possible string pairs from possible data
        possibleData.erase(possibleData.begin() + min(p.first, p.second));
        possibleData.erase(possibleData.begin() + max(p.first, p.second)-1);

        //remove merged strings from cache
        cache.insertNewOverlap(data[p.first], data[p.second], jointString);

        //add new joint string to possible data
        possibleData.push_back(jointString);

        //Recursively combine this possible set of data to find the shortest common superstrings
        combineStrings(possibleData);
    }
}
