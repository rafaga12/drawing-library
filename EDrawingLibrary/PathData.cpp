#include "pch.h"
#include "PathData.h"

using namespace ModernComponents::System::Drawing::Drawing2D;

using namespace Windows::Foundation;
using namespace Platform;
using namespace Platform::Collections;

#pragma region Private

class PathData::Private
{
public:
	Platform::Collections::Vector<Windows::Foundation::Point>^ points;
	Platform::Collections::Vector<byte>^ types;

	void addPoint(Windows::Foundation::Point, Drawing2D::PathPointType);
	bool isFirstPoint();
};

#pragma endregion Private

/** <summary>
        Initializes a new instance of the PathData class.
    </summary>
*/
PathData::PathData() : impl(new Private())
{
	impl->points = ref new Vector<Point>();
	impl->types = ref new Vector<byte>();
}

PathData::~PathData()
{
}

Array<Point>^ PathData::Points::get()
{
	Array<Point>^ temp = ref new Array<Point>(impl->points->Size);
	impl->points->GetMany(0, temp);
	return temp;
}

void PathData::Points::set(const Array<Point>^ value)
{
	Vector<Point>^ temp = ref new Vector<Point>(value);
	impl->points = temp;
}

Array<byte>^ PathData::Types::get()
{
	Array<byte>^ temp = ref new Array<byte>(impl->types->Size);
	impl->types->GetMany(0, temp);
	return temp;
}

void PathData::Types::set(const Array<byte>^ value)
{
	Vector<byte>^ temp = ref new Vector<byte>(value);
	impl->types = temp;
}

void PathData::addPoint(Windows::Foundation::Point point, PathPointType type)
{
	impl->points->Append(point);
	impl->types->Append((byte)type);
}

bool PathData::isFirstPoint()
{
	if(impl->points->Size == 0)
	{
		return true;
	}
	return false;
}