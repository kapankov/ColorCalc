#include "Color.h"

using namespace COLORNS;

int main()
{
 //   Color clr;
    Color clr(RgbColor(0.3, 0.4, 0.5));
    std::cout << clr.GetRGB() << std::endl;
}
