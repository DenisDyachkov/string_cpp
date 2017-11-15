#include "string/string.hpp"

int main()
{
    string test;
    while (std::cin >> test) {
        std::cout << "Enter string \"" << test << "\" " << test.length() << " " << test.capacity() << std::endl;
        if (test == "exit") {
            break;
        }
        test.clear();
    }
    return 0;
}