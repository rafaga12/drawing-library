#pragma once

#include "Drawing2DEnumerations.h"
namespace ModernComponents
{
	namespace System
	{
		namespace Drawing
		{
			namespace Drawing2D
			{
				/** <summary>
						Contains the graphical data that makes up a <see cref="GraphicsPath"/> object.
					</summary>
					<remarks>
						<para>
							The graphical data is stored into the <see cref="Points"/> and <see cref="Types"/> arrays.
						</para>
					</remarks>
				*/
				[Windows::Foundation::Metadata::WebHostHidden]
				public ref class PathData sealed
				{
				public:
					PathData();
					virtual ~PathData();

					/** <summary>
					        Gets or sets an array of Points objects through which the path is 
							constructed.
					    </summary>
					    <value>
					        <para>An array of <see cref="Windows::Foundation::Point"/> that represents the points through which 
							the path is constructed.</para>
					    </value>
					*/
					 property Platform::Array<Windows::Foundation::Point>^ Points
					 {
						 Platform::Array<Windows::Foundation::Point>^ get();
						 void set(const Platform::Array<Windows::Foundation::Point>^ value);
					 };

					 /** <summary>
					        Gets or sets the types of the corresponding points in the path.
					    </summary>
					    <value>
					        <para>An array of bytes that specify the types of the corresponding points in the path.</para>
					    </value>
					*/
					 property Platform::Array<byte>^ Types
					 {
						 Platform::Array<byte>^ get();
						 void set(const Platform::Array<byte>^ value);
					 };
				internal:
					void addPoint(Windows::Foundation::Point, Drawing2D::PathPointType);
					bool isFirstPoint();
				private:
					class Private;
					std::unique_ptr<Private> impl;
				};
			};
		};
	};
};

