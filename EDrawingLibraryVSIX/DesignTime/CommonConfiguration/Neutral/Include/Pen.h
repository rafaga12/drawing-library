#pragma once
namespace ModernComponents
{
	namespace System
	{
		namespace Drawing
		{
			/** <summary>
			        Defines an object with the properties of the outline of drawn lines and curves.
			    </summary>
			    <remarks>
			        <para>
			            A Pen draws a line of specified width and style. The line drawn by a Pen can be filled in a variety of fill styles, 
						including solid colors and textures. The fill style depends on brush or texture that is used as the fill object.
			        </para>
			    </remarks>
			    <example>
			        <code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
					<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
			    </example>
			*/
			[Windows::Foundation::Metadata::WebHostHidden]
			public ref class Pen sealed
			{
			public:
				[Windows::Foundation::Metadata::DefaultOverload]
				Pen(Windows::UI::Xaml::Media::Brush^ brush, float width);
				Pen(Windows::UI::Color color, float width);

				[Windows::Foundation::Metadata::DefaultOverload]
				Pen(Windows::UI::Xaml::Media::Brush^ brush);
				Pen(Windows::UI::Color color);

				virtual ~Pen();

				/** <summary>
				        Gets or sets the <see cref="Windows::UI::Xaml::Media::Brush"/> that determines the attributes of this 
						<see cref="Pen"/>.
				    </summary>
				    <value>
				        <para>Type: <see cref="Windows::UI::Xaml::Media::Brush"/></para>
				        <para>A <see cref="Windows::UI::Xaml::Media::Brush"/> that determines the attributes of this 
						<see cref="Pen"/>.</para>
				    </value>
				    <remarks>
				        <para>Assigning this property causes the pen to draw filled lines, it also overrides the <see cref="Color"/> 
						property of the <see cref="Pen"/>.</para>
				    </remarks>
				    <example>
				        <code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
						<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
				    </example>
				*/
				property Windows::UI::Xaml::Media::Brush^ Brush
				{
					Windows::UI::Xaml::Media::Brush^ get();
					void set(Windows::UI::Xaml::Media::Brush^ value);
				}

				/** <summary>
				        Gets or sets the color of this <see cref="Pen"/>.
				    </summary>
				    <value>
				        <para>Type: <see cref="Windows::UI::Color"/></para>
				        <para>A <see cref="Windows::UI::Color"/> object that determines the color of this 
						<see cref="Pen"/>.</para>
				    </value>
				    <example>
				        <code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
						<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
				    </example>
				*/
				property Windows::UI::Color Color
				{
					Windows::UI::Color get();
					void set(Windows::UI::Color value);
				}

				/** <summary>
				        Gets or sets the width of this <see cref="Pen"/> on pixels.
				    </summary>
				    <value>
				        <para>The width of this <see cref="Pen"/>.</para>
				    </value>
				    <remarks>
				        <para>The unit of measure is pixels. A width of 0 or less, will result in the <see cref="Pen"/> drawing as if the 
						width were one pixel.</para>
				    </remarks>
				    <example>
				        <code language="cs" source="..\EDrawingLibraryTest\MainPage.xaml.cs" region="Usage"/>
						<code language="vb" source="..\EDrawingLibraryTestVB\MainPage.xaml.vb" region="Usage"/>
				    </example>
				*/
				property float Width
				{
					float get();
					void set(float value);
				}
			private:

				class Private;
				std::unique_ptr<Private> impl;

			};
		};
	};
};
