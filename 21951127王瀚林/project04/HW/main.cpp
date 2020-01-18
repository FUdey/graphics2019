#define GLEW
#define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"
#include "Cat.h"
#include "Block.h"

const GLuint WIDTH = 800, HEIGHT = 600;
GLuint texture[3];
glm::mat4 trans = glm::mat4(1.0f);

GLfloat vertices[] = {
-0.5f, 0.0f, -0.5f,  0.0f, 0.0f,
 0.5f, 0.0f, -0.5f,  1.0f, 0.0f,
 0.5f,  1.0f, -0.5f,  1.0f, 1.0f,
 0.5f,  1.0f, -0.5f,  1.0f, 1.0f,
-0.5f,  1.0f, -0.5f,  0.0f, 1.0f,
-0.5f, 0.0f, -0.5f,  0.0f, 0.0f,

-0.5f, 0.0f,  0.5f,  0.0f, 0.0f,
 0.5f, 0.0f,  0.5f,  1.0f, 0.0f,
 0.5f,  1.0f,  0.5f,  1.0f, 1.0f,
 0.5f,  1.0f,  0.5f,  1.0f, 1.0f,
-0.5f,  1.0f,  0.5f,  0.0f, 1.0f,
-0.5f, 0.0f,  0.5f,  0.0f, 0.0f,

-0.5f,  1.0f,  0.5f,  1.0f, 0.0f,
-0.5f,  1.0f, -0.5f,  1.0f, 1.0f,
-0.5f, 0.0f, -0.5f,  0.0f, 1.0f,
-0.5f, 0.0f, -0.5f,  0.0f, 1.0f,
-0.5f, 0.0f,  0.5f,  0.0f, 0.0f,
-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

 0.5f,  1.0f,  0.5f,  1.0f, 0.0f,
 0.5f,  1.0f, -0.5f,  1.0f, 1.0f,
 0.5f, 0.0f, -0.5f,  0.0f, 1.0f,
 0.5f, 0.0f, -0.5f,  0.0f, 1.0f,
 0.5f, 0.0f,  0.5f,  0.0f, 0.0f,
 0.5f,  1.0f,  0.5f,  1.0f, 0.0f,

-0.5f, 0.0f, -0.5f,  0.0f, 1.0f,
 0.5f, 0.0f, -0.5f,  1.0f, 1.0f,
 0.5f, 0.0f,  0.5f,  1.0f, 0.0f,
 0.5f, 0.0f,  0.5f,  1.0f, 0.0f,
-0.5f, 0.0f,  0.5f,  0.0f, 0.0f,
-0.5f, 0.0f, -0.5f,  0.0f, 1.0f,

-0.5f,  1.0f, -0.5f,  0.0f, 1.0f,
 0.5f,  1.0f, -0.5f,  1.0f, 1.0f,
 0.5f,  1.0f,  0.5f,  1.0f, 0.0f,
 0.5f,  1.0f,  0.5f,  1.0f, 0.0f,
-0.5f,  1.0f,  0.5f,  0.0f, 0.0f,
-0.5f,  1.0f, -0.5f,  0.0f, 1.0f
};


//���λ��
glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
Camera camera(cameraPos, glm::radians(-40.0f), glm::radians(180.0f), cameraUp);

//������
float lastX, lastY;
bool firstMouse = true;

//ˢ��ʱ������
double OPTIMAL_TIME = 1.0 / 60.0;

//ƽ̨��è��ʼ��
Cat cat;
Block block[3];


