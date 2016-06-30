#include "main.hpp"
#include "message.hpp"
#include "gdi.hpp"
#include "window_class.hpp"
#include "window.hpp"

#include <cstdlib>

#include <utility>

#include <Windows.h>


class my_window :
	public win::window
{
protected:
	::LRESULT message_proc(unsigned int msg, ::WPARAM wparam, ::LPARAM lparam) noexcept override
	{
		switch (msg)
		{
		case WM_DESTROY:
			win::post_quit_message(EXIT_SUCCESS);
			return 0;

		default:
			return win::window::message_proc(msg, wparam, lparam);
		}
	}
};

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

		my_window wnd;
		wnd.create(instance, 0, 0, 0, icon, small_icon, cursor, ::std::move(background), nullptr , u8"name", WS_OVERLAPPEDWINDOW, 0, 100, 100, 800, 600, basic_window_ptr{}, nullptr, nullptr);
		wnd.show(show_cmd);
		wnd.update();

		::MSG msg = {0};
		while (get_message(msg, nullptr, 0, 0))
		{
			translate_message(msg);
			dispatch_message(msg);
		}

		return static_cast<int>(msg.wParam);
	}

} // namespace win