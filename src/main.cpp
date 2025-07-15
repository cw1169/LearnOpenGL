#include <iostream>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


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


    // Build Shader ----------------
	Shader shader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");


    // Textures ------------------

    // generate a texture ID and bind it to the GL_TEXTURE_2D target
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Handles texture wrapping if needed
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // Texture Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // uses linear filtering for minification between mipmaps
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // use linear filtering for magnification

	// loading texture using stb_image
    int width, height, nrChannels;
    unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

    // generate texture
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // (texture target, mipmap layer, storage format, width, height, legacy stuff, source format, source datatype, image data)
        glGenerateMipmap(GL_TEXTURE_2D); // generate mipmaps
    }
    else 
    {
		std::cout << "Failed to load texture" << std::endl;
    }
	stbi_image_free(data); // free the image data after generating the texture



	// Vertex Attributes -------------
    // Steps
	// 0, Copy vertices array to gpu memory (VBO)
	// 1, set vertiex attribute pointers (how to interpret the data in the VBO)
	// 2, use the shader program (which will use the vertex attributes)
    // 3, draw object
    // VBO shaders
     // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);




	// render loop (double buffer)
    while (!glfwWindowShouldClose(window))
    {
		processInput(window); // process input (check for key presses, mouse movements, etc.)
		// drawing code ------------------
  
		// clear the screen
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // set the clear color (background color)
		glClear(GL_COLOR_BUFFER_BIT); // clear the color buffer (set the background color)
        
		shader.use(); // use the shader program

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