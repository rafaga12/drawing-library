#include "pch.h"
#include "EllipticalArc.h"

using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

/*EllipticalArc::EllipticalArc(void) : twoPi(2 * M_PI)
{
}


/*EllipticalArc::~EllipticalArc(void)
{
}*/

/* Build an elliptical arc from its canonical geometrical elements.
* @param cx abscissa of the center of the ellipse
* @param cy ordinate of the center of the ellipse
* @param a semi-major axis
* @param b semi-minor axis
* @param theta orientation of the major axis with respect to the x axis
* @param lambda1 start angle of the arc
* @param lambda2 end angle of the arc
* @param isPieSlice if true, the lines between the center of the ellipse
* and the endpoints are part of the shape (it is pie slice like)
*/

EllipticalArc::EllipticalArc(double cx, double cy, double a, double b, double theta, double lambda1, double lambda2, boolean isPieSlice) : twoPi(2 * M_PI)
{

    this->cx         = cx;
    this->cy         = cy;
    this->a          = a;
    this->b          = b;
    this->theta      = theta;
    this->isPieSlice = isPieSlice;
	
    eta1       = atan2(sin(lambda1) / b,
                            cos(lambda1) / a);
    eta2       = atan2(sin(lambda2) / b,
                            cos(lambda2) / a);
    cosTheta   = cos(theta);
    sinTheta   = sin(theta);
    defaultFlatness = 0.5; // half a pixel

    // make sure we have eta1 <= eta2 <= eta1 + 2 PI
    eta2 -= twoPi * floor((eta2 - eta1) / twoPi);

    // the preceding correction fails if we have exactly et2 - eta1 = 2 PI
    // it reduces the interval to zero length
    if ((lambda2 - lambda1 > M_PI) && (eta2 - eta1 < M_PI)) {
      eta2 += 2 * M_PI;
    }
	
    //computeFocii();
    //computeEndPoints();
    //computeBounds();
    //computeDerivedFlatnessParameters();
  }

/* Compute the locations of the focii. */
/*void EllipticalArc::computeFocii() {

    double d  = sqrt(a * a - b * b);
    double dx = d * cosTheta;
    double dy = d * sinTheta;

    xF1 = cx - dx;
    yF1 = cy - dy;
    xF2 = cx + dx;
    yF2 = cy + dy;

  }*/

/* Compute the locations of the endpoints. */
/*void EllipticalArc::computeEndPoints() {

    // start point
    double aCosEta1 = a * cos(eta1);
    double bSinEta1 = b * sin(eta1);
    x1 = cx + aCosEta1 * cosTheta - bSinEta1 * sinTheta;
    y1 = cy + aCosEta1 * sinTheta + bSinEta1 * cosTheta;

    // end point
    double aCosEta2 = a * cos(eta2);
    double bSinEta2 = b * sin(eta2);
    x2 = cx + aCosEta2 * cosTheta - bSinEta2 * sinTheta;
    y2 = cy + aCosEta2 * sinTheta + bSinEta2 * cosTheta;

  }*/

/* Compute the bounding box. */
/*void EllipticalArc::computeBounds() {

    double bOnA = b / a;
    double etaXMin, etaXMax, etaYMin, etaYMax;
    if (abs(sinTheta) < 0.1) {
      double tanTheta = sinTheta / cosTheta;
      if (cosTheta < 0) {
        etaXMin = -atan(tanTheta * bOnA);
        etaXMax = etaXMin + M_PI;
        etaYMin = 0.5 * M_PI - atan(tanTheta / bOnA);
        etaYMax = etaYMin + M_PI;
      } else {
        etaXMax = -atan(tanTheta * bOnA);
        etaXMin = etaXMax - M_PI;
        etaYMax = 0.5 * M_PI - atan(tanTheta / bOnA);
        etaYMin = etaYMax - M_PI;
      }
    } else {
      double invTanTheta = cosTheta / sinTheta;
      if (sinTheta < 0) {
        etaXMax = 0.5 * M_PI + atan(invTanTheta / bOnA);
        etaXMin = etaXMax - M_PI;
        etaYMin = atan(invTanTheta * bOnA);
        etaYMax = etaYMin + M_PI;
      } else {
        etaXMin = 0.5 * M_PI + atan(invTanTheta / bOnA);
        etaXMax = etaXMin + M_PI;
        etaYMax = atan(invTanTheta * bOnA);
        etaYMin = etaYMax - M_PI;
      }
    }

    etaXMin -= twoPi * floor((etaXMin - eta1) / twoPi);
    etaYMin -= twoPi * floor((etaYMin - eta1) / twoPi);
    etaXMax -= twoPi * floor((etaXMax - eta1) / twoPi);
    etaYMax -= twoPi * floor((etaYMax - eta1) / twoPi);

    xLeft = (etaXMin <= eta2)
      ? (cx + a * cos(etaXMin) * cosTheta - b * sin(etaXMin) * sinTheta)
      : min(x1, x2);
    yUp = (etaYMin <= eta2)
      ? (cy + a * cos(etaYMin) * sinTheta + b * sin(etaYMin) * cosTheta)
      : min(y1, y2);
    width = ((etaXMax <= eta2)
             ? (cx + a * cos(etaXMax) * cosTheta - b * sin(etaXMax) * sinTheta)
             : max(x1, x2)) - xLeft;
    height = ((etaYMax <= eta2)
              ? (cy + a * cos(etaYMax) * sinTheta + b * sin(etaYMax) * cosTheta)
              : max(y1, y2)) - yUp;

  }*/

