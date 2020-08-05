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
		channels() {}
		channels(double ch1, double ch2, double ch3): 
		m_ch1(ch1), m_ch2(ch2), m_ch3(ch3) {}
	};

	class RgbColor : public channels
	{

	public:
		RgbColor() = default;
		RgbColor(double Red, double Green, double Blue);
		RgbColor(const RgbColor& rgb);
		double GetRed() const noexcept;
		double GetGreen() const noexcept;
		double GetBlue() const noexcept;

		friend std::ostream& operator<< (std::ostream& out, const RgbColor& rgb);

		RgbColor& operator= (const RgbColor &rgb);
	};

	std::ostream& operator<< (std::ostream& out, const RgbColor& rgb);

	class Color
	{
		RgbColor m_rgb;
	public: 
		Color() = default;
		Color(const RgbColor& rgb);
		RgbColor& GetRGB();
		// перегруженный оператор приведения типа
		// using: static_cast<RgbColor>(clr)
		operator RgbColor() { return m_rgb; }
	};
};

#endif