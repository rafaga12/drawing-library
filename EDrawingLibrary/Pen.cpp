#include "pch.h"
#include "Pen.h"

using namespace ModernComponents::System::Drawing;

using namespace Windows;
using namespace Windows::UI;
namespace M = Windows::UI::Xaml::Media;
using namespace Microsoft::WRL;

#pragma region Private

class Pen::Private
{
public:
	Windows::UI::Xaml::Media::Brush^ brush;
	Windows::UI::Color color;
	float width;
};

#pragma endregion Private

/** <summary>
		Creates a new instance of the <see cref="Pen"/> class with the specified <see cref="M::Brush"/>.
	</summary>
	<param name="brush">
		<para>A <see cref="M::Brush"/> that determines the properies of this <see cref="Pen"/>.</para>
	</param>
	<param name="width">
		<para>A float that determines the width of this <see cref="Pen"/>.</para>
	</param>
	<remarks>
		<para>The <see cref="M::Brush"/> property determines how the <see cref="Pen"/> draws lines. 
		The width of the <see cref="Pen"/> is set by the width parameter.</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
	</example>
*/
Pen::Pen(M::Brush^ brush, float width) : impl(new Private())
{
	this->Brush = brush;
	this->Width = width;
}

/** <summary>
		Creates a new instance of the <see cref="Pen"/> class with the specified <see cref="M::Brush"/>.
	</summary>
	<param name="brush">
		<para>A <see cref="M::Brush"/> that determines the properies of this <see cref="Pen"/>.</para>
	</param>
	<remarks>
		<para>The <see cref="M::Brush"/> property determines how the <see cref="Pen"/> draws lines. 
		The width of the <see cref="Pen"/> is set to 1 by default.</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
	</example>
*/
Pen::Pen(M::Brush^ brush) : impl(new Private())
{
	this->Brush = brush;
	this->Width = 1;
}

/** <summary>
		Creates a new instance of the <see cref="Pen"/> class with the specified <see cref="UI::Color"/>.
	</summary>
	<param name="color">
		<para>A <see cref="UI::Color"/> that determines the properies of this <see cref="Pen"/>.</para>
	</param>
	<param name="width">
		<para>A float that determines the width of this <see cref="Pen"/>.</para>
	</param>
	<remarks>
		<para>The <see cref="UI::Color"/> property determines how the <see cref="Pen"/> draws lines. 
		The width of the <see cref="Pen"/> is set by the width parameter.</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
	</example>
*/
Pen::Pen(UI::Color color, float width) : impl(new Private())
{
	this->Color = color;
	this->Width = width;
}

/** <summary>
		Creates a new instance of the <see cref="Pen"/> class with the specified <see cref="UI::Color"/>.
	</summary>
	<param name="color">
		<para>A <see cref="UI::Color"/> that determines the properies of this <see cref="Pen"/>.</para>
	</param>
	<remarks>
		<para>The <see cref="UI::Color"/> property determines how the <see cref="Pen"/> draws lines. 
		The width of the <see cref="Pen"/> is set to 1 by default.</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
	</example>
*/
Pen::Pen(UI::Color color) : impl(new Private())
{
	this->Color = color;
	this->Width = 1;
}

Pen::~Pen()
{
}

M::Brush^ Pen::Brush::get()
{
	return impl->brush;
}

void Pen::Brush::set(M::Brush^ value)
{
	impl->brush = value;
	//Platform::String^ X = brush->GetType()->FullName;
	if(impl->brush->GetType()->FullName->Equals(L"Windows.UI.Xaml.Media.SolidColorBrush"))
	{
		impl->color = ((Windows::UI::Xaml::Media::SolidColorBrush^)value)->Color;
	}else if (impl->brush->GetType()->FullName->Equals(L"Windows.UI.Xaml.Media.GradientBrush") || impl->brush->GetType()->FullName->Equals(L"Windows.UI.Xaml.Media.LinearGradientBrush")) {
		impl->color = ((Windows::UI::Xaml::Media::LinearGradientBrush^)value)->GradientStops->GetAt(0)->Color;
	}else{
		impl->color = Windows::UI::Colors::Black;
	}
}

UI::Color Pen::Color::get()
{
	return impl->color;
}

void Pen::Color::set(UI::Color value)
{
	impl->color = value;
	impl->brush = ref new M::SolidColorBrush(impl->color);
}

float Pen::Width::get()
{
	return impl->width;
}

void Pen::Width::set(float value)
{
	if(value > 0)
	{
		impl->width = value;
	}else{
		impl->width = 1;
	}
}