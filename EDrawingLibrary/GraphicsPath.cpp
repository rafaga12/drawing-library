#include "pch.h"
#include "GraphicsPath.h"
#include "Utilities\EllipticalArc.h"

using namespace ModernComponents::System::Drawing::Drawing2D;

using namespace Windows::Foundation;
using namespace Platform;
using namespace Platform::Collections;

#pragma region Private

class GraphicsPath::Private
{
public:
	Drawing2D::PathData^ pathData;
};

#pragma endregion Private

/** <summary>
        Initializes a new instance of the GraphicsPath class.
    </summary>
    <example>
        <code language="cs" source="..\EDrawingLibraryTest\MainPage2.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage2.xaml.vb" region="Usage"/>
    </example>
*/
GraphicsPath::GraphicsPath() : impl(new Private())
{
	impl->pathData = ref new Drawing2D::PathData();
}

/** <summary>
        Initializes a new instance of the GraphicsPath class with the specified points and point types.
    </summary>
	<param name="__param0">
        <para>An array of points that defines the coordinates of the points that make up this <see cref="GraphicsPath"/>.</para>
    </param>
    <param name="__param1">
        <para>An array of <see cref="PathPointType"/> elements that specifies the type of each corresponding point in the pts array.</para>
    </param>
    <example>
        <code language="cs" source="..\EDrawingLibraryTest\MainPage2.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage2.xaml.vb" region="Usage"/>
    </example>
*/
GraphicsPath::GraphicsPath(const Platform::Array<Point>^ pts, const Platform::Array<byte>^ types) : impl(new Private())
{
	impl->pathData = ref new Drawing2D::PathData();
	if(pts->Length == types->Length)
	{
		if(types->Length > 0)
		{
			unsigned int i = 0;
			for(i; i < types->Length; i++)
			{
				PathPointType CurrentType = (PathPointType)types[i];
				if(i == 0)
				{
					CurrentType  = PathPointType::Start;
				}
				if(CurrentType == PathPointType::Bezier)
				{
					if(i + 2 < types->Length)
					{
						if(types[i+1] == (byte)PathPointType::Bezier && types[i+2] == (byte)PathPointType::Bezier)
						{ 
							impl->pathData->addPoint(pts[i], CurrentType);
							impl->pathData->addPoint(pts[i+1], CurrentType);
							impl->pathData->addPoint(pts[i+2], CurrentType);
							i += 2;
							continue;
						}
					}
					break;
				}else{
					impl->pathData->addPoint(pts[i], CurrentType);
				}
			}
		}
	}else{
		DX::ThrowIfFailed(STATUS_ACCESS_VIOLATION);
	}
}

GraphicsPath::~GraphicsPath()
{
}

PathData^ GraphicsPath::PathData::get()
{
	return impl->pathData;
}

/** <summary>
		Appends the specified <see cref="GraphicsPath"/> to this path.
	</summary>
	<param name="addingPath">
		<para>The <see cref="GraphicsPath"/> to add.</para>
	</param>
	<param name="connect">
		<para>A Boolean value that specifies whether the first figure in the added path is part of the 
		last figure in this path.</para>
	</param>
	<remarks>
		<para>
			If the current path is open, the connect argument makes the last point from the current path 
			to connect with the first point of the added path.
		</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage4.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage4.xaml.vb" region="Usage"/>
	</example>
*/
void GraphicsPath::AddPath(GraphicsPath^ addingPath, bool connect)
{
	for (unsigned int i = 0; i < addingPath->PathData->Points->Length; i++)
	{
		if (impl->pathData->isFirstPoint())
		{
			impl->pathData->addPoint(addingPath->PathData->Points[i], PathPointType::Start);
		}
		else
		{
			if (i == 0)
			{
				if (connect)
				{
					if ((impl->pathData->Types[impl->pathData->Types->Length - 1] & (byte)PathPointType::CloseSubpath) != (byte)PathPointType::CloseSubpath)
					{
						impl->pathData->addPoint(addingPath->PathData->Points[i], PathPointType::Line);
					}
					else
					{
						impl->pathData->addPoint(addingPath->PathData->Points[i], PathPointType::Start);
					}
				}
				else
				{
					impl->pathData->addPoint(addingPath->PathData->Points[i], PathPointType::Start);
				}
			}
			else
			{
				impl->pathData->addPoint(addingPath->PathData->Points[i], (PathPointType)addingPath->PathData->Types[i]);
			}
		}
	}
	
}

