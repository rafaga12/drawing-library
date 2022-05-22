#pragma once

namespace ModernComponents
{
	namespace System
	{
		namespace Drawing
		{
			namespace Drawing2D
			{
				[Windows::Foundation::Metadata::WebHostHidden]
				ref class GraphicsPath;
				/** <summary>
						Specifies the type of point in a <see cref="GraphicsPath"/> object. 
					</summary>
				*/
				public enum class PathPointType
				{
					/** <summary>The starting point of a GraphicsPath object.</summary> */
					Start          = 0,
					/** <summary>A line segment.</summary> */
					Line           = 1,
					/** <summary>A default Bézier curve.</summary> */
					Bezier         = 3,
					/** <summary>A mask point.</summary> */
					PathTypeMask   = 0x7,
					/** <summary>The corresponding segment is dashed.</summary> */
					PathDashMode   = 0x10,
					/** <summary>A path marker.</summary> */
					PathMarker     = 0x20,
					/** <summary>The endpoint of a subpath.</summary> */
					CloseSubpath   = 0x80,
					/** <summary>A cubic Bézier curve.</summary> */
					Bezier3        = 3 
				};
			};
		};
	};
};
