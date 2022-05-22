#include "pch.h"
#include "D2DUtils.h"

using namespace Concurrency;
using namespace D2D1;
using namespace Windows::UI;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Microsoft::WRL;

D2DUtils::D2DUtils(void)
{
}


D2DUtils::~D2DUtils(void)
{
}

ColorF D2DUtils::ColorToColorF(Color color)
{
	return ColorF(color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f);
}

ComPtr<ID2D1Brush> D2DUtils::BrushToID2D1Brush(ComPtr<ID2D1DeviceContext> m_d2dContext, ComPtr<IWICImagingFactory2> m_wicFactory, Brush^ brush)
{
	if(brush->GetType()->FullName->Equals(L"Windows.UI.Xaml.Media.SolidColorBrush"))
	{
		ComPtr<ID2D1SolidColorBrush> returnBrush;
		DX::ThrowIfFailed(
			m_d2dContext->CreateSolidColorBrush(
				D2DUtils::ColorToColorF(((SolidColorBrush^)brush)->Color),
				&returnBrush
			)
		);
		return returnBrush;
	}else if (brush->GetType()->FullName->Equals(L"Windows.UI.Xaml.Media.LinearGradientBrush")) {
		ComPtr<ID2D1LinearGradientBrush> returnBrush;
		ComPtr<ID2D1GradientStopCollection> pGradientStops = NULL;
		
		LinearGradientBrush^ tempBrush = ((LinearGradientBrush^)brush);
		unsigned int size = tempBrush->GradientStops->Size;
		
		D2D1_GRADIENT_STOP* gradientStops = new D2D1_GRADIENT_STOP[size];
		unsigned int i = 0;
		for(i; i < size; i++)
		{
			gradientStops[i].color = D2DUtils::ColorToColorF(tempBrush->GradientStops->GetAt(i)->Color);
			gradientStops[i].position = static_cast<float>(tempBrush->GradientStops->GetAt(i)->Offset);
		}
		D2D1_GAMMA interpolationMode = D2D1_GAMMA_2_2;
		if(tempBrush->ColorInterpolationMode == ColorInterpolationMode::ScRgbLinearInterpolation)
		{
			interpolationMode = D2D1_GAMMA_1_0;
		}
		D2D1_EXTEND_MODE spreadMode = D2D1_EXTEND_MODE_CLAMP;
		if(tempBrush->SpreadMethod == GradientSpreadMethod::Reflect)
		{
			spreadMode = D2D1_EXTEND_MODE_MIRROR;
		}else if(tempBrush->SpreadMethod == GradientSpreadMethod::Repeat){
			spreadMode = D2D1_EXTEND_MODE_WRAP;
		}
		
		DX::ThrowIfFailed(
			m_d2dContext->CreateGradientStopCollection(
				gradientStops,
				size,
				interpolationMode,
				spreadMode,
				&pGradientStops
			)
		);

		DX::ThrowIfFailed(
			m_d2dContext->CreateLinearGradientBrush(
				D2D1::LinearGradientBrushProperties(
					D2D1::Point2F(tempBrush->StartPoint.X, tempBrush->StartPoint.Y),
					D2D1::Point2F(tempBrush->EndPoint.X, tempBrush->EndPoint.Y)
				),
				pGradientStops.Get(),
				&returnBrush
			)
		);
		return returnBrush;
	}else if (brush->GetType()->FullName->Equals(L"Windows.UI.Xaml.Media.ImageBrush")) {
		ImageBrush^ tempBrush = ((ImageBrush^)brush);
		
		ComPtr<ID2D1BitmapBrush> returnBrush;
		ComPtr<ID2D1Bitmap> m_pBitmap;
		
		if(tempBrush->ImageSource->GetType()->FullName->Equals(L"Windows.UI.Xaml.Media.Imaging.BitmapImage"))
		{
			BitmapImage^ tempImage = ((BitmapImage^)(tempBrush->ImageSource));
			
			std::wstring imagePath(tempImage->UriSource->Path->Data()); 
			size_t start_pos = imagePath.find(L"/");
			if(start_pos == 0)
			{
				replace(imagePath, L"/", L"");
			}
			
			ComPtr<IWICBitmapDecoder> decoder;
			DX::ThrowIfFailed(
				m_wicFactory->CreateDecoderFromFilename(
					imagePath.c_str(),
					nullptr,
					GENERIC_READ,
					WICDecodeMetadataCacheOnDemand,
					&decoder
				)
			);

			ComPtr<IWICBitmapFrameDecode> frame;
			DX::ThrowIfFailed(
				decoder->GetFrame(0, &frame)
				);

			ComPtr<IWICFormatConverter> m_wicFormatConverter;
			DX::ThrowIfFailed(
				m_wicFactory->CreateFormatConverter(&m_wicFormatConverter)
				);

			DX::ThrowIfFailed(
				m_wicFormatConverter->Initialize(
					frame.Get(),
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					nullptr,
					0.0f,
					WICBitmapPaletteTypeCustom
					)
				);

			DX::ThrowIfFailed(
				m_d2dContext->CreateBitmapFromWicBitmap(
					m_wicFormatConverter.Get(),
					NULL,
					&m_pBitmap
				)
			);
		}else if(tempBrush->ImageSource->GetType()->FullName->Equals(L"Windows.UI.Xaml.Media.Imaging.WriteableBitmap")){
			WriteableBitmap^ tempBitmap = ((WriteableBitmap^)(tempBrush->ImageSource));
			
			ComPtr<ID2D1BitmapBrush> returnBrush;
			ComPtr<IBufferByteAccess> bufferByteAccess;
			reinterpret_cast<IInspectable*>(tempBitmap->PixelBuffer)->QueryInterface(IID_PPV_ARGS(&bufferByteAccess));
			byte* pixels = nullptr;
			bufferByteAccess->Buffer(&pixels);

			DX::ThrowIfFailed(
				m_d2dContext->CreateBitmap(
					D2D1::SizeU(tempBitmap->PixelWidth,tempBitmap->PixelHeight),
					pixels,
					tempBitmap->PixelWidth*4,
					D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED)),
					&m_pBitmap
				)
			);
		}
		
		DX::ThrowIfFailed(
			m_d2dContext->CreateBitmapBrush(
				m_pBitmap.Get(),
				&returnBrush
			)
		);
		return returnBrush;
	}

	ComPtr<ID2D1SolidColorBrush> returnBrush;
	DX::ThrowIfFailed(
		m_d2dContext->CreateSolidColorBrush(
		D2DUtils::ColorToColorF(Colors::Black),
			&returnBrush
		)
	);
	return returnBrush;
}

void D2DUtils::replace(std::wstring& str, const std::wstring& from, const std::wstring& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return;
    str.replace(start_pos, from.length(), to);
}

void D2DUtils::replaceAll(std::wstring& str, const std::wstring& from, const std::wstring& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}
