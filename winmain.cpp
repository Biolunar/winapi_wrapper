#include "error.hpp"
#include "utility.hpp"
#include "main.hpp"

#include <cassert>

#include <exception>

#include <Windows.h>
#include <CommCtrl.h>


int WINAPI wWinMain(::HINSTANCE const instance, ::HINSTANCE const prev_instance, wchar_t* const cmd_line, int const show_cmd)
{
	assert(!prev_instance);

	try
	{
		::INITCOMMONCONTROLSEX cc = {0};
		cc.dwSize = sizeof cc;
		cc.dwICC = ICC_WIN95_CLASSES | ICC_DATE_CLASSES | ICC_USEREX_CLASSES | ICC_COOL_CLASSES | ICC_INTERNET_CLASSES | ICC_PAGESCROLLER_CLASS | ICC_NATIVEFNTCTL_CLASS | ICC_STANDARD_CLASSES | ICC_LINK_CLASS;
		if (!::InitCommonControlsEx(&cc))
			::win::throw_error(u8"InitCommonControlsEx");

		return ::win::main(instance, ::win::to_utf8(cmd_line), show_cmd);
	}
	catch (::std::exception const& e)
	{
		::MessageBoxW(nullptr, ::win::to_utf16(e.what()).c_str(), ::win::to_utf16(u8"WoodBar Exception").c_str(), MB_ICONERROR);
	}
	catch (...)
	{
		::MessageBoxW(nullptr, ::win::to_utf16(u8"Unknown error.").c_str(), ::win::to_utf16(u8"WoodBar Exception").c_str(), MB_ICONERROR);
	}

	return EXIT_FAILURE;
}