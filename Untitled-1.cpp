#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include "Untitled-1.hpp"
#include "clipp.h"
using namespace std;
using namespace clipp;
#define ENCRYPT 0
#define DECRYPT 1

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
    string key = "0";
    bool mode = ENCRYPT;
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
        cout << "read file: " + filepath + " error" << endl;
        cout << make_man_page(cli, argv[0]) << endl;
        return 1;
    }
    vector<bitset<64>> in_file_data;
    bitset<64> tmp_data = 0;
    while (in_file.read((char *)&tmp_data, sizeof(tmp_data)))
    {
        in_file_data.push_back(tmp_data);
        tmp_data.reset();
    }
    in_file.close();

    if (mode == ENCRYPT)
    {
        vector<bitset<64>> out_file_data;
        for (size_t i = 0; i < in_file_data.size(); i++)
        {
            bitset<64> tmp_ip = initPermutation(in_file_data[i]);
            bitset<32> tmp_left1, tmp_right, tmp_left2;
            for (size_t j = 0; j < 32; j++)
            { //分为两部分
                tmp_right[j] = tmp_ip[j];
                tmp_left1[j] = tmp_ip[j + 32];
            }
            for (size_t j = 0; j < 16; j++)
            { //16轮迭代
                /* code */
                tmp_left2 = tmp_right;
                tmp_right = tmp_left1 ^ f(tmp_right, subkey[j]);
                tmp_left1 = tmp_left2;
            }
            bitset<64> tmp_comb;
            for (size_t j = 0; j < 32; j++)
            { //合并两部分
                tmp_comb[j] = tmp_left1[j];
                tmp_comb[j + 32] = tmp_right[j];
            }
            bitset<64> tmp_ipinv = initPermutationInv(tmp_comb);
            out_file_data.push_back(tmp_ipinv);
        }

        ofstream out_file(filepath + ".data", ios::out | ios::binary);
        if (!out_file.is_open())
        {
            cout << "write file: " + filepath + ".data error" << endl;
            cout << make_man_page(cli, argv[0]) << endl;
            return 1;
        }
        for (size_t i = 0; i < out_file_data.size(); i++)
        {
            out_file.write((char *)&out_file_data[i], sizeof(out_file_data[i]));
        }
        out_file.close();
    }

    else if (mode == DECRYPT)
    {
        vector<bitset<64>> out_file_data;
        for (size_t i = 0; i < in_file_data.size(); i++)
        {
            bitset<64> tmp_ip = initPermutation(in_file_data[i]);
            bitset<32> tmp_left1, tmp_right, tmp_left2;
            for (size_t j = 0; j < 32; j++)
            { //分为两部分
                tmp_right[j] = tmp_ip[j];
                tmp_left1[j] = tmp_ip[j + 32];
            }
            for (size_t j = 0; j < 16; j++)
            { //16轮迭代
                /* code */
                tmp_left2 = tmp_right;
                tmp_right = tmp_left1 ^ f(tmp_right, subkey[15-j]);
                tmp_left1 = tmp_left2;
            }
            bitset<64> tmp_comb;
            for (size_t j = 0; j < 32; j++)
            { //合并两部分
                tmp_comb[j] = tmp_left1[j];
                tmp_comb[j + 32] = tmp_right[j];
            }
            bitset<64> tmp_ipinv = initPermutationInv(tmp_comb);
            out_file_data.push_back(tmp_ipinv);
        }

        ofstream out_file(filepath + ".rec", ios::out | ios::binary);
        if (!out_file.is_open())
        {
            cout << "write file: " + filepath + ".data error" << endl;
            cout << make_man_page(cli, argv[0]) << endl;
            return 1;
        }
        for (size_t i = 0; i < out_file_data.size(); i++)
        {
            out_file.write((char *)&out_file_data[i], sizeof(out_file_data[i]));
        }
        out_file.close();
    }
    
    return 0;
}