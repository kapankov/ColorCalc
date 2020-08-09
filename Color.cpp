#include "Color.h"

#include <algorithm>
#include <cmath>

namespace COLORNS
{

	double GetLuminance(const double r, const double g, const double b)
	{
		// BT.601 (NTSC 1953)
		constexpr double  Pr = .299;
		constexpr double  Pg = .587;
		constexpr double  Pb = .114;

		// should use these parameters
		// why? see https://habr.com/ru/post/304210/ (rus)
		// BT.709 (Rec.709)
/*		constexpr double  Pr = .2126;
		constexpr double  Pg = .7152;
		constexpr double  Pb = .0722;*/

		return sqrt(r * r * Pr + g * g * Pg + b * b * Pb);
	}

	void GetHSPVL(const double r, const double g, const double b,
		double& h, double& s, double& p, double& v, double& l)
	{
		double max = std::max(std::max(r, g), b);
		double min = std::min(std::min(r, g), b);
		double d = max - min;
		h = 0;
		s = max == 0 ? 0 : (d / max);
		p = GetLuminance(r, g, b);
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

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// brucelindblum.com CIE Color Calculator C++ porting
	typedef struct _XYZ
	{
		double X{ 0.0 };
		double Y{ 0.0 };
		double Z{ 0.0 };
	} XYZ;

	enum class AdaptationEnum
	{
		amBradford = 0,
		amVonKries = 1,
//		amXYZScaling = 2,
		amNone = 2
	};

	enum class RgbEnum
	{
		AdobeRgb = 0,
		AppleRgb = 1,
		BestRgb = 2,
		BetaRgb = 3,
		BruceRgb = 4,
		CieRgb = 5,
		ColorMatchRgb = 6,
		DonRgb4 = 7,
		EciRgb2 = 8,
		EktaSpacePS5 = 9,
		NtscRgb = 10,
		PalSecamRgb = 11,
		ProPhotoRgb = 12,
		SmpteCRgb = 13,
		sRGB = 14,
		WideGamutRgb = 15
	};

	enum class IlluminantEnum
	{
		A = 0,
		B = 1,
		C = 2,
		D50 = 3,
		D55 = 4,
		D65 = 5,
		D75 = 6,
		E = 7,
		F2 = 8,
		F7 = 9,
		F11 = 10
	};

	typedef struct _Mtx3x3
	{
		double m[3][3];
	} Mtx3x3;

	typedef struct _RgbModel
	{
		XYZ RefWhiteRGB;
		double GammaRGB;
		Mtx3x3 MtxRGB2XYZ;
		Mtx3x3 MtxXYZ2RGB;
	} RgbModel;

	XYZ GetRefWhite(IlluminantEnum i = IlluminantEnum::D50)
	{
		XYZ RefWhite;
		RefWhite.Y = 1.0;
		switch (i)
		{
		case IlluminantEnum::A:	// A (ASTM E308-01)
			RefWhite.X = 1.09850;
			RefWhite.Z = 0.35585;
			break;
		case IlluminantEnum::B:	// B (Wyszecki & Stiles, p. 769)
			RefWhite.X = 0.99072;
			RefWhite.Z = 0.85223;
			break;
		case IlluminantEnum::C:	// C (ASTM E308-01)
			RefWhite.X = 0.98074;
			RefWhite.Z = 1.18232;
			break;
		case IlluminantEnum::D50:	// D50 (ASTM E308-01)
			RefWhite.X = 0.96422;
			RefWhite.Z = 0.82521;
			break;
		case IlluminantEnum::D55:	// D55 (ASTM E308-01)
			RefWhite.X = 0.95682;
			RefWhite.Z = 0.92149;
			break;
		case IlluminantEnum::D65:	// D65 (ASTM E308-01)
			RefWhite.X = 0.95047;
			RefWhite.Z = 1.08883;
			break;
		case IlluminantEnum::D75:	// D75 (ASTM E308-01)
			RefWhite.X = 0.94972;
			RefWhite.Z = 1.22638;
			break;
		default:
		case IlluminantEnum::E:	// E (ASTM E308-01)
			RefWhite.X = 1.00000;
			RefWhite.Z = 1.00000;
			break;
		case IlluminantEnum::F2:	// F2 (ASTM E308-01)
			RefWhite.X = 0.99186;
			RefWhite.Z = 0.67393;
			break;
		case IlluminantEnum::F7:	// F7 (ASTM E308-01)
			RefWhite.X = 0.95041;
			RefWhite.Z = 1.08747;
			break;
		case IlluminantEnum::F11:	// F11 (ASTM E308-01)
			RefWhite.X = 1.00962;
			RefWhite.Z = 0.64350;
			break;
		}
		return RefWhite;
	}

	double Determinant3x3(const Mtx3x3& m)
	{
		double det = m.m[0][0] * (m.m[2][2] * m.m[1][1] - m.m[2][1] * m.m[1][2]) -
			m.m[1][0] * (m.m[2][2] * m.m[0][1] - m.m[2][1] * m.m[0][2]) +
			m.m[2][0] * (m.m[1][2] * m.m[0][1] - m.m[1][1] * m.m[0][2]);

		return (det);
	}

	void MtxInvert3x3(const Mtx3x3& m, Mtx3x3& i)
	{
		double scale = 1.0 / Determinant3x3(m);

		i.m[0][0] = scale * (m.m[2][2] * m.m[1][1] - m.m[2][1] * m.m[1][2]);
		i.m[0][1] = -scale * (m.m[2][2] * m.m[0][1] - m.m[2][1] * m.m[0][2]);
		i.m[0][2] = scale * (m.m[1][2] * m.m[0][1] - m.m[1][1] * m.m[0][2]);

		i.m[1][0] = -scale * (m.m[2][2] * m.m[1][0] - m.m[2][0] * m.m[1][2]);
		i.m[1][1] = scale * (m.m[2][2] * m.m[0][0] - m.m[2][0] * m.m[0][2]);
		i.m[1][2] = -scale * (m.m[1][2] * m.m[0][0] - m.m[1][0] * m.m[0][2]);

		i.m[2][0] = scale * (m.m[2][1] * m.m[1][0] - m.m[2][0] * m.m[1][1]);
		i.m[2][1] = -scale * (m.m[2][1] * m.m[0][0] - m.m[2][0] * m.m[0][1]);
		i.m[2][2] = scale * (m.m[1][1] * m.m[0][0] - m.m[1][0] * m.m[0][1]);
	}

	void MtxTranspose3x3(Mtx3x3& m)
	{
		double v = m.m[0][1];
		m.m[0][1] = m.m[1][0];
		m.m[1][0] = v;

		v = m.m[0][2];
		m.m[0][2] = m.m[2][0];
		m.m[2][0] = v;

		v = m.m[1][2];
		m.m[1][2] = m.m[2][1];
		m.m[2][1] = v;
	}

	RgbModel GetRGBModel(RgbEnum Model = RgbEnum::sRGB)
	{
		RgbModel result;
		result.RefWhiteRGB.Y = 1.00000;
		double xr, yr, xg, yg, xb, yb;

		switch (Model)
		{
		case RgbEnum::AdobeRgb:	/* Adobe RGB (1998) */
			xr = 0.64;
			yr = 0.33;
			xg = 0.21;
			yg = 0.71;
			xb = 0.15;
			yb = 0.06;

			result.RefWhiteRGB.X = 0.95047;
			result.RefWhiteRGB.Z = 1.08883;

			result.GammaRGB = 2.2;
			break;
		case RgbEnum::AppleRgb:	/* AppleRGB */
			xr = 0.625;
			yr = 0.340;
			xg = 0.280;
			yg = 0.595;
			xb = 0.155;
			yb = 0.070;

			result.RefWhiteRGB.X = 0.95047;
			result.RefWhiteRGB.Z = 1.08883;

			result.GammaRGB = 1.8;
			break;
		case RgbEnum::BestRgb:	/* Best RGB */
			xr = 0.7347;
			yr = 0.2653;
			xg = 0.2150;
			yg = 0.7750;
			xb = 0.1300;
			yb = 0.0350;

			result.RefWhiteRGB.X = 0.96422;
			result.RefWhiteRGB.Z = 0.82521;

			result.GammaRGB = 2.2;
			break;
		case RgbEnum::BetaRgb:	/* Beta RGB */
			xr = 0.6888;
			yr = 0.3112;
			xg = 0.1986;
			yg = 0.7551;
			xb = 0.1265;
			yb = 0.0352;

			result.RefWhiteRGB.X = 0.96422;
			result.RefWhiteRGB.Z = 0.82521;

			result.GammaRGB = 2.2;
			break;
		case RgbEnum::BruceRgb:	/* Bruce RGB */
			xr = 0.64;
			yr = 0.33;
			xg = 0.28;
			yg = 0.65;
			xb = 0.15;
			yb = 0.06;

			result.RefWhiteRGB.X = 0.95047;
			result.RefWhiteRGB.Z = 1.08883;

			result.GammaRGB = 2.2;
			break;
		case RgbEnum::CieRgb:	/* CIE RGB */
			xr = 0.735;
			yr = 0.265;
			xg = 0.274;
			yg = 0.717;
			xb = 0.167;
			yb = 0.009;

			result.RefWhiteRGB.X = 1.00000;
			result.RefWhiteRGB.Z = 1.00000;

			result.GammaRGB = 2.2;
			break;
		case RgbEnum::ColorMatchRgb:	/* ColorMatch RGB */
			xr = 0.630;
			yr = 0.340;
			xg = 0.295;
			yg = 0.605;
			xb = 0.150;
			yb = 0.075;

			result.RefWhiteRGB.X = 0.96422;
			result.RefWhiteRGB.Z = 0.82521;

			result.GammaRGB = 1.8;
			break;
		case RgbEnum::DonRgb4:	/* Don RGB 4 */
			xr = 0.696;
			yr = 0.300;
			xg = 0.215;
			yg = 0.765;
			xb = 0.130;
			yb = 0.035;

			result.RefWhiteRGB.X = 0.96422;
			result.RefWhiteRGB.Z = 0.82521;

			result.GammaRGB = 2.2;
			break;
		case RgbEnum::EciRgb2:	/* ECI RGB v2 */
			xr = 0.67;
			yr = 0.33;
			xg = 0.21;
			yg = 0.71;
			xb = 0.14;
			yb = 0.08;

			result.RefWhiteRGB.X = 0.96422;
			result.RefWhiteRGB.Z = 0.82521;

			result.GammaRGB = 0.0;
			break;
		case RgbEnum::EktaSpacePS5:	/* Ekta Space PS5 */
			xr = 0.695;
			yr = 0.305;
			xg = 0.260;
			yg = 0.700;
			xb = 0.110;
			yb = 0.005;

			result.RefWhiteRGB.X = 0.96422;
			result.RefWhiteRGB.Z = 0.82521;

			result.GammaRGB = 2.2;
			break;
		case RgbEnum::NtscRgb:	/* NTSC RGB */
			xr = 0.67;
			yr = 0.33;
			xg = 0.21;
			yg = 0.71;
			xb = 0.14;
			yb = 0.08;

			result.RefWhiteRGB.X = 0.98074;
			result.RefWhiteRGB.Z = 1.18232;

			result.GammaRGB = 2.2;
			break;
		case RgbEnum::PalSecamRgb:	/* PAL/SECAM RGB */
			xr = 0.64;
			yr = 0.33;
			xg = 0.29;
			yg = 0.60;
			xb = 0.15;
			yb = 0.06;

			result.RefWhiteRGB.X = 0.95047;
			result.RefWhiteRGB.Z = 1.08883;

			result.GammaRGB = 2.2;
			break;
		case RgbEnum::ProPhotoRgb:	/* ProPhoto RGB */
			xr = 0.7347;
			yr = 0.2653;
			xg = 0.1596;
			yg = 0.8404;
			xb = 0.0366;
			yb = 0.0001;

			result.RefWhiteRGB.X = 0.96422;
			result.RefWhiteRGB.Z = 0.82521;

			result.GammaRGB = 1.8;
			break;
		case RgbEnum::SmpteCRgb:	/* SMPTE-C RGB */
			xr = 0.630;
			yr = 0.340;
			xg = 0.310;
			yg = 0.595;
			xb = 0.155;
			yb = 0.070;

			result.RefWhiteRGB.X = 0.95047;
			result.RefWhiteRGB.Z = 1.08883;

			result.GammaRGB = 2.2;
			break;
		case RgbEnum::sRGB:	/* sRGB */
			xr = 0.64;
			yr = 0.33;
			xg = 0.30;
			yg = 0.60;
			xb = 0.15;
			yb = 0.06;

			result.RefWhiteRGB.X = 0.95047;
			result.RefWhiteRGB.Z = 1.08883;

			result.GammaRGB = -2.2;
			break;
		case RgbEnum::WideGamutRgb:	/* Wide Gamut RGB */
			xr = 0.735;
			yr = 0.265;
			xg = 0.115;
			yg = 0.826;
			xb = 0.157;
			yb = 0.018;

			result.RefWhiteRGB.X = 0.96422;
			result.RefWhiteRGB.Z = 0.82521;

			result.GammaRGB = 2.2;
			break;
		}

		Mtx3x3 m = { { {xr / yr, xg / yg, xb / yb}, {1.0, 1.0, 1.0}, {(1.0 - xr - yr) / yr, (1.0 - xg - yg) / yg, (1.0 - xb - yb) / yb} } };
		Mtx3x3 mi = { { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0} } };
		MtxInvert3x3(m, mi);

		double sr = result.RefWhiteRGB.X * mi.m[0][0] + result.RefWhiteRGB.Y * mi.m[0][1] + result.RefWhiteRGB.Z * mi.m[0][2];
		double sg = result.RefWhiteRGB.X * mi.m[1][0] + result.RefWhiteRGB.Y * mi.m[1][1] + result.RefWhiteRGB.Z * mi.m[1][2];
		double sb = result.RefWhiteRGB.X * mi.m[2][0] + result.RefWhiteRGB.Y * mi.m[2][1] + result.RefWhiteRGB.Z * mi.m[2][2];

		result.MtxRGB2XYZ.m[0][0] = sr * m.m[0][0];
		result.MtxRGB2XYZ.m[0][1] = sg * m.m[0][1];
		result.MtxRGB2XYZ.m[0][2] = sb * m.m[0][2];
		result.MtxRGB2XYZ.m[1][0] = sr * m.m[1][0];
		result.MtxRGB2XYZ.m[1][1] = sg * m.m[1][1];
		result.MtxRGB2XYZ.m[1][2] = sb * m.m[1][2];
		result.MtxRGB2XYZ.m[2][0] = sr * m.m[2][0];
		result.MtxRGB2XYZ.m[2][1] = sg * m.m[2][1];
		result.MtxRGB2XYZ.m[2][2] = sb * m.m[2][2];

		MtxTranspose3x3(result.MtxRGB2XYZ);

		MtxInvert3x3(result.MtxRGB2XYZ, result.MtxXYZ2RGB);

		return result;
	}

