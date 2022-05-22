#pragma once
class EllipticalArc
{
public:
	EllipticalArc(double cx, double cy, double a, double b, double theta, double lambda1, double lambda2, boolean isPieSlice);
	Platform::Collections::Vector<Windows::Foundation::Point>^ buildPointsList();
protected:
  /* Abscissa of the center of the ellipse. */
  double cx;

  /* Ordinate of the center of the ellipse. */
  double cy;

  /* Semi-major axis. */
  double a;

  /* Semi-minor axis. */
  double b;

  /* Orientation of the major axis with respect to the x axis. */
  double theta;

  /* Start angle of the arc. */
  double eta1;

  /* End angle of the arc. */
  double eta2;

  /* Abscissa of the start point. */
  //double x1;

  /* Ordinate of the start point. */
  //double y1;

  /* Abscissa of the end point. */
  //double x2;

  /* Ordinate of the end point. */
  //double y2;

  /* Abscissa of the first focus. */
  //double xF1;

  /* Ordinate of the first focus. */
  //double yF1;

  /* Abscissa of the second focus. */
  //double xF2;

  /* Ordinate of the second focus. */
  //double yF2;

  /*double f;
  double e2;
  double g;
  double g2;*/

  double estimateError(double etaA, double etaB);
private:

  const double twoPi;

  /* Orientation of the major axis with respect to the x axis. */
  double cosTheta;
  double sinTheta;

  /* Abscissa of the leftmost point of the arc. */
  //double xLeft;

  /* Ordinate of the highest point of the arc. */
  //double yUp;

  /* Horizontal width of the arc. */
  //double width;

  /* Vertical height of the arc. */
  //double height;

  /* Indicator for center to endpoints line inclusion. */
  boolean isPieSlice;

  /* Default flatness for Bézier curve approximation. */
  double defaultFlatness;

  //void computeFocii();
  /*void computeEndPoints();
  void computeBounds();*/
  //void computeDerivedFlatnessParameters();

  double rationalFunction(double x, double c[]);

  
};

