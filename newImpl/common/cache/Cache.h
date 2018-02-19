#include <string>
#include <map>
#include <vector>
#include <utility>
#include<iostream>
#include<algorithm>

using namespace std;

class Cache {
  private:
    map<string, vector<pair<string, int>>> data;

  public:
    Cache(){ }
    ~Cache(){ }

    void Sort() {
      for(auto & entry : data)
      {
        std::sort(entry.second.begin(), entry.second.end(),
            [](const pair<string, int>& p1, const pair<string, int>& p2)->bool{ return p1.second > p2.second;
            });
      }
    }

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

    void insertNewOverlap(const string& a, const string& b, const string& ab) {
      /*
       *Remove entry key "a" from map
       *Replace entry key "b" with "ab"
       *Remove entry key "b"
       *Replace "a" with "ab" througout the value vectors
       *Remove entry where "b" points to "a"
       */

      data.erase(a);
      data[ab] = std::move(data[b]);
      data.erase(b);
      for(auto it = data[ab].begin(); it != data[ab].end(); ++it){
        if(it->first == a){
          data[ab].erase(it);
        }
      }
      //std::remove_if(data[ab].begin(), data[ab].end(), [&](const pair<string, int> &entry)->bool{ return (entry.first == a); });

      for (auto & entry : data) {
        if (entry.first != ab) {
          for (auto & v : entry.second) {
            if (v.first == a)
              v.first = ab;
          }
        }
      }
    }

    friend ostream& operator<<(ostream& os, Cache& cache) {
      for(auto& entry : cache.data) {
        os << entry.first << endl;
        for(auto& v : entry.second) {
          os << "\t" << v.first << " -- " << v.second << endl;
        }
      }
      return os;
    }

};
