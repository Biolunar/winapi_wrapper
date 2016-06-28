#ifndef HEADER_MESSAGE_HPP_INCLUDED
#define HEADER_MESSAGE_HPP_INCLUDED


#include <Windows.h>


namespace win
{

	bool get_message(::MSG& message, ::HWND window, unsigned int filter_min, unsigned int filter_max);
	bool peek_message(::MSG& message, ::HWND window, unsigned int filter_min, unsigned int filter_max, unsigned int remove);
	bool translate_message(::MSG const& message);
	::LRESULT dispatch_message(::MSG const& message);
	void post_quit_message(int exit_code);
	void post_message(::HWND window, unsigned int message, ::WPARAM wparam, ::LPARAM lparam);

} // namespace win


#endif // HEADER_MESSAGE_HPP_INCLUDED