/** <summary>
        Appends a line segment to this <see cref="GraphicsPath"/>.
    </summary>
    <param name="pt1">
        <para>A <see cref="Point"/> that represents the starting point of the line.</para>
    </param>
    <param name="pt2">
        <para>A <see cref="Point"/> that represents the end point of the line.</para>
    </param>
    <remarks>
        <para>This method adds a line connecting the two points specified to the end of this <see cref="GraphicsPath"/>. If there are 
		previous lines or curves, a line segment is added to connect the last point to the first point of the new line segment.</para>
    </remarks>
    <example>
        <code language="cs" source="..\EDrawingLibraryTest\MainPage2.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage2.xaml.vb" region="Usage"/>
    </example>
*/
void GraphicsPath::AddLine(Point pt1, Point pt2)
{
	if(impl->pathData->isFirstPoint())
	{
		impl->pathData->addPoint(pt1, PathPointType::Start);
	}else{
		impl->pathData->addPoint(pt1, PathPointType::Line);
	}
	impl->pathData->addPoint(pt2, PathPointType::Line);
}

/** <summary>
        Appends a line segment to this <see cref="GraphicsPath"/>.
    </summary>
    <param name="x1">
        <para>The x-coordinate of the starting point.</para>
    </param>
    <param name="y1">
        <para>The y-coordinate of the starting point.</para>
    </param>
    <param name="x2">
        <para>The x-coordinate of the end point.</para>
    </param>
    <param name="y2">
        <para>The y-coordinate of the end point.</para>
    </param>
    <remarks>
        <para>This method adds a line connecting the two points specified to the end of this <see cref="GraphicsPath"/>. If there are 
		previous lines or curves, a line segment is added to connect the last point to the first point of the new line segment.</para>
    </remarks>
    <example>
        <code language="cs" source="..\EDrawingLibraryTest\MainPage2.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage2.xaml.vb" region="Usage"/>
    </example>
*/
void GraphicsPath::AddLine(float x1, float y1, float x2, float y2)
{
	this->AddLine(Point(x1, y1), Point(x2, y2));
}

/** <summary>
        Appends a Bezier curve segment to this <see cref="GraphicsPath"/>.
    </summary>
    <param name="pt1">
        <para>A <see cref="Point"/> that represents the starting point of the curve.</para>
    </param>
    <param name="pt2">
        <para>A <see cref="Point"/> that represents the first control point of the curve.</para>
    </param>
	<param name="pt3">
        <para>A <see cref="Point"/> that represents the second control point of the curve.</para>
    </param>
    <param name="pt4">
        <para>A <see cref="Point"/> that represents the end point of the curve.</para>
    </param>
    <remarks>
        <para>This method adds a cubic Bezier curve from the first to the fourth point by using the second and third points as control 
		points to the end of this <see cref="GraphicsPath"/>. If there are previous lines or curves, a line segment is added to connect 
		the last point to the first point of the new line segment.</para>
    </remarks>
    <example>
        <code language="cs" source="..\EDrawingLibraryTest\MainPage2.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage2.xaml.vb" region="Usage"/>
    </example>
*/
void GraphicsPath::AddBezier(Point pt1, Point pt2, Point pt3, Point pt4)
{
	if(impl->pathData->isFirstPoint())
	{
		impl->pathData->addPoint(pt1, PathPointType::Start);
	}else{
		impl->pathData->addPoint(pt1, PathPointType::Line);
	}
	impl->pathData->addPoint(pt2, PathPointType::Bezier);
	impl->pathData->addPoint(pt3, PathPointType::Bezier);
	impl->pathData->addPoint(pt4, PathPointType::Bezier);
}

