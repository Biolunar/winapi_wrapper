#ifndef HEADER_WINDOW_HPP_INCLUDED
#define HEADER_WINDOW_HPP_INCLUDED


#include "resource.hpp"
#include "resource_ptr.hpp"
#include "window_class.hpp"

#include <string>
#include <thread>

#include <Windows.h>


namespace win
{

	namespace detail
	{

		class window_api
		{
		public:
			virtual ~window_api() noexcept {}

			bool show(int cmd);
			void update();
			void move(int x, int y, int width, int height, bool repaint);

		private:
			virtual ::HWND get_handle() const = 0;
		};

	} // namespace detail

	namespace resources
	{

		class basic_window :
			public resource<::HWND>,
			public detail::window_api
		{
		public:
			void create(::HINSTANCE instance, ::std::string const& class_name, ::std::string const& name, ::DWORD style, ::DWORD ext_style, int x, int y, int width, int height, resource_ptr<resources::basic_window> const& parent, ::HMENU menu, void* param);
			void create(::HINSTANCE instance, window_class_ptr const& wc, ::std::string const& name, ::DWORD style, ::DWORD ext_style, int x, int y, int width, int height, resource_ptr<resources::basic_window> const& parent, ::HMENU menu, void* param);
			void destroy();

		private:
			::HWND get_handle() const override final { return handle; }
			void set_subclass();
			void remove_subclass();
			static ::LRESULT CALLBACK subclass_wnd_proc(::HWND wnd, unsigned int msg, ::WPARAM wparam, ::LPARAM lparam, ::UINT_PTR id_subclass, ::DWORD_PTR ref_data);
			void create(::HINSTANCE instance, wchar_t const* class_name, wchar_t const* name, ::DWORD style, ::DWORD ext_style, int x, int y, int width, int height, handle_type parent, ::HMENU menu, void* param);
		};

	} // namespace resources

	using basic_window_ptr = resource_ptr<resources::basic_window>;

	namespace resources
	{

		class control :
			public basic_window
		{
		public:
			void create(::HINSTANCE const instance, ::std::string const& type, ::std::string const& name, ::DWORD const style, ::DWORD const ext_style, int const x, int const y, int const width, int const height, basic_window_ptr const& parent, ::WORD const id, void* const param)
			{
				basic_window::create(instance, type, name, style, ext_style, x, y, width, height, parent, reinterpret_cast<::HMENU>(id), param);
			}
		};

	} // namespace resources

	using control_ptr = resource_ptr<resources::control>;

	namespace resources
	{

		class button :
			public control
		{
		public:
			void create(::HINSTANCE const instance, ::std::string const& name, ::DWORD const style, ::DWORD const ext_style, int const x, int const y, int const width, int const height, basic_window_ptr const& parent, ::WORD const id, void* const param)
			{
				control::create(instance, u8"Button", name, style, ext_style, x, y, width, height, parent, id, param);
			}
		};

	} // namespace resources

	using button_ptr = resource_ptr<resources::button>;

	namespace resources
	{

		class combo_box :
			public control
		{
		public:
			void create(::HINSTANCE const instance, ::std::string const& name, ::DWORD const style, ::DWORD const ext_style, int const x, int const y, int const width, int const height, basic_window_ptr const& parent, ::WORD const id, void* const param)
			{
				control::create(instance, u8"ComboBox", name, style, ext_style, x, y, width, height, parent, id, param);
			}
		};

	} // namespace resources

	using combo_box_ptr = resource_ptr<resources::combo_box>;

	namespace resources
	{

		class edit :
			public control
		{
		public:
			void create(::HINSTANCE const instance, ::std::string const& name, ::DWORD const style, ::DWORD const ext_style, int const x, int const y, int const width, int const height, basic_window_ptr const& parent, ::WORD const id, void* const param)
			{
				control::create(instance, u8"Edit", name, style, ext_style, x, y, width, height, parent, id, param);
			}
		};

	} // namespace resources

	using edit_ptr = resource_ptr<resources::edit>;

	namespace resources
	{

		class list_box :
			public control
		{
		public:
			void create(::HINSTANCE const instance, ::std::string const& name, ::DWORD const style, ::DWORD const ext_style, int const x, int const y, int const width, int const height, basic_window_ptr const& parent, ::WORD const id, void* const param)
			{
				control::create(instance, u8"ListBox", name, style, ext_style, x, y, width, height, parent, id, param);
			}
		};

	} // namespace resources

	using list_box_ptr = resource_ptr<resources::list_box>;

	namespace resources
	{

		class mdi_client :
			public control
		{
		public:
			void create(::HINSTANCE const instance, ::std::string const& name, ::DWORD const style, ::DWORD const ext_style, int const x, int const y, int const width, int const height, basic_window_ptr const& parent, ::WORD const id, void* const param)
			{
				control::create(instance, u8"MDIClient", name, style, ext_style, x, y, width, height, parent, id, param);
			}
		};

	} // namespace resources

	using mdi_client_ptr = resource_ptr<resources::mdi_client>;

	namespace resources
	{

		class scroll_bar :
			public control
		{
		public:
			void create(::HINSTANCE const instance, ::std::string const& name, ::DWORD const style, ::DWORD const ext_style, int const x, int const y, int const width, int const height, basic_window_ptr const& parent, ::WORD const id, void* const param)
			{
				control::create(instance, u8"ScrollBar", name, style, ext_style, x, y, width, height, parent, id, param);
			}
		};

	} // namespace resources

	using scroll_bar_ptr = resource_ptr<resources::scroll_bar>;

	namespace resources
	{

		class static_control :
			public control
		{
		public:
			void create(::HINSTANCE const instance, ::std::string const& name, ::DWORD const style, ::DWORD const ext_style, int const x, int const y, int const width, int const height, basic_window_ptr const& parent, ::WORD const id, void* const param)
			{
				control::create(instance, u8"Static", name, style, ext_style, x, y, width, height, parent, id, param);
			}
		};

	} // namespace resources

	using static_control_ptr = resource_ptr<resources::static_control>;

	class window :
		public detail::window_api
	{
	public:
		void create(::HINSTANCE instance, int class_style, int class_extra, int window_extra, icon_ptr const& icon, icon_ptr const& icon_small, cursor_ptr const& cursor, brush_ptr background, wchar_t const* menu_name, ::std::string const& name, ::DWORD style, ::DWORD ext_style, int x, int y, int width, int height, basic_window_ptr const& parent, ::HMENU menu, void* param);
		void destroy();

		::HINSTANCE instance() const { return m_wc.get().first; }
		::HWND handle() const { return m_wnd.get(); }

	protected:
		virtual ::LRESULT message_proc(unsigned int msg, ::WPARAM wparam, ::LPARAM lparam) noexcept;

	private:
		::HWND get_handle() const override final { return m_wnd.get(); }
		static window* get_this(::HWND handle);
		static void set_this(::HWND handle, window* w);

		static ::LRESULT CALLBACK callback_wnd_proc(::HWND wnd, unsigned int msg, ::WPARAM wparam, ::LPARAM lparam) noexcept;

		thread_local static window* m_this;

		window_class_ptr m_wc;
		basic_window_ptr m_wnd;
	};

} // namespace win


#endif // HEADER_WINDOW_HPP_INCLUDED
