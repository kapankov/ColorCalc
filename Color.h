#ifndef _COLOR_H_
#define _COLOR_H_

#include <iostream>

namespace COLORNS
{
	// base class with color channels
	class channels
	{
	protected:
		double m_ch1{ 0.0 };
		double m_ch2{ 0.0 };
		double m_ch3{ 0.0 };
	};

	class rgbcolor : public channels
	{

	public:
		double GetRed()
		{
			return m_ch1;
		}
		double GetGreen()
		{
			return m_ch2;
		}
		double GetBlue()
		{
			return m_ch3;
		}
		friend std::ostream& operator<< (std::ostream& out, const rgbcolor& rgb);
	};

	std::ostream& operator<< (std::ostream& out, const rgbcolor& rgb);
};

#endif