/** <summary>
        Appends a Bezier curve segment to this <see cref="GraphicsPath"/>.
    </summary>
    <param name="x1">
        <para>The x-coordinate of the starting point.</para>
    </param>
    <param name="y1">
        <para>The y-coordinate of the starting point.</para>
    </param>
    <param name="x2">
        <para>The x-coordinate of the first control point.</para>
    </param>
    <param name="y2">
        <para>The y-coordinate of the first control point.</para>
    </param>
	<param name="x3">
        <para>The x-coordinate of the second control point.</para>
    </param>
    <param name="y3">
        <para>The y-coordinate of the second control point.</para>
    </param>
    <param name="x4">
        <para>The x-coordinate of the end point.</para>
    </param>
    <param name="y4">
        <para>The y-coordinate of the end point.</para>
    </param>
    <remarks>
        <para>This method adds a cubic Bezier curve from the first to the fourth point by using the second and third points as control 
		points to the end of this <see cref="GraphicsPath"/>. If there are previous lines or curves, a line segment is added to connect 
		the last point to the first point of the new line segment.</para>
    </remarks>
    <example>
        <code language="cs" source="..\EDrawingLibraryTest\MainPage2.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage2.xaml.vb" region="Usage"/>
    </example>
*/
void GraphicsPath::AddBezier(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	this->AddBezier(Point(x1, y1), Point(x2, y2), Point(x3, y3), Point(x4, y4));
}

/** <summary>
        Appends an arc segment to this <see cref="GraphicsPath"/>.
    </summary>
    <param name="rect">
        <para>A <see cref="Rect"/> that represents the rectangular bounds of the ellipse from which the arc is taken.</para>
    </param>
    <param name="startAngle">
        <para>Angle in degrees measured clockwise from the x-axis to the starting point of the arc.</para>
    </param>
    <param name="sweepAngle">
        <para>Angle in degrees measured clockwise from the startAngle parameter to ending point of the arc.</para>
    </param>
    <remarks>
        <para>This method adds an arc that is traced along the perimeter of the ellipse bounded by the specified rectangle. If there are 
		previous lines or curves, a line segment is added to connect the last point to the first point of the new arc segment.</para>
    </remarks>
    <example>
        <code language="cs" source="..\EDrawingLibraryTest\MainPage2.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage2.xaml.vb" region="Usage"/>
    </example>
*/
void GraphicsPath::AddArc(Rect rect, float startAngle, float sweepAngle)
{
	double a = rect.Width / 2;
	double b = rect.Height / 2;
	double cx = a + rect.X;
	double cy = b + rect.Y;
	if(sweepAngle < 0)
	{
		sweepAngle = abs(sweepAngle);
		startAngle -= sweepAngle;
	}
	if(sweepAngle > 360)
	{
		sweepAngle = 360;
	}
	double lambda1 = startAngle * (M_PI / 180);
	double lambda2 = (startAngle + sweepAngle) * (M_PI / 180);
	
	EllipticalArc newArc = EllipticalArc(cx, cy, a, b, 0, lambda1, lambda2, false);
	Vector<Point>^ vPoints = newArc.buildPointsList();

	unsigned int i = 0;
	for(i; i < vPoints->Size; i++)
	{
		if(i == 0)
		{
			if(impl->pathData->isFirstPoint())
			{
				impl->pathData->addPoint(vPoints->GetAt(i), PathPointType::Start);
			}else{
				impl->pathData->addPoint(vPoints->GetAt(i), PathPointType::Line);
			} 
		}else{
			impl->pathData->addPoint(vPoints->GetAt(i), PathPointType::Bezier);
		}
	}
}

/** <summary>
        Appends an arc segment to this <see cref="GraphicsPath"/>.
    </summary>
    <param name="x">
        <para>The x-coordinate of the upper-left corner of the bounding rectangle that defines the arc.</para>
    </param>
    <param name="y">
        <para>The y-coordinate of the upper-left corner of the bounding rectangle that defines the arc.</para>
    </param>
    <param name="width">
        <para>Width of the bounding rectangle that defines the arc.</para>
    </param>
    <param name="height">
        <para>Height of the bounding rectangle that defines the arc.</para>
    </param>
    <param name="startAngle">
        <para>Angle in degrees measured clockwise from the x-axis to the starting point of the arc.</para>
    </param>
    <param name="sweepAngle">
        <para>Angle in degrees measured clockwise from the startAngle parameter to ending point of the arc.</para>
    </param>
    <remarks>
        <para>This method adds an arc that is traced along the perimeter of the ellipse bounded by the specified rectangle. If there are 
		previous lines or curves, a line segment is added to connect the last point to the first point of the new arc segment.</para>
    </remarks>
    <example>
        <code language="cs" source="..\EDrawingLibraryTest\MainPage2.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage2.xaml.vb" region="Usage"/>
    </example>
*/
void GraphicsPath::AddArc(float x, float y, float width, float height, float startAngle, float sweepAngle)
{
	this->AddArc(Rect(x, y, width, height), startAngle, sweepAngle);
}

