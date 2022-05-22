#include "pch.h"
#include "Graphics.h"
#include "Utilities\D2DUtils.h"

#pragma region DirectXBase.cpp
#include <windows.ui.xaml.media.dxinterop.h>
#include <math.h>
#pragma endregion DirectXBase.cpp

using namespace ModernComponents::System::Drawing;

using namespace Concurrency;
using namespace D2D1;
using namespace DirectX;
using namespace Platform;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Text;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::Security::Cryptography;
using namespace Windows::Security::Cryptography::Core;

using namespace Drawing2D;

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Platform::Collections;
using namespace Windows::Security::Cryptography::Certificates;
using namespace Windows::UI::Xaml::Media::Imaging;



#pragma region Private

// Provides an interface for an application that owns DeviceResources to be notified of the device being lost or created.
interface IDeviceNotify
{
	virtual void OnDeviceLost() = 0;
	virtual void OnDeviceRestored() = 0;
};

enum class DeviceType
{
	SwapChainPanel = 0,
	SurfaceImageSource = 1
};

class Graphics::Private
{
	public:
		Private(DeviceType renderDevice);

		bool CurrentlyDrawing;
		int FrameCount;

		DeviceType RenderDevice;

		Buffer^ imageBuffer;

		void DrawPath(Pen^ pen, Drawing2D::GraphicsPath^ path, bool closed);

		void DrawString(String^ s, Font^ font, ComPtr<ID2D1Brush> brush, D2D1_RECT_F layoutRectangle, DWRITE_TEXT_ALIGNMENT horizontalAlignment, DWRITE_PARAGRAPH_ALIGNMENT verticalAlignment);

		void ToStream(_In_ REFGUID, _In_ IStream*);

#pragma region DirectXBase.h

		void SetSwapChainPanel(Windows::UI::Xaml::Controls::SwapChainPanel^ panel); 
		void SetSurfaceImageSource(Windows::UI::Xaml::Media::Imaging::SurfaceImageSource^ image, Windows::Foundation::Size size);
		void SetLogicalSize(Windows::Foundation::Size logicalSize);
		void SetCurrentOrientation(Windows::Graphics::Display::DisplayOrientations currentOrientation);
		void SetDpi(float dpi);
		void SetCompositionScale(float compositionScaleX, float compositionScaleY);
		void ValidateDevice(); 
		void HandleDeviceLost(); 
		void RegisterDeviceNotify(IDeviceNotify* deviceNotify);
		void Trim();
		void Present(); 

		// Device Accessors.
		Windows::Foundation::Size GetOutputSize() const					{ return m_outputSize; }
		Windows::Foundation::Size GetLogicalSize() const				{ return m_logicalSize; }

		// D3D Accessors.
		ID3D11Device2*			GetD3DDevice() const					{ return m_d3dDevice.Get(); }
		ID3D11DeviceContext2*	GetD3DDeviceContext() const				{ return m_d3dContext.Get(); }
		IDXGISwapChain1*		GetSwapChain() const					{ return m_swapChain.Get(); }
		D3D_FEATURE_LEVEL		GetDeviceFeatureLevel() const			{ return m_d3dFeatureLevel; }
		ID3D11RenderTargetView*	GetBackBufferRenderTargetView() const	{ return m_d3dRenderTargetView.Get(); }
		ID3D11DepthStencilView* GetDepthStencilView() const				{ return m_d3dDepthStencilView.Get(); }
		D3D11_VIEWPORT			GetScreenViewport() const				{ return m_screenViewport; }
		DirectX::XMFLOAT4X4		GetOrientationTransform3D() const		{ return m_orientationTransform3D; }

		// D2D Accessors.
		ID2D1Factory2*			GetD2DFactory() const					{ return m_d2dFactory.Get(); }
		ID2D1Device1*			GetD2DDevice() const					{ return m_d2dDevice.Get(); }
		ID2D1DeviceContext1*	GetD2DDeviceContext() const				{ return m_d2dContext.Get(); }
		ID2D1Bitmap1*			GetD2DTargetBitmap() const				{ return m_d2dTargetBitmap.Get(); }
		IDWriteFactory2*		GetDWriteFactory() const				{ return m_dwriteFactory.Get(); }
		IWICImagingFactory2*	GetWicImagingFactory() const			{ return m_wicFactory.Get(); }
		D2D1::Matrix3x2F		GetOrientationTransform2D() const		{ return m_orientationTransform2D; }

		//private
		void CreateDeviceIndependentResources(); 
		void CreateDeviceResources(); 
		void CreateWindowSizeDependentResources(); 
		DXGI_MODE_ROTATION ComputeDisplayRotation();

		// Direct3D objects.
		Microsoft::WRL::ComPtr<ID3D11Device2>			m_d3dDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext2>	m_d3dContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain1>			m_swapChain;

		// Direct3D rendering objects. Required for 3D.
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_d3dRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_d3dDepthStencilView;
		D3D11_VIEWPORT									m_screenViewport;

		// Direct2D drawing components.
		Microsoft::WRL::ComPtr<ID2D1Factory2>		m_d2dFactory;
		Microsoft::WRL::ComPtr<ID2D1Device1>		m_d2dDevice;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext1>	m_d2dContext;
		Microsoft::WRL::ComPtr<ID2D1Bitmap1>		m_d2dTargetBitmap;

		// DirectWrite drawing components.
		Microsoft::WRL::ComPtr<IDWriteFactory2>		m_dwriteFactory;
		Microsoft::WRL::ComPtr<IWICImagingFactory2>	m_wicFactory;

		// Cached reference to the XAML panel.
		Windows::UI::Xaml::Controls::SwapChainPanel^    m_swapChainPanel;
		Windows::UI::Xaml::Media::Imaging::SurfaceImageSource^    m_surfaceImageSource;

		// Cached device properties.
		D3D_FEATURE_LEVEL								m_d3dFeatureLevel;
		Windows::Foundation::Size						m_d3dRenderTargetSize;
		Windows::Foundation::Size						m_outputSize;
		Windows::Foundation::Size						m_logicalSize;
		Windows::Graphics::Display::DisplayOrientations	m_nativeOrientation;
		Windows::Graphics::Display::DisplayOrientations	m_currentOrientation;
		float											m_dpi;
		float											m_compositionScaleX;
		float											m_compositionScaleY;

		// Transforms used for display orientation.
		D2D1::Matrix3x2F	m_orientationTransform2D;
		DirectX::XMFLOAT4X4	m_orientationTransform3D;

		// The IDeviceNotify can be held directly as it owns the DeviceResources.
		IDeviceNotify* m_deviceNotify;

#pragma endregion DirectXBase.h

#pragma region SurfaceImageSource

		POINT offset;
		Windows::Foundation::Rect UpdateRect;

		bool SISInitialized;

		void SurfaceImageSourceIntialize(Windows::Foundation::Rect updateRect);
		void BeginDraw(Windows::Foundation::Rect updateRect);
		void BeginDraw()    { BeginDraw(Windows::Foundation::Rect(0, 0, (float)m_logicalSize.Width, (float)m_logicalSize.Height)); }
		HRESULT EndDraw();
		void SurfaceImageSourcePresent();

#pragma endregion SurfaceImageSource
};

#pragma endregion Private

/** <summary>
        Creates a new Graphics object.
    </summary>
    <param name="window">
        <para>The current <see cref="CoreWindow"/> object of the app.</para>
    </param>
    <param name="panel">
        <para>The <see cref="SwapChainBackgroundPanel"/> were you will draw.</para>
    </param>
    <remarks>
        <para>This constructor creates a new Graphics object using the specified <see cref="CoreWindow"/> and 
   		<see cref="SwapChainBackgroundPanel"/> object, both objects objects must not be null.</para>
    </remarks>
    <example>
        <code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Initialization"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Initialization"/>
    </example>
*/
Graphics::Graphics(CoreWindow^ window, SwapChainPanel^ panel) : impl(new Private(DeviceType::SwapChainPanel))
{
	/*impl->Initialize(window, panel, DisplayProperties::LogicalDpi);
	window->SizeChanged += ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &Graphics::OnWindowSizeChanged);
	DisplayProperties::LogicalDpiChanged += ref new DisplayPropertiesEventHandler(this, &Graphics::OnLogicalDpiChanged);
	DisplayProperties::OrientationChanged += ref new DisplayPropertiesEventHandler(this, &Graphics::OnOrientationChanged);
	DisplayProperties::DisplayContentsInvalidated += ref new DisplayPropertiesEventHandler(this, &Graphics::OnDisplayContentsInvalidated);
	CoreApplication::Suspending += ref new Windows::Foundation::EventHandler<Windows::ApplicationModel::SuspendingEventArgs ^>(this, &Graphics::OnSuspending);
	//CompositionTarget::Rendering::add(ref new EventHandler<Object^>(this, &Graphics::OnRendering));
	//m_renderNeeded = true;*/
	
	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();
	currentDisplayInformation->DpiChanged += ref new TypedEventHandler<DisplayInformation^, Object^>(this, &Graphics::OnDpiChanged);
	currentDisplayInformation->OrientationChanged += ref new TypedEventHandler<DisplayInformation^, Object^>(this, &Graphics::OnOrientationChanged);
	DisplayInformation::DisplayContentsInvalidated += ref new TypedEventHandler<DisplayInformation^, Object^>(this, &Graphics::OnDisplayContentsInvalidated);

	panel->CompositionScaleChanged += ref new TypedEventHandler<SwapChainPanel^, Object^>(this, &Graphics::OnCompositionScaleChanged);
	panel->SizeChanged += ref new SizeChangedEventHandler(this, &Graphics::OnSwapChainPanelSizeChanged);

	CoreApplication::Suspending += ref new Windows::Foundation::EventHandler<Windows::ApplicationModel::SuspendingEventArgs ^>(this, &Graphics::OnSuspending);

	// At this point we have access to the device. 
	// We can create the device-dependent resources.
	//impl = std::make_shared<Private>();
	impl->SetSwapChainPanel(panel);

}

