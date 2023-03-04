#pragma once
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

namespace ifs {
	namespace win {
		void disableQuickEdit();

		fs::path currentExePath();

		// Create quoted paths with double backslashes. There could be problems with other special characters. For possible solution see: https://stackoverflow.com/questions/39228912/stdregex-escape-special-characters-for-use-in-regex
		std::string escQuotedPathString(fs::path path);

		// Get program install location by name
		fs::path getInstallPath(std::string programName);

		// Class for easy construction, execution and error handling of CMD commands
		// with elusive "correct" syntax.  See: https://stackoverflow.com/questions/27975969/how-to-run-an-executable-with-spaces-using-stdsystem-on-windows
		// Works with environment variables such as %appdata%
		// Just use << to push std::filesystem::path-s and strings, and use run() to execute the command.
		// If the command returns non-zero, throws ifs::win::cmd_error. See below.
		// The command is automatically cleared after the run() method, if no error is thrown.
		//
		//  ---------------------- Example: ---------------------------------
		// ifs::win::command c;
		// std::filesystem::path source = "c:\\my folder\\file.txt";
		// std::filesystem::path destination = "c:\\another folder\\another file.txt";
		// try {
		//		c << "copy " << source << " " << destination;
		//		c.run();
		// }
		// catch (ifs::win::cmd_error& e) {
		//		std::cout << "default error message: " << e.what() << std::endl;
		//		std::cout << "command: " << e.command() << std::endl;
		//		std::cout << "result: " << e.res() << std::endl;
		// }
		// 
		// Thanks to [user5234](https://github.com/user5234) who participated in coding this class.
		// 
		// ------------------------------------------------------------------
		class command : public std::stringstream {

		public:
			void run();

			std::string runAndGetOutput();
		};

		class cmd_error : public std::runtime_error {

		protected:
			std::string mCommand;
			int mRes;
			std::string mWhat;
			
		public:

			cmd_error(const std::string& command, const int& res, const std::string& what = "") : runtime_error("command: " + command + " , result: " + std::to_string(res)) {
				mCommand = command;
				mRes = res;
				mWhat = what;
			}

			const std::string& command() {
				return mCommand;
			}

			const int& res() {
				return mRes;
			}

			const char* what() {
				return ("" != mWhat) ? mWhat.c_str() : runtime_error::what();
			}

		};

		bool GetCMDColors(short& color);

		bool GetCMDColors(short& BG, short& FG);


	}


}