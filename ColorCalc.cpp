#include "Color.h"
#include <limits>

using namespace COLORNS;
using namespace std;

template <typename T>
T stdcin_inrange (std::string message, T low, T up)
{
    T value = 0;
    while ((std::cout << message)
        && (!(std::cin >> value) || value > up || value < low)) {
        std::cout << "That's not a number between ";
        std::cout << low;
        std::cout << " and ";
        std::cout << up;
        std::cout << ";\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return value;
};


int main()
{
    int i = 0;
    double ch1 = -1.0;
    double ch2 = -1.0;
    double ch3 = -1.0;
    Color clr;

    i = stdcin_inrange<int>("Please, select input color model (1 - RGB, 2 - HSV, 3 - XYZ, 4 - Lab): ", 1, 4);

    switch (i)
    {
    case 1:
        ch1 = stdcin_inrange<double>("Please, enter red (0..1.0): ", 0.0, 1.0);
        ch2 = stdcin_inrange<double>("Please, enter green (0..1.0): ", 0.0, 1.0);
        ch3 = stdcin_inrange<double>("Please, enter blue (0..1.0): ", 0.0, 1.0);
        clr = RgbColor(ch1, ch2, ch3);
        cout << clr.GetHSV() << ", lightness:" << clr.GetHSV().GetLightness() << ", relative luminance:" << clr.GetHSV().GetLuminance() << endl;
        cout << clr.GetXYZ() << endl;
        cout << clr.GetLAB() << endl;
        break;
    case 2:
        ch1 = stdcin_inrange<double>("Please, enter hue (0..360): ", 0.0, 360.0);
        ch2 = stdcin_inrange<double>("Please, enter saturation (0..1.0): ", 0.0, 1.0);
        ch3 = stdcin_inrange<double>("Please, enter brightness value (0..1.0): ", 0.0, 1.0);
        clr = HsvColor(ch1, ch2, ch3);
        cout << "lightness:" << clr.GetHSV().GetLightness() << ", relative luminance:" << clr.GetHSV().GetLuminance() << endl;
        cout << clr.GetRGB() << endl;
        cout << clr.GetXYZ() << endl;
        cout << clr.GetLAB() << endl;
        break;
    case 3:
        ch1 = stdcin_inrange<double>("Please, enter X (0..1.0): ", 0.0, 1.0);
        ch2 = stdcin_inrange<double>("Please, enter Y (0..1.0): ", 0.0, 1.0);
        ch3 = stdcin_inrange<double>("Please, enter Z (0..1.0): ", 0.0, 1.0);
        clr = XyzColor(ch1, ch2, ch3);
        cout << clr.GetRGB() << endl;
        cout << clr.GetHSV() << ", lightness:" << clr.GetHSV().GetLightness() << ", relative luminance:" << clr.GetHSV().GetLuminance() << endl;
        cout << clr.GetLAB() << endl;
        break;
    case 4:
        ch1 = stdcin_inrange<double>("Please, enter L (0..100.0): ", 0.0, 100.0);
        ch2 = stdcin_inrange<double>("Please, enter a (0..100.0): ", 0.0, 100.0);
        ch3 = stdcin_inrange<double>("Please, enter b (0..100.0): ", 0.0, 100.0);
        clr = LabColor(ch1, ch2, ch3);
        cout << clr.GetXYZ() << endl;
        cout << clr.GetRGB() << endl;
        cout << clr.GetHSV() << ", lightness:" << clr.GetHSV().GetLightness() << ", relative luminance:" << clr.GetHSV().GetLuminance() << endl;
        break;
    default:
        return 0;
    }
}