/** <summary>
		Creates a new Graphics object.
	</summary>
	<param name="brush">
		<para>The <see cref="ImageBrush"/> object that will hold the DirectX surface used for drawing.</para>
	</param>
	<param name="size">
		<para>The size in pixels of the drawing surface.</para>
	</param>
	<param name="isOpaque">
		<para>Determines whether the drawing surface will be opaque or not.</para>
	</param>
	<remarks>
		<para>This constructor creates a new Graphics object using the specified <see cref="ImageBrush"/> and
		size, the <see cref="ImageBrush"/> object must not be null.</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage3.xaml.cs" region="Initialization"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage3.xaml.vb" region="Initialization"/>
	</example>
*/
Graphics::Graphics(ImageBrush^ brush, Windows::Foundation::Size size, bool isOpaque) : impl(new Private(DeviceType::SurfaceImageSource))
{
	CoreApplication::Suspending += ref new Windows::Foundation::EventHandler<Windows::ApplicationModel::SuspendingEventArgs ^>(this, &Graphics::OnSuspending);
	
	SurfaceImageSource^ DrawingSurfaceImageSource = ref new SurfaceImageSource(static_cast<int>(size.Width), static_cast<int>(size.Height), isOpaque);
	brush->ImageSource = DrawingSurfaceImageSource;

	impl->SetSurfaceImageSource(DrawingSurfaceImageSource, size);

}

Graphics::~Graphics()
{
}

// DisplayInformation event handlers.

void Graphics::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
	impl->SetDpi(sender->LogicalDpi);
}

void Graphics::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
	impl->SetCurrentOrientation(sender->CurrentOrientation);
}


void Graphics::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
	impl->ValidateDevice();
}

void Graphics::OnCompositionScaleChanged(SwapChainPanel^ sender, Object^ args)
{
	impl->SetCompositionScale(sender->CompositionScaleX, sender->CompositionScaleY);
}

void Graphics::OnSwapChainPanelSizeChanged(Object^ sender, SizeChangedEventArgs^ e)
{
	impl->SetLogicalSize(e->NewSize);
}

void Graphics::OnSuspending(Object ^sender, SuspendingEventArgs ^args)
{
	impl->Trim();
}

/** <summary>
        Draws a <see cref="GraphicsPath"/> with the given <see cref="Pen"/>.
    </summary>
    <param name="pen">
        <para><see cref="Pen"/> that determines the characteristics of the outline.</para>
    </param>
    <param name="path">
        <para><see cref="GraphicsPath"/> that represents the path to draw.</para>
    </param>
    <remarks>
        <para>A <see cref="GraphicsPath"/> consists of a series of line and curve segments.</para>
    </remarks>
    <example>
        <code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
    </example>
*/
void Graphics::DrawPath(Pen^ pen, GraphicsPath^ path)
{
	impl->DrawPath(pen, path, false);
}

/** <summary>
        Draws a line connecting the two points specified by the coordinate pairs.
    </summary>
    <param name="pen">
        <para><see cref="Pen"/> that determines the color, width and style of the line.</para>
    </param>
    <param name="x1">
        <para>The x-coordinate of the first point.</para>
    </param>
    <param name="y1">
        <para>The y-coordinate of the first point.</para>
    </param>
    <param name="x2">
        <para>The x-coordinate of the second point.</para>
    </param>
    <param name="y2">
        <para>The y-coordinate of the second point.</para>
    </param>
    <remarks>
        <para>This method draws a line connecting the two points specified by the x1, y1, x2, and y2 parameters.</para>
    </remarks>
    <example>
        <code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
    </example>
*/
void Graphics::DrawLine(Pen^ pen, float x1, float y1, float x2, float y2)
{
	this->DrawLine(pen, Point(x1, y1), Point(x2, y2));
}

/** <summary>
		Draws a line connecting the two points specified.
	</summary>
	<param name="pen">
		<para><see cref="Pen"/> that determines the color, width and style of the line.</para>
	</param>
	<param name="pt1">
		<para>A <see cref="Point"/> that represents the starting point of the line.</para>
	</param>
	<param name="pt2">
		<para>A <see cref="Point"/> that represents the end point of the line.</para>
	</param>
	<remarks>
		<para>This method draws a line connecting the two points specified by the pt1 and pt2 parameters.</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
	</example>
*/
void Graphics::DrawLine(Pen^ pen, Point pt1, Point pt2)
{
	GraphicsPath^ line = ref new GraphicsPath();
	line->AddLine(pt1, pt2);
	DrawPath(pen, line);
}

/** <summary>
        Draws a Bézier spline defined by four ordered pairs of coordinates that represent points.
    </summary>
    <param name="pen">
        <para><see cref="Pen"/> that determines the color, width and style of the curve.</para>
    </param>
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
        <para>The x-coordinate of the ending point.</para>
    </param>
    <param name="y4">
        <para>The y-coordinate of the ending point.</para>
    </param>
    <remarks>
        <para>The Bézier spline is drawn from the first point to the fourth point. The second and third points are control points that determine the shape of the curve.</para>
    </remarks>
    <example>
        <code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
    </example>
*/
void Graphics::DrawBezier(Pen^ pen, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	this->DrawBezier(pen, Point(x1, y1), Point(x2, y2), Point(x3, y3), Point(x4, y4));
}

/** <summary>
		Draws a Bézier spline defined by four points.
	</summary>
	<param name="pen">
		<para><see cref="Pen"/> that determines the color, width and style of the curve.</para>
	</param>
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
		<para>The Bézier spline is drawn from the first point to the fourth point. The second and third points are control points that determine the shape of the curve.</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
	</example>
*/
void Graphics::DrawBezier(Pen^ pen, Point pt1, Point pt2, Point pt3, Point pt4)
{
	GraphicsPath^ bezier = ref new GraphicsPath();
	bezier->AddBezier(pt1, pt2, pt3, pt4);
	DrawPath(pen, bezier);
}

/** <summary>
        Draws an arc representing a portion of an ellipse specified by the 4 points provided.
    </summary>
    <param name="pen">
        <para><see cref="Pen"/> that determines the color, width and style of the arc.</para>
    </param>
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
        <para>This method draws an arc that is a portion of the perimeter of an ellipse. The ellipse is defined by the boundaries 
        of a rectangle. The arc is the portion of the perimeter of the ellipse between the startAngle parameter and the startAngle 
        + sweepAngle parameters.</para>
    </remarks>
    <example>
        <code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
    </example>
*/
void Graphics::DrawArc(Pen^ pen, float x, float y, float width, float height, float startAngle, float sweepAngle)
{
	this->DrawArc(pen, Windows::Foundation::Rect(x, y, width, height), startAngle, sweepAngle);
}

/** <summary>
		Draws an arc representing a portion of an ellipse specified by the rectangle provided.
	</summary>
	<param name="pen">
		<para><see cref="Pen"/> that determines the color, width and style of the arc.</para>
	</param>
	<param name="rect">
        <para>A Rect that represents the rectangular bounds of the ellipse from which the arc is taken.</para>
    </param>
	<param name="startAngle">
		<para>Angle in degrees measured clockwise from the x-axis to the starting point of the arc.</para>
	</param>
	<param name="sweepAngle">
		<para>Angle in degrees measured clockwise from the startAngle parameter to ending point of the arc.</para>
	</param>
	<remarks>
		<para>This method draws an arc that is a portion of the perimeter of an ellipse. The ellipse is defined by the boundaries
		of a rectangle. The arc is the portion of the perimeter of the ellipse between the startAngle parameter and the startAngle
		+ sweepAngle parameters.</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
	</example>
*/
void Graphics::DrawArc(Pen^ pen, Windows::Foundation::Rect rect, float startAngle, float sweepAngle)
{
	GraphicsPath^ arc = ref new GraphicsPath();
	arc->AddArc(rect, startAngle, sweepAngle);
	DrawPath(pen, arc);
}

