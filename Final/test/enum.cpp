#include <iostream>

namespace TF {
    enum class sym {
        hh = 1,
        mm = hh
    };
}

namespace TT {
    enum class sym2 {
        uu = 1,
        yy = 2
    };
}

int main()
{
    std::cout << static_cast<int>(TF::sym::mm);
    return 0; 
}