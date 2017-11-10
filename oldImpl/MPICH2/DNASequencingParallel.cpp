/*
 * Team: #HPCSquadGoals
 * Team members: Anthony Enem, Ali Khalid, Kenadi Campbell
 * Project: DNA Squencing with Shortest Common Superstring
 * Code: Parallel Version
 *
 * Input file: HPCSquadGoalsInput1.txt
 * Compilation Instructions: mpic++ -std=c++11 [file name] -o [exe name]
 * Execution Instructions: mpirun -np [number of processes] [exe name]
 *
 * **The number of processors used must be at least N*(N-1)/2, where N is the number
 * **of strings in the input file. For optimal results, the number of processors
 * **should be as close to N*(N-1)/2 as possible.
 *
 * Output is written to HPCSquadGoalsOutput.txt
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
#include<mpi.h>

using namespace std;


/*************************************************************************
 * Recursive Function to combine a vector of strings into their shortest
 * common superstring
 * Parameters: A vector of strings
 */
void combineStrings(vector<string>&, set<string, SortByLength>&);

int proc_rank, comm_sz, signal = 1;
int str_data[2];

//Main function
int main(int argc, char** argv)
{
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

	if(proc_rank == 0)
	{
		//Routine for process 0

		double mpist, mpiend;
		struct timespec now, tmstart;

		int size;
		string str;
		vector<string> data;
		set<string, SortByLength> Result;

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

		//Start timers
		mpist = MPI_Wtime();
		clock_gettime(CLOCK_REALTIME, &tmstart);

		//Find all possible combinations for shortest common superstring
		combineStrings(data, Result);

		//End timers
		mpiend = MPI_Wtime();
		clock_gettime(CLOCK_REALTIME, &now);
		double seconds = (double)((now.tv_sec+now.tv_nsec*1e-9) - (double)(tmstart.tv_sec+tmstart.tv_nsec*1e-9));

		//Send signal for other processes to quit
		signal = -1;
		for(int i = 1; i < comm_sz; i++){
			MPI_Send(&signal, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
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

		//Print Results
		cout << "There are " << Result.size() << " possible shortest common superstrings with length " << Result.begin()->length() << "." << endl << endl;

		for(const string& str : Result){
			cout << str << endl;
		}

		cout << endl << "C Time: " << seconds << " seconds." << endl;
		cout << "MPI time: " << mpiend-mpist << endl;
	}
	else{
		//Other processes will get a signal from process 0
		//Signal 1 means to compare 2 strings, Signal -1 means to quit
		//
		string a, b;
		int overlap1, overlap2;
		signal = 0;
		MPI_Request request;

		//Do while loop so other processes can keep receiving and comparing strings from process 0 until process 0 is done with combining
		do{
			//Receive signal from process 0
			MPI_Irecv(&signal, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
			MPI_Wait(&request, MPI_STATUS_IGNORE);

			if(signal == 1)
			{
				//Receive lengths of both strings
				MPI_Recv(str_data, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				a.resize(str_data[0]);
				b.resize(str_data[1]);

				//receive both strings
				MPI_Recv(&a[0], str_data[0], MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				MPI_Recv(&b[0], str_data[1], MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

				//Get overlap for both strings
				overlap1 = overlapStrings(a, b);
				overlap2 = overlapStrings(b, a);

				if(overlap1 > overlap2){
					//string a should come before string b
					str_data[0] = overlap1;
					str_data[1] = 0;
				}
				else if(overlap1 < overlap2){
					//string b should come before string a
					str_data[0]= overlap2;
					str_data[1]=1;
				}
				else{
					//both strings have the same overlap, so use both combinations
					str_data[0]=overlap1;
					str_data[1]=2;
				}

				//Send overlap data back to process 0
				MPI_Send(str_data, 2, MPI_INT, 0, 0, MPI_COMM_WORLD);

			}

		}while(signal != -1); //repeat until process 0 sends the quit signal (-1)

	}

	MPI_Finalize();

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

	vector<pair<int, int>> possiblePairs, stringPairs;

	int proc = 1;
	//Get the pairs of strings with the most overlap
	for(int i = 0; i < data.size(); i++){
		for(int j = i+1; j < data.size(); j++){

			str_data[0] = data[i].length();
			str_data[1] = data[j].length();

			//send signal to proc for checking overlap
			MPI_Send(&signal, 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
			//Send lengths of strings to be compared
			MPI_Send(str_data, 2, MPI_INT, proc, 0, MPI_COMM_WORLD);
			//Send characters to be compared
			MPI_Send(&data[i][0], str_data[0], MPI_CHAR, proc, 0, MPI_COMM_WORLD);
			MPI_Send(&data[j][0], str_data[1], MPI_CHAR, proc, 0, MPI_COMM_WORLD);

			stringPairs.push_back(make_pair(i, j));

			++proc;
		}
	}

	//Get maximum possible pairs
	maxOverlap = -1;

	for(int i = 1; i < proc; ++i){
		//Receive overlap data from process i
		MPI_Recv(str_data, 2, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		//If current overlap is the maximum
		if(str_data[0] > maxOverlap){
			//remove previous possible pairs
			possiblePairs.clear();

			if(str_data[1] == 0){
				//add new pair in same order
				possiblePairs.push_back(stringPairs[i-1]);
			}
			else if(str_data[1]==1){
				//add new pair in reverse order
				possiblePairs.push_back(make_pair(stringPairs[i-1].second, stringPairs[i-1].first));
			}
			else{
				//add both possible pairs
				possiblePairs.push_back(stringPairs[i-1]);
				possiblePairs.push_back(make_pair(stringPairs[i-1].second, stringPairs[i-1].first));
			}
			maxOverlap = str_data[0];

		}
		else if(str_data[0] == maxOverlap){
			//current overlap is the same as the maximum, so add possible pair(s) without removing the previous pairs

			if(str_data[1] == 0){
				possiblePairs.push_back(stringPairs[i-1]);
			}
			else if(str_data[1]==1){
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
		combineStrings(possibleData, Result);
	}

}