/** <summary>
		Draws a pie representing a portion of an ellipse specified by the 4 points provided.
	</summary>
	<param name="pen">
		<para><see cref="Pen"/> that determines the color, width and style of the pie.</para>
	</param>
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
		<para>This method draws a pie that is a portion of the perimeter of an ellipse. The ellipse is defined by the boundaries
		of a rectangle. The pie is the portion of the perimeter of the ellipse between the startAngle parameter and the startAngle
		+ sweepAngle parameters.</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage5.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage5.xaml.vb" region="Usage"/>
	</example>
*/
void Graphics::DrawPie(Pen^ pen, float x, float y, float width, float height, float startAngle, float sweepAngle)
{
	this->DrawPie(pen, Windows::Foundation::Rect(x, y, width, height), startAngle, sweepAngle);
}

/** <summary>
		Draws a pie representing a portion of an ellipse specified by the rectangle provided.
	</summary>
	<param name="pen">
		<para><see cref="Pen"/> that determines the color, width and style of the pie.</para>
	</param>
	<param name="rect">
		<para>A Rect that represents the rectangular bounds of the ellipse from which the pie is taken.</para>
	</param>
	<param name="startAngle">
		<para>Angle in degrees measured clockwise from the x-axis to the starting point of the pie.</para>
	</param>
	<param name="sweepAngle">
		<para>Angle in degrees measured clockwise from the startAngle parameter to ending point of the pie.</para>
	</param>
	<remarks>
		<para>This method draws a pie that is a portion of the perimeter of an ellipse. The ellipse is defined by the boundaries
		of a rectangle. The pie is the portion of the perimeter of the ellipse between the startAngle parameter and the startAngle
		+ sweepAngle parameters.</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage5.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage5.xaml.vb" region="Usage"/>
	</example>
*/
void Graphics::DrawPie(Pen^ pen, Windows::Foundation::Rect rect, float startAngle, float sweepAngle)
{
	GraphicsPath^ pie = ref new GraphicsPath();
	pie->AddPie(rect, startAngle, sweepAngle);
	DrawPath(pen, pie);
}

/** <summary>
		Draws an ellipse specified by a coordinate pair, a width, and a height.
	</summary>
	<param name="pen">
		<para><see cref="Pen"/> that determines the color, width and style of the ellipse.</para>
	</param>
	<param name="x">
		<para>The x-coordinate of the upper-left corner of the bounding rectangle of the ellipse.</para>
	</param>
	<param name="y">
		<para>The y-coordinate of the upper-left corner of the bounding rectangle of the ellipse.</para>
	</param>
	<param name="width">
		<para>Width of the bounding rectangle of the ellipse.</para>
	</param>
	<param name="height">
		<para>Height of the bounding rectangle of the ellipse.</para>
	</param>
	<remarks>
		<para>
			This method draws an ellipse.
		</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage5.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage5.xaml.vb" region="Usage"/>
	</example>
*/
void Graphics::DrawEllipse(Pen^ pen, float x, float y, float width, float height)
{
	this->DrawEllipse(pen, Windows::Foundation::Rect(x, y, width, height));
}

/** <summary>
		Draws an ellipse.
	</summary>
	<param name="pen">
		<para><see cref="Pen"/> that determines the color, width and style of the ellipse.</para>
	</param>
	<param name="rect">
		<para>A Rect that represents the rectangular bounds of the ellipse.</para>
	</param>
	<remarks>
		<para>
			This method draws a rectangle.
		</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage5.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage5.xaml.vb" region="Usage"/>
	</example>
*/
void Graphics::DrawEllipse(Pen^ pen, Windows::Foundation::Rect rect)
{
	GraphicsPath^ ellipse = ref new GraphicsPath();
	ellipse->AddEllipse(rect);
	DrawPath(pen, ellipse);
}

/** <summary>
		Draws a cardinal spline through the specified points in an array using a specified tension.
	</summary>
	<param name="pen">
		<para><see cref="Pen"/> that determines the color, width and style of the curve.</para>
	</param>
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
			This method draws a cardinal spline that passes through each defined point in the array; 
			the array of points must contain at least three points for a curve to be drawn, if only two 
			points are provided the result will be an straight line.
		</para>
		<para>
			The value of the offset parameter specifies the number of elements to skip in the array. 
			The first element after the skipped elements represents the starting point of the curve.
		</para>
		<para>
			The value of the numberOfSegments parameter specifies the number of segments, after the 
			starting point, to draw in the curve. The value of the numberOfSegments parameter must be at 
			least 1. The value of the offset parameter plus the value of the numberOfSegments parameter 
			must be less than the number of elements in the array of the points parameter.
		</para>
		<para>
			The tension parameter determines the shape of the spline. If the value of the tension 
			parameter is 0.0F, this method draws straight line segments to connect the points. Usually, 
			the tension parameter is less than or equal to 1.0F. Values over 1.0F produce unusual 
			results.
		</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage3.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage3.xaml.vb" region="Usage"/>
	</example>
*/
void Graphics::DrawCurve(Pen^ pen, const Array<Point>^ points, int offset, int numberOfSegments, float tension)
{
	GraphicsPath^ curve = ref new GraphicsPath();
	curve->AddCurve(points, offset, numberOfSegments, tension);
	DrawPath(pen, curve);
}

/** <summary>
		Draws a cardinal spline through the specified points in an array using a specified tension.
	</summary>
	<param name="pen">
		<para><see cref="Pen"/> that determines the color, width and style of the curve.</para>
	</param>
	<param name="points">
		<para>Array of <see cref="Point"/> that define the spline.</para>
	</param>
	<param name="tension">
		<para>Value greater than or equal to 0.0F that specifies the tension of the curve.</para>
	</param>
	<remarks>
		<para>
			This method draws a cardinal spline that passes through each defined point in the array;
			the array of points must contain at least three points for a curve to be drawn, if only two
			points are provided the result will be an straight line.
		</para>
		<para>
			The tension parameter determines the shape of the spline. If the value of the tension
			parameter is 0.0F, this method draws straight line segments to connect the points. Usually,
			the tension parameter is less than or equal to 1.0F. Values over 1.0F produce unusual
			results.
		</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage3.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage3.xaml.vb" region="Usage"/>
	</example>
*/
void Graphics::DrawCurve(Pen^ pen, const Array<Point>^ points, float tension)
{
	this->DrawCurve(pen, points, 0, points->Length - 1, tension);
}

/** <summary>
		Draws a closed cardinal spline through the specified points in an array using a specified tension.
	</summary>
	<param name="pen">
		<para><see cref="Pen"/> that determines the color, width and style of the curve.</para>
	</param>
	<param name="points">
		<para>Array of <see cref="Point"/> that define the spline.</para>
	</param>
	<param name="tension">
		<para>Value greater than or equal to 0.0F that specifies the tension of the curve.</para>
	</param>
	<remarks>
		<para>
			This method draws a cardinal spline that passes through each defined point in the array;
			the array of points must contain at least three points for a curve to be drawn, if only two
			points are provided the result will be an straight line. If the last point does not match 
			the first point, an additional curve segment is added from the last point to the first point 
			to close it.
		</para>
		<para>
			The tension parameter determines the shape of the spline. If the value of the tension
			parameter is 0.0F, this method draws straight line segments to connect the points. Usually,
			the tension parameter is less than or equal to 1.0F. Values over 1.0F produce unusual
			results.
		</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage3.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage3.xaml.vb" region="Usage"/>
	</example>
*/
void Graphics::DrawClosedCurve(Pen^ pen, const Array<Point>^ points, float tension)
{
	GraphicsPath^ curve = ref new GraphicsPath();
	curve->AddClosedCurve(points, tension);
	DrawPath(pen, curve);
}

/** <summary>
		Draws a closed cardinal spline through the specified points in an array.
	</summary>
	<param name="pen">
		<para><see cref="Pen"/> that determines the color, width and style of the curve.</para>
	</param>
	<param name="points">
		<para>Array of <see cref="Point"/> that define the spline.</para>
	</param>
	<remarks>
		<para>
			This method draws a cardinal spline that passes through each defined point in the array;
			the array of points must contain at least three points for a curve to be drawn, if only two
			points are provided the result will be an straight line. If the last point does not match
			the first point, an additional curve segment is added from the last point to the first point
			to close it.
		</para>
		<para>
			The tension of the drawn curve is 0.5.
		</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage3.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage3.xaml.vb" region="Usage"/>
	</example>
*/
void Graphics::DrawClosedCurve(Pen^ pen, const Array<Point>^ points)
{
	this->DrawClosedCurve(pen, points, 0.5f);
}

