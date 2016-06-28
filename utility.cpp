#include "utility.hpp"

#include <locale>
#include <codecvt>


namespace win
{

	::std::wstring to_utf16(::std::string const& str)
	{
		::std::wstring_convert<::std::codecvt_utf8_utf16<wchar_t, 0x10ffff, static_cast<::std::codecvt_mode>(::std::little_endian | ::std::consume_header)>> c;
		return c.from_bytes(str);
	}

	::std::string to_utf8(::std::wstring const& wstr)
	{
		::std::wstring_convert<::std::codecvt_utf8_utf16<wchar_t, 0x10ffff, static_cast<::std::codecvt_mode>(::std::little_endian | ::std::consume_header)>> c;
		return c.to_bytes(wstr);
	}

} // namespace win