/** <summary>
		Appends a pie segment to this <see cref="GraphicsPath"/>.
	</summary>
	<param name="rect">
		<para>A <see cref="Rect"/> that represents the rectangular bounds of the ellipse from which the pie is taken.</para>
	</param>
	<param name="startAngle">
		<para>Angle in degrees measured clockwise from the x-axis to the starting point of the pie.</para>
	</param>
	<param name="sweepAngle">
		<para>Angle in degrees measured clockwise from the startAngle parameter to ending point of the pie.</para>
	</param>
	<remarks>
		<para>This method adds a pie that is traced along the perimeter of the ellipse bounded by the specified rectangle. If there are
		previous lines or curves, a line segment is added to connect the last point to the first point of the new pie segment.</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage5.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage5.xaml.vb" region="Usage"/>
	</example>
*/
void GraphicsPath::AddPie(Rect rect, float startAngle, float sweepAngle)
{
	double a = rect.Width / 2;
	double b = rect.Height / 2;
	double cx = a + rect.X;
	double cy = b + rect.Y;
	if (sweepAngle < 0)
	{
		sweepAngle = abs(sweepAngle);
		startAngle -= sweepAngle;
	}
	if (sweepAngle > 360)
	{
		sweepAngle = 360;
	}
	double lambda1 = startAngle * (M_PI / 180);
	double lambda2 = (startAngle + sweepAngle) * (M_PI / 180);

	EllipticalArc newArc = EllipticalArc(cx, cy, a, b, 0, lambda1, lambda2, false);
	Vector<Point>^ vPoints = newArc.buildPointsList();

	impl->pathData->addPoint(Point(cx, cy), PathPointType::Start);

	unsigned int i = 0;
	for (i; i < vPoints->Size; i++)
	{
		if (i == 0)
		{
			
			impl->pathData->addPoint(vPoints->GetAt(i), PathPointType::Line);
		}
		else{
			impl->pathData->addPoint(vPoints->GetAt(i), PathPointType::Bezier);
		}
	}

	impl->pathData->addPoint(Point(cx, cy), PathPointType::Line);
}

/** <summary>
		Appends a pie segment to this <see cref="GraphicsPath"/>.
	</summary>
	<param name="x">
		<para>The x-coordinate of the upper-left corner of the bounding rectangle that defines the pie.</para>
	</param>
	<param name="y">
		<para>The y-coordinate of the upper-left corner of the bounding rectangle that defines the pie.</para>
	</param>
	<param name="width">
		<para>Width of the bounding rectangle that defines the pie.</para>
	</param>
	<param name="height">
		<para>Height of the bounding rectangle that defines the pie.</para>
	</param>
	<param name="startAngle">
		<para>Angle in degrees measured clockwise from the x-axis to the starting point of the pie.</para>
	</param>
	<param name="sweepAngle">
		<para>Angle in degrees measured clockwise from the startAngle parameter to ending point of the pie.</para>
	</param>
	<remarks>
		<para>This method adds a pie that is traced along the perimeter of the ellipse bounded by the specified rectangle. If there are
		previous lines or curves, a line segment is added to connect the last point to the first point of the new pie segment.</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage5.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage5.xaml.vb" region="Usage"/>
	</example>
*/
void GraphicsPath::AddPie(float x, float y, float width, float height, float startAngle, float sweepAngle)
{
	this->AddPie(Rect(x, y, width, height), startAngle, sweepAngle);
}

/** <summary>
		Adds an ellipse to this path.
	</summary>
	<param name="rect">
		<para>A Rect that represents the bounds of the ellipse to be added.</para>
	</param>
	<remarks>
		<para>
			This method adds an ellipse to the current path.
		</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage5.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage5.xaml.vb" region="Usage"/>
	</example>
*/
void GraphicsPath::AddEllipse(Rect rect)
{
	this->AddArc(rect, 0, 360);
}

