#include "../time_tool.h"

#include<iostream>

using namespace std;

int main(int argc, char** argv)
{
	//TIME_TOOL TEST CASE
	cout << "BEGIN UNITTEST" << endl;	
	clib::TimeUtility timeUtility;
    string result = (1390838400 == timeUtility.Extract("2014年1月28日")) ? "PASS" : "FAIL";
    cout << "CASE1: " << result  << endl;
	return 0;
}
