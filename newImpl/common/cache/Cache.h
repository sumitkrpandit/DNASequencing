#include <map>
#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>

using namespace std;

class Cache {
  private:
    map<int, vector<pair<int, int>>> data;

  public:
    Cache(){ }
    ~Cache(){ }

    int size() {
      return data.size();
    }

    void Sort() {
      for(auto & entry : data) {
        std::sort(entry.second.begin(), entry.second.end(),
            [](const pair<int, int>& p1, const pair<int, int>& p2)->bool {
              return p1.second > p2.second;
            });
      }
    }

    //Inserts an entry of two fragment indices (first, second) with their overlap value
    void put(const int& first, const int& second, const int& value) {
      data[first].push_back(make_pair(second, value));
    }

    //Returns the entry with maximum overlap value
    pair<int, pair<int, int>> get() {
      pair<int, pair<int, int>> p = make_pair(-1, make_pair(-1, -1));

      for(auto & entry : data) {
        if(entry.second.front().second > p.second.second) {
          p = make_pair(entry.first, entry.second.front());
        }
      }
      return p;
    }

    void insertNewOverlap(const int& first, const int& second) {
      //This function combines "first" and "second" fragments in this order
      //The index "second" is used as the index for the new combined fragment

      /*
       *Remove entry key "first" from map ("first" can't be combined with other fragments)
       *Remove entry second->first        ("second" can't come before "first")
       *Remove [ANY]->second              (No other fragment can come directly before second)
       *Replace [ANY]->first with [ANY]->second (Other fragments have an overlap with first, so they have
                                                  the same overlap values for the new combined fragment)
       */

      data.erase(first);

      if(data.find(second) != data.end()) {
        //Remove entry where second overlaps with first (second->first)
        auto first_it = remove_if(data[second].begin(), data[second].end(),
            [&](const pair<int, int>& entry)->bool {
              return entry.first == first;
            });
        data[second].erase(first_it);
      }

      for (auto & entry : data) {
        if (entry.first != second) {

          //Remove entry where any other fragment overlaps with second ([ANY]->second)
          auto second_it = remove_if(entry.second.begin(), entry.second.end(),
              [&](const pair<int, int>& p)->bool {
              return p.first == second;
              });
          entry.second.erase(second_it);

          //Change the second part of all overlap where [ANY]->first to [ANY]->second
          for (auto & p : entry.second) {
            if (p.first == first)
              p.first = second;
          }
        }
      }
    }

    friend ostream& operator<<(ostream& os, Cache& cache) {
      for(auto& entry : cache.data) {
        os << entry.first << endl;
        for(auto& p : entry.second) {
          os << "\t" << p.first << " -- " << p.second << endl;
        }
      }
      return os;
    }

};
