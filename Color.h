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

	class XyzColor : public channels
	{

	public:
		XyzColor() = default;
		XyzColor(double X, double Y, double Z);
		XyzColor(const XyzColor& xyz);
		double GetX() const noexcept;
		double GetY() const noexcept;
		double GetZ() const noexcept;

		friend class Color;

		friend std::ostream& operator<< (std::ostream& out, const XyzColor& xyz);

		XyzColor& operator= (const XyzColor& xyz);
	};

	std::ostream& operator<< (std::ostream& out, const XyzColor& hsv);
	class RgbColor : public channels
	{
		double m_gamma{-2.2};
	public:
		RgbColor() = default;
		RgbColor(double Red, double Green, double Blue);
		RgbColor(const RgbColor& rgb);
		double GetRed() const noexcept;
		double GetGreen() const noexcept;
		double GetBlue() const noexcept;
		double GetGamma() const noexcept;

		friend class Color;

		friend std::ostream& operator<< (std::ostream& out, const RgbColor& rgb);

		RgbColor& operator= (const RgbColor &rgb);
	};

	std::ostream& operator<< (std::ostream& out, const RgbColor& rgb);

	class HsvColor : public channels
	{
		double m_lightness{ -1.0 };
		double m_luminance{ -1.0 };
	public:
		HsvColor() = default;
		HsvColor(double Red, double Green, double Blue);
		HsvColor(const HsvColor& hsv);
		double GetHue() const noexcept;
		double GetSaturation() const noexcept;
		double GetValue() const noexcept;
		double GetBrightness() const noexcept;
		double GetLightness() noexcept;
		double GetLuminance() noexcept;

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
			unsigned long xyz : 1;
		} models;
		typedef union _flags
		{
			models mods;
			unsigned long reset{static_cast<unsigned long>(-1)};
		} flags;

		RgbColor m_rgb;
		HsvColor m_hsv;
		XyzColor m_xyz;

		flags m_valid;
	public: 
		Color() = default;
		Color(const RgbColor& rgb);
		Color(const HsvColor& hsv);
		XyzColor GetXYZ();
		RgbColor GetRGB();
		HsvColor GetHSV();
		// перегруженные операторы приведения типа
		// using: static_cast<RgbColor>(clr)
		operator XyzColor() { return GetXYZ(); }
		operator RgbColor() { return GetRGB(); }
		operator HsvColor() { return GetHSV(); }
	};
};

#endif