/** <summary>
		Draws a polygon defined by the points in an array.
	</summary>
	<param name="pen">
		<para><see cref="Pen"/> that determines the color, width and style of the polygon.</para>
	</param>
	<param name="points">
		<para>Array of <see cref="Point"/> that define the polygon.</para>
	</param>
	<remarks>
		<para>
			Every pair of two consecutive points in the array specifies a side of the polygon. If the 
			last point and the first point of the array do not coincide, they specify the last side of 
			the polygon.
		</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage3.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage3.xaml.vb" region="Usage"/>
	</example>
*/
void Graphics::DrawPolygon(Pen^ pen, const Array<Point>^ points)
{
	GraphicsPath^ polygon = ref new GraphicsPath();
	polygon->AddPolygon(points);
	impl->DrawPath(pen, polygon, true);
}

/** <summary>
		Draws a rectangle specified by a coordinate pair, a width, and a height.
	</summary>
	<param name="pen">
		<para><see cref="Pen"/> that determines the color, width and style of the rectangle.</para>
	</param>
	<param name="x">
		<para>The x-coordinate of the upper-left corner of the rectangle.</para>
	</param>
	<param name="y">
		<para>The y-coordinate of the upper-left corner of the rectangle.</para>
	</param>
	<param name="width">
		<para>Width of the rectangle.</para>
	</param>
	<param name="height">
		<para>Height of the rectangle.</para>
	</param>
	<remarks>
		<para>
			This method draws a rectangle.
		</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage3.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage3.xaml.vb" region="Usage"/>
	</example>
*/
void Graphics::DrawRectangle(Pen^ pen, float x, float y, float width, float height)
{
	this->DrawRectangle(pen, Windows::Foundation::Rect(x, y, width, height));
}

/** <summary>
		Draws a rectangle.
	</summary>
	<param name="pen">
		<para><see cref="Pen"/> that determines the color, width and style of the rectangle.</para>
	</param>
	<param name="rect">
        <para>A Rect that represents the rectangle to be drawn.</para>
    </param>
	<remarks>
		<para>
			This method draws a rectangle.
		</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage3.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage3.xaml.vb" region="Usage"/>
	</example>
*/
void Graphics::DrawRectangle(Pen^ pen, Windows::Foundation::Rect rect)
{
	GraphicsPath^ rectangle = ref new GraphicsPath();
	rectangle->AddRectangle(rect);
	impl->DrawPath(pen, rectangle, true);
}

/** <summary>
        Draws the specified text string at the specified location with the specified Brush and Font objects.
    </summary>
    <param name="s">
        <para>String to draw.</para>
    </param>
    <param name="font">
        <para><see cref="Font"/> that determines the text format of the string.</para>
    </param>
    <param name="brush">
        <para><see cref="Brush"/> that determines the color and texture of the drawn text.</para>
    </param>
    <param name="x">
        <para>The x-coordinate of the upper-left corner of the drawn text.</para>
    </param>
    <param name="y">
        <para>The y-coordinate of the upper-left corner of the drawn text.</para>
    </param>
    <remarks>
        <para>This method draws a string in the location defined by the x and y parameters, using the Font object to determine the 
        properties of the text and the Brush object to determinate its color.</para>
    </remarks>
    <example>
        <code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
    </example>
*/
void Graphics::DrawString(String^ s, Font^ font, Brush^ brush, float x, float y)
{
	this->DrawString(s, font, brush, Point(x, y));
}

