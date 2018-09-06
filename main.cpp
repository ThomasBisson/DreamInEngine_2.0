//#define GLEW_STATIC
//#include <GLEW/glew.h>
//#include <GLFW/glfw3.h>
//
//#include <iostream>
//#include <SpriteRenderer.h>
//#include <ResourceManager.h>
//#include <glm/gtc/matrix_transform.hpp>
//
/////////////////// IMGUI /////////////////
//#include <ImGui/imgui.h>
//#include <ImGui/imgui_impl_glfw.h>
//#include <ImGui/imgui_impl_opengl3.h>
//#pragma region Headers included, based on IMGUI_IMPL_OPENGL_LOADER_... directive
////#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
////#include <GL/gl3w.h>    // Initialize with gl3wInit()
////#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
////#include <GL/glew.h>    // Initialize with glewInit()
////#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
////#include <glad/glad.h>  // Initialize with gladLoadGL()
////#else
////#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
////#endif
//#pragma endregion
///////////////////////////////////////////
//
//void glfw_error_callback(int error, const char* description)
//{
//	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
//}
//
//// GLFW function declerations
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
//
//// The Width of the screen
//const GLint SCREEN_WIDTH = 1080;
//// The height of the screen
//const GLint SCREEN_HEIGHT = 720;
//
////DreamInEngine(SCREEN_WIDTH, SCREEN_HEIGHT);
//
//GLboolean fullscreen = false;
//
//enum ProjectionType
//{
//	PROJECTION_ORIGIN_CENTER,
//	PROJECTION_NORMALIZED_ORIGIN_CENTER,
//	PROJECTION_NORMALIZED_ORIGIN_TOP_LEFT,
//};
//
//int main(int argc, char *argv[])
//{
//	ProjectionType projection_type = PROJECTION_NORMALIZED_ORIGIN_CENTER;
//
//	glfwSetErrorCallback(glfw_error_callback);
//	if (!glfwInit()) return 1;
//
//#if __APPLE__
//	const char* glsl_version = "#version 150";
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
//#else
//	const char* glsl_version = "#version 130";
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//#endif	
//	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//
//	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "FuryRunner", (fullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);
//	if (!window) return 1;
//	glfwMakeContextCurrent(window);
//	glfwSwapInterval(1);
//
//	//glewExperimental = GL_TRUE;
//	if (glewInit() != GLEW_OK)
//	{
//		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
//		return 1;
//	}
//
//	glfwSetKeyCallback(window, key_callback);
//
//	// OpenGL configuration
//	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
//
//	// DeltaTime variables
//	GLfloat deltaTime = 0.0f;
//	GLfloat lastFrame = 0.0f;
//	GLfloat startFrame = glfwGetTime(); int nbFrames = 0;
//
//	float vertices[] = {
//		// first triangle
//		0.5f,  0.5f, 0.0f,  // top right
//		0.5f, -0.5f, 0.0f,  // bottom right
//		-0.5f,  0.5f, 0.0f,  // top left 
//		// second triangle
//		0.5f, -0.5f, 0.0f,  // bottom right
//		-0.5f, -0.5f, 0.0f,  // bottom left
//		-0.5f,  0.5f, 0.0f   // top left
//	};
//
//	unsigned int VBO, VAO;
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//
//	glBindVertexArray(VAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//	// vertex attributes (position)
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//
//	ResourceManager::LoadShader("resources/shaders/square.vert", "resources/shaders/square.frag", nullptr, "square_shader");
//	ResourceManager::GetShader("square_shader").Use();
//
//#pragma region ImGui (setup)
//	///////////// IMGUI (SETUP) /////////////
//	// Setup Dear ImGui binding
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImGuiIO& io = ImGui::GetIO(); (void)io;
//	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
//	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
//
//	ImGui_ImplGlfw_InitForOpenGL(window, true);
//	ImGui_ImplOpenGL3_Init(glsl_version);
//
//	// Setup style
//	ImGui::StyleColorsDark();
//	//ImGui::StyleColorsClassic();
//
//	// Load Fonts
//	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
//	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
//	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
//	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
//	// - Read 'misc/fonts/README.txt' for more instructions and details.
//	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
//	//io.Fonts->AddFontDefault();
//	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
//	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
//	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
//	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
//	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
//	//IM_ASSERT(font != NULL);
//
//	bool show_demo_window = true;
//	bool show_another_window = false;
//	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
//#pragma endregion 
//
//	while (!glfwWindowShouldClose(window))
//	{
//		// Catch Queued Events
//		glfwPollEvents();
//
//		// Start the Dear ImGui frame
//		ImGui_ImplOpenGL3_NewFrame();
//		ImGui_ImplGlfw_NewFrame();
//		ImGui::NewFrame();
//
//#pragma region ImGui Window creation
//		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
//		if (show_demo_window)
//			ImGui::ShowDemoWindow(&show_demo_window);
//
//		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
//		{
//			static float f = 0.0f;
//			static int counter = 0;
//
//			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
//
//			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
//			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
//			ImGui::Checkbox("Another Window", &show_another_window);
//
//			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
//			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
//
//			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
//				counter++;
//			ImGui::SameLine();
//			ImGui::Text("counter = %d", counter);
//
//			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//			ImGui::End();
//		}
//
//		// 3. Show another simple window.
//		if (show_another_window)
//		{
//			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
//			ImGui::Text("Hello from another window!");
//			if (ImGui::Button("Close Me"))
//				show_another_window = false;
//			ImGui::End();
//		}
//#pragma endregion
//
//		// Calculate delta time
//		GLfloat currentFrame = glfwGetTime();
//		deltaTime = currentFrame - lastFrame;
//		lastFrame = currentFrame;
//
//		//nbFrames++;
//		//if (currentFrame - startFrame >= 1.0f)
//		//{
//		//	//printf("%f ms/frame\n", 1000.0 / double(nbFrames));
//		//	nbFrames = 0;
//		//	startFrame += 1.0;
//		//}
//
//		// Manage user input(s)
//
//		// Update Game state
//
//		glm::mat4 projection(1.0f);
//		switch (projection_type)
//		{
//		case PROJECTION_NORMALIZED_ORIGIN_CENTER:
//			projection = glm::ortho(static_cast<GLfloat>(SCREEN_WIDTH*(-1) / 2), static_cast<GLfloat>(SCREEN_WIDTH / 2), static_cast<GLfloat>(SCREEN_HEIGHT*(-1) / 2), static_cast<GLfloat>(SCREEN_HEIGHT / 2), -1.0f, 1.0f);
//			ResourceManager::GetShader("square_shader").SetMatrix4("projection", projection);
//			break;
//		case PROJECTION_NORMALIZED_ORIGIN_TOP_LEFT:
//			projection = glm::ortho(0.0f, static_cast<GLfloat>(SCREEN_WIDTH), static_cast<GLfloat>(SCREEN_HEIGHT), 0.0f, -1.0f, 1.0f);
//			ResourceManager::GetShader("square_shader").SetMatrix4("projection", projection);
//			break;
//		default:
//			break;
//		}
//
//		// Rendering GL entities
//		glm::mat4 model(1.0f);					// POSITION
//		model = glm::translate(model, glm::vec3(-310.0f, -150, 0.0f));
//		model = glm::scale(model, glm::vec3(100.0f, 100.0f, 1.0f));
//		ResourceManager::GetShader("square_shader").SetMatrix4("model", model);
//		glBindVertexArray(VAO);
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//		model = glm::mat4(1.0f);               // x  // y  // z
//		model = glm::translate(model, glm::vec3(50.0f, 200.0f, 0.0f));
//		model = glm::scale(model, glm::vec3(100.0f, 100.0f, 1.0f));
//		ResourceManager::GetShader("square_shader").SetMatrix4("model", model);
//		glBindVertexArray(VAO);
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//
//		ImGui::Render();
//
//		// Clear ?
//		int display_w, display_h;
//		glfwMakeContextCurrent(window);
//		glfwGetFramebufferSize(window, &display_w, &display_h);
//		glViewport(0, 0, display_w, display_h);
//		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT/* | GLFW_DOUBLEBUFFER*/);
//		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//		glfwMakeContextCurrent(window);
//		glfwSwapBuffers(window);
//	}
//
//	// ImGui cleanup
//	ImGui_ImplOpenGL3_Shutdown();
//	ImGui_ImplGlfw_Shutdown();
//	ImGui::DestroyContext();
//
//	glfwDestroyWindow(window);
//	glfwTerminate();
//	return 0;
//}
//
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
//{
//	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, GL_TRUE);
//	if (key >= 0 && key < 1024)
//	{
//		if (action == GLFW_REPEAT) return; // Blocks REPEAT behavior
//
//		// DreamInEngine.Keys[key] = action;
//
//		// Did this beause, according to documentation:
//		// #define GLFW_RELEASE 0
//		// #define GLFW_PRESS 1
//		// #define GLFW_REPEAT 2
//	}
//}

