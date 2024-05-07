#ifndef LOG_H
#define LOG_H
#include <global.h>

class logger {
public:
	static logger& get_instance() noexcept;
	void set_file(const std::string file_path) noexcept;
	
	void inf(const std::string& msg) noexcept;
	void wrn(const std::string& msg) noexcept;
	void err(const std::string& msg) noexcept;
	void ftl(const std::string& msg) noexcept;

protected:
	std::fstream m_file_stream;
	std::string m_file_path;
	std::mutex m_mutex;

	enum msg_type {
		INF, // Information message
		WRN, // Warning message
		ERR, // Error message
		FTL, // Fatal message
	};

	logger() = default;
	~logger() = default;

	void message(msg_type type, const std::string msg = "logger default message") noexcept;
};

#endif