	void GetAdaptation(AdaptationEnum Method, Mtx3x3& MtxAdaptMa, Mtx3x3& MtxAdaptMaI)
	{
		switch (Method)
		{
		case AdaptationEnum::amBradford:	/* Bradford */
			MtxAdaptMa.m[0][0] = 0.8951;
			MtxAdaptMa.m[0][1] = -0.7502;
			MtxAdaptMa.m[0][2] = 0.0389;
			MtxAdaptMa.m[1][0] = 0.2664;
			MtxAdaptMa.m[1][1] = 1.7135;
			MtxAdaptMa.m[1][2] = -0.0685;
			MtxAdaptMa.m[2][0] = -0.1614;
			MtxAdaptMa.m[2][1] = 0.0367;
			MtxAdaptMa.m[2][2] = 1.0296;

			MtxInvert3x3(MtxAdaptMa, MtxAdaptMaI);
			break;
		case AdaptationEnum::amVonKries:	/* von Kries */
			MtxAdaptMa.m[0][0] = 0.40024;
			MtxAdaptMa.m[0][1] = -0.22630;
			MtxAdaptMa.m[0][2] = 0.00000;
			MtxAdaptMa.m[1][0] = 0.70760;
			MtxAdaptMa.m[1][1] = 1.16532;
			MtxAdaptMa.m[1][2] = 0.00000;
			MtxAdaptMa.m[2][0] = -0.08081;
			MtxAdaptMa.m[2][1] = 0.04570;
			MtxAdaptMa.m[2][2] = 0.91822;

			MtxInvert3x3(MtxAdaptMa, MtxAdaptMaI);
			break;
			//		case AdaptationEnum::amXYZScaling:	/* XYZ Scaling */
		case AdaptationEnum::amNone:	/* None */
			MtxAdaptMa.m[0][0] = 1.0;
			MtxAdaptMa.m[0][1] = 0.0;
			MtxAdaptMa.m[0][2] = 0.0;
			MtxAdaptMa.m[1][0] = 0.0;
			MtxAdaptMa.m[1][1] = 1.0;
			MtxAdaptMa.m[1][2] = 0.0;
			MtxAdaptMa.m[2][0] = 0.0;
			MtxAdaptMa.m[2][1] = 0.0;
			MtxAdaptMa.m[2][2] = 1.0;

			MtxAdaptMaI.m[0][0] = 1.0;
			MtxAdaptMaI.m[0][1] = 0.0;
			MtxAdaptMaI.m[0][2] = 0.0;
			MtxAdaptMaI.m[1][0] = 0.0;
			MtxAdaptMaI.m[1][1] = 1.0;
			MtxAdaptMaI.m[1][2] = 0.0;
			MtxAdaptMaI.m[2][0] = 0.0;
			MtxAdaptMaI.m[2][1] = 0.0;
			MtxAdaptMaI.m[2][2] = 1.0;
			break;
		}
	}

