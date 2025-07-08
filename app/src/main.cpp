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
    //variables for toggling between GL_LINE and GL_FILL
    static bool s_wireframeMode = false;
    static bool s_enterState = false;
    bool enterPressed = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;
    //if the user presses enter, toggle wireframe mode
    if (enterPressed && !s_enterState)
    {
        s_wireframeMode = !s_wireframeMode;
        glPolygonMode(GL_FRONT_AND_BACK, s_wireframeMode ? GL_LINE : GL_FILL);
    }
    s_enterState = enterPressed;

}





// Vertex Shader Source Code -------------- (calculates vertex position)
const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
	"out vec3 ourColor;\n" // specify the output color to the fragment shader
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
	"   ourColor = aColor;\n" // set output color
    "}\0";

// Fragment Shader Source Code (orange full opacity)
const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "uniform vec4 u_ourColor;\n" // uniform variable we set this variable in the OpenGL code.
    "void main()\n" 
    "{\n"
	"   FragColor = u_ourColor;\n" // set the output color to the input color
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
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };
    /*
	// for element buffer object (EBO) (to avoid duplicating vertices, we can use indices to reference the vertices)
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    */






    // vertex array object (VAO) ------------------ (easy to switch between attribute configurations, can bind VOA to draw function)
	unsigned int VAO; // Vertex Array Object
	glGenVertexArrays(1, &VAO); // Generate a vertex array and store its ID in VAO
	glBindVertexArray(VAO); // Bind the vertex array object (VAO) to the current context





    /*
	// Element Buffer Object (EBO) ------------------ (to avoid duplicating vertices, we can use indices to reference the vertices)
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    // 3. copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    */


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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// enable the vertex attribute array at index 0 (the first attribute)
    glEnableVertexAttribArray(0);

	// linking color attributes to the shader program
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);






	// render loop (double buffer)
    while (!glfwWindowShouldClose(window))
    {
		processInput(window); // process input (check for key presses, mouse movements, etc.)
		// drawing code ------------------
  
		// clear the screen
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // set the clear color (background color)
		glClear(GL_COLOR_BUFFER_BIT); // clear the color buffer (set the background color)
        glUseProgram(shaderProgram);

        // update the uniform color
        
        // update the uniform color
        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "u_ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);


		// bind the VAO and draw the triangle
        glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // draw the triangle using the EBO and the indices
		glDrawArrays(GL_TRIANGLES, 0, 3); // draw the triangle using the VBO and the vertices
        glBindVertexArray(0); // unbind the VAO (optional, but good practice)
		glfwPollEvents(); // check for events (like key presses, mouse movements, etc.)
        glfwSwapBuffers(window); // swap the front and back buffers

    }
	// Cleanup and exit
    glfwTerminate();
    return 0;
}