/** <summary>
		Adds an ellipse to this path.
	</summary>
	<param name="x">
        <para>The x-coordinate of the upper-left corner of the bounding rectangle that defines the ellipse.</para>
    </param>
    <param name="y">
        <para>The y-coordinate of the upper-left corner of the bounding rectangle that defines the ellipse.</para>
    </param>
    <param name="width">
        <para>Width of the bounding rectangle that defines the ellipse.</para>
    </param>
    <param name="height">
        <para>Height of the bounding rectangle that defines the ellipse.</para>
    </param>
	<remarks>
		<para>
			This method adds an ellipse to the current path.
		</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage5.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage5.xaml.vb" region="Usage"/>
	</example>
*/
void GraphicsPath::AddEllipse(float x, float y, float width, float height)
{
	this->AddEllipse(Rect(x, y, width, height));
}

/** <summary>
		Adds a spline curve to the current figure using a specified tension.
	</summary>
	<param name="points">
		<para>Array of <see cref="Point"/> that define the spline.</para>
	</param>
	<param name="offset">
		<para>Offset from the first element in the array of the points to the starting point in the curve.</para>
	</param>
	<param name="numberOfSegments">
		<para>Number of segments after the starting point to include in the curve.</para>
	</param>
	<param name="tension">
		<para>Value greater than or equal to 0.0F that specifies the tension of the curve.</para>
	</param>
	<remarks>
		<para>
			The user must keep the original points if they are needed. The original points are converted 
			to cubic Bézier control points internally, therefore there is no mechanism for returning the 
			original points.
		</para>
		<para>
			The curve begins at the point in the array specified by the offset parameter and includes 
			the number of points (segments) specified by numberOfSegments.
		</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage4.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage4.xaml.vb" region="Usage"/>
	</example>
*/
void GraphicsPath::AddCurve(const Array<Point>^ points, int offset, int numberOfSegments, float tension)
{
	if (offset < 0)
	{
		offset = 0;
	}
	if (numberOfSegments < 1)
	{
		numberOfSegments = 1;
	}
	if (tension < 0)
	{
		tension = 0;
	}
	unsigned int lastIndex = offset + numberOfSegments + 1;
	if (lastIndex > points->Length)
	{
		lastIndex = points->Length;
	}
	for (unsigned int i = offset; i <= lastIndex - 2; i++)
	{
		Point P0;
		Point P1 = points[i];
		Point P2 = points[i + 1];
		Point P3;
		if (i == 0)
		{
			P0 = P1;
		}
		else
		{
			P0 = points[i - 1];
		}
		if (i == points->Length - 2)
		{
			P3 = P2;
		}
		else
		{
			P3 = points[i + 2];
		}


		float mx1 = tension * (P2.X - P0.X);
		float my1 = tension * (P2.Y - P0.Y);

		float mx2 = tension * (P3.X - P1.X);
		float my2 = tension * (P3.Y - P1.Y);

		Point CP1(P1.X + mx1 / 3, P1.Y + my1 / 3);
		Point CP2(P2.X - mx2 / 3, P2.Y - my2 / 3);
		if (i == 0)
		{
			if (impl->pathData->isFirstPoint())
			{
				impl->pathData->addPoint(P1, PathPointType::Start);
			}
			else{
				impl->pathData->addPoint(P1, PathPointType::Line);
			}
		}
		impl->pathData->addPoint(CP1, PathPointType::Bezier);
		impl->pathData->addPoint(CP2, PathPointType::Bezier);
		impl->pathData->addPoint(P2, PathPointType::Bezier);

	}
}

/** <summary>
		Adds a spline curve to the current figure using a specified tension.
	</summary>
	<param name="points">
		<para>Array of <see cref="Point"/> that define the spline.</para>
	</param>
	<param name="tension">
		<para>Value greater than or equal to 0.0F that specifies the tension of the curve.</para>
	</param>
	<remarks>
		<para>
			The user must keep the original points if they are needed. The original points are converted
			to cubic Bézier control points internally, therefore there is no mechanism for returning the
			original points.
		</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage4.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage4.xaml.vb" region="Usage"/>
	</example>
*/
void GraphicsPath::AddCurve(const Array<Point>^ points, float tension)
{
	this->AddCurve(points, 0, points->Length - 1, tension);
}

