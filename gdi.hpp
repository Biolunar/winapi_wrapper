#ifndef HEADER_GDI_HPP_INCLUDED
#define HEADER_GDI_HPP_INCLUDED


#include "resource.hpp"
#include "resource_ptr.hpp"
#include "error.hpp"

#include <Windows.h>


namespace win
{

	class color
	{
	public:
		color() noexcept : m_handle{RGB(0, 0, 0)} {}
		explicit color(::COLORREF handle) noexcept : m_handle{handle} {}
		color(BYTE red, BYTE green, BYTE blue) noexcept : m_handle{RGB(red, green, blue)} {}

		void swap(color& other) noexcept { ::std::swap(m_handle, other.m_handle); }

		operator ::COLORREF() const noexcept { return get(); }

		::COLORREF get() const noexcept { return m_handle; }
		::BYTE red() const noexcept { return GetRValue(m_handle); }
		::BYTE green() const noexcept { return GetGValue(m_handle); }
		::BYTE blue() const noexcept { return GetBValue(m_handle); }

	private:
		::COLORREF m_handle;
	};

	enum class sys_color: int
	{
		scrollbar = 0,
		background = 1,
		active_caption= 2,
		inactive_caption = 3,
		menu = 4,
		window = 5,
		window_frame = 6,
		menu_text = 7,
		window_text = 8,
		caption_text = 9,
		active_border = 10,
		inactive_border = 11,
		app_workspace = 12,
		highlight = 13,
		highlight_text = 14,
		btn_face = 15,
		btn_shadow = 16,
		gray_text = 17,
		btn_text = 18,
		inactive_caption_text = 19,
		btn_highlight = 20,

		dk_shadow_3d = 21,
		light_3d = 22,
		info_text = 23,
		info_bk = 24,
		hotlight = 26,
		gradient_active_caption = 27,
		gradient_inactive_caption = 28,
		menu_hilight = 29,
		menu_bar = 30,
		desktop = background,
		face_3d = btn_face,
		shadow_3d = btn_shadow,
		highlight_3d = btn_highlight,
		hilight_3d = btn_highlight,
		btn_hilight = btn_highlight,
	};

	namespace load_flags
	{

		enum type
		{
			default_color = LR_DEFAULTCOLOR,
			monochrome = LR_MONOCHROME,
			color = LR_COLOR,
			copy_return_org = LR_COPYRETURNORG,
			copy_delete_org = LR_COPYDELETEORG,
			load_from_file = LR_LOADFROMFILE,
			load_transparent = LR_LOADTRANSPARENT,
			default_size = LR_DEFAULTSIZE,
			vga_color = LR_VGACOLOR,
			load_map_3d_colors = LR_LOADMAP3DCOLORS,
			create_dib_section = LR_CREATEDIBSECTION,
			copy_from_resource = LR_COPYFROMRESOURCE,
			shared = LR_SHARED,
		};

	} // namespace load_flags

	namespace resources
	{

		class brush :
			public resource<::HBRUSH>
		{
		public:
			void create(color const& c)
			{
				destroy();

				handle = ::CreateSolidBrush(c);
				if (!handle)
					throw_error(u8"CreateSolidBrush");
			}
			void create(sys_color const sc)
			{
				destroy();

				handle = ::GetSysColorBrush(static_cast<int>(sc));
				if (!handle)
					throw_error(u8"GetSysColorBrush");
			}
			void destroy() override
			{
				if (handle)
				{
					if (!::DeleteObject(handle))
						throw_error(u8"DeleteObject");
					handle = nullptr;
				}
			}
		};

		class cursor :
			public resource<::HCURSOR>
		{
		public:
			void create(::HINSTANCE const instance, int const x_hot_spot, int const y_hot_spot, int const width, int const height, void const* const and_plane, void const* const xor_plane)
			{
				destroy();

				handle = ::CreateCursor(instance, x_hot_spot, y_hot_spot, width, height, and_plane, xor_plane);
				if (!handle)
					throw_error(u8"CreateCursor");
			}
			void load(::HINSTANCE const instance, wchar_t const* const name)
			{
				destroy();

				handle = ::LoadCursorW(instance, name);
				if (!handle)
					throw_error(u8"LoadCursorW");
				m_shared = true;
			}
			void load(::HINSTANCE const instance, wchar_t const* const name, int const width, int const height, unsigned int const load_flags)
			{
				destroy();

				handle = static_cast<handle_type>(::LoadImageW(instance, name, IMAGE_CURSOR, width, height, static_cast<unsigned int>(load_flags)));
				if (!handle)
					throw_error(u8"LoadImageW");
				m_shared = load_flags == load_flags::shared ? true : false;
			}
			void destroy() override
			{
				if (handle && !m_shared)
				{
						if (!::DestroyCursor(handle))
							throw_error(u8"DestroyCursor");
						handle = nullptr;
						m_shared = false;
				}
			}

		private:
			bool m_shared = false;
		};

		class icon :
			public resource<::HICON>
		{
		public:
			void load(::HINSTANCE const instance, wchar_t const* const name)
			{
				destroy();

				handle = ::LoadIconW(instance, name);
				if (!handle)
					throw_error(u8"LoadIconW");
				m_shared = true;
			}
			void load(::HINSTANCE const instance, wchar_t const* const name, int const width, int const height, unsigned int const load_flags)
			{
				destroy();

				handle = static_cast<handle_type>(::LoadImageW(instance, name, IMAGE_ICON, width, height, static_cast<unsigned int>(load_flags)));
				if (!handle)
					throw_error(u8"LoadImageW");
				m_shared = load_flags == load_flags::shared ? true : false;
			}
			void destroy() override
			{
				if (handle && !m_shared)
				{
					if (!::DestroyIcon(handle))
						throw_error(u8"DestroyIcon");
					handle = nullptr;
					m_shared = false;
				}
			}

		private:
			bool m_shared = false;
		};

	} // namespace resources

	using brush_ptr = resource_ptr<resources::brush>;
	using cursor_ptr = resource_ptr<resources::cursor>;
	using icon_ptr = resource_ptr<resources::icon>;

} // namespace win


#endif // HEADER_GDI_HPP_INCLUDED