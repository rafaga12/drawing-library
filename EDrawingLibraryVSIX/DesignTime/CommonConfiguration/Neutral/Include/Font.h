#pragma once
namespace ModernComponents
{
	namespace System
	{
		namespace Drawing
		{
			/** <summary>
			        Defines a particular format for text, including font face, size, and style attributes.
			    </summary>
			    <remarks>
			        <para>
			            Windows Store apps only support built in fonts. If you attempt to use a font that is not supported, or the font is 
						not installed on the machine that is running the application, then Segoe UI font will be used instead.
			        </para>
			    </remarks>
			    <example>
			        <code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
					<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
			    </example>
			*/
			[Windows::Foundation::Metadata::WebHostHidden]
			public ref class Font sealed
			{
			public:
				[Windows::Foundation::Metadata::DefaultOverload]
				Font(Windows::UI::Xaml::Media::FontFamily^ family, float emSize, Windows::UI::Text::FontStyle style, Windows::UI::Text::FontWeight weight, Windows::UI::Text::FontStretch stretch);
				Font(Platform::String^ familyName, float emSize, Windows::UI::Text::FontStyle style, Windows::UI::Text::FontWeight weight, Windows::UI::Text::FontStretch stretch);
				virtual ~Font();

				/** <summary>
				        Gets the <see cref="Windows::UI::Xaml::Media::FontFamily"/> associated with this <see cref="Font"/>.
				    </summary>
				    <value>
				        <para>Type: <see cref="Windows::UI::Xaml::Media::FontFamily"/></para>
				        <para>The <see cref="Windows::UI::Xaml::Media::FontFamily"/> associated with this <see cref="Font"/>.</para>
				    </value>
				    <remarks>
				        <para>A <see cref="Windows::UI::Xaml::Media::FontFamily"/> represents a group of fonts that have a similar font 
						face, but may have different sizes and styles.</para>
				    </remarks>
				    <example>
				        <code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
						<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
				    </example>
				*/
				property Windows::UI::Xaml::Media::FontFamily^ FontFamily
				{
					Windows::UI::Xaml::Media::FontFamily^ get();
				}

				/** <summary>
				        Gets the style information of this <see cref="Font"/>.
				    </summary>
				    <value>
				        <para>Type: <see cref="Windows::UI::Text::FontStyle"/></para>
				        <para>A <see cref="Windows::UI::Text::FontStyle"/> enumeration that contains the style information of this 
						<see cref="Font"/>.</para>
				    </value>
				    <remarks>
				        <para>At difference from GDI+ code, the FontStyle property does not contain information about the weight or 
						stretch format of the Font, this information was moved to their respective properties.</para>
				    </remarks>
				    <example>
				        <code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
						<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
				    </example>
				*/
				property Windows::UI::Text::FontStyle Style
				{
					Windows::UI::Text::FontStyle get();
				}

				/** <summary>
				        Gets the weight information of this <see cref="Font"/>.
				    </summary>
				    <value>
				        <para>Type: <see cref="Windows::UI::Text::FontWeight"/></para>
				        <para>A <see cref="Windows::UI::Text::FontWeight"/> enumeration that contains the weight information of this 
						<see cref="Font"/>.</para>
				    </value>
				    <remarks>
				        <para>This property doesn't exist on GDI+ code, and was added to have separate information about the style, 
						weight and stretch format of the font.</para>
				    </remarks>
				    <example>
				        <code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
						<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
				    </example>
				*/
				property Windows::UI::Text::FontWeight Weight
				{
					Windows::UI::Text::FontWeight get();
				}

				/** <summary>
				        Gets the stretch information of this <see cref="Font"/>.
				    </summary>
				    <value>
				        <para>Type: <see cref="Windows::UI::Text::FontStretch"/></para>
				        <para>A <see cref="Windows::UI::Text::FontStretch"/> enumeration that contains the stretch information of this 
						<see cref="Font"/>.</para>
				    </value>
				    <remarks>
				        <para>This property doesn't exist on GDI+ code, and was added to have separate information about the style, 
						weight and stretch format of the font.</para>
				    </remarks>
				    <example>
				        <code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
						<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
				    </example>
				*/
				property Windows::UI::Text::FontStretch Stretch
				{
					Windows::UI::Text::FontStretch get();
				}

				/** <summary>
				        Gets the size of this <see cref="Font"/> measured in points.
				    </summary>
				    <value>
				        <para>The size of this <see cref="Font"/>.</para>
				    </value>
				    <remarks>
				        <para>The size of the font is measured in points.</para>
				    </remarks>
				    <example>
				        <code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
						<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
				    </example>
				*/
				property float Size
				{
					float get();
				}
			private:

				class Private;
				std::unique_ptr<Private> impl;

			};
		};
	};
};

