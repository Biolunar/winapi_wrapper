#include "main.hpp"

#include <cstdlib>

#include <Windows.h>


namespace win
{

	int main(::HINSTANCE const instance, ::std::string const& cmd_line, int const show_cmd)
	{
		::PostQuitMessage(EXIT_SUCCESS);

		::MSG msg = {0};
		while (::GetMessageW(&msg, nullptr, 0, 0))
		{
			::TranslateMessage(&msg);
			::DispatchMessageW(&msg);
		}

		return static_cast<int>(msg.wParam);
	}

} // namespace win