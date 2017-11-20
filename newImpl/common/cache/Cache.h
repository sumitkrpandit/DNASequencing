#include <string>
#include <map>
#include <vector>
#include <utility>

using namespace std;

class Cache {
  private:
    map<string, map<string, int>> data;

  public:
    Cache(){ }
    ~Cache(){ }

    //Inserts the strings with strings of all possible overlap
    void put(const string& a, const string& b, const int& value) {
      data[a][b] = value;
    }

    //Returns the cached value for a to b.
    //If the overlap value is not in the cache, it returns -1
    int get(const string& a, const string& b) {
      return data[a][b];
    }

    //Erases the overlap data for string a
    void remove(const string& a, const string& b){
      data.erase(a);
      data[b].erase(a);
    }

    void insertNewOverlap(string& a, string& b, string& ab) {
      remove(a, b);
      map<string, int> abMap(data[b]);
      data[ab] = abMap;
    }

};