/** <summary>
		Adds a closed curve to the current figure using a specified tension.
	</summary>
	<param name="points">
		<para>Array of <see cref="Point"/> that define the curve.</para>
	</param>
	<param name="tension">
		<para>Value greater than or equal to 0.0F that specifies the tension of the curve.</para>
	</param>
	<remarks>
		<para>
			The user must keep the original points if they are needed. The original points are converted
			to cubic Bézier control points internally, therefore there is no mechanism for returning the
			original points.  If the first point and the last point in the points array are not the same 
			point, the curve is closed by connecting these two points.
		</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage4.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage4.xaml.vb" region="Usage"/>
	</example>
*/
void GraphicsPath::AddClosedCurve(const Array<Point>^ points, float tension)
{
	for (unsigned int i = 0; i <= points->Length - 1; i++)
	{
		Point P0;
		Point P1 = points[i];
		Point P2;
		Point P3;
		if (i == 0)
		{
			P0 = points[points->Length - 1];
		}
		else
		{
			P0 = points[i - 1];
		}
		if (i == points->Length - 2)
		{
			P2 = points[i + 1];
			P3 = points[0];
		}
		else if (i == points->Length - 1)
		{
			P2 = points[0];
			P3 = points[1];
		}
		else
		{
			P2 = points[i + 1];
			P3 = points[i + 2];
		}
		
		float mx1 = tension * (P2.X - P0.X);
		float my1 = tension * (P2.Y - P0.Y);

		float mx2 = tension * (P3.X - P1.X);
		float my2 = tension * (P3.Y - P1.Y);

		Point CP1(P1.X + mx1 / 3, P1.Y + my1 / 3);
		Point CP2(P2.X - mx2 / 3, P2.Y - my2 / 3);
		if (i == 0)
		{
			if (impl->pathData->isFirstPoint())
			{
				impl->pathData->addPoint(P1, PathPointType::Start);
			}
			else{
				impl->pathData->addPoint(P1, PathPointType::Line);
			}
		}
		impl->pathData->addPoint(CP1, PathPointType::Bezier);
		impl->pathData->addPoint(CP2, PathPointType::Bezier);
		if (i == points->Length - 1)
		{
			impl->pathData->addPoint(P2, PathPointType::Bezier | PathPointType::CloseSubpath);
		}
		else
		{
			impl->pathData->addPoint(P2, PathPointType::Bezier);
		}

	}
}

/** <summary>
		Adds a closed curve to the current figure using a specified tension.
	</summary>
	<param name="points">
		<para>Array of <see cref="Point"/> that define the curve.</para>
	</param>
	<remarks>
		<para>
			The user must keep the original points if they are needed. The original points are converted
			to cubic Bézier control points internally, therefore there is no mechanism for returning the
			original points.  If the first point and the last point in the points array are not the same
			point, the curve is closed by connecting these two points.  The tension value is set to 0.5.
		</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage4.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage4.xaml.vb" region="Usage"/>
	</example>
*/
void GraphicsPath::AddClosedCurve(const Array<Point>^ points)
{
	this->AddClosedCurve(points, 0.5);
}

/** <summary>
		Adds a polygon to this path defined by the points in an array.
	</summary>
	<param name="points">
		<para>Array of <see cref="Point"/> that define the polygon.</para>
	</param>
	<remarks>
		<para>
			The points in the points array specify the vertices of a polygon. If the
			last point and the first point of the array do not coincide, they specify the last side of
			the polygon.
		</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage4.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage4.xaml.vb" region="Usage"/>
	</example>
*/
void GraphicsPath::AddPolygon(const Array<Point>^ points)
{
	for (unsigned int i = 0; i <= points->Length - 1; i++)
	{
		if (impl->pathData->isFirstPoint())
		{
			impl->pathData->addPoint(points[i], PathPointType::Start);
		}
		else{
			impl->pathData->addPoint(points[i], PathPointType::Line);
		}
	}
	impl->pathData->addPoint(points[0], PathPointType::Line | PathPointType::CloseSubpath);
}

/** <summary>
		Adds a rectangle to this path.
	</summary>
	<param name="rect">
		<para>A Rect that represents the rectangle to be added.</para>
	</param>
	<remarks>
		<para>
			This method adds a rectangle to the current path.
		</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage4.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage4.xaml.vb" region="Usage"/>
	</example>
*/
void GraphicsPath::AddRectangle(Rect rect)
{
	Array<Point>^ points = ref new Array<Point>(4);
	points[0] = Point(rect.Left, rect.Top);
	points[1] = Point(rect.Right, rect.Top);
	points[2] = Point(rect.Right, rect.Bottom);
	points[3] = Point(rect.Left, rect.Bottom);

	this->AddPolygon(points);
}