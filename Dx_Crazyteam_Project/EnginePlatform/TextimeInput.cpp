#include "PreCompile.h"
#include "TextimeInput.h"

#include <EnginePlatform/EngineInput.h>


std::string UTextimeInput::ReadText ="";
std::string UTextimeInput::MidText = "";
std::string UTextimeInput::ComstrText="";
bool UTextimeInput::bHangeul=false;
HWND UTextimeInput::hwnd;
HIMC UTextimeInput::himc;

void UTextimeInput::IMEInput()
{
	if (true == UEngineInput::IsDown(VK_BACK))
	{
		if (ReadText.size() >0)
		{
			ReadText.pop_back();
		}
		return;
	}

	if (true == UEngineInput::IsDown(VK_SPACE))
	{
		ReadText += " ";
		return;
	}

	if (true == UEngineInput::IsDown(VK_TAB))
	{
		if (false == bHangeul)
		{
			bHangeul = true;
		}
		else
		{
			bHangeul = false;
		}
		SetNativeMode(bHangeul);
		return;
	}

	for (int i = '0'; i <= '9'; i++)
	{
		if (true == UEngineInput::IsDown(i))
		{
			ReadText += static_cast<char>(i);
			return;
		}
	}

	if (false == bHangeul)
	{
		return;
	}

	for (int i = 'A'; i <= 'Z'; i++)
	{
		if (true == UEngineInput::IsDown(i))
		{
			ReadText += tolower(static_cast<char>(i));
			return;
		}
	}
}

std::string UTextimeInput::GetReadText()
{
	return ReadText;
}



void UTextimeInput::SetIme(HWND _hWnd,UINT _msg, WPARAM _wparam, LPARAM _lParam)
{
	hwnd = _hWnd;
	himc = ImmGetContext(_hWnd);

	int len = 0;
	switch (_msg)
	{
	case WM_IME_COMPOSITION:
		if (_lParam & GCS_COMPSTR) // 조합중 글자 
		{
			len = ImmGetCompositionString(himc, GCS_COMPSTR, NULL, 0);
			
			if (len > 0)
			{
				ComstrText.resize(len);
				ImmGetCompositionString(himc, GCS_COMPSTR, &ComstrText[0], len);
				
				//ReadText += ComstrText;
			}
			//ComstrText= '\0';
			//ImmReleaseContext(hwnd, himc);
		}else if(_lParam & GCS_RESULTSTR) // 완성중 글자
		{
			len = ImmGetCompositionString(himc, GCS_RESULTSTR, NULL, 0);
			std::string Write;
			if (len > 0)
			{
				Write.resize(len);
				ImmGetCompositionString(himc, GCS_RESULTSTR, &Write[0], len);

				//strcpy(&Write[0], &ComstrText[0]);
				ReadText += Write;

				//ReadText += MidText;
			}
			Write = '\0';
		}
		ComstrText = '\0';
		ImmReleaseContext(hwnd, himc); 
		break;
	case WM_CHAR: // 영어랑 숫자 처리
		if (_wparam == 8)
		{
			if (ReadText.size() > 0)
			{
				ReadText.pop_back();
			}
		}
		else
		{
			ReadText += _wparam & 0xff;
		}
		break;
	case WM_KEYDOWN:
		break;
	default:
		bHangeul = false;
		break;
	}
}

void UTextimeInput::SetNativeMode(bool bHangeul)
{
	DWORD dwConv, dwSent;

	DWORD dwTemp;



	ImmGetConversionStatus(himc, &dwConv, &dwSent);

	dwTemp = dwConv & ~IME_CMODE_LANGUAGE;

	if (bHangeul) {

		dwTemp |= IME_CMODE_NATIVE;

	}
	else {

		dwTemp |= IME_CMODE_ALPHANUMERIC;

	}

	dwConv = dwTemp;

	ImmSetConversionStatus(himc, dwConv, dwSent);

	ImmReleaseContext(hwnd, himc);
}