	double InvCompand(double companded, const double gamma)
	{
		double linear;
		if (gamma > 0.0)
		{
			linear = (companded >= 0.0) ? pow(companded, gamma) : -pow(-companded, gamma);
		}
		else if (gamma < 0.0)
		{
			/* sRGB */
			double sign = 1.0;
			if (companded < 0.0)
			{
				sign = -1.0;
				companded = -companded;
			}
			linear = (companded <= 0.04045) ? (companded / 12.92) : pow((companded + 0.055) / 1.055, 2.4);
			linear *= sign;
		}
		else
		{
			/* L* */
			double sign = 1.0;
			if (companded < 0.0)
			{
				sign = -1.0;
				companded = -companded;
			}
			linear = (companded <= 0.08) ? (2700.0 * companded / 24389.0) : ((((1000000.0 * companded + 480000.0) * companded + 76800.0) * companded + 4096.0) / 1560896.0);
			linear *= sign;
		}
		return(linear);
	}

	// RGB to XYZ
	// http://brucelindbloom.com/index.html?Eqn_RGB_to_XYZ.html
	void RGB2XYZ(const double r, const double g, const double b, 
		const double gamma, const RgbModel& model,
		double& x, double& y, double& z, const XYZ& RefWhite,
		const AdaptationEnum Method = AdaptationEnum::amBradford)
	{
		Mtx3x3 MtxAdaptMa;
		Mtx3x3 MtxAdaptMaI;
		GetAdaptation(Method, MtxAdaptMa, MtxAdaptMaI);

		// Inverse Gamma Companding
		double R = InvCompand(r, gamma);
		double G = InvCompand(g, gamma);
		double B = InvCompand(b, gamma);

		// Linear RGB to XYZ
		x = R * model.MtxRGB2XYZ.m[0][0] + G * model.MtxRGB2XYZ.m[1][0] + B * model.MtxRGB2XYZ.m[2][0];
		y = R * model.MtxRGB2XYZ.m[0][1] + G * model.MtxRGB2XYZ.m[1][1] + B * model.MtxRGB2XYZ.m[2][1];
		z = R * model.MtxRGB2XYZ.m[0][2] + G * model.MtxRGB2XYZ.m[1][2] + B * model.MtxRGB2XYZ.m[2][2];

		// Chromatic Adaptation
		if (Method != AdaptationEnum::amNone)
		{
			double Ad = RefWhite.X * MtxAdaptMa.m[0][0] + RefWhite.Y * MtxAdaptMa.m[1][0] + RefWhite.Z * MtxAdaptMa.m[2][0];
			double Bd = RefWhite.X * MtxAdaptMa.m[0][1] + RefWhite.Y * MtxAdaptMa.m[1][1] + RefWhite.Z * MtxAdaptMa.m[2][1];
			double Cd = RefWhite.X * MtxAdaptMa.m[0][2] + RefWhite.Y * MtxAdaptMa.m[1][2] + RefWhite.Z * MtxAdaptMa.m[2][2];

			double As = model.RefWhiteRGB.X * MtxAdaptMa.m[0][0] + model.RefWhiteRGB.Y * MtxAdaptMa.m[1][0] + model.RefWhiteRGB.Z * MtxAdaptMa.m[2][0];
			double Bs = model.RefWhiteRGB.X * MtxAdaptMa.m[0][1] + model.RefWhiteRGB.Y * MtxAdaptMa.m[1][1] + model.RefWhiteRGB.Z * MtxAdaptMa.m[2][1];
			double Cs = model.RefWhiteRGB.X * MtxAdaptMa.m[0][2] + model.RefWhiteRGB.Y * MtxAdaptMa.m[1][2] + model.RefWhiteRGB.Z * MtxAdaptMa.m[2][2];

			double X = x * MtxAdaptMa.m[0][0] + y * MtxAdaptMa.m[1][0] + z * MtxAdaptMa.m[2][0];
			double Y = x * MtxAdaptMa.m[0][1] + y * MtxAdaptMa.m[1][1] + z * MtxAdaptMa.m[2][1];
			double Z = x * MtxAdaptMa.m[0][2] + y * MtxAdaptMa.m[1][2] + z * MtxAdaptMa.m[2][2];

			X *= (Ad / As);
			Y *= (Bd / Bs);
			Z *= (Cd / Cs);

			x = X * MtxAdaptMaI.m[0][0] + Y * MtxAdaptMaI.m[1][0] + Z * MtxAdaptMaI.m[2][0];
			y = X * MtxAdaptMaI.m[0][1] + Y * MtxAdaptMaI.m[1][1] + Z * MtxAdaptMaI.m[2][1];
			z = X * MtxAdaptMaI.m[0][2] + Y * MtxAdaptMaI.m[1][2] + Z * MtxAdaptMaI.m[2][2];
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	XyzColor::XyzColor(double X, double Y, double Z) :
		channels(X, Y, Z)
	{}

	XyzColor::XyzColor(const XyzColor& xyz) :
		channels(xyz.m_ch1, xyz.m_ch2, xyz.m_ch3)
	{}

	double XyzColor::GetX() const noexcept
	{
		return m_ch1;
	}

	double XyzColor::GetY() const noexcept
	{
		return m_ch2;
	}

	double XyzColor::GetZ() const noexcept
	{
		return m_ch3;
	}

	XyzColor& XyzColor::operator=(const XyzColor& xyz)
	{
		m_ch1 = xyz.m_ch1;
		m_ch2 = xyz.m_ch2;
		m_ch3 = xyz.m_ch3;
		return *this;
	}

	std::ostream& operator<<(std::ostream& out, const XyzColor& xyz)
	{
		out << "xyz(" << xyz.m_ch1 << ", " << xyz.m_ch2 << ", " << xyz.m_ch3 << ")";
		return out;
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

	double RgbColor::GetGamma() const noexcept
	{
		return m_gamma;
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
		channels(hsv.m_ch1, hsv.m_ch2, hsv.m_ch3), 
		m_lightness(hsv.m_lightness), 
		m_luminance(hsv.m_luminance)
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

	double HsvColor::GetBrightness() const noexcept
	{
		return GetValue();
	}

	double HsvColor::GetLightness() noexcept
	{
		if (m_lightness < 0)
		{
			double r = 0;
			double g = 0;
			double b = 0;
			GetRGBfromHSV(m_ch1, m_ch2, m_ch3,
				r, g, b);
			double max = std::max(std::max(r, g), b);
			double min = std::min(std::min(r, g), b);
			m_lightness = (min + max) / 2;
		}
		return m_lightness;
	}

	double HsvColor::GetLuminance() noexcept
	{
		if (m_luminance < 0)
		{
			double r = 0;
			double g = 0;
			double b = 0;
			GetRGBfromHSV(m_ch1, m_ch2, m_ch3,
				r, g, b);
			m_luminance = COLORNS::GetLuminance(r, g, b);
		}
		return m_luminance;
	}

	HsvColor& HsvColor::operator= (const HsvColor& hsv)
	{
		m_ch1 = hsv.m_ch1;
		m_ch2 = hsv.m_ch2;
		m_ch3 = hsv.m_ch3;
		m_lightness = hsv.m_lightness;
		m_luminance = hsv.m_luminance;
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
	XyzColor Color::GetXYZ()
	{
		if (m_valid.mods.rgb == 1)
		{
			RGB2XYZ(m_rgb.m_ch1, m_rgb.m_ch2, m_rgb.m_ch3,
				m_rgb.m_gamma, GetRGBModel(),
				m_xyz.m_ch1, m_xyz.m_ch2, m_xyz.m_ch3, GetRefWhite());
		}
		return m_xyz;
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
				GetHSPVL(m_rgb.m_ch1, m_rgb.m_ch2, m_rgb.m_ch3,
					m_hsv.m_ch1, m_hsv.m_ch2, m_hsv.m_luminance, m_hsv.m_ch3, m_hsv.m_lightness);
				m_valid.mods.hsv = 1;
			}
		}
		return m_hsv;
	}

};