/*void EllipticalArc::computeDerivedFlatnessParameters() {
    f   = (a - b) / a;
    e2  = f * (2.0 - f);
    g   = 1.0 - f;
    g2  = g * g;
  }*/

/* Estimate the approximation error for a sub-arc of the instance.
* @param degree degree of the Bézier curve to use (1, 2 or 3)
* @param tA start angle of the sub-arc
* @param tB end angle of the sub-arc
* @return upper bound of the approximation error between the Bézier
* curve and the real ellipse
*/
double EllipticalArc::estimateError(double etaA, double etaB) {

	// coefficients for error estimation
	// while using cubic Bézier curves for approximation
	// 0 < b/a < 1/4
	double coeffs3Low[2][4][4] = 
	{
		{
			{  3.85268,   -21.229,      -0.330434,    0.0127842  },
			{ -1.61486,     0.706564,    0.225945,    0.263682   },
			{ -0.910164,    0.388383,    0.00551445,  0.00671814 },
			{ -0.630184,    0.192402,    0.0098871,   0.0102527  }
		}, {
			{ -0.162211,    9.94329,     0.13723,     0.0124084  },
			{ -0.253135,    0.00187735,  0.0230286,   0.01264    },
			{ -0.0695069,  -0.0437594,   0.0120636,   0.0163087  },
			{ -0.0328856,  -0.00926032, -0.00173573,  0.00527385 }
		}
	};

	// coefficients for error estimation
	// while using cubic Bézier curves for approximation
	// 1/4 <= b/a <= 1
	double coeffs3High[2][4][4] = 
	{
		{
			{  0.0899116, -19.2349,     -4.11711,     0.183362   },
			{  0.138148,   -1.45804,     1.32044,     1.38474    },
			{  0.230903,   -0.450262,    0.219963,    0.414038   },
			{  0.0590565,  -0.101062,    0.0430592,   0.0204699  }
		}, {
			{  0.0164649,   9.89394,     0.0919496,   0.00760802 },
			{  0.0191603,  -0.0322058,   0.0134667,  -0.0825018  },
			{  0.0156192,  -0.017535,    0.00326508, -0.228157   },
			{ -0.0236752,   0.0405821,  -0.0173086,   0.176187   }
		}
	};

	// safety factor to convert the "best" error approximation
	// into a "max bound" error
	double safety[4] = {0.001, 4.98, 0.207, 0.0067};


    double eta  = 0.5 * (etaA + etaB);

    double x    = b / a;
    double dEta = etaB - etaA;
    double cos2 = cos(2 * eta);
    double cos4 = cos(4 * eta);
    double cos6 = cos(6 * eta);

    // select the right coeficients set according to b/a
    double coeffs[2][4][4];
    
	if(x < 0.25)
	{
		memcpy(coeffs, coeffs3Low, 2 * 4 * 4 * sizeof(double));
	}else{
		memcpy(coeffs, coeffs3High, 2 * 4 * 4 * sizeof(double));
	}

    double c0 = rationalFunction(x, coeffs[0][0])
        + cos2 * rationalFunction(x, coeffs[0][1])
        + cos4 * rationalFunction(x, coeffs[0][2])
        + cos6 * rationalFunction(x, coeffs[0][3]);

    double c1 = rationalFunction(x, coeffs[1][0])
        + cos2 * rationalFunction(x, coeffs[1][1])
        + cos4 * rationalFunction(x, coeffs[1][2])
        + cos6 * rationalFunction(x, coeffs[1][3]);

    return rationalFunction(x, safety) * a * exp(c0 + c1 * dEta);
  }

