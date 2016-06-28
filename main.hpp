#ifndef HEADER_MAIN_HPP_INCLUDED
#define HEADER_MAIN_HPP_INCLUDED


#include <Windows.h>

#include <string>


namespace win
{

	int main(::HINSTANCE const instance, ::std::string const& cmd_line, int const show_cmd);

} // namespace win


#endif // HEADER_MAIN_HPP_INCLUDED