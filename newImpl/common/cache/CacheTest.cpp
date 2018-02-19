#include"Cache.h"
#include"../StringProcessing.h"
#include<iostream>
#include<string>
#include<vector>

using namespace std;

int main() {
  Cache cache;

  int n;
  cin >> n;

  string str;
  vector<string> v;
  while(cin >> str){
    v.push_back(str);
  }

  for(int i = 0; i < v.size(); ++i){
    for(int j = 0; j < v.size(); j++){
      if(i != j){
        cache.put(v[i], v[j], overlapStrings(v[i], v[j]));
      }
    }
  }
  cache.Sort();
  auto p = cache.get();
  cout << p.first << "\t" << p.second.first << "\t" << p.second.second << "\n\n";
  cout << cache << endl;
  cache.insertNewOverlap("ABC", "BCD", "ABCD");
  p = cache.get();
  cout << p.first << "\t" << p.second.first << "\t" << p.second.second << "\n\n";
  cout << endl << cache << endl;



}
