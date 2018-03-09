/*
 * Team members: Anthony Enem, Ali Khalid
 * Project: DNA Squencing with Shortest Common Superstring
 * Code: Parallel Version
 *
 * Compilation Instructions: use the make file to comile
 *
 */

#include <StringProcessing.h>
#include <cache/Cache.h>
#include <ReadFile.h>
#include <iostream>
#include <fstream>
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

mpi::environment env;
mpi::communicator world;

ostream& operator<<(ostream& os, const OverlapPair& p) {
  os << p.first << " - " << p.second.first << " - " << p.second.second << endl;
  return os;
}

//Main function
int main(int argc, char* argv[])
{
  if (argc < 2) {
    cerr << "Wrong usage. Must specify input file!\n";
    return -1;
  }

  vector<string> data;
  struct timespec now, tmstart;

  if(world.rank() == 0) {
    //Routine for process 0
    string str;

    ifstream infile(argv[1]);
    if (infile) {
      //Read each input string and add to vector
      data = readFile(infile);
    }
    else {
      cerr << "Failed to create ifstream from input file!\n";
    }

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

  //Function used to compare overlap pairs by the overlap length
  //Returns the pair with larger overlap length
  auto compareOverlapPairs = [](const OverlapPair& p1, const OverlapPair& p2) {
        if(p1.second.second > p2.second.second)
          return p1;
        return p2;
      };

  do {
    p = all_reduce(world, cache.get(), compareOverlapPairs);

    //Merge first and second fragments into new fragment
    newString = mergeStrings(make_pair(data[p.first], make_pair(data[p.second.first], p.second.second)));
    //Replace second fragment with new fragment
    replace(data.begin(), data.end(), data[p.second.first], newString);
    //Replace first fragment with empty string to conserve memory
    replace(data.begin(), data.end(), data[p.first], string(""));

    cache.insertNewOverlap(p.first, p.second.first);

    //Get sum of cache sizes
    sum = all_reduce(world, cache.size(), std::plus<int>());
    //Base case: Reduce on cache size
  } while (sum > 1);

  return newString;
}
