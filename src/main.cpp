#include <iostream>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

// Callback function to adjust the viewport when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// processing input
// close the window when the escape key is pressed
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Vertex Shader Source Code
const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

// Fragment Shader Source Code (orange full opacity)
const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";



int main()
{
	// Initialize GLFW tell it to use OpenGL 3.3 core profile
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    

	// Create a GLFW window -------------------
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	// Make the window's context current
    glfwMakeContextCurrent(window);

	// Initialize GLAD to configure OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

	// tell GLFW the size of the viewport
    glViewport(0, 0, 800, 600);

    // window resize callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// Vertex Attributes -------------
    // Steps
	// 0, Copy vertices array to gpu memory (VBO)
	// 1, set vertiex attribute pointers (how to interpret the data in the VBO)
	// 2, use the shader program (which will use the vertex attributes)
    // 3, draw object
    // VBO shaders
	unsigned int VBO; // Vertex Buffer Object
	glGenBuffers(1, &VBO); // Generate a buffer and store its ID in VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // I am using this buffer as a vertex buffer

	// Vertex data for a triangle
    // 2D triangle as Z axis is 0
    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };

	// Copy the vertex data to the buffer
	// target buffer type, size of the data, pointer to the data, usage pattern (static = data is set once and used many times)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



	//vertex shader------------ calculates vertex position
	//create vertex shader object and assign it an ID
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// attach the shader source code to the shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

	// Check for shader compilation errors
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }



	// Fragment Shader ------------ calcualtes pixel color
	// create fragment shader object and assign it an ID
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// attach the shader source code to the shader object
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);



	// Shader Program ------------- Links shader steps together
	// create a shader program object
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    
	// attach the vertex and fragment shaders to the shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram); // link the shader program

	//check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }

	// use the shader program
    glUseProgram(shaderProgram);

	// delete the shader objects as they are no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



	// linking vertex attributes to the shader program ------------ (tell the shaders how to interpret the data stored in the gpu)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// enable the vertex attribute array at index 0 (the first attribute)
    glEnableVertexAttribArray(0);


	// render loop (double buffer)
    while (!glfwWindowShouldClose(window))
    {
		processInput(window); // process input (check for key presses, mouse movements, etc.)
		glfwSwapBuffers(window); // swap the front and back buffers
		glfwPollEvents(); // check for events (like key presses, mouse movements, etc.)
    }
	// Cleanup and exit
    glfwTerminate();
    return 0;
}