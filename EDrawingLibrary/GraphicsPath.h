#pragma once

#include "PathData.h"

namespace ModernComponents
{
	namespace System
	{
		namespace Drawing
		{
			namespace Drawing2D
			{
				/** <summary>
				        Represents a series of connected lines and curves.
				    </summary>
				    <remarks>
				        <para>
				            Applications use paths to draw outlines of shapes, fill the interiors of shapes, and create clipping regions. The 
				            graphics engine maintains the coordinates of geometric shapes in a path in world coordinate space.
				        </para>
				        <para>
				            A path may be composed of any number of figures (subpaths). Each figure is either composed of a sequence of connected 
				            lines and curves or a geometric shape primitive. The starting point of a figure is the first point in the sequence of 
				            connected lines and curves. The ending point is the last point in the sequence. The starting and ending points of a 
				            geometric shape primitive are defined by the primitive specification.
				        </para>
				        <para>
				            A figure that consists of a sequence of connected lines and curves (whose starting and ending points may be coincident) 
				            is an open figure, unless it is closed explicitly. A figure can be closed explicitly, by using the CloseFigure method, 
				            which closes the current figure by connecting a line from the ending point to the starting point. A figure that consists 
				            of a geometric shape primitive is a closed figure.
				        </para>
				        <para>
				            For purposes of filling and clipping (for example, if a path is rendered using FillPath), all open figures are closed by 
				            adding a line from the figure's first point to its last point.
				        </para>
				        <para>
				            A figure has a direction that describes how line and curve segments are traced between the starting point and the ending 
				            point. The direction is defined in the order that lines and curves are added to a figure, or is defined by the geometric 
				            shape primitive. The direction is used in determining the path interiors for clipping and fill.
				        </para>
				    </remarks>
				    <example>
				        <code language="cs" source="..\EDrawingLibraryTest\MainPage2.xaml.cs" region="Usage"/>
						<code language="vb" source="..\EDrawingLibraryTestVB\MainPage2.xaml.vb" region="Usage"/>
				    </example>
				*/
				[Windows::Foundation::Metadata::WebHostHidden]
				public ref class GraphicsPath sealed
				{
				public:
					GraphicsPath();
					GraphicsPath(const Platform::Array<Windows::Foundation::Point>^, const Platform::Array<byte>^);

					virtual ~GraphicsPath();

					/** <summary>
					        Gets a <see cref="PathData"/> that encapsulates the points and types of this GraphicsPath.
					    </summary>
					    <value>
					        <para>Type: <see cref="PathData"/></para>
					        <para>A <see cref="PathData"/> that encapsulates both the points and types of this GraphicsPath.</para>
					    </value>
					*/
					property Drawing2D::PathData^ PathData
					{
						Drawing2D::PathData^ get();
					};

					void AddPath(GraphicsPath^ addingPath, bool connect);

					void AddLine(Windows::Foundation::Point pt1, Windows::Foundation::Point pt2);
					void AddLine(float x1, float y1, float x2, float y2);

					void AddBezier(Windows::Foundation::Point pt1, Windows::Foundation::Point pt2, Windows::Foundation::Point pt3, Windows::Foundation::Point pt4);
					void AddBezier(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

					void AddArc(Windows::Foundation::Rect rect, float startAngle, float sweepAngle);
					void AddArc(float x, float y, float width, float height, float startAngle, float sweepAngle);

					void AddPie(Windows::Foundation::Rect rect, float startAngle, float sweepAngle);
					void AddPie(float x, float y, float width, float height, float startAngle, float sweepAngle);

					void AddEllipse(Windows::Foundation::Rect rect);
					void AddEllipse(float x, float y, float width, float height);

					void AddCurve(const Platform::Array<Windows::Foundation::Point>^ points, int offset, int numberOfSegments, float tension);
					void AddCurve(const Platform::Array<Windows::Foundation::Point>^ points, float tension);

					void AddClosedCurve(const Platform::Array<Windows::Foundation::Point>^ points, float tension);
					void AddClosedCurve(const Platform::Array<Windows::Foundation::Point>^ points);

					void AddPolygon(const Platform::Array<Windows::Foundation::Point>^ points);

					void AddRectangle(Windows::Foundation::Rect rect);
				private:
					class Private;
					std::unique_ptr<Private> impl;
					
				};
			};
		};
	};
};