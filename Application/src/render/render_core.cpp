#include <render/render_core.h>

render_core& render_core::get_instance() {
	static render_core core;
	return core;
}


void render_core::set_width(uint32_t width) {
	m_width = width;
}

void render_core::set_height(uint32_t height) {
	m_height = height;
}

std::shared_ptr<gl_framebuffer> render_core::get_framebuffer() {
	return m_scene_buffer;
}

void render_core::init() {
	global_camera = gl_camera(glm::vec3(0.0f, 0.0f, 3.0f));
	m_scene_buffer = std::shared_ptr<gl_framebuffer>(new gl_framebuffer());
	m_scene_buffer->init(m_width, m_height);
}

void render_core::update() {
	if (m_first) {
		float vertices[] = {
			// positions         // texture coords
			 0.5f,  0.5f, 0.0f,  1.0f, 1.0f, // top right
			 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,  0.0f, 1.0f  // top left 
		};
		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};
		m_first = false;

		auto fs = fs_helper::get_instance();
		std::string vert_path = (std::filesystem::path(fs.get_shaders_path()) / "base.vert").string();
		std::string frag_path = (std::filesystem::path(fs.get_shaders_path()) / "base.frag").string();
		base_program.init(vert_path.c_str(), frag_path.c_str());

		glGenVertexArrays(1, &VAO);
		vertex_buff.init(GL_ARRAY_BUFFER);
		indexes_buff.init(GL_ELEMENT_ARRAY_BUFFER);

		glBindVertexArray(VAO);
		vertex_buff.bind();
		vertex_buff.write<float>(vertices, sizeof(vertices));

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		indexes_buff.bind();
		indexes_buff.write(indices, 6);

		// VAO close
		vertex_buff.unbind();
		glBindVertexArray(0);

		std::string texture_path = (std::filesystem::path(fs.get_textures_path()) / "skybox.jpg").string();
		texture_1.init(texture_path.c_str());

		base_program.use();
		glUniform1i(glGetUniformLocation(base_program.get_id(), "texture1"), 0);
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_scene_buffer->bind();
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	float width = m_scene_buffer->get_Width();
	float height = m_scene_buffer->get_height();

	if (width > 0.0f && height > 0.0f) {
		glViewport(0, 0, width, height);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_1.get_id());

		glm::mat4 view = glm::mat4(1.0f);
		view = global_camera.get_view();

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);

		base_program.set_mat4("projection", projection);
		base_program.set_mat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		float angle = 20.0f * glfwGetTime();
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

		base_program.use();
		base_program.set_mat4("model", model);
		glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	m_scene_buffer->unbind();
}

void render_core::destroy() {

}