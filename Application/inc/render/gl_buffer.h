#ifndef GL_BUFFER_H
#define GL_BUFFER_H
#include <global.h>

class gl_buffer {
public:
	gl_buffer();
	virtual ~gl_buffer();

	void bind() const;
	void unbind() const;

	void init(int type);
	void destroy();

	template <typename T>
	void write(std::vector<T> data) {
		if (!m_is_initialized)
			return;
		glBufferData(m_buffer_type, data.size() * sizeof(T), data.data(), GL_STATIC_DRAW);
	}

	template <typename T>
	void write(T* data, int data_size) {
		if (!m_is_initialized)
			return;
		glBufferData(m_buffer_type, data_size * sizeof(T), data, GL_STATIC_DRAW);
	}

protected:
	uint32_t m_buffer_type;
	uint32_t m_buffer_obj;
	bool m_is_initialized;
};

#endif