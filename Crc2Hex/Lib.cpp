#include <iostream>
#include <string>

using namespace std;

// Convert String in Gro�buchstaben
string UCase(string s)
{
	for (unsigned int i = 0; i < s.length(); i++)
	{
		s[i] = toupper(s[i]);
	}
	return s;
}

