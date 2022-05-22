#pragma once

//#include "Utilities\DirectXBase.h"
#include "Pen.h"
#include "Font.h"
#include "GraphicsPath.h"

namespace ModernComponents
{
	namespace System
	{
		namespace Drawing
		{
			/** <summary>
			        Encapsulates a Directx surface to allow easy drawing of figures emulating GDI+ drawing calls inside Windows Runtime applications. This class cannot be inherited.
			    </summary>
			    <remarks>
			        <para>
			            The Graphics class provides methods for drawing objects on a Directx surface. A Graphics is associated with a specific Directx surface.
			        </para>
			        <para>
			            You can create a Graphics object by calling the respective constructor and passing it a SwapChainBackgroundPanel element and the current window to it.
			        </para>
			        <para>
			            You can draw many different shapes and lines by using a Graphics object. For more information about how to draw lines 
			            and shapes, see the specific DrawGraphicalElement method for the line or shape you want to draw.
			        </para>
			    </remarks>
			    <example>
					<pre language="cs">
						<code language="cs" nested="true" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Initialization"/>
						<code language="cs" nested="true" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
					</pre>
			    </example>
			*/
			[Windows::Foundation::Metadata::WebHostHidden]
			public ref class Graphics sealed
			{
			public:
				Graphics(Windows::UI::Core::CoreWindow^ window, Windows::UI::Xaml::Controls::SwapChainPanel^ panel);
				Graphics(Windows::UI::Xaml::Media::ImageBrush^ brush, Windows::Foundation::Size size, bool isOpaque);
				virtual ~Graphics();

				void DrawPath(Pen^ pen, Drawing2D::GraphicsPath^ path);
				
				void DrawLine(Pen^ pen, Windows::Foundation::Point pt1, Windows::Foundation::Point pt2);
				void DrawLine(Pen^ pen, float x1, float y1, float x2, float y2);
				
				void DrawBezier(Pen^ pen, Windows::Foundation::Point pt1, Windows::Foundation::Point pt2, Windows::Foundation::Point pt3, Windows::Foundation::Point pt4);
				void DrawBezier(Pen^ pen, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
				
				void DrawArc(Pen^ pen, Windows::Foundation::Rect rect, float startAngle, float sweepAngle);
				void DrawArc(Pen^ pen, float x, float y, float width, float height, float startAngle, float sweepAngle);

				void DrawPie(Pen^ pen, Windows::Foundation::Rect rect, float startAngle, float sweepAngle);
				void DrawPie(Pen^ pen, float x, float y, float width, float height, float startAngle, float sweepAngle);

				void DrawEllipse(Pen^ pen, Windows::Foundation::Rect rect);
				void DrawEllipse(Pen^ pen, float x, float y, float width, float height);
				
				void DrawCurve(Pen^ pen, const Platform::Array<Windows::Foundation::Point>^ points, int offset, int numberOfSegments, float tension);
				void DrawCurve(Pen^ pen, const Platform::Array<Windows::Foundation::Point>^ points, float tension);
				
				void DrawClosedCurve(Pen^ pen, const Platform::Array<Windows::Foundation::Point>^ points, float tension);
				void DrawClosedCurve(Pen^ pen, const Platform::Array<Windows::Foundation::Point>^ points);
				
				void DrawPolygon(Pen^ pen, const Platform::Array<Windows::Foundation::Point>^ points);
				
				void DrawRectangle(Pen^ pen, float x, float y, float width, float height);
				void DrawRectangle(Pen^ pen, Windows::Foundation::Rect rect);


				void DrawString(Platform::String^ s, Font^ font, Windows::UI::Xaml::Media::Brush^ brush, Windows::Foundation::Point point);
				void DrawString(Platform::String^ s, Font^ font, Windows::UI::Xaml::Media::Brush^ brush, float x, float y);
	
				void Clear(Windows::UI::Color color);
				void Flush();

				void ToFile();

			private:
				
				// DisplayInformation event handlers.
				void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
				void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
				void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);

				// Other event handlers.
				void OnCompositionScaleChanged(Windows::UI::Xaml::Controls::SwapChainPanel^ sender, Object^ args);
				void OnSwapChainPanelSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);

				class Private;
				std::unique_ptr<Private> impl;
				
				void OnSuspending(Platform::Object ^sender, Windows::ApplicationModel::SuspendingEventArgs ^args);
			};
		};
	};
};