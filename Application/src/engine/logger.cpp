#include <engine/logger.h>

logger& logger::get_instance() noexcept {
	static logger instance;
	return instance;
}

void logger::set_file(const std::string file_path) noexcept {
	if (m_file_stream.is_open())
		m_file_stream.close();

	m_file_path = file_path;
	m_file_stream.open(m_file_path, std::ios::in);
}

void logger::inf(const std::string& msg) noexcept {
	message(msg_type::INF, msg);
}

void logger::wrn(const std::string& msg) noexcept {
	message(msg_type::WRN, msg);
}

void logger::err(const std::string& msg) noexcept {
	message(msg_type::ERR, msg);
}

void logger::ftl(const std::string& msg) noexcept {
	message(msg_type::FTL, msg);
}

void logger::message(msg_type type, const std::string msg) noexcept {
	std::scoped_lock lk(m_mutex);
	std::stringstream ss;

	if (!m_file_stream.is_open())
		return;

	ss << "[";
	switch (type) {
		case logger::INF:
			ss << "INF]:> ";
			break;
		case logger::WRN:
			ss << "WRN]:> ";
			break;
		case logger::ERR:
			ss << "ERR]:> ";
			break;
		case logger::FTL:
			ss << "FTL]:> ";
			break;
		default:
			ss << "DEF]:> ";
			break;
	}

	ss << msg << std::endl;
	m_file_stream << ss.str();

	if (type == msg_type::FTL) {
		m_file_stream.flush();
		std::exit(-1);
	}
}