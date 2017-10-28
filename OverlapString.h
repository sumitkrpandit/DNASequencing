//This file implememts the overlap function for 2 strings and
//returns the length of the overlaps.

#include <string>
#include <algorithm>
#include <utility>

using namespace std;

/***********************************************************
 *Parameter: String a and String b by reference as input
 *           strings.
 *Return: Returns the overlap length of the 2 input stirngs
 *Description: This function loops from 1 to the length
 *             of the minimum string. Creates temporary
 *             strings that are compared with each other
 *             to find the overlap
 * *********************************************************/
int overlapStrings(string& a, string& b) {
  int overlap_length = 0;
  int a_length = a.length() - 1;
  int min_length = min(a.length(), b.length());


  for(int i = 1; i < min_length; i++) {

    string c = a.substr(a_length + 1 - i, i);
    string d = b.substr(0, i);

    if(c == d)
      overlap_length = i;

    else if (overlap_length)
      break;

  }

  return overlap_length;
}


/***************************************************************
 *Parameter: Recieves a pair of strings
 *Return: Returns a pair of ints with both overlaps
 *Description: Finds both possible overlaps using a first and b
 *			       second and using b first and a second. The first
 *	           in the pair represents the overlap of tail of a
 *             with head of b. And second element of the pair
 *             represents the overlap in the reverse order.
 * ************************************************************/
pair<int, int> overlapStrings(pair<string, string> str_pair) {

  int first = overlapStrings(str_pair.first, str_pair.second);
  int second = overlapStrings(str_pair.second, str_pair.first);

  return make_pair(first, second);
}
