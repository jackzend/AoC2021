#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <algorithm>
#include <numeric>
using namespace std;

string bin_v = "";

//vector<int> vnum_v; // version number vector

void hex_to_bin(string &hexdec)
{
    long int i = 0;

    while (hexdec[i])
    {

        switch (hexdec[i])
        {
        case '0':
            bin_v += "0000";
            break;
        case '1':
            bin_v += "0001";
            break;
        case '2':
            bin_v += "0010";
            break;
        case '3':
            bin_v += "0011";
            break;
        case '4':
            bin_v += "0100";
            break;
        case '5':
            bin_v += "0101";
            break;
        case '6':
            bin_v += "0110";
            break;
        case '7':
            bin_v += "0111";
            break;
        case '8':
            bin_v += "1000";
            break;
        case '9':
            bin_v += "1001";
            break;
        case 'A':
        case 'a':
            bin_v += "1010";
            break;
        case 'B':
        case 'b':
            bin_v += "1011";
            break;
        case 'C':
        case 'c':
            bin_v += "1100";
            break;
        case 'D':
        case 'd':
            bin_v += "1101";
            break;
        case 'E':
        case 'e':
            bin_v += "1110";
            break;
        case 'F':
        case 'f':
            bin_v += "1111";
            break;
        default:
            cout << "\nInvalid hexadecimal digit "
                 << hexdec[i];
        }
        i++;
    }
}

string get_n_bits(long int n) // gets next n bits and destroys that part of the string
{
    string temp{begin(bin_v), begin(bin_v) + n};

    bin_v.erase(begin(bin_v), begin(bin_v) + n);

    //cout << bin_v << endl;
    return temp;
}

void parseInput(string &fname)
{
    ifstream f;
    f.open(fname);
    string temp;
    f >> temp;
    //cout << temp << endl;
    hex_to_bin(temp);

    cout << bin_v << endl;
}

unsigned long long int processPacket() // assume we're at the beginning of the next packet
{
    unsigned short version_number = stoi(get_n_bits(3), nullptr, 2);
    //vnum_v.push_back(version_number);
    unsigned short type_number = stoi(get_n_bits(3), nullptr, 2);

    //cout << "Version Number: " << version_number << " Type Number: " << type_number << endl;

    if (type_number == 4)
    {
        /*string literal_string = "";
        while (get_n_bits(1) == "1")
        {
            literal_string += get_n_bits(4);
        }
        literal_string += get_n_bits(4);

        unsigned long long int literal = stoll(literal_string, nullptr, 2);
        //cout << "Literal Value: " << literal << endl;

        /*while (lit_packet_length % 4 != 0)
        {
            get_n_bits(1);
            ++lit_packet_length;
        }*/
        //cout << "Literal Length: " << lit_packet_length << endl;

        //cout << bin_v << endl;
        //return literal; // base case
        bool not_ended = true;
        std::string value_str;
        while (not_ended)
        {
            if (bin_v.front() == '0')
                not_ended = false;
            bin_v.erase(bin_v.begin());

            value_str.append({bin_v.begin(), bin_v.begin() + 4});
            bin_v.erase(bin_v.begin(), bin_v.begin() + 4);
        }
        return std::stoll(value_str, nullptr, 2);
    }
    else
    {
        vector<unsigned long long int> subPacket_nums;

        string length_id = get_n_bits(1);
        //cout << length_id;
        unsigned short t = stoi(length_id);

        if (t == 0)
        {
            //cout << " 0 " << endl;
            unsigned short bit_length = stoi(get_n_bits(15), nullptr, 2);
            unsigned short remaining_length = bin_v.size() - bit_length;

            // cout << "Subpackets bit length: " << bit_length << endl;
            size_t comp = static_cast<size_t>(remaining_length);
            while (bin_v.size() > comp)
            {
                subPacket_nums.push_back(processPacket());
            }
        }
        else
        {
            //cout << " 1 " << endl;
            unsigned short num_subpackets = stoi(get_n_bits(11), nullptr, 2);
            // cout << "Num Subpackets: " << num_subpackets << endl;

            for (unsigned short i = 0; i < num_subpackets; i++)
            {
                subPacket_nums.push_back(processPacket());
            }
        }

        if (type_number == 0)
        {
            unsigned long long int value = 0;
            for (auto &it : subPacket_nums)
            {
                value += it;
            }
            return value;
        }
        else if (type_number == 1)
        {
            unsigned long long int value = 1;
            for (auto &it : subPacket_nums)
            {
                value *= it;
            }
            return value;
        }
        else if (type_number == 2)
        {
            return *min_element(subPacket_nums.begin(), subPacket_nums.end());
        }
        else if (type_number == 3)
        {
            return *max_element(subPacket_nums.begin(), subPacket_nums.end());
        }
        else if (type_number == 5)
        {
            return (unsigned long long int)(subPacket_nums[0] > subPacket_nums[1]);
        }
        else if (type_number == 6)
        {
            return (unsigned long long int)(subPacket_nums[0] < subPacket_nums[1]);
        }

        else if (type_number == 7)
        {
            return (unsigned long long int)(subPacket_nums[0] == subPacket_nums[1]);
        }
        //return accumulate(begin(subPacket_nums), end(subPacket_nums), version_number);
    }
}

void partOne()
{

    cout << processPacket() << endl;
}
void partTwo() {}

int main(int argc, char **argv)
{
    string fname = argv[1];
    parseInput(fname);
    partOne();
    partTwo();
}