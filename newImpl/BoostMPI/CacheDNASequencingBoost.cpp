/*
 * Team: #HPCSquadGoals
 * Team members: Anthony Enem, Ali Khalid
 * Project: DNA Squencing with Shortest Common Superstring
 * Code: Parallel Version
 *
 * Compilation Instructions: use the make file to comile
 *
 */

#include "../common/StringProcessing.h"
#include "../common/cache/Cache.h"
#include "../common/ReadFile.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <utility>
#include <cstdio>
#include <time.h>
#include <utility>
#include <boost/mpi.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>


using namespace std;
namespace mpi = boost::mpi;

#define OverlapPair pair<int, pair<int, int>>

/*************************************************************************
 * Recursive Function to combine a vector of strings into their shortest
 * common superstring
 * Parameters: A vector of strings
 */
string combineStrings(Cache&, vector<string>&);

#define QUIT 0
#define PROCESS_DATA 1

mpi::environment env;
mpi::communicator world;

ostream& operator<<(ostream& os, const OverlapPair& p) {
  os << p.first << " - " << p.second.first << " - " << p.second.second << endl;
  return os;
}

//Main function
int main()
{
  vector<string> data;
  struct timespec now, tmstart;

  if(world.rank() == 0) {
    //Routine for process 0
    string str;

    //Read each input string and add to vector
    data = readFile();

    sort(data.begin(), data.end(), SortByLength()); //sort by length
    removeSubstrings(data); //remove substrings

    clock_gettime(CLOCK_REALTIME, &tmstart); //start timer
  }

  Cache cache;
  broadcast(world, data, 0);

  //Calculate Cache values (P + TP(n))
  for(int i = 0, index; (index = world.rank() + world.size()*i) < data.size(); i++) {
    for(int j = 0; j < data.size(); ++j){
      if(j != index){
        cache.put(index, j, overlapStrings(data[index], data[j]));
      }
    }
  }

  //Sort Cache data
  cache.Sort();

  //Find all possible combinations for shortest common superstring
  string result = combineStrings(cache, data);

  if(world.rank() == 0){
    cout << "Result Length: " << result.length() << endl << endl;
    cout << result << endl << endl;
    clock_gettime(CLOCK_REALTIME, &now); //end timer
    double seconds = (double)((now.tv_sec+now.tv_nsec*1e-9) - (double)(tmstart.tv_sec+tmstart.tv_nsec*1e-9));

    cout << endl << "C Time: " << seconds << " seconds." << endl;
  }

  return 0;
}


string combineStrings(Cache& cache, vector<string>& data) {
  int sum;

  OverlapPair p;
  string newString;

  do {
    p = all_reduce(world, cache.get(),
        [](const OverlapPair& p1, const OverlapPair& p2){
          if(p1.second.second >= p2.second.second)
            return p1;
          return p2;
        });

    newString = mergeStrings(make_pair(data[p.first], make_pair(data[p.second.first], p.second.second)));
    replace(data.begin(), data.end(), data[p.second.first], newString);

    cache.insertNewOverlap(p.first, p.second.first, p.second.first);
    sum = all_reduce(world, cache.size(), std::plus<int>());

    //Base case: Reduce on cache size
  } while (sum != 1);

  return newString;
}
