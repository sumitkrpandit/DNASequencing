/*
 * Team: #HPCSquadGoals
 * Team members: Anthony Enem, Ali Khalid
 * Project: DNA Squencing with Shortest Common Superstring
 * Code: Sequential Version
 *
 * Compilation Instructions: make
 * Execution Instructions: make test
 */

#include "../common/StringProcessing.h"
#include<iostream>
#include<string>
#include<algorithm>
#include<vector>
#include<set>
#include<utility>
#include<cstdio>
#include<time.h>

using namespace std;


/*************************************************************************
 * Recursive Function to combine a vector of strings into their shortest
 * common superstring
 * Parameters: A vector of strings
 */
void combineStrings(vector<string>&, set<string, SortByLength>&);


//Main function
int main(int argc, char** argv)
{
	set<string, SortByLength> Result;

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
	combineStrings(data, Result);

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

void combineStrings(vector<string>& data, set<string, SortByLength>& Result)
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

			//check for overlap between start of data[j] and end of data[i]
			overlap = overlapStrings(data[i], data[j]);
			if(overlap > maxOverlap){
				possiblePairs.clear(); //erase old possible pairs
				maxOverlap = overlap;
				possiblePairs.push_back(make_pair(i, j)); //add new possible pair
			}
			else if(overlap == maxOverlap){
				possiblePairs.push_back(make_pair(i, j)); //add new possible pair
			}

			//check for overlap between start of data[i] and end of data[j]
			overlap = overlapStrings(data[j], data[i]);
			if(overlap > maxOverlap){
				possiblePairs.clear(); //erase old possible pairs
				maxOverlap = overlap;
				possiblePairs.push_back(make_pair(j, i));
			}
			else if(overlap == maxOverlap){
				possiblePairs.push_back(make_pair(j, i)); //add new possible pair
			}
		}
	}

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
		combineStrings(possibleData, Result);
	}

}
