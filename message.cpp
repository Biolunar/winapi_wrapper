#include "message.hpp"
#include "error.hpp"


namespace win
{

	bool get_message(::MSG& message, ::HWND window, unsigned int filter_min, unsigned int filter_max)
	{
		BOOL const ret = ::GetMessageW(&message, window, filter_min, filter_max);
		if (ret == -1)
			throw_error(u8"GetMessageW");
		return ret ? true : false;
	}

	bool peek_message(::MSG& message, ::HWND window, unsigned int filter_min, unsigned int filter_max, unsigned int remove)
	{
		return ::PeekMessageW(&message, window, filter_min, filter_max, remove) ? true : false;
	}

	bool translate_message(::MSG const& message)
	{
		return ::TranslateMessage(&message) ? true : false;
	}

	::LRESULT dispatch_message(::MSG const& message)
	{
		return ::DispatchMessageW(&message);
	}

	void post_quit_message(int exit_code)
	{
		::PostQuitMessage(exit_code);
	}

	void post_message(::HWND window, unsigned int message, ::WPARAM wparam, ::LPARAM lparam)
	{
		if (!::PostMessageW(window, message, wparam, lparam))
			throw_error(u8"PostMessageW");
	}

} // namespace win