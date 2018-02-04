#include <string>
#include <map>
#include <vector>
#include <utility>

using namespace std;

class Cache {
  private:
    map<string, vector<pair<string, int>>> data;

  public:
    Cache(){ }
    ~Cache(){ }

    //Inserts the strings with strings of all possible overlap
    void put(const string& a, const string& b, const int& value) {
      data[a].push_back(make_pair(b, value));
    }

    //Returns the cached value for a to b.
    //If the overlap value is not in the cache, it returns -1
    pair<string, int> get(const string& a) {
      return data[a].front();
    }

    //Erases the overlap data for string a
    void remove(const string& a, const string& b){
    }

    void insertNewOverlap(string& a, string& b, string& ab) {
      auto it = data.find(b);
      it.first = ab;

      for (auto & entry : data) {
        if (entry.first != ab) {
          for (auto & v : entry.second) {
            if (v.first == a)
              v.first = ab;
          }
        }
      }

};
