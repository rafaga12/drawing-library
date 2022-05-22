#pragma once

class D2DUtils
{
public:
	D2DUtils(void);
	~D2DUtils(void);
	static D2D1::ColorF ColorToColorF(Windows::UI::Color color);
	static Microsoft::WRL::ComPtr<ID2D1Brush> BrushToID2D1Brush(Microsoft::WRL::ComPtr<ID2D1DeviceContext> m_d2dContext, Microsoft::WRL::ComPtr<IWICImagingFactory2> m_wicFactory, Windows::UI::Xaml::Media::Brush^ brush);
	static void replaceAll(std::wstring& str, const std::wstring& from, const std::wstring& to);
	static void replace(std::wstring& str, const std::wstring& from, const std::wstring& to);
};

