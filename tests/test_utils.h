#include <iostream>
#include <stdlib.h>
#include <string>

/* just some formatting */
inline void new_test(std::string test_mssg)
{
    std::cout << "-------------------------------------\n"
              << "NEW TEST: " << test_mssg << "\n";
}

/* function to generate rando string of certain length from SO */
inline std::string gen_random_str(int len)
{
    std::string str;
    for (int i = 0; i < len; ++i)
    {
        int randomChar = rand() % (26 + 26 + 10);
        if (randomChar < 26)
            str += ('a' + randomChar);
        else if (randomChar < 26 + 26)
            str += ('A' + randomChar - 26);
        else
            str += ('0' + randomChar - 26 - 26);
    }
    return str;
}