/** <summary>
		Draws the specified text string at the specified location with the specified Brush and Font objects.
	</summary>
	<param name="s">
		<para>String to draw.</para>
	</param>
	<param name="font">
		<para><see cref="Font"/> that determines the text format of the string.</para>
	</param>
	<param name="brush">
		<para><see cref="Brush"/> that determines the color and texture of the drawn text.</para>
	</param>
	<param name="point">
		<para>The point of the upper-left corner of the drawn text.</para>
	</param>
	<remarks>
		<para>This method draws a string in the location defined by the point parameter, using the Font object to determine the
		properties of the text and the Brush object to determinate its color.</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
	</example>
*/
void Graphics::DrawString(String^ s, Font^ font, Brush^ brush, Point point)
{
	ComPtr<ID2D1Brush> tempBrush = D2DUtils::BrushToID2D1Brush(impl->m_d2dContext, impl->m_wicFactory, brush);
	D2D1_SIZE_F renderTargetSize = impl->m_d2dContext->GetSize();
	impl->DrawString(s, font, tempBrush, D2D1::RectF(point.X, point.Y, renderTargetSize.width, renderTargetSize.height), DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
}

/** <summary>
        Forces execution of all pending graphics operations. A call to this function is necessary to display the results of the drawing 
		operations.
    </summary>
    <remarks>
        <para>This method starts a flush and wait until it finishes so it displays the drawn results.</para>
    </remarks>
    <example>
        <code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
    </example>
*/
void Graphics::Flush()
{
	std::shared_ptr<GUID> wicFormat = std::make_shared<GUID>(GUID_ContainerFormatBmp);
	InMemoryRandomAccessStream^ randomAccessStream = ref new InMemoryRandomAccessStream();
	// Convert the RandomAccessStream to an IStream.
    ComPtr<IStream> stream;
    DX::ThrowIfFailed(
        CreateStreamOverRandomAccessStream(randomAccessStream, IID_PPV_ARGS(&stream))
    );
	
	impl->EndDraw();
	impl->ToStream(*wicFormat, stream.Get());
	impl->imageBuffer = ref new Buffer((int)randomAccessStream->Size);
	randomAccessStream->Seek(0);
	create_task(randomAccessStream->ReadAsync(impl->imageBuffer, (int)randomAccessStream->Size, InputStreamOptions::None)).then([=](IBuffer^ buffer){});

	if (impl->RenderDevice == DeviceType::SwapChainPanel)
	{
		impl->Present();
	}
	else
	{
		impl->SurfaceImageSourcePresent();
	}
}

/** <summary>
        Clears the entire drawing surface and fills it with the specified background color.
    </summary>
    <param name="color">
        <para><see cref="Color"/> object that represents the background of the drawing surface.</para>
    </param>
    <remarks>
        <para>The Clear method clears the state of the current Graphics object.</para>
    </remarks>
    <example>
        <code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
    </example>
*/
void Graphics::Clear(Color color)
{
	impl->BeginDraw();
	impl->m_d2dContext->Clear(D2DUtils::ColorToColorF(color));
}

/** <summary>
        Saves the current state of the Graphics object to a file.
    </summary>
    <remarks>
        <para>This method saves the current state of the Graphics object to a raster bitmap file showing a file picker interface.</para>
    </remarks>
    <example>
        <code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Save"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Save"/>
    </example>
*/
void Graphics::ToFile()
{
    if (impl->m_d2dTargetBitmap == nullptr)
    {
        return;
    }

    // Set state to Saving to block another copy of this function being called before saving completes.
    
    
    // Prepare a file picker for customers to input image file name.
    Pickers::FileSavePicker^ savePicker = ref new Pickers::FileSavePicker();
    auto bmpExtensions = ref new Platform::Collections::Vector<Platform::String^>();
    bmpExtensions->Append(".bmp");
    savePicker->FileTypeChoices->Insert("BMP file", bmpExtensions);
    savePicker->DefaultFileExtension = ".bmp";
    savePicker->SuggestedFileName = "SaveScreen";
    savePicker->SuggestedStartLocation = Pickers::PickerLocationId::PicturesLibrary;

    create_task(savePicker->PickSaveFileAsync()).then([=](StorageFile^ file)
    {
        if (file == nullptr)
        {
            // If user clicks "Cancel", reset the saving state, then cancel the current task.
            cancel_current_task();
        }

        return file->OpenAsync(FileAccessMode::ReadWrite);

    }).then([=](Streams::IRandomAccessStream^ randomAccessStream)
    {
		randomAccessStream->WriteAsync(impl->imageBuffer);
	});
}

#pragma region PrivateFunctions

Graphics::Private::Private(DeviceType renderDevice) :
m_screenViewport(),
m_d3dFeatureLevel(D3D_FEATURE_LEVEL_9_1),
m_d3dRenderTargetSize(),
m_outputSize(),
m_logicalSize(),
m_nativeOrientation(DisplayOrientations::None),
m_currentOrientation(DisplayOrientations::None),
m_dpi(-1.0f),
m_compositionScaleX(1.0f),
m_compositionScaleY(1.0f),
m_deviceNotify(nullptr),

CurrentlyDrawing(false),
FrameCount(0),
RenderDevice(renderDevice),

SISInitialized(false)
{
	CreateDeviceIndependentResources();
	if (RenderDevice == DeviceType::SwapChainPanel)
	{
		CreateDeviceResources();
	}
	
}

void Graphics::Private::DrawPath(Pen^ pen, GraphicsPath^ path, bool closed)
{
	BeginDraw();

	// Note that the m_orientationTransform2D matrix is post-multiplied here
	// in order to correctly orient the text to match the display orientation.
	// This post-multiplication step is required for any draw calls that are
	// made to the swap chain's target bitmap. For draw calls to other targets,
	// this transform should not be applied.
	if (RenderDevice == DeviceType::SwapChainPanel)
	{
		m_d2dContext->SetTransform(m_orientationTransform2D);
	}


	ComPtr<ID2D1Brush> brush = D2DUtils::BrushToID2D1Brush(m_d2dContext, m_wicFactory, pen->Brush);

	ComPtr<ID2D1StrokeStyle> m_pStrokeStyleCustomOffsetZero;

	DX::ThrowIfFailed(
		m_d2dFactory->CreateStrokeStyle(
		D2D1::StrokeStyleProperties(
		D2D1_CAP_STYLE_FLAT,
		D2D1_CAP_STYLE_FLAT,
		D2D1_CAP_STYLE_ROUND,
		D2D1_LINE_JOIN_MITER,
		10.0f,
		D2D1_DASH_STYLE_SOLID,
		0.0f),
		NULL,
		0,
		&m_pStrokeStyleCustomOffsetZero
		)
		);

	ComPtr<ID2D1PathGeometry> m_pPathGeometry;
	ComPtr<ID2D1GeometrySink> pSink;
	DX::ThrowIfFailed(
		m_d2dFactory->CreatePathGeometry(&m_pPathGeometry)
		);
	DX::ThrowIfFailed(
		m_pPathGeometry->Open(&pSink)
		);

	const Platform::Array<Point>^ points = path->PathData->Points;
	const Platform::Array<byte>^ types = path->PathData->Types;

	bool FigureEnded = true;
	unsigned int i = 0;
	for (i; i < points->Length; i++)
	{
		if ((types[i] & (byte)PathPointType::PathTypeMask) == (byte)PathPointType::Start)
		{
			if (!FigureEnded)
			{
				if (!closed)
				{
					pSink->EndFigure(D2D1_FIGURE_END_OPEN);
				}
				else
				{
					pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
				}
				FigureEnded = true;
			}
			pSink->BeginFigure(Point2F(points[i].X, points[i].Y), D2D1_FIGURE_BEGIN_HOLLOW);
			FigureEnded = false;
		}
		else if ((types[i] & (byte)PathPointType::PathTypeMask) == (byte)PathPointType::Line){
			pSink->AddLine(Point2F(points[i].X, points[i].Y));
		}
		else if ((types[i] & (byte)PathPointType::PathTypeMask) == (byte)PathPointType::Bezier)
		{
			if (i + 2 < points->Length)
			{
				pSink->AddBezier(D2D1::BezierSegment(Point2F(points[i].X, points[i].Y), Point2F(points[i + 1].X, points[i + 1].Y), Point2F(points[i + 2].X, points[i + 2].Y)));
				i += 2;
			}
			else{
				DX::ThrowIfFailed(D2DERR_UNSUPPORTED_OPERATION);
			}
		}
		if ((types[i] & (byte)PathPointType::CloseSubpath) == (byte)PathPointType::CloseSubpath)
		{
			if (!closed)
			{
				pSink->EndFigure(D2D1_FIGURE_END_OPEN);
			}
			else
			{
				pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
			}
			FigureEnded = true;
		}
	}
	
	if (!FigureEnded)
	{
		if (!closed)
		{
			pSink->EndFigure(D2D1_FIGURE_END_OPEN);
		}
		else
		{
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		}
		FigureEnded = true;
	}
	
	DX::ThrowIfFailed(
		pSink->Close()
		);

	m_d2dContext->DrawGeometry(m_pPathGeometry.Get(), brush.Get(), pen->Width, m_pStrokeStyleCustomOffsetZero.Get());

}

void Graphics::Private::DrawString(String^ s, Font^ font, ComPtr<ID2D1Brush> brush, D2D1_RECT_F layoutRectangle, DWRITE_TEXT_ALIGNMENT horizontalAlignment, DWRITE_PARAGRAPH_ALIGNMENT verticalAlignment)
{
	BeginDraw();

	// Note that the m_orientationTransform2D matrix is post-multiplied here
	// in order to correctly orient the text to match the display orientation.
	// This post-multiplication step is required for any draw calls that are
	// made to the swap chain's target bitmap. For draw calls to other targets,
	// this transform should not be applied.
	if (RenderDevice == DeviceType::SwapChainPanel)
	{
		m_d2dContext->SetTransform(m_orientationTransform2D);
	}
	
	ComPtr<IDWriteTextFormat> m_pTextFormat;

	DX::ThrowIfFailed(
        m_dwriteFactory->CreateTextFormat(
			font->FontFamily->Source->Data(),
            NULL,
			(DWRITE_FONT_WEIGHT)font->Weight.Weight,
			(DWRITE_FONT_STYLE)font->Style,
			(DWRITE_FONT_STRETCH)font->Stretch,
			font->Size,
            L"", //locale
            &m_pTextFormat
        )
	);

	//Align to left
	m_pTextFormat->SetTextAlignment(horizontalAlignment);
	//Align to top
	m_pTextFormat->SetParagraphAlignment(verticalAlignment);

	m_d2dContext->DrawText(
		s->Data(),
		s->Length(),
		m_pTextFormat.Get(),
		layoutRectangle,
		brush.Get()
	);
}

// Save render target bitmap to a stream using WIC.
void Graphics::Private::ToStream(_In_ REFGUID wicFormat, _In_ IStream* stream)
{
    // Create and initialize WIC Bitmap Encoder.
    ComPtr<IWICBitmapEncoder> wicBitmapEncoder;
    DX::ThrowIfFailed(
		m_wicFactory->CreateEncoder(
            wicFormat,
            nullptr,    // No preferred codec vendor.
            &wicBitmapEncoder
            )
        );

    DX::ThrowIfFailed(
        wicBitmapEncoder->Initialize(
            stream,
            WICBitmapEncoderNoCache
            )
        );

    // Create and initialize WIC Frame Encoder.
    ComPtr<IWICBitmapFrameEncode> wicFrameEncode;
    DX::ThrowIfFailed(
        wicBitmapEncoder->CreateNewFrame(
            &wicFrameEncode,
            nullptr     // No encoder options.
            )
        );

    DX::ThrowIfFailed(
        wicFrameEncode->Initialize(nullptr)
        );

    // Retrieve D2D Device.
    ComPtr<ID2D1Device> d2dDevice;

	m_d2dContext->GetDevice(&d2dDevice);

    // Create IWICImageEncoder.
    ComPtr<IWICImageEncoder> imageEncoder;
    DX::ThrowIfFailed(
		m_wicFactory->CreateImageEncoder(
            d2dDevice.Get(),
            &imageEncoder
            )
        );

	WICImageParameters imageEncoderParameters;
	imageEncoderParameters.PixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
	imageEncoderParameters.DpiX = m_dpi;
	imageEncoderParameters.DpiY = m_dpi;
	imageEncoderParameters.Left = static_cast<float>(offset.x);
	imageEncoderParameters.Top = static_cast<float>(offset.y);
	imageEncoderParameters.PixelWidth = static_cast<unsigned int>(UpdateRect.Width);
	imageEncoderParameters.PixelHeight = static_cast<unsigned int>(UpdateRect.Height);
	
    DX::ThrowIfFailed(
        imageEncoder->WriteFrame(
			m_d2dTargetBitmap.Get(),
            wicFrameEncode.Get(),
			&imageEncoderParameters     // Use default WICImageParameter options.
            )
        );

    DX::ThrowIfFailed(
        wicFrameEncode->Commit()
        );

    DX::ThrowIfFailed(
        wicBitmapEncoder->Commit()
        );

    // Flush all memory buffers to the next-level storage object.
    DX::ThrowIfFailed(
        stream->Commit(STGC_DEFAULT)
        );
}

#pragma region DirectXBase.cpp

// Constants used to calculate screen rotations.
namespace ScreenRotation
{
	// 0-degree Z-rotation
	static const XMFLOAT4X4 Rotation0(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);

	// 90-degree Z-rotation
	static const XMFLOAT4X4 Rotation90(
		0.0f, 1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);

	// 180-degree Z-rotation
	static const XMFLOAT4X4 Rotation180(
		-1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);

	// 270-degree Z-rotation
	static const XMFLOAT4X4 Rotation270(
		0.0f, -1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
};

// Configures resources that don't depend on the Direct3D device.
void Graphics::Private::CreateDeviceIndependentResources()
{
	// Initialize Direct2D resources.
	D2D1_FACTORY_OPTIONS options;
	ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
	// If the project is in a debug build, enable Direct2D debugging via SDK Layers.
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

	// Initialize the Direct2D Factory.
	DX::ThrowIfFailed(
		D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory2),
		&options,
		&m_d2dFactory
		)
		);

	// Initialize the DirectWrite Factory.
	DX::ThrowIfFailed(
		DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory2),
		&m_dwriteFactory
		)
		);

	// Initialize the Windows Imaging Component (WIC) Factory.
	DX::ThrowIfFailed(
		CoCreateInstance(
		CLSID_WICImagingFactory2,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_wicFactory)
		)
		);
}

