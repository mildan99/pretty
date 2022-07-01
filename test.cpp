#include <iostream>
#include <string_view>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <iostream>
#include <algorithm>
#include <ranges>
#include <pretty_pirnt.hpp>
int main(int argc, char const *argv[])
{
     std::pair p{6.6,3};
    int i=2;

    memes::println(1,3);
    memes::println(i);
    memes::println(p);
    memes::println(std::pair{3,6.6});
    memes::println(std::vector<std::array<int,2>>{{1,6},{2,3}});
    memes::println(std::map<int,std::string>{{{1,"111"},{2,"2222"},{3,"8564"}}});
    std::map<int,std::array<double,2>> m{{1,{1.0,1.1}},{2,{1.4,8.8}},{3,{3.1416,2.71828}}};
    memes::println(m);
    char c[]="acards";
    c[0]='m';
    memes::println(std::tuple{"a",1,1.23,"|",std::pair{c,std::string{"ds"}}});

    return 0;
}
