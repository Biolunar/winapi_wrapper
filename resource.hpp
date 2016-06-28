#ifndef HEADER_RESOURCE_HPP_INCLUDED
#define HEADER_RESOURCE_HPP_INCLUDED


#include <type_traits>
#include <utility>


namespace win
{

	template <typename T>
	class resource_ptr;

	namespace resources
	{

		template <typename HandleType>
		class resource
		{
		public:
			virtual inline void destroy() = 0;

		protected:
			using handle_type = HandleType;

			static_assert(::std::is_default_constructible<handle_type>::value, "handle_type must be default constructible"); // for resource()
			static_assert(::std::is_move_constructible<handle_type>::value, "handle_type must be move constructible"); // for ::std::swap()
			static_assert(::std::is_move_assignable<handle_type>::value, "handle_type must be move assignable"); // for ::std::swap()

			resource() noexcept(::std::is_nothrow_default_constructible<handle_type>::value) : handle{} {}
			resource(resource&& other) noexcept(noexcept(base{}) && noexcept(swap(other))) : resource{} { swap(other); }
			virtual ~resource() noexcept {}
			resource& operator=(resource&& other) noexcept(noexcept(swap(other)))
			{
				swap(other);
				return *this;
			}

			virtual void swap(resource& other) noexcept(::std::is_nothrow_move_constructible<handle_type>::value && ::std::is_nothrow_move_assignable<handle_type>::value) { ::std::swap(handle, other.handle); }

			handle_type handle;

		private:
			template <typename U> friend class resource_ptr;
		};

		template <typename T>
		void resource<T>::destroy()
		{
		}

	} // namespace resources

} // namespace win


#endif // HEADER_RESOURCE_HPP_INCLUDED