/* Compute the value of a rational function.
* This method handles rational functions where the numerator is
* quadratic and the denominator is linear
* @param x absissa for which the value should be computed
* @param c coefficients array of the rational function
*/

double EllipticalArc::rationalFunction(double x, double c[]) {
    return (x * (x * c[0] + c[1]) + c[2]) / (x + c[3]);
  }

/* Build an approximation of the instance outline.
* @param degree degree of the Bézier curve to use
* @param threshold acceptable error
* @param at affine transformation to apply
* @return a path iterator
*/

Vector<Point>^ EllipticalArc::buildPointsList() {
	// find the number of Bézier curves needed
    boolean found = false;
    int n = 1;
    while ((! found) && (n < 1024)) {
	  double dEta = (eta2 - eta1) / n;
      if (dEta <= 0.5 * M_PI) {
        double etaB = eta1;
        found = true;
        for (int i = 0; found && (i < n); ++i) {
          double etaA = etaB;
          etaB += dEta;
          found = (estimateError(etaA, etaB) <= defaultFlatness);
        }
      }
      n = n << 1;
    }

    
    double dEta = (eta2 - eta1) / n;
    double etaB = eta1;

    double cosEtaB  = cos(etaB);
    double sinEtaB  = sin(etaB);
    double aCosEtaB = a * cosEtaB;
    double bSinEtaB = b * sinEtaB;
    double aSinEtaB = a * sinEtaB;
    double bCosEtaB = b * cosEtaB;
    double xB       = cx + aCosEtaB * cosTheta - bSinEtaB * sinTheta;
    double yB       = cy + aCosEtaB * sinTheta + bSinEtaB * cosTheta;
    double xBDot    = -aSinEtaB * cosTheta - bCosEtaB * sinTheta;
    double yBDot    = -aSinEtaB * sinTheta + bCosEtaB * cosTheta;

    /*if (isPieSlice) {
      path.moveTo((float) cx, (float) cy);
      path.lineTo((float) xB, (float) yB);
    } else {
      path.moveTo((float) xB, (float) yB);
    }*/

    double t     = tan(0.5 * dEta);
    double alpha = sin(dEta) * (sqrt(4 + 3 * t * t) - 1) / 3;

	Vector<Point>^ points = ref new Vector<Point>();

    for (int i = 0; i < n; ++i) {

      double etaA  = etaB;
      double xA    = xB;
      double yA    = yB;
      double xADot = xBDot;
      double yADot = yBDot;

      etaB    += dEta;
      cosEtaB  = cos(etaB);
      sinEtaB  = sin(etaB);
      aCosEtaB = a * cosEtaB;
      bSinEtaB = b * sinEtaB;
      aSinEtaB = a * sinEtaB;
      bCosEtaB = b * cosEtaB;
      xB       = cx + aCosEtaB * cosTheta - bSinEtaB * sinTheta;
      yB       = cy + aCosEtaB * sinTheta + bSinEtaB * cosTheta;
      xBDot    = -aSinEtaB * cosTheta - bCosEtaB * sinTheta;
      yBDot    = -aSinEtaB * sinTheta + bCosEtaB * cosTheta;

	  if(i == 0)
	  {
		  points->Append(Point(static_cast<float>(xA), static_cast<float>(yA)));
	  }
	  points->Append(Point(static_cast<float>(xA + alpha * xADot), static_cast<float>(yA + alpha * yADot)));
	  points->Append(Point(static_cast<float>(xB - alpha * xBDot), static_cast<float>(yB - alpha * yBDot)));
	  points->Append(Point(static_cast<float>(xB), static_cast<float>(yB)));

    }

    /*if (isPieSlice) {
      path.closePath();
    }*/

    //return path.getPathIterator(at);
	return points;

  }