#include "window.h"

const char* window_title = "CSE163 Project 2";
Cube * cube;
GLint shaderProgram;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "shaders/shader.vert"
#define FRAGMENT_SHADER_PATH "shaders/shader.frag"

#define OFF_PATH "models/cow.off"
#define PLANE "models/plane.off"
#define TEAPOT "models/teapot.off"
#define TORUS "models/torus.off"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 1.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

camera scene_camera(cam_pos, cam_look_at, cam_up);
double last_x, last_y;

bool left_mouse_down, right_mouse_down;

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

off_model cow, plane, pot, torus;
off_model* active;

void Window::initialize_objects()
{
	cube = new Cube();

    cow = off_model(OFF_PATH);
    plane = off_model(PLANE);
    pot = off_model(TEAPOT);
    torus = off_model(TORUS);
    
    active = &cow;

	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(cube);
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(scene_camera.get_pos(), scene_camera.get_look_at(), scene_camera.get_up());
	}
}

void Window::idle_callback()
{
	// Call the update function the cube
	//cube->update();
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	glUseProgram(shaderProgram);
	
    GLint viewPosLoc = glGetUniformLocation(shaderProgram, "view_pos");
    glUniform3f(viewPosLoc, scene_camera.get_pos().x, scene_camera.get_pos().y, scene_camera.get_pos().z);  


	// Render the cube
	//cube->draw(shaderProgram);
    active->draw(shaderProgram);


	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{       
        if (key == GLFW_KEY_S && mods == GLFW_MOD_SHIFT){
            active->scale(true);
        }
        if (key == GLFW_KEY_S && mods == 0){
            active->scale(false);
        }
        if (key == GLFW_KEY_C && mods == 0){
            active->edge_collapse(0, 1);
        }
        if (key == GLFW_KEY_C && mods == GLFW_MOD_SHIFT){
            active->edge_collapse(0, 9);
        }
        if (key == GLFW_KEY_P && mods == 0){
            active->print_vfa();
        }
        if (key == GLFW_KEY_R && mods == 0){
            active->read_edge_collapses();
        }
        if (key == GLFW_KEY_V && mods == 0){
            active->vertex_split();
        }
        if (key == GLFW_KEY_U){
            active = &cow;
        }
        if (key == GLFW_KEY_I){
            active = &plane;
        }
        if (key == GLFW_KEY_O){
            active = &torus;
        }
        if (key == GLFW_KEY_P){
            active = &pot;
        }
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
}

void Window::mouse_button_callback(GLFWwindow* window, int key, int action, int mods){
    if(key == GLFW_MOUSE_BUTTON_RIGHT){
        right_mouse_down = (action == GLFW_PRESS);
    }
    if(key == GLFW_MOUSE_BUTTON_LEFT){
        left_mouse_down = (action == GLFW_PRESS);
        glfwGetCursorPos(window, &last_x, &last_y);
    }
}

void Window::mouse_move_callback(GLFWwindow* window, double xpos, double ypos){
    if(left_mouse_down){
        //Trackball rotation
        scene_camera.rotate(last_x, last_y, xpos, ypos);
        // Now update the camera
        V = glm::lookAt(scene_camera.get_pos(), scene_camera.get_look_at(), scene_camera.get_up());
        last_x = xpos;
        last_y = ypos;
    }
}
