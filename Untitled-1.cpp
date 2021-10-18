#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include "Untitled-1.hpp"
#include "clipp.h"
using namespace std;
using namespace clipp;

bitset<64> strToBits(string str)
{

    cout << str.substr(0, 8) << endl;
    bitset<64> strbit = bitset<64>('sac');
    cout << strbit << endl;
    for (size_t i = 0; i < (str.length() < 8 ? str.length() : 8); i++)
    {
        /* code */
    }

    return strbit;
}

int main(int argc, char *argv[])
{
    bitset<4> in("1001");
    cout << in << endl;
    cout << (in << 2) << endl;

    string filepath = "";
    string key = "123456";
    strToBits("123456789");
    bool mode = 0;
    auto cli = ((option("-f", "--filepath") & value("input file", filepath)) % "file path",
                (option("-k", "--key") & value("key", key)) % "key for encrypt/decrypt",
                (option("-m", "--mode") & value("mode", mode)) % "0 for encrypt, 1 for decrypt");
    if (!parse(argc, argv, cli) || filepath.empty())
    {
        cout << make_man_page(cli, argv[0]) << endl;
        return 1;
    }
    cout << "file path: " << filepath << endl;
    cout << "key: " << key << endl;
    if (mode == 0)
    {
        cout << "mode: encrypt" << endl;
    }
    else if (mode == 1)
    {
        cout << "mode: decrypt" << endl;
    }

    return 0;
}