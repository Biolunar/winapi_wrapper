#include "window.hpp"
#include "error.hpp"
#include "utility.hpp"
#include "message.hpp"

#include <cassert>
#include <cstdlib> // abort

#include <sstream>

#include <CommCtrl.h>


namespace win
{

	namespace detail
	{

		bool window_api::show(int const cmd)
		{
			return ::ShowWindow(get_handle(), cmd) ? true : false;
		}

		void window_api::update()
		{
			if (!::UpdateWindow(get_handle()))
				throw_error(u8"UpdateWindow");
		}

		void window_api::move(int x, int y, int width, int height, bool repaint)
		{
			if (!::MoveWindow(get_handle(), x, y, width, height, repaint))
				throw_error(u8"MoveWindow");
		}

	} // namespace detail

	namespace resources
	{

		::LRESULT CALLBACK basic_window::subclass_wnd_proc(::HWND const wnd, unsigned int const msg, ::WPARAM const wparam, ::LPARAM const lparam, ::UINT_PTR const id_subclass, ::DWORD_PTR const ref_data)
		{
			basic_window* const that = reinterpret_cast<basic_window* const>(ref_data);
			assert(that);

			if (msg == WM_NCDESTROY)
				that->handle = nullptr;

			return ::DefSubclassProc(wnd, msg, wparam, lparam);
		}

		void basic_window::set_subclass()
		{
			assert(handle);

			if (!::SetWindowSubclass(handle, &subclass_wnd_proc, 0, reinterpret_cast<::DWORD_PTR>(this)))
				throw_error(u8"SetWindowSubclass");
		}

		void basic_window::remove_subclass()
		{
			assert(handle);

			if (!::RemoveWindowSubclass(handle, &subclass_wnd_proc, 0))
				throw_error(u8"RemoveWindowSubclass");
		}

		void basic_window::create(::HINSTANCE const instance, wchar_t const* const class_name, wchar_t const* const name, ::DWORD const style, ::DWORD const ext_style, int const x, int const y, int const width, int const height, handle_type parent, ::HMENU const menu, void* const param)
		{
			destroy();

			handle = ::CreateWindowExW(ext_style, class_name, name, style, x, y, width, height, parent, menu, instance, param);
			if (!handle)
				throw_error(u8"CreateWindowExW");

			set_subclass();
		}

		void basic_window::create(::HINSTANCE const instance, ::std::string const& class_name, ::std::string const& name, ::DWORD const style, ::DWORD const ext_style, int const x, int const y, int const width, int const height, resource_ptr<resources::basic_window> const& parent, ::HMENU const menu, void* const param)
		{
			create(instance, to_utf16(class_name).c_str(), to_utf16(name).c_str(), style, ext_style, x, y, width, height, parent.get(), menu, param);
		}

		void basic_window::create(::HINSTANCE const instance, window_class_ptr const& wc, ::std::string const& name, ::DWORD const style, ::DWORD const ext_style, int const x, int const y, int const width, int const height, resource_ptr<resources::basic_window> const& parent, ::HMENU const menu, void* const param)
		{
			create(instance, reinterpret_cast<wchar_t const* const>(wc.get().second), to_utf16(name).c_str(), style, ext_style, x, y, width, height, parent.get(), menu, param);
		}

		void basic_window::destroy()
		{
			if (handle)
			{
				remove_subclass();
				if (!::DestroyWindow(handle))
					throw_error(u8"DestroyWindow");
				handle = nullptr;
			}
		}

	} // namespace resources

	thread_local window* window::m_this = nullptr;

	void window::create(::HINSTANCE const instance, int const class_style, int const class_extra, int const window_extra, icon_ptr const& icon, icon_ptr const& icon_small, cursor_ptr const& cursor, brush_ptr background, wchar_t const* const menu_name, ::std::string const& name, ::DWORD const style, ::DWORD const ext_style, int const x, int const y, int const width, int const height, basic_window_ptr const& parent, ::HMENU const menu, void* const param)
	{
		destroy();

		::std::ostringstream class_name;
		class_name << u8"window class " << this;
		m_wc->create(instance, &callback_wnd_proc, class_name.str(), class_style, cursor, ::std::move(background), icon, icon_small, menu_name, class_extra, window_extra);

		try
		{
			m_this = this;
			m_wnd->create(instance, m_wc, name, style, ext_style, x, y, width, height, parent, menu, param);
			assert(!m_this);
		}
		catch (...)
		{
			m_this = nullptr;

			// Get rid of WM_QUIT so that MessageBox work properly afterwards
			::MSG msg = {0};
			peek_message(msg, nullptr, WM_QUIT, WM_QUIT, PM_REMOVE);

			throw;
		}
	}

	void window::destroy()
	{
		m_wnd->destroy();
		m_wc->destroy();
	}

	window* window::get_this(::HWND handle)
	{
		// TODO: use an own map instead of the syscall

		::SetLastError(0);
		auto const ret = ::GetWindowLongPtrW(handle, GWLP_USERDATA);
		auto const err = ::GetLastError();
		if (!ret && err)
			throw_error(u8"GetWindowLongPtrW");
		return reinterpret_cast<window*>(ret);
	}

	void window::set_this(::HWND handle, window* const w)
	{
		// TODO: use an own map instead of the syscall

		::SetLastError(0);
		auto const ret = ::SetWindowLongPtrW(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(w));
		auto const err = ::GetLastError();
		if (!ret && err)
			throw_error(u8"SetWindowLongPtrW");
	}

	::LRESULT CALLBACK window::callback_wnd_proc(::HWND const wnd, unsigned int const msg, ::WPARAM const wparam, ::LPARAM const lparam) noexcept
	{
		try
		{
			if (m_this)
			{
				set_this(wnd, m_this);
				m_this = nullptr;
			}

			window* const that = get_this(wnd);
			assert(that);

			if (!that->m_wnd.get())
				that->m_wnd.reset(wnd);

			auto const ret = that->message_proc(msg, wparam, lparam);

			if (msg == WM_NCDESTROY)
				that->m_wnd.release();

			return ret;
		}
		catch (::std::exception const& err)
		{
			::OutputDebugStringW(to_utf16(err.what()).c_str());
		}
		catch (...)
		{
			::OutputDebugStringW(L"Unknown exception inside " __FUNCTION__ L".");
		}

		abort();
	}

	::LRESULT window::message_proc(unsigned int const msg, ::WPARAM const wparam, ::LPARAM const lparam) noexcept
	{
		assert(m_wnd.get());

		return ::DefWindowProcW(m_wnd.get(), msg, wparam, lparam);
	}

} // namespace win