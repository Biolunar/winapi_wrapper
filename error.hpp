#ifndef HEADER_ERROR_HPP_INCLUDED
#define HEADER_ERROR_HPP_INCLUDED


#include <system_error>
#include <string>


namespace win
{

	::std::error_category const& windows_category();

	void throw_error(::std::string const& function);

} // namespace win


#endif // HEADER_ERROR_HPP_INCLUDED