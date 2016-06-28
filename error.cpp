#include "error.hpp"
#include "utility.hpp"

#include <locale>
#include <codecvt>

#include <Windows.h>


namespace
{

	class windows_error_category :
		public ::std::error_category
	{
	public:
		char const* name() const noexcept override { return u8"windows"; }
		::std::string message(int condition) const override
		{
			// A small hack to convert the system messages from the default charset to UTF-8
			::std::wstring_convert<::std::codecvt<wchar_t, char, ::std::mbstate_t>, wchar_t> convert;
			return ::win::to_utf8(convert.from_bytes(::std::system_category().message(condition)));
		}
		::std::error_condition default_error_condition(int code) const noexcept override { return ::std::system_category().default_error_condition(code); }
		bool equivalent(int code, ::std::error_condition const& cond) const noexcept override { return ::std::system_category().equivalent(code, cond); }
		bool equivalent(::std::error_code const& code, int cond) const noexcept override { return ::std::system_category().equivalent(code, cond); }
	};

} // namespace

namespace win
{

	::std::error_category const& windows_category()
	{
		static windows_error_category cat;
		return cat;
	}

	void throw_error(::std::string const& function)
	{
		throw ::std::system_error{static_cast<int>(::GetLastError()), windows_category(), function};
	}

} // namespace win