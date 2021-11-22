#include <iostream>
#include <vector>
#include <sstream>
#include <functional>
#include <unordered_map>
#include <algorithm>

template < typename container, typename type >
bool is_in (const container& c, const type& e) { return c.find(e) != c.end(); }

struct command
{
    using args = const std::vector<std::string>&;
    int length;
    std::function<std::string(const std::vector<std::string>&)> fn;
};

std::unordered_map<std::string,command> comms = {
    {"+",{2,[](command::args v){return std::to_string(std::stoi(v[0])+std::stoi(v[1]));}}},
    {"-",{2,[](command::args v){return std::to_string(std::stoi(v[0])-std::stoi(v[1]));}}},
    {"*",{2,[](command::args v){return std::to_string(std::stoi(v[0])*std::stoi(v[1]));}}},
    {"/",{2,[](command::args v){return std::to_string(std::stoi(v[0])/std::stoi(v[1]));}}},
    {"%",{2,[](command::args v){return std::to_string(std::stoi(v[0])%std::stoi(v[1]));}}},
    {"reverse",{1,[](command::args v){std::string s = v[0]; std::reverse(s.begin(),s.end()); return s;}}},
    {"&",{2,[](command::args v){return ((v[0] != "0") && (v[1] != "0"))?"1":"0";}}},
    {"|",{2,[](command::args v){return ((v[0] != "0") || (v[1] != "0"))?"1":"0";}}},
    {"!",{1,[](command::args v){return (v[0] == "0")?"1":"0";}}}
};

std::string calc (std::string input)
{
    std::vector<std::string> st;
    //std::reverse(input.begin(),input.end()); // notacja polska
    std::stringstream ss(input);
    for (std::string s; ss >> s;)
    {
        //std::reverse(s.begin(),s.end()); // notacja polska
        if (is_in(comms,s) && comms[s].length <= st.size())
        {
            auto it = st.end()-comms[s].length;
            std::vector<std::string> args(it,st.end());
            st.erase(it,st.end());
            st.push_back(comms[s].fn(args));
        }
        else st.push_back(s);
    }
    std::string res;
    for (const std::string& s: st) (res += s) += " ";
    return res;
}

int main (int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cout << "$[in]: ";
        for (std::string input; getline(std::cin,input,'\n'); std::cout << "$[out]: " << calc(input) << "\n$[in]: ");
    }
    else
    {
        std::string input;
        for (int i{1}; i < argc; ++i) (input += argv[i]) += " ";
        std::cout << "$[out]: " << calc(input) << "\n";
    }

}