// Configures the Direct3D device, and stores handles to it and the device context.
void Graphics::Private::CreateDeviceResources()
{
	// This flag adds support for surfaces with a different color channel ordering
	// than the API default. It is required for compatibility with Direct2D.
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	if (DX::SdkLayersAvailable())
	{
		// If the project is in a debug build, enable debugging via SDK Layers with this flag.
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	}
#endif

	// This array defines the set of DirectX hardware feature levels this app will support.
	// Note the ordering should be preserved.
	// Don't forget to declare your application's minimum required feature level in its
	// description.  All applications are assumed to support 9.1 unless otherwise stated.
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	// Create the Direct3D 11 API device object and a corresponding context.
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;

	HRESULT hr = D3D11CreateDevice(
		nullptr,					// Specify nullptr to use the default adapter.
		D3D_DRIVER_TYPE_HARDWARE,	// Create a device using the hardware graphics driver.
		0,							// Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
		creationFlags,				// Set debug and Direct2D compatibility flags.
		featureLevels,				// List of feature levels this app can support.
		ARRAYSIZE(featureLevels),	// Size of the list above.
		D3D11_SDK_VERSION,			// Always set this to D3D11_SDK_VERSION for Windows Store apps.
		&device,					// Returns the Direct3D device created.
		&m_d3dFeatureLevel,			// Returns feature level of device created.
		&context					// Returns the device immediate context.
		);

	if (FAILED(hr))
	{
		// If the initialization fails, fall back to the WARP device.
		// For more information on WARP, see: 
		// http://go.microsoft.com/fwlink/?LinkId=286690
		DX::ThrowIfFailed(
			D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_WARP, // Create a WARP device instead of a hardware device.
			0,
			creationFlags,
			featureLevels,
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION,
			&device,
			&m_d3dFeatureLevel,
			&context
			)
			);
	}

	// Store pointers to the Direct3D 11.1 API device and immediate context.
	DX::ThrowIfFailed(
		device.As(&m_d3dDevice)
		);

	DX::ThrowIfFailed(
		context.As(&m_d3dContext)
		);

	// Create the Direct2D device object and a corresponding context.
	ComPtr<IDXGIDevice3> dxgiDevice;
	DX::ThrowIfFailed(
		m_d3dDevice.As(&dxgiDevice)
		);

	DX::ThrowIfFailed(
		m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice)
		);

	DX::ThrowIfFailed(
		m_d2dDevice->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
		&m_d2dContext
		)
		);

	if (RenderDevice == DeviceType::SurfaceImageSource)
	{
		// Set DPI to the display's current DPI.
		SetDpi(DisplayInformation::GetForCurrentView()->LogicalDpi);

		// Query for ISurfaceImageSourceNative interface.
		Microsoft::WRL::ComPtr<ISurfaceImageSourceNative> sisNative;
		DX::ThrowIfFailed(
			reinterpret_cast<IUnknown*>(m_surfaceImageSource)->QueryInterface(IID_PPV_ARGS(&sisNative))
			);

		// Associate the DXGI device with the SurfaceImageSource.
		DX::ThrowIfFailed(
			sisNative->SetDevice(dxgiDevice.Get())
			);
	}
}

