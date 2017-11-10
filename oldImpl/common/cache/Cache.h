#include<string>
#include<map>
#include<utility>

using namespace std;

#define PSS pair<string, string>

class Cache {
    private:
        map<PSS, int> data;

    public:
        Cache(){ }
        ~Cache(){ }

        //Inserts the overlap value for a to b into the cache
        void put(const string& a, const string& b, const int& value) {
            PSS pss = make_pair(a, b);
            data[pss] = value;
        }

        //Returns the cached value for a to b.
        //If the overlap value is not in the cache, it returns -1
        int get(const string& a, const string& b) {
            PSS pss = make_pair(a, b);
            int value = -1;
            if(data.find(pss) != data.end()){
                value = data[pss];
            }

            return value;
        }

        //Erases the overlap data for string a and b in any order (a to b AND b to a)
        void remove(const string& a, const string& b){
            data.erase(make_pair(a, b));
            data.erase(make_pair(b, a));
        }

};
