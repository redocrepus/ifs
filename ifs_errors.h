#pragma once
#include <stdexcept>

namespace ifs {

	class cli_arg_error : public std::runtime_error {
		
	public:
		cli_arg_error(const std::string& s) : runtime_error(s) {};
	};
}