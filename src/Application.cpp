#include <cmath>

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "ConsoleMagic.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


const unsigned int WINDOW_W = 800, WINDOW_H = 600;
const float ASPECT_RATIO = float(WINDOW_W) / float(WINDOW_H);


static bool paused = false;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        paused = !paused;
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize GLFW. */
    if (glfwInit())
    {
        std::cout << "GLFW Initialization Success" << std::endl;
    }
    else
    {
        std::cout << "GLFW Initialization Failure" << std::endl;
        return -1;
    }

    /* IDK what this does but it makes stuff run faster. */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a non-fullscreen window. */
    if (window = glfwCreateWindow(WINDOW_W, WINDOW_H, "2D RPG Game Engine", NULL, NULL))
    {
        std::cout << "GLFW Window Creation Success" << std::endl;
    }
    else
    {
        std::cout << "GLFW Window Creation Failure" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);

    /* Make the window's context current. */
    glfwMakeContextCurrent(window);
    /* Initialize GLEW. "glewInit" has to be called when there is a valid GLFW window context. */
    if (glewInit() == GLEW_OK)
    {
        std::cout << "GLEW Initialization Success" << std::endl;
    }
    else
    {
        std::cout << "GLEW Initialization Failure" << std::endl;
        glfwTerminate();
        return -1;
    }

    //Output current GLFW version.
    outputLine();
    std::cout << "GLFW Version: " << glfwGetVersionString() << std::endl;
    outputLine();

    //Putting program code in its own scope to avoid weird OpenGL errors.
    {
        //Positional Data.
        float quad_data[16] = {
           200.0f, 500.0f, 0.0f, 1.0f,   //north west
           600.0f, 500.0f, 1.0f, 1.0f,   //north east
           600.0f, 100.0f, 1.0f, 0.0f,   //south east
           200.0f, 100.0f, 0.0f, 0.0f    //south west
        };
        //Specifies the order in which to connect the vertices of "quad_data" such that it creates two unique triangles.
        unsigned int quad_idxs[6] = {
            0, 1, 3,    //northern western triangle
            1, 2, 3     //southern eastern triangle
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray vao;  //Vertex Attribute Object;
        VertexBuffer vbo(quad_data, 16 * sizeof(float));     //Vertex Buffer Object

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        vao.AddBuffer(vbo, layout);

        IndexBuffer ibo(quad_idxs, 6);

        glm::mat4 proj = glm::ortho(0.0f, float(WINDOW_W), 0.0f, float(WINDOW_H), -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(100, 100, 0));
        
        glm::mat4 mvp = proj * view * model;

        Shader BasicShader("res/shaders/BasicShader.shader");
        BasicShader.Bind();
        BasicShader.SetUniformMat4f("u_MVP", mvp);

        Texture texture("res/textures/grass_side.png");
        texture.Bind();
        BasicShader.SetUniform1i("u_Texture", 0);


        vao.Unbind();
        BasicShader.Unbind();
        vbo.Unbind();
        ibo.Unbind();

        Renderer renderer;


        float ri = 0, r = 0, gi = 0, g = 0, bi = 0, b = 0;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            if (!paused)
            {
                ri += 0.01f; r = (sinf(ri) + 1) / 2; r = (r < 0.1f) ? 0.1f : r;
                gi += 0.02f; g = (sinf(gi) + 1) / 2; g = (g < 0.1f) ? 0.1f : g;
                bi += 0.03f; b = (sinf(bi) + 1) / 2; b = (b < 0.1f) ? 0.1f : b;
            }

            /* Render here */
            renderer.Clear();

            BasicShader.Bind();
            BasicShader.SetUniform4f("u_Color", r, g, b, 1.0f);

            renderer.Draw(vao, ibo, BasicShader);

            /* Swap frot and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    std::cout << "Application Closed Successfully" << std::endl;
    glfwTerminate();
    return 1;
}