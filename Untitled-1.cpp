#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
//#include "Untitled-1.hpp"
using namespace std;

int main()
{
    bitset<4> in("1001");
    cout << in << endl;
    
    cout<<(in<<2)<<endl;
    return 0;
}