// These resources need to be recreated every time the window size is changed.
void Graphics::Private::CreateWindowSizeDependentResources()
{
	// Clear the previous window size specific context.
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	m_d3dContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
	m_d3dRenderTargetView = nullptr;
	m_d2dContext->SetTarget(nullptr);
	m_d2dTargetBitmap = nullptr;
	m_d3dDepthStencilView = nullptr;
	m_d3dContext->Flush();

	// Calculate the necessary swap chain and render target size in pixels.
	m_outputSize.Width = m_logicalSize.Width * m_compositionScaleX;
	m_outputSize.Height = m_logicalSize.Height * m_compositionScaleY;

	// Prevent zero size DirectX content from being created.
	m_outputSize.Width = max(m_outputSize.Width, 1);
	m_outputSize.Height = max(m_outputSize.Height, 1);

	// The width and height of the swap chain must be based on the window's
	// natively-oriented width and height. If the window is not in the native
	// orientation, the dimensions must be reversed.
	DXGI_MODE_ROTATION displayRotation = ComputeDisplayRotation();

	bool swapDimensions = displayRotation == DXGI_MODE_ROTATION_ROTATE90 || displayRotation == DXGI_MODE_ROTATION_ROTATE270;
	m_d3dRenderTargetSize.Width = swapDimensions ? m_outputSize.Height : m_outputSize.Width;
	m_d3dRenderTargetSize.Height = swapDimensions ? m_outputSize.Width : m_outputSize.Height;

	if (m_swapChain != nullptr)
	{
		// If the swap chain already exists, resize it.
		HRESULT hr = m_swapChain->ResizeBuffers(
			2, // Double-buffered swap chain.
			static_cast<UINT>(m_d3dRenderTargetSize.Width),
			static_cast<UINT>(m_d3dRenderTargetSize.Height),
			DXGI_FORMAT_B8G8R8A8_UNORM,
			0
			);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			// If the device was removed for any reason, a new device and swap chain will need to be created.
			HandleDeviceLost();

			// Everything is set up now. Do not continue execution of this method. HandleDeviceLost will reenter this method 
			// and correctly set up the new device.
			return;
		}
		else
		{
			DX::ThrowIfFailed(hr);
		}
	}
	else
	{
		// Otherwise, create a new one using the same adapter as the existing Direct3D device.
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };

		swapChainDesc.Width = static_cast<UINT>(m_d3dRenderTargetSize.Width); // Match the size of the window.
		swapChainDesc.Height = static_cast<UINT>(m_d3dRenderTargetSize.Height);
		swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // This is the most common swap chain format.
		swapChainDesc.Stereo = false;
		swapChainDesc.SampleDesc.Count = 1; // Don't use multi-sampling.
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2; // Use double-buffering to minimize latency.
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // All Windows Store apps must use this SwapEffect.
		swapChainDesc.Flags = 0;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

		// This sequence obtains the DXGI factory that was used to create the Direct3D device above.
		ComPtr<IDXGIDevice3> dxgiDevice;
		DX::ThrowIfFailed(
			m_d3dDevice.As(&dxgiDevice)
			);

		ComPtr<IDXGIAdapter> dxgiAdapter;
		DX::ThrowIfFailed(
			dxgiDevice->GetAdapter(&dxgiAdapter)
			);

		ComPtr<IDXGIFactory2> dxgiFactory;
		DX::ThrowIfFailed(
			dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory))
			);

		// When using XAML interop, the swap chain must be created for composition.
		DX::ThrowIfFailed(
			dxgiFactory->CreateSwapChainForComposition(
			m_d3dDevice.Get(),
			&swapChainDesc,
			nullptr,
			&m_swapChain
			)
			);

		// Associate swap chain with SwapChainPanel
		// UI changes will need to be dispatched back to the UI thread
		m_swapChainPanel->Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([=]()
		{
			// Get backing native interface for SwapChainPanel
			ComPtr<ISwapChainPanelNative> panelNative;
			DX::ThrowIfFailed(
				reinterpret_cast<IUnknown*>(m_swapChainPanel)->QueryInterface(IID_PPV_ARGS(&panelNative))
				);

			DX::ThrowIfFailed(
				panelNative->SetSwapChain(m_swapChain.Get())
				);
		}, CallbackContext::Any));

		// Ensure that DXGI does not queue more than one frame at a time. This both reduces latency and
		// ensures that the application will only render after each VSync, minimizing power consumption.
		DX::ThrowIfFailed(
			dxgiDevice->SetMaximumFrameLatency(1)
			);
	}

	// Set the proper orientation for the swap chain, and generate 2D and
	// 3D matrix transformations for rendering to the rotated swap chain.
	// Note the rotation angle for the 2D and 3D transforms are different.
	// This is due to the difference in coordinate spaces.  Additionally,
	// the 3D matrix is specified explicitly to avoid rounding errors.

	switch (displayRotation)
	{
	case DXGI_MODE_ROTATION_IDENTITY:
		m_orientationTransform2D = Matrix3x2F::Identity();
		m_orientationTransform3D = ScreenRotation::Rotation0;
		break;

	case DXGI_MODE_ROTATION_ROTATE90:
		m_orientationTransform2D =
			Matrix3x2F::Rotation(90.0f) *
			Matrix3x2F::Translation(m_logicalSize.Height, 0.0f);
		m_orientationTransform3D = ScreenRotation::Rotation270;
		break;

	case DXGI_MODE_ROTATION_ROTATE180:
		m_orientationTransform2D =
			Matrix3x2F::Rotation(180.0f) *
			Matrix3x2F::Translation(m_logicalSize.Width, m_logicalSize.Height);
		m_orientationTransform3D = ScreenRotation::Rotation180;
		break;

	case DXGI_MODE_ROTATION_ROTATE270:
		m_orientationTransform2D =
			Matrix3x2F::Rotation(270.0f) *
			Matrix3x2F::Translation(0.0f, m_logicalSize.Width);
		m_orientationTransform3D = ScreenRotation::Rotation90;
		break;

	default:
		throw ref new FailureException();
	}

	DX::ThrowIfFailed(
		m_swapChain->SetRotation(displayRotation)
		);

	// Setup inverse scale on the swap chain
	DXGI_MATRIX_3X2_F inverseScale = { 0 };
	inverseScale._11 = 1.0f / m_compositionScaleX;
	inverseScale._22 = 1.0f / m_compositionScaleY;
	ComPtr<IDXGISwapChain2> spSwapChain2;
	DX::ThrowIfFailed(
		m_swapChain.As<IDXGISwapChain2>(&spSwapChain2)
		);

	DX::ThrowIfFailed(
		spSwapChain2->SetMatrixTransform(&inverseScale)
		);


	// Create a render target view of the swap chain back buffer.
	ComPtr<ID3D11Texture2D> backBuffer;
	DX::ThrowIfFailed(
		m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer))
		);

	DX::ThrowIfFailed(
		m_d3dDevice->CreateRenderTargetView(
		backBuffer.Get(),
		nullptr,
		&m_d3dRenderTargetView
		)
		);

	// Create a depth stencil view for use with 3D rendering if needed.
	CD3D11_TEXTURE2D_DESC depthStencilDesc(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		static_cast<UINT>(m_d3dRenderTargetSize.Width),
		static_cast<UINT>(m_d3dRenderTargetSize.Height),
		1, // This depth stencil view has only one texture.
		1, // Use a single mipmap level.
		D3D11_BIND_DEPTH_STENCIL
		);

	ComPtr<ID3D11Texture2D> depthStencil;
	DX::ThrowIfFailed(
		m_d3dDevice->CreateTexture2D(
		&depthStencilDesc,
		nullptr,
		&depthStencil
		)
		);

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	DX::ThrowIfFailed(
		m_d3dDevice->CreateDepthStencilView(
		depthStencil.Get(),
		&depthStencilViewDesc,
		&m_d3dDepthStencilView
		)
		);

	// Set the 3D rendering viewport to target the entire window.
	m_screenViewport = CD3D11_VIEWPORT(
		0.0f,
		0.0f,
		m_d3dRenderTargetSize.Width,
		m_d3dRenderTargetSize.Height
		);

	m_d3dContext->RSSetViewports(1, &m_screenViewport);

	// Create a Direct2D target bitmap associated with the
	// swap chain back buffer and set it as the current target.
	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
		m_dpi,
		m_dpi
		);

	ComPtr<IDXGISurface2> dxgiBackBuffer;
	DX::ThrowIfFailed(
		m_swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer))
		);

	DX::ThrowIfFailed(
		m_d2dContext->CreateBitmapFromDxgiSurface(
		dxgiBackBuffer.Get(),
		&bitmapProperties,
		&m_d2dTargetBitmap
		)
		);

	m_d2dContext->SetTarget(m_d2dTargetBitmap.Get());

	// Grayscale text anti-aliasing is recommended for all Windows Store apps.
	m_d2dContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
}

// This method is called when the XAML control is created (or re-created).
void Graphics::Private::SetSwapChainPanel(SwapChainPanel^ panel)
{
	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	m_swapChainPanel = panel;
	m_logicalSize = Windows::Foundation::Size(static_cast<float>(panel->ActualWidth), static_cast<float>(panel->ActualHeight));
	m_nativeOrientation = currentDisplayInformation->NativeOrientation;
	m_currentOrientation = currentDisplayInformation->CurrentOrientation;
	m_compositionScaleX = panel->CompositionScaleX;
	m_compositionScaleY = panel->CompositionScaleY;
	m_dpi = currentDisplayInformation->LogicalDpi;
	m_d2dContext->SetDpi(m_dpi, m_dpi);

	CreateWindowSizeDependentResources();
}

// This method is called when the XAML control is created (or re-created).
void Graphics::Private::SetSurfaceImageSource(SurfaceImageSource^ image, Windows::Foundation::Size size)
{
	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();
	m_logicalSize = size;
	m_nativeOrientation = currentDisplayInformation->NativeOrientation;
	m_currentOrientation = currentDisplayInformation->CurrentOrientation;
	m_compositionScaleX = 1.0;
	m_compositionScaleY = 1.0;
	m_dpi = currentDisplayInformation->LogicalDpi;
	
	m_surfaceImageSource = image;

	CreateDeviceResources();
}

// This method is called in the event handler for the SizeChanged event.
void Graphics::Private::SetLogicalSize(Windows::Foundation::Size logicalSize)
{
	if (m_logicalSize != logicalSize)
	{
		m_logicalSize = logicalSize;
		CreateWindowSizeDependentResources();
	}
}

// This method is called in the event handler for the DpiChanged event.
void Graphics::Private::SetDpi(float dpi)
{
	if (dpi != m_dpi)
	{
		m_dpi = dpi;
		m_d2dContext->SetDpi(m_dpi, m_dpi);
		if (RenderDevice == DeviceType::SwapChainPanel)
		{
			CreateWindowSizeDependentResources();
		}
	}
}

// This method is called in the event handler for the OrientationChanged event.
void Graphics::Private::SetCurrentOrientation(DisplayOrientations currentOrientation)
{
	if (m_currentOrientation != currentOrientation)
	{
		m_currentOrientation = currentOrientation;
		CreateWindowSizeDependentResources();
	}
}

// This method is called in the event handler for the CompositionScaleChanged event.
void Graphics::Private::SetCompositionScale(float compositionScaleX, float compositionScaleY)
{
	if (m_compositionScaleX != compositionScaleX ||
		m_compositionScaleY != compositionScaleY)
	{
		m_compositionScaleX = compositionScaleX;
		m_compositionScaleY = compositionScaleY;
		CreateWindowSizeDependentResources();
	}
}

// This method is called in the event handler for the DisplayContentsInvalidated event.
void Graphics::Private::ValidateDevice()
{
	// The D3D Device is no longer valid if the default adapter changed since the device
	// was created or if the device has been removed.

	// First, get the information for the default adapter from when the device was created.

	ComPtr<IDXGIDevice3> dxgiDevice;
	DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

	ComPtr<IDXGIAdapter> deviceAdapter;
	DX::ThrowIfFailed(dxgiDevice->GetAdapter(&deviceAdapter));

	ComPtr<IDXGIFactory2> deviceFactory;
	DX::ThrowIfFailed(deviceAdapter->GetParent(IID_PPV_ARGS(&deviceFactory)));

	ComPtr<IDXGIAdapter1> previousDefaultAdapter;
	DX::ThrowIfFailed(deviceFactory->EnumAdapters1(0, &previousDefaultAdapter));

	DXGI_ADAPTER_DESC previousDesc;
	DX::ThrowIfFailed(previousDefaultAdapter->GetDesc(&previousDesc));

	// Next, get the information for the current default adapter.

	ComPtr<IDXGIFactory2> currentFactory;
	DX::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&currentFactory)));

	ComPtr<IDXGIAdapter1> currentDefaultAdapter;
	DX::ThrowIfFailed(currentFactory->EnumAdapters1(0, &currentDefaultAdapter));

	DXGI_ADAPTER_DESC currentDesc;
	DX::ThrowIfFailed(currentDefaultAdapter->GetDesc(&currentDesc));

	// If the adapter LUIDs don't match, or if the device reports that it has been removed,
	// a new D3D device must be created.

	if (previousDesc.AdapterLuid.LowPart != currentDesc.AdapterLuid.LowPart ||
		previousDesc.AdapterLuid.HighPart != currentDesc.AdapterLuid.HighPart ||
		FAILED(m_d3dDevice->GetDeviceRemovedReason()))
	{
		// Release references to resources related to the old device.
		dxgiDevice = nullptr;
		deviceAdapter = nullptr;
		deviceFactory = nullptr;
		previousDefaultAdapter = nullptr;

		// Create a new device and swap chain.
		HandleDeviceLost();
	}
}

