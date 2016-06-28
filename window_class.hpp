#ifndef HEADER_WINDOW_CLASS_HPP_INCLUDED
#define HEADER_WINDOW_CLASS_HPP_INCLUDED


#include "resource.hpp"
#include "resource_ptr.hpp"
#include "gdi.hpp"

#include <utility>
#include <string>

#include <Windows.h>


namespace win
{

	namespace resources
	{

		class window_class :
			public resource<::std::pair<::HINSTANCE, ::ATOM>>
		{
		public:
			void create(::HINSTANCE instance, ::WNDPROC wnd_proc, ::std::string const& name, int style, cursor_ptr const& cursor, brush_ptr background, icon_ptr const& icon, icon_ptr const& icon_small, wchar_t const* menu_name, int class_extra, int window_extra);
			void destroy();
		};

	} // namespace resources

	using window_class_ptr = resource_ptr<resources::window_class>;

} // namespace win


#endif // HEADER_WINDOW_CLASS_HPP_INCLUDED