#ifndef HEADER_UTILITY_HPP_INCLUDED
#define HEADER_UTILITY_HPP_INCLUDED


#include <string>


namespace win
{

	// UTF-8 to UTF-16 and vice versa conversion
	// They are in the 'win' namespace because under other platforms it’s not guaranteed that wstring is able to hold UTF-16
	::std::wstring to_utf16(::std::string const& str);
	::std::string to_utf8(::std::wstring const& wstr);

} // namespace win


#endif // HEADER_UTILITY_HPP_INCLUDED