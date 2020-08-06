#include "Color.h"

#include <algorithm>
#include <cmath>

namespace COLORNS
{
	// BT.709 (Rec.709)
	constexpr double  Pr = .2126;
	constexpr double  Pg = .7152;
	constexpr double  Pb = .0722;

	void GetHSPVL(const double r, const double g, const double b,
		double& h, double& s, double& p, double& v, double& l)
	{
		double max = std::max(std::max(r, g), b);
		double min = std::min(std::min(r, g), b);
		double d = max - min;
		h = 0;
		s = max == 0 ? 0 : (d / max);
		p = sqrt(r * r * Pr + g * g * Pg + b * b * Pb);
		v = max;
		l = (min + max) / 2;
		if (d != 0)
		{
			if (r == max) h = (g - b) / d;
			else if (g == max) h = 2 + (b - r) / d;
			else h = 4 + (r - g) / d;
			h *= 60;
			if (h < 0) h += 360;
		}
	}

	void GetRGBfromHSV(const double h, const double s, const double v,
		double& r, double& g, double& b)
	{
		r = 0;
		g = 0;
		b = 0;
		double H = static_cast<double>(static_cast<int>(h) % 360) / 360;
		double S = s;
		double V = v;
		if (S)
		{
			H *= 6;
			int vi = static_cast<int>(floor(H));
			double v1 = V * (1 - S);
			double v2 = V * (1 - S * (H - vi));
			double v3 = V * (1 - S * (1 - (H - vi)));
			switch (vi)
			{
			case 0:
			{
				r = V;
				g = v3;
				b = v1;
				break;
			}
			case 1:
			{
				r = v2;
				g = V;
				b = v1;
				break;
			}
			case 2:
			{
				r = v1;
				g = V;
				b = v3;
				break;
			}
			case 3:
			{
				r = v1;
				g = v2;
				b = V;
				break;
			}
			case 4:
			{
				r = v3;
				g = v1;
				b = V;
				break;
			}
			default:
			{
				r = V;
				g = v1;
				b = v2;
				break;
			}
			}
		}
		else
		{
			r = g = b = V;
		}
	}

	RgbColor::RgbColor(double Red, double Green, double Blue):
		channels(Red, Green, Blue)
	{}
	RgbColor::RgbColor(const RgbColor& rgb):
		channels(rgb.m_ch1, rgb.m_ch2, rgb.m_ch3)
	{}
	double RgbColor::GetRed() const noexcept
	{
		return m_ch1;
	}
	double RgbColor::GetGreen() const noexcept
	{
		return m_ch2;
	}
	double RgbColor::GetBlue() const noexcept
	{
		return m_ch3;
	}

	RgbColor& RgbColor::operator= (const RgbColor &rgb)
	{
		m_ch1 = rgb.m_ch1;
		m_ch2 = rgb.m_ch2;
		m_ch3 = rgb.m_ch3;
		return *this;
	}

	std::ostream& operator<< (std::ostream& out, const RgbColor& rgb)
	{
		out << "rgb(" << rgb.m_ch1 << ", " << rgb.m_ch2 << ", " << rgb.m_ch3 << ")";
		return out;
	}

	HsvColor::HsvColor(double Hue, double Saturation, double Value) :
		channels(Hue, Saturation, Value)
	{}
	HsvColor::HsvColor(const HsvColor& hsv) :
		channels(hsv.m_ch1, hsv.m_ch2, hsv.m_ch3)
	{}
	double HsvColor::GetHue() const noexcept
	{
		return m_ch1;
	}
	double HsvColor::GetSaturation() const noexcept
	{
		return m_ch2;
	}
	double HsvColor::GetValue() const noexcept
	{
		return m_ch3;
	}

	HsvColor& HsvColor::operator= (const HsvColor& hsv)
	{
		m_ch1 = hsv.m_ch1;
		m_ch2 = hsv.m_ch2;
		m_ch3 = hsv.m_ch3;
		return *this;
	}

	std::ostream& operator<< (std::ostream& out, const HsvColor& hsv)
	{
		out << "hsv(" << hsv.m_ch1 << ", " << hsv.m_ch2 << ", " << hsv.m_ch3 << ")";
		return out;
	}

	Color::Color(const RgbColor& rgb):
		m_rgb(rgb)
	{
		m_valid.reset = 0;
		m_valid.mods.rgb = 1;
	}
	Color::Color(const HsvColor & hsv):
		m_hsv(hsv)
	{
		m_valid.reset = 0;
		m_valid.mods.hsv = 1;
	}
	RgbColor Color::GetRGB()
	{
		if (m_valid.mods.rgb == 0)
		{
			// convert
			GetRGBfromHSV(m_hsv.m_ch1, m_hsv.m_ch2, m_hsv.m_ch3,
				m_rgb.m_ch1, m_rgb.m_ch2, m_rgb.m_ch3);
			m_valid.mods.rgb = 1;
		}
		return m_rgb;
	}

	HsvColor Color::GetHSV()
	{
		if (m_valid.mods.hsv == 0)
		{
			// convert
			if (m_valid.mods.rgb)
			{
				double p = 0;
				double l = 0;
				GetHSPVL(m_rgb.m_ch1, m_rgb.m_ch2, m_rgb.m_ch3,
					m_hsv.m_ch1, m_hsv.m_ch2, p, m_hsv.m_ch3, l);
				m_valid.mods.hsv = 1;
			}
		}
		return m_hsv;
	}

};