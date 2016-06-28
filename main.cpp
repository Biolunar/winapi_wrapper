#include "main.hpp"
#include "message.hpp"
#include "gdi.hpp"
#include "window_class.hpp"

#include <cstdlib>

#include <utility>

#include <Windows.h>


namespace
{

	::LRESULT CALLBACK wnd_proc(::HWND const wnd, unsigned int const msg, ::WPARAM const wparam, ::LPARAM const lparam)
	{
		return 0;
	}

} // namespace


namespace win
{

	int main(::HINSTANCE const instance, ::std::string const& cmd_line, int const show_cmd)
	{
		cursor_ptr cursor;
		cursor->load(nullptr, IDC_CROSS);
		brush_ptr background;
		background->create(sys_color::btn_face);
		icon_ptr icon, small_icon;
		icon->load(nullptr, IDI_EXCLAMATION, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), load_flags::shared);
		small_icon->load(nullptr, IDI_ASTERISK, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), load_flags::shared);

		window_class_ptr wc;
		wc->create(instance, ::wnd_proc, "test", 0, cursor, ::std::move(background), icon, small_icon, nullptr, 0, 0);

		post_quit_message(EXIT_SUCCESS);

		::MSG msg = {0};
		while (get_message(msg, nullptr, 0, 0))
		{
			translate_message(msg);
			dispatch_message(msg);
		}

		return static_cast<int>(msg.wParam);
	}

} // namespace win