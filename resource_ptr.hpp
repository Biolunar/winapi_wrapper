#ifndef HEADER_RESOURCE_PTR_HPP_INCLUDED
#define HEADER_RESOURCE_PTR_HPP_INCLUDED


#include "resource.hpp"
#include "utility.hpp"

#include <cassert>

#include <type_traits>
#include <memory>

#include <Windows.h>


namespace win
{

	template <typename ResourceType>
	class resource_ptr final
	{
	public:
		using resource_type = ResourceType;
		using handle_type = typename resource_type::handle_type;

		static_assert(::std::is_base_of<resources::resource<typename resource_type::handle_type>, resource_type>::value, "resource_ptr can only manage types derived from resource");
		static_assert(::std::is_copy_constructible<handle_type>::value, "handle_type must be copy constructible");
		static_assert(::std::is_assignable<handle_type&, handle_type>::value, "handle_type must be assignable");

		resource_ptr() :
			m_object{::std::make_unique<resource_type>()}
		{
			assert(m_object);
		}
		resource_ptr(nullptr_t) :
			resource_ptr{}
		{
			assert(m_object);
		}
		explicit resource_ptr(handle_type const& handle) :
			resource_ptr{}
		{
			assert(m_object);
			m_object->handle = handle;
		}
		resource_ptr(resource_ptr&& other) :
			resource_ptr{}
		{
			assert(m_object);
			swap(other);
			assert(other.m_object);
		}
		/* This constructor is more trouble than it’s worth, e.g. incredible false compiler errors if you mess up something
		template <typename... Parameters>
		resource_ptr(Parameters&&... params) :
			resource_ptr{}
		{
			m_object->create(params...);
		}*/
		~resource_ptr() noexcept
		{
			assert(m_object);
			reset();
		}
		resource_ptr& operator=(resource_ptr&& other) noexcept
		{
			assert(m_object);
			swap(other);
			assert(other.m_object);
			return *this;
		}

		// Modifiers
		handle_type release() noexcept
		{
			assert(m_object);

			// if the following two lines throw an exception, let the runtime call ::std::terminate
			handle_type const ret = m_object->handle;
			m_object->handle = handle_type{};

			return ret;
		}
		void reset() noexcept
		{
			assert(m_object);
			try
			{
				// An exception here is actually a logic error in your code. Fix your code!
				m_object->destroy();
			}
			catch (::std::exception const& err)
			{
				::OutputDebugStringW(to_utf16(err.what()).c_str());
			}
			catch (...)
			{
				::OutputDebugStringW(L"Unknown exception inside ::win::resource_ptr::reset().");
			}

			release(); // make sure it is actually zeroed even if it leaks memory
			assert(m_object->handle == handle_type{});
		}
		void reset(handle_type const& handle) noexcept
		{
			assert(m_object);
			reset();
			m_object->handle = handle;
		}
		void swap(resource_ptr& other) noexcept
		{
			assert(m_object);
			m_object.swap(other.m_object);
			assert(other.m_object);
		}

		// Observers
		handle_type const& get() const noexcept
		{
			assert(m_object);
			return m_object->handle;
		}
		explicit operator bool() const noexcept
		{
			assert(m_object);
			return m_object->handle != handle_type{} ? true : false;
		}
		resource_type const& operator*() const { return *m_object; }
		resource_type& operator*() { return *m_object; }
		resource_type const* operator->() const noexcept
		{
			assert(m_object);
			return m_object.get();
		}
		resource_type* operator->() noexcept
		{
			assert(m_object);
			return m_object.get();
		}

	private:
		::std::unique_ptr<resource_type> m_object;
	};

} // namespace win


#endif // HEADER_RESOURCE_PTR_HPP_INCLUDED
