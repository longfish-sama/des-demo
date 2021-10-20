#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include "Untitled-1.hpp"
#include "clipp.h"
using namespace std;
using namespace clipp;

bitset<64> strKeyToBits(string str)
{
    bitset<64> bit_str;
    bitset<8> tmp_bit;
    for (size_t i = 0; i < (str.length() < 8 ? str.length() : 8); i++)
    {
        tmp_bit = str[i];
        bit_str[(7 - i) * 8 + 7] = tmp_bit[7];
        bit_str[(7 - i) * 8 + 6] = tmp_bit[6];
        bit_str[(7 - i) * 8 + 5] = tmp_bit[5];
        bit_str[(7 - i) * 8 + 4] = tmp_bit[4];
        bit_str[(7 - i) * 8 + 3] = tmp_bit[3];
        bit_str[(7 - i) * 8 + 2] = tmp_bit[2];
        bit_str[(7 - i) * 8 + 1] = tmp_bit[1];
        bit_str[(7 - i) * 8 + 0] = tmp_bit[0];
    }
    return bit_str;
}

int main(int argc, char *argv[])
{
    bitset<4> in("1001");
    cout << in << endl;
    cout << (in << 2) << endl;

    string filepath = "LICENSE";
    string key = "123456";
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

    bitset<48> subkey[16];
    genSubKey(strKeyToBits(key), subkey);

    ifstream in_file(filepath, ios::in | ios::binary);
    if (!in_file.is_open())
    {
        cout << make_man_page(cli, argv[0]) << endl;
        return 1;
    }
    vector<bitset<64>> in_file_data;
    bitset<64> tmp_data;
    while (in_file.read((char *)&tmp_data, sizeof(tmp_data)))
    {
        in_file_data.push_back(tmp_data);
        tmp_data.reset();
    }
    in_file.close();
    
    cout<<in_file.gcount()<<endl;

    return 0;
}