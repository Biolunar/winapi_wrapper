#include "main.hpp"
#include "gdi.hpp"

#include <cstdlib>

#include <Windows.h>


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