#include "window_class.hpp"
#include "error.hpp"
#include "utility.hpp"

#include <cassert>


namespace win
{

	namespace resources
	{

		void window_class::create(::HINSTANCE const instance, ::WNDPROC const wnd_proc, ::std::string const& name, int const style, cursor_ptr const& cursor, brush_ptr background, icon_ptr const& icon, icon_ptr const& icon_small, wchar_t const* const menu_name, int const class_extra, int const window_extra)
		{
			destroy();
			handle.first = instance;

			auto const wname = to_utf16(name);

			::WNDCLASSEXW wc = {0};
			wc.cbSize = sizeof wc;
			wc.style = style;
			wc.lpfnWndProc = wnd_proc;
			wc.cbClsExtra = class_extra;
			wc.cbWndExtra = window_extra;
			wc.hInstance = handle.first;
			wc.hIcon = icon.get();
			wc.hCursor = cursor.get();
			wc.hbrBackground = background.get();
			wc.lpszMenuName = menu_name;
			wc.lpszClassName = wname.c_str();
			wc.hIconSm = icon_small.get();

			handle.second = ::RegisterClassExW(&wc);
			if (!handle.second)
				throw_error(u8"RegisterClassExW");
			background.release(); // unregistering the class destroys the brush
		}

		void window_class::destroy()
		{
			if (handle.second)
			{
				if (!::UnregisterClassW(reinterpret_cast<LPCWSTR>(handle.second), handle.first))
					throw_error(u8"UnregisterClassW");
				handle.first = nullptr;
				handle.second = 0;
			}
		}

	} // namespace resources

} // namespace win