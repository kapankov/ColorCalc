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
        cout << "Please, select input color model (1 - RGB, 2 - HSV, 3 - XYZ, 4 - Lab): ";
        if ((cin >> i) && (i>0 && i<5))
        {
            cin.clear();
            break;
        }
        else
        {
            cout << "Please enter a valid integer (1 to 4)" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    for (;;) {
        if (i == 1)
            cout << "Please, enter red (0..1.0): ";
        else if (i == 2)
            cout << "Please, enter hue (0..360): ";
        else if (i == 3)
            cout << "Please, enter X (0..1.0): ";
        else
            cout << "Please, enter L (0..100.0): ";

        cin >> ch1;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (!cin.fail() && (i==1?ch1>=0 && ch1<=1.0:ch1>=0 && ch1<=360.0))
            break;
        else
        {
            if (i == 2)
                cout << "Please enter a valid double value (0..360.0)" << endl;
            if (i == 4)
                cout << "Please enter a valid double value (0..100.0)" << endl;
            else
                cout << "Please enter a valid double value (0..1.0)" << endl;
            cin.clear();
        }
    }

    for (;;) {
        if (i == 1)
            cout << "Please, enter green (0..1.0): ";
        else if (i == 2)
            cout << "Please, enter saturation (0..1.0): ";
        else if (i == 3)
            cout << "Please, enter Y (0..1.0): ";
        else
            cout << "Please, enter a (-100..100.0): ";

        cin >> ch2;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (!cin.fail() && (ch2>=0 && ch2<=1.0))
            break;
        else
        {
            if (i == 4)
                cout << "Please enter a valid double value (-100.0..100.0)" << endl;
            else
                cout << "Please enter a valid double value (0..1.0)" << endl;
            cin.clear();
        }
    }

    for (;;) {
        if (i==1)
            cout << "Please, enter blue value (0..1.0): ";
        else if (i == 1)
            cout << "Please, enter value (0..1.0): ";
        else if (i == 1)
            cout << "Please, enter Z (0..1.0): ";
        else
            cout << "Please, enter b (-100.0..100.0): ";

        cin >> ch3;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (!cin.fail() && (ch3>=0 && ch3<=1.0))
            break;
        else
        {
            if (i == 4)
                cout << "Please enter a valid double value (-100.0..100.0)" << endl;
            else
                cout << "Please enter a valid double value (0..1.0)" << endl;
            cin.clear();
        }
    }

    if (i==1)
    {
        Color clr(RgbColor(ch1, ch2, ch3));
        cout << clr.GetHSV() << ", lightness:" << clr.GetHSV().GetLightness() << ", relative luminance:" << clr.GetHSV().GetLuminance() << endl;
        cout << clr.GetXYZ() << endl;
        cout << clr.GetLAB() << endl;
    }
    else if (i == 2)
    {
        Color clr(HsvColor(ch1, ch2, ch3));
        cout << "lightness:" << clr.GetHSV().GetLightness() << ", relative luminance:" << clr.GetHSV().GetLuminance() << endl;
        cout << clr.GetRGB() << endl;
        cout << clr.GetXYZ() << endl;
        cout << clr.GetLAB() << endl;
    }
    else if (i == 3)
    {
        Color clr(XyzColor(ch1, ch2, ch3));
        cout << clr.GetRGB() << endl;
        cout << clr.GetHSV() << ", lightness:" << clr.GetHSV().GetLightness() << ", relative luminance:" << clr.GetHSV().GetLuminance() << endl;
        cout << clr.GetLAB() << endl;
    }
    else
    {
        Color clr(LabColor(ch1, ch2, ch3));
        cout << clr.GetXYZ() << endl;
        cout << clr.GetRGB() << endl;
        cout << clr.GetHSV() << ", lightness:" << clr.GetHSV().GetLightness() << ", relative luminance:" << clr.GetHSV().GetLuminance() << endl;
    }

}
