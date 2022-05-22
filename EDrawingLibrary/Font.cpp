#include "pch.h"
#include "Font.h"

using namespace ModernComponents::System::Drawing;

using namespace Platform;
using namespace Windows::UI::Text;
namespace M = Windows::UI::Xaml::Media;

#pragma region Private

class Font::Private
{
public:
	Private();

	Windows::UI::Xaml::Media::FontFamily^ fontFamily;
	Windows::UI::Text::FontStyle style;
	Windows::UI::Text::FontWeight weight;
	Windows::UI::Text::FontStretch stretch;

	float size;
};

#pragma endregion Private

/** <summary>
		Creates a new instance of the <see cref="Font"/> class with the specified size, style, weight and stretch options.
	</summary>
	<param name="family">
		<para>The <see cref="M::FontFamily"/> of the new <see cref="Font"/>.</para>
	</param>
	<param name="emSize">
		<para>The size of the new font.</para>
	</param>
	<param name="style">
		<para>The style of the new font.</para>
	</param>
	<param name="weight">
		<para>The weight of the new font.</para>
	</param>
	<param name="stretch">
		<para>The stretch options of the new font.</para>
	</param>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
	</example>
*/
Font::Font(M::FontFamily^ family, float emSize, FontStyle style, FontWeight weight, FontStretch stretch) : impl(new Private())
{
	impl->fontFamily = family;
	impl->size = emSize;
	impl->style = style;
	impl->weight = weight;
	impl->stretch = stretch;
}

/** <summary>
		Creates a new instance of the <see cref="Font"/> class with the specified size, style, weight and stretch options.
	</summary>
	<param name="familyName">
		<para>A string representation of the <see cref="M::FontFamily"/> for the new <see cref="Font"/>.</para>
	</param>
	<param name="emSize">
		<para>The size of the new font.</para>
	</param>
	<param name="style">
		<para>The style of the new font.</para>
	</param>
	<param name="weight">
		<para>The weight of the new font.</para>
	</param>
	<param name="stretch">
		<para>The stretch options of the new font.</para>
	</param>
	<remarks>
		<para>The familyName must be a font installed on the system.</para>
	</remarks>
	<example>
		<code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
		<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
	</example>
*/
Font::Font(String^ familyName, float emSize, FontStyle style, FontWeight weight, FontStretch stretch) : impl(new Private())
{
	impl->fontFamily = ref new M::FontFamily(familyName);
	impl->size = emSize;
	impl->style = style;
	impl->weight = weight;
	impl->stretch = stretch;
}

Font::~Font()
{
}

M::FontFamily^ Font::FontFamily::get()
{
	return impl->fontFamily;
}

FontStyle Font::Style::get()
{
	return impl->style;
}

FontWeight Font::Weight::get()
{
	return impl->weight;
}

FontStretch Font::Stretch::get()
{
	return impl->stretch;
}

float Font::Size::get()
{
	return impl->size;
}

#pragma region PrivateFunctions

Font::Private::Private()
{
}

#pragma endregion PrivateFunctions