// Recreate all device resources and set them back to the current state.
void Graphics::Private::HandleDeviceLost()
{
	m_swapChain = nullptr;

	if (m_deviceNotify != nullptr)
	{
		m_deviceNotify->OnDeviceLost();
	}

	CreateDeviceResources();
	m_d2dContext->SetDpi(m_dpi, m_dpi);
	CreateWindowSizeDependentResources();

	if (m_deviceNotify != nullptr)
	{
		m_deviceNotify->OnDeviceRestored();
	}
}

// Register our DeviceNotify to be informed on device lost and creation.
void Graphics::Private::RegisterDeviceNotify(IDeviceNotify* deviceNotify)
{
	m_deviceNotify = deviceNotify;
}

// Call this method when the app suspends. It provides a hint to the driver that the app 
// is entering an idle state and that temporary buffers can be reclaimed for use by other apps.
void Graphics::Private::Trim()
{
	ComPtr<IDXGIDevice3> dxgiDevice;
	m_d3dDevice.As(&dxgiDevice);

	dxgiDevice->Trim();
}

// Present the contents of the swap chain to the screen.
void Graphics::Private::Present()
{
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	HRESULT hr = m_swapChain->Present(1, 0);

	// Discard the contents of the render target.
	// This is a valid operation only when the existing contents will be entirely
	// overwritten. If dirty or scroll rects are used, this call should be removed.
	m_d3dContext->DiscardView(m_d3dRenderTargetView.Get());

	// Discard the contents of the depth stencil.
	m_d3dContext->DiscardView(m_d3dDepthStencilView.Get());

	// If the device was removed either by a disconnection or a driver upgrade, we 
	// must recreate all device resources.
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		HandleDeviceLost();
	}
	else
	{
		DX::ThrowIfFailed(hr);
	}
}

// This method determines the rotation between the display device's native Orientation and the
// current display orientation.
DXGI_MODE_ROTATION Graphics::Private::ComputeDisplayRotation()
{
	DXGI_MODE_ROTATION rotation = DXGI_MODE_ROTATION_UNSPECIFIED;

	// Note: NativeOrientation can only be Landscape or Portrait even though
	// the DisplayOrientations enum has other values.
	switch (m_nativeOrientation)
	{
	case DisplayOrientations::Landscape:
		switch (m_currentOrientation)
		{
		case DisplayOrientations::Landscape:
			rotation = DXGI_MODE_ROTATION_IDENTITY;
			break;

		case DisplayOrientations::Portrait:
			rotation = DXGI_MODE_ROTATION_ROTATE270;
			break;

		case DisplayOrientations::LandscapeFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE180;
			break;

		case DisplayOrientations::PortraitFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE90;
			break;
		}
		break;

	case DisplayOrientations::Portrait:
		switch (m_currentOrientation)
		{
		case DisplayOrientations::Landscape:
			rotation = DXGI_MODE_ROTATION_ROTATE90;
			break;

		case DisplayOrientations::Portrait:
			rotation = DXGI_MODE_ROTATION_IDENTITY;
			break;

		case DisplayOrientations::LandscapeFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE270;
			break;

		case DisplayOrientations::PortraitFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE180;
			break;
		}
		break;
	}
	return rotation;
}

#pragma endregion DirectXBase.cpp

#pragma region SurfaceImageSource

// Begins drawing, allowing updates to content in the specified area.
void Graphics::Private::BeginDraw(Windows::Foundation::Rect updateRect)
{
	if (!CurrentlyDrawing)
	{
		CurrentlyDrawing = true;
		UpdateRect = updateRect;

		if (RenderDevice == DeviceType::SwapChainPanel)
		{
			offset.x = 0;
			offset.y = 0;

			m_d2dContext->BeginDraw();
		}
		else
		{
			SurfaceImageSourceIntialize(updateRect);

			// Begin drawing using D2D context.
			m_d2dContext->BeginDraw();

			// Apply a clip and transform to constrain updates to the target update area.
			// This is required to ensure coordinates within the target surface remain
			// consistent by taking into account the offset returned by BeginDraw, and
			// can also improve performance by optimizing the area that is drawn by D2D.
			// Apps should always account for the offset output parameter returned by 
			// BeginDraw, since it may not match the passed updateRect input parameter's location.
			m_d2dContext->PushAxisAlignedClip(
				D2D1::RectF(
				static_cast<float>(offset.x),
				static_cast<float>(offset.y),
				static_cast<float>(offset.x + updateRect.Width),
				static_cast<float>(offset.y + updateRect.Height)
				),
				D2D1_ANTIALIAS_MODE_ALIASED
				);

			m_d2dContext->SetTransform(
				D2D1::Matrix3x2F::Translation(
				static_cast<float>(offset.x),
				static_cast<float>(offset.y)
				)
				);
			
		}
	}
}

// Ends drawing updates started by a previous BeginDraw call.
HRESULT Graphics::Private::EndDraw()
{
	CurrentlyDrawing = false;
	if (RenderDevice == DeviceType::SwapChainPanel)
	{
		return m_d2dContext->EndDraw();
	}
	else
	{
		// Remove the transform and clip applied in BeginDraw since
		// the target area can change on every update.
		m_d2dContext->SetTransform(D2D1::IdentityMatrix());
		m_d2dContext->PopAxisAlignedClip();

		// Remove the render target and end drawing.
		return m_d2dContext->EndDraw();
	}
}

void Graphics::Private::SurfaceImageSourceIntialize(Windows::Foundation::Rect updateRect)
{
	if (!SISInitialized)
	{
		ComPtr<IDXGISurface> surface;

		// Express target area as a native RECT type.
		RECT updateRectNative;
		updateRectNative.left = static_cast<LONG>(updateRect.Left);
		updateRectNative.top = static_cast<LONG>(updateRect.Top);
		updateRectNative.right = static_cast<LONG>(updateRect.Right);
		updateRectNative.bottom = static_cast<LONG>(updateRect.Bottom);

		// Query for ISurfaceImageSourceNative interface.
		Microsoft::WRL::ComPtr<ISurfaceImageSourceNative> sisNative;
		DX::ThrowIfFailed(
			reinterpret_cast<IUnknown*>(m_surfaceImageSource)->QueryInterface(IID_PPV_ARGS(&sisNative))
			);

		// Begin drawing - returns a target surface and an offset to use as the top left origin when drawing.
		HRESULT beginDrawHR = sisNative->BeginDraw(updateRectNative, &surface, &offset);

		if (SUCCEEDED(beginDrawHR))
		{
			// Create render target.
			//ComPtr<ID2D1Bitmap1> bitmap;
			DX::ThrowIfFailed(
				m_d2dContext->CreateBitmapFromDxgiSurface(
				surface.Get(),
				nullptr,
				&m_d2dTargetBitmap
				)
				);

			// Set context's render target.

			m_d2dContext->SetTarget(m_d2dTargetBitmap.Get());

			SISInitialized = true;
		}
		else if (beginDrawHR == DXGI_ERROR_DEVICE_REMOVED || beginDrawHR == DXGI_ERROR_DEVICE_RESET)
		{
			// If the device has been removed or reset, attempt to recreate it and continue drawing.
			CreateDeviceResources();
			SurfaceImageSourceIntialize(updateRect);
		}
		else
		{
			// Notify the caller by throwing an exception if any other error was encountered.
			DX::ThrowIfFailed(beginDrawHR);
		}
	}
}

void Graphics::Private::SurfaceImageSourcePresent()
{
	m_d2dContext->SetTarget(nullptr);

	// Query for ISurfaceImageSourceNative interface.
	Microsoft::WRL::ComPtr<ISurfaceImageSourceNative> sisNative;
	DX::ThrowIfFailed(
		reinterpret_cast<IUnknown*>(m_surfaceImageSource)->QueryInterface(IID_PPV_ARGS(&sisNative))
		);

	DX::ThrowIfFailed(
		sisNative->EndDraw()
		);

	SISInitialized = false;
}

#pragma endregion SurfaceImageSource

#pragma endregion PrivateFunctions
