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
  cout << cache << endl;
  cache.insertNewOverlap("ABC", "BCD", "ABCD");
  cout << endl << cache << endl;



}
