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

		friend class Color;

		friend std::ostream& operator<< (std::ostream& out, const RgbColor& rgb);

		RgbColor& operator= (const RgbColor &rgb);
	};

	std::ostream& operator<< (std::ostream& out, const RgbColor& rgb);

	class HsvColor : public channels
	{

	public:
		HsvColor() = default;
		HsvColor(double Red, double Green, double Blue);
		HsvColor(const HsvColor& hsv);
		double GetHue() const noexcept;
		double GetSaturation() const noexcept;
		double GetValue() const noexcept;

		friend class Color;
		friend std::ostream& operator<< (std::ostream& out, const HsvColor& hsv);

		HsvColor& operator= (const HsvColor& rgb);
	};

	std::ostream& operator<< (std::ostream& out, const HsvColor& hsv);

	class Color
	{
		typedef struct _models
		{
			unsigned long rgb : 1;
			unsigned long hsv : 1;
		} models;
		typedef union _flags
		{
			models mods;
			unsigned long reset{static_cast<unsigned long>(-1)};
		} flags;

		RgbColor m_rgb;
		HsvColor m_hsv;

		flags m_valid;
	public: 
		Color() = default;
		Color(const RgbColor& rgb);
		Color(const HsvColor& hsv);
		RgbColor GetRGB();
		HsvColor GetHSV();
		// перегруженные операторы приведения типа
		// using: static_cast<RgbColor>(clr)
		operator RgbColor() { return GetRGB(); }
		operator HsvColor() { return GetHSV(); }
	};
};

#endif