#include <ResourceManager.h>

#include <GLFWEnvironment.h>

#include <scenes/SceneManager.h>

int main() {
	// @TODO: in Init() method when we will be able
	//    Window w = new Window(glfwenvironment);
	//    shadermanager sm;
	//    texturemanager tm;
	//    imgui imgui;
	//    Graphics g = new Graphics(w,sm,tm,imgui);
	//    Textu t = new Textu("hh.png");
	//    w.setTexture(t, 4, 5);
	//    t.setPosition(getWigth()+55, getHeight);
	// @TODO: see width & height params;
	GLFWEnvironment* m_glfw_environment = new GLFWEnvironment("DreamIn Engine", 800, 600);
	m_glfw_environment->init();

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		throw std::runtime_error("glewInit failed");

	/*m_ImGui_HUD = new ImGuiHUD(*this, m_glfw_environment, true);
	m_ImGui_HUD->init();*/

	// @TODO: in imgui
	// push up play button => { imgui.disable_interface(); }

	// Load Shaders (program, vertex, fragment)
	ResourceManager::LoadShader("resources/shaders/sprite.vert", "resources/shaders/sprite.frag", nullptr, "sprite_shader");

	// Load textures
	ResourceManager::LoadTexture("resources/textures/container.jpg", GL_TRUE, "container");
	ResourceManager::LoadTexture("resources/textures/awesomeface.png", GL_TRUE, "face");
	ResourceManager::LoadTexture("resources/textures/pokeball_200px.png", GL_TRUE, "pokeball");

	SceneManager *sm = new SceneManager(m_glfw_environment);

	/* Create one Scene in the Scene Manager */
	sm->createScene("Aloha");

	/* Get the Scene Manager's first scene */
	//auto scene1 = sm->getScenes()[0];
	//std::cout << sm->getScene("Aloha")->getName() << std::endl;
	sm->getScene("Aloha")->getEntities().emplace_back(Entity("container_entity", 0));
	sm->getScene("Aloha")->getEntities().emplace_back(Entity("face_entity", 1));
	sm->getScene("Aloha")->getEntities().emplace_back(Entity("pokeball_entity", 2));
	std::cout << sm->getScene("Aloha")->getEntities()[0].name << std::endl;

	sm->new_sprite("Aloha", sm->getScene("Aloha")->getEntities()[0], ResourceManager::GetTexture("container"), glm::vec2(50.0f, 50.0f));
	sm->new_sprite("Aloha", sm->getScene("Aloha")->getEntities()[1], ResourceManager::GetTexture("face"), glm::vec2(100.0f, 100.0f));
	sm->new_sprite("Aloha", sm->getScene("Aloha")->getEntities()[2], ResourceManager::GetTexture("pokeball"), glm::vec2(200.0f, 200.0f));

	sm->addBox2D("Aloha", sm->getScene("Aloha")->getEntities()[0],sm->getScene("Aloha")->getSprites().get(sm->getScene("Aloha")->getEntities()[0].id), true);

	sm->start();

	return 0;
}