void processInput(GLFWwindow* window); //�������
void LoadGLTextures(GLuint* texture, const char* bmp_file_name, int tex_id);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	GLFWwindow* window = glfwCreateWindow(800, 600, "opengl", NULL, NULL);
	glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    Shader testShader = Shader("shader.vert", "shader.frag");

    block[0].SetBlock(0, 1.0f);
    block[1].SetBlock(-2, 1.0f);
    block[2].SetBlock(-5, 1.0f);

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glGenBuffers(1, &EBO);   
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    LoadGLTextures(texture, "cat.jpg", 0);
    LoadGLTextures(texture, "block.jpg", 1);


    // λ�ò���
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // ��ɫ����
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    //glEnableVertexAttribArray(1);
    // ���ʲ���
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); 



    // �任�����ʼ��
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::rotate(modelMat, glm::radians(-55.0f), glm::vec3(1.0f, 0, 0.0f));
    glm::mat4 viewMat = glm::mat4(1.0f);
    //viewMat = glm::translate(viewMat, glm::vec3(0, 0, -3.0f));
    //viewMat = camera.GetViewMatrix();
    glm::mat4 projMat = glm::mat4(1.0f);
    projMat = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // ˢ������������ʼ��
    double lastTime = glfwGetTime();
    double accumulator = 0.0;
    double now = lastTime;

    // ����任�����ʼ��
    glm::vec3 baseTrans = glm::vec3(0.0f, 0.0f, 0.0f); //è�����任����
    glm::vec3 pressScale = glm::vec3(1.0f); // è��ѹ����
    glm::vec3 jumpTrans = glm::vec3(0.0f); // ��Ծ����
    glm::vec3 baseTrans_plt = glm::vec3(0.0f, -0.2f, 0.0f); //ƽ̨����λ�ƾ���
    glm::vec3 baseScale_plt = glm::vec3(2.0f, 0.2f, 2.0f); //ƽ̨�������ž���
    glm::vec3 moveTrans_plt[3] = { glm::vec3(0.0f) }; //��ƽ̨λ�ƾ���
    

	while (!glfwWindowShouldClose(window)) {    
		processInput(window); //������
		glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //��1/60����м���
        double now = glfwGetTime();
        double delta = now - lastTime;

        accumulator += delta;
        lastTime = now;
        //std::cout << delta << std::endl;

        //��ָ��ˢ����ˢ������
        while (accumulator >= OPTIMAL_TIME)
        {
            accumulator -= OPTIMAL_TIME;
            //è״̬ˢ��
            cat.UpdateCat();
            if (cat.isDead())
            {
                cat.Reset();
                block[0].SetBlock(0, 1.0f);
                block[1].SetBlock(-2, 1.0f);
                block[2].SetBlock(-5, 1.0f);
            }
            pressScale = glm::vec3(0.5f, 0.5f - cat.Power * 0.3f, 0.5f);
            jumpTrans = glm::vec3(0, cat.Height, 0);

            //ƽ̨״̬ˢ��
            bool isSafe = false;
            for (int i = 0; i < 3; i++)
            {
                if (cat.inAir()) {
                    block[i].MoveBlock(); 
                    if (block[i].isCoverd(0.2f)) //ƽ̨���
                    {
                        isSafe = true;
                    }
                }
                else
                {
                    isSafe = true;
                }
                moveTrans_plt[i] = glm::vec3(0, 0, block[i].zPos);
            }
            cat.Safe = isSafe;
            std::cout << cat.Status << std::endl;
            
        }

        viewMat = camera.GetViewMatrix();
        testShader.use();
        
        //è��Ⱦ
        glm::mat4 modelMat_cat = glm::mat4(1.0f);
        
        modelMat_cat = glm::translate(modelMat_cat, baseTrans);
        modelMat_cat = glm::scale(modelMat_cat, pressScale);
        modelMat_cat = glm::translate(modelMat_cat, jumpTrans);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glUniform1i(glGetUniformLocation(testShader.ID, "ourTexture"), 0);

        glUniformMatrix4fv(glGetUniformLocation(testShader.ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat_cat));
        glUniformMatrix4fv(glGetUniformLocation(testShader.ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
        glUniformMatrix4fv(glGetUniformLocation(testShader.ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));

        glBindVertexArray(VAO);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        //ƽ̨��Ⱦ
        for (int i = 0; i < 3; i++)
        {
            glm::mat4 modelMat_plt = glm::mat4(1.0f);

            modelMat_plt = glm::translate(modelMat_plt, baseTrans_plt);
            modelMat_plt = glm::scale(modelMat_plt, baseScale_plt);
            modelMat_plt = glm::scale(modelMat_plt, glm::vec3(1.0f * block[i].Width, 1.0f, 1.0f * block[i].Width));
            modelMat_plt = glm::translate(modelMat_plt, moveTrans_plt[i]);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture[1]);
            glUniform1i(glGetUniformLocation(testShader.ID, "ourTexture"), 1);

            glUniformMatrix4fv(glGetUniformLocation(testShader.ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat_plt));
            glUniformMatrix4fv(glGetUniformLocation(testShader.ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
            glUniformMatrix4fv(glGetUniformLocation(testShader.ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));

            glBindVertexArray(VAO);
            //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


		glfwSwapBuffers(window);
        glfwPollEvents();
        camera.UpdateCameraPos();
	}

	glfwTerminate();
	return 0;
}




void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.speedZ = 0.1f;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.speedZ = -0.1f;
    }
    else{
        camera.speedZ = 0;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cat.ChargeSpeed = 0.05f;
    }
    else {
        if (cat.ChargeSpeed != 0) {
            cat.ChargeSpeed = 0;
        }
    }
}


/*
��������
*/
void LoadGLTextures(GLuint* texture, const char* bmp_file_name, int tex_id) {


    glGenTextures(1, &texture[tex_id]);
    glBindTexture(GL_TEXTURE_2D, texture[tex_id]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    //stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(bmp_file_name, &width, &height, &nrChannels, 0);
    std::cout << "found tex channel:" << nrChannels << std::endl;
    if (data)
    {
        if (nrChannels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        else if (nrChannels == 1)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
        else
            std::cout << "unknow channel" << std::endl;
        std::cout << "load texture" << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float deltaX, deltaY;
    deltaX = xPos - lastX;
    deltaY = yPos - lastY;

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(deltaX, deltaY);
}