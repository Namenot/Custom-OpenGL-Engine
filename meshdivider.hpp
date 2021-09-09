#include <iostream>
#include <vector>
#include <array>


class meshDivider
{
private:

    int maxvectorsize;

    std::vector<std::array<short, maxvertecies*3>>;

public:

    void set_maxvectorsize(int newsize)
    {
        maxvectorsize = newsize;
        devidemesh();
    }

    /* How does this work:

    - every face is noted into a list by its center polar coordiates
    -

    */


};
