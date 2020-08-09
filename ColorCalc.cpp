#include "Color.h"
#include <limits>

using namespace COLORNS;
using namespace std;

int main()
{
    int i = 0;
    double ch1 = -1.0;
    double ch2 = -1.0;
    double ch3 = -1.0;
    for (;;) {
        cout << "Please, select input color model (1 - RGB or 2 - HSV): ";
        if ((cin >> i) && (i==1 || i==2))
        {
            cin.clear();
            break;
        }
        else
        {
            cout << "Please enter a valid integer (1 or 2)" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    for (;;) {
        if (i==1)
            cout << "Please, enter red value (0..1.0): ";
        else
            cout << "Please, enter hue value (0..360): ";
        cin >> ch1;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (!cin.fail() && (i==1?ch1>=0 && ch1<=1.0:ch1>=0 && ch1<=360.0))
            break;
        else
        {
            if (i==1)
                cout << "Please enter a valid double value (0..1.0)" << endl;
            else
                cout << "Please enter a valid double value (0..360.0)" << endl;
            cin.clear();
        }
    }

    for (;;) {
        if (i==1)
            cout << "Please, enter green value (0..1.0): ";
        else
            cout << "Please, enter saturation value (0..1.0): ";
        cin >> ch2;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (!cin.fail() && (ch2>=0 && ch2<=1.0))
            break;
        else
        {
            cout << "Please enter a valid double value (0..1.0)" << endl;
            cin.clear();
        }
    }

    for (;;) {
        if (i==1)
            cout << "Please, enter blue value (0..1.0): ";
        else
            cout << "Please, enter value (0..1.0): ";
        cin >> ch3;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (!cin.fail() && (ch3>=0 && ch3<=1.0))
            break;
        else
        {
            cout << "Please enter a valid double value (0..1.0)" << endl;
            cin.clear();
        }
    }

    if (i==1)
    {
        Color clr(RgbColor(ch1, ch2, ch3));
        cout << clr.GetHSV() << ", lightness:" << clr.GetHSV().GetLightness() << ", relative luminance:" << clr.GetHSV().GetLuminance() << endl;
        cout << clr.GetXYZ() << endl;
    }
    else
    {
        Color clr(HsvColor(ch1, ch2, ch3));
        cout << clr.GetRGB() << endl;
        cout << clr.GetXYZ() << endl;
    }

}
