#include <UGL/UGL>
#include <UGM/UGM>

#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "../../tool/Camera.h"
#include "../../tool/SimpleLoader.h"

#include <iostream>
#include <map>
#include <vector>
#include "ANN/ANN.h"	
using namespace Ubpa;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
gl::Texture2D loadTexture(char const* path);
gl::Texture2D genDisplacementmap(const SimpleLoader::OGLResources* resources);

// settings
unsigned int scr_width = 800;
unsigned int scr_height = 600;
float displacement_bias = 0.f;
float displacement_scale = 1.f;
float displacement_lambda = 0.2f;
bool have_denoise = false;

// camera
Camera camera(pointf3(0.0f, 0.0f, 3.0f));
float lastX = scr_width / 2.0f;
float lastY = scr_height / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(scr_width, scr_height, "HW8 - denoise", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    gl::Enable(gl::Capability::DepthTest);

    // build and compile our shader zprogram
    // ------------------------------------
    gl::Shader vs(gl::ShaderType::VertexShader, "../data/shaders/p3t2n3_denoise.vert");
    gl::Shader fs(gl::ShaderType::FragmentShader, "../data/shaders/light.frag");
    gl::Program program(&vs, &fs);
    rgbf ambient{ 0.2f,0.2f,0.2f };
    program.SetTex("albedo_texture", 0);
    program.SetTex("displacementmap", 1);
    program.SetVecf3("point_light_pos", { 0,5,0 });
    program.SetVecf3("point_light_radiance", { 100,100,100 });
    program.SetVecf3("ambient_irradiance", ambient);
    program.SetFloat("roughness", 0.5f );
    program.SetFloat("metalness", 0.f);

    // load model
    // ------------------------------------------------------------------
    auto spot = SimpleLoader::LoadObj("../data/models/spot_triangulated_good.obj", true);
    // world space positions of our cubes
    pointf3 instancePositions[] = {
        pointf3(0.0f,  0.0f,  0.0f),
        pointf3(2.0f,  5.0f, -15.0f),
        pointf3(-1.5f, -2.2f, -2.5f),
        pointf3(-3.8f, -2.0f, -12.3f),
        pointf3(2.4f, -0.4f, -3.5f),
        pointf3(-1.7f,  3.0f, -7.5f),
        pointf3(1.3f, -2.0f, -2.5f),
        pointf3(1.5f,  2.0f, -2.5f),
        pointf3(1.5f,  0.2f, -1.5f),
        pointf3(-1.3f,  1.0f, -1.5f)
    };

    // load and create a texture 
    // -------------------------
    gl::Texture2D spot_albedo = loadTexture("../data/textures/spot_albedo.png");

    gl::Texture2D displacementmap = genDisplacementmap(spot);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        gl::ClearColor({ ambient, 1.0f });
        gl::Clear(gl::BufferSelectBit::ColorBufferBit | gl::BufferSelectBit::DepthBufferBit); // also clear the depth buffer now!

        program.SetVecf3("camera_pos", camera.Position);

        // bind textures on corresponding texture units
        program.Active(0, &spot_albedo);
        program.Active(1, &displacementmap);

        // pass projection matrix to shader (note that in this case it could change every frame)
        transformf projection = transformf::perspective(to_radian(camera.Zoom), (float)scr_width / (float)scr_height, 0.1f, 100.f);
        program.SetMatf4("projection", projection);

        // camera/view transformation
        program.SetMatf4("view", camera.GetViewMatrix());
        program.SetFloat("displacement_bias", displacement_bias);
        program.SetFloat("displacement_scale", displacement_scale);
        program.SetFloat("displacement_lambda", displacement_lambda);
        program.SetBool("have_denoise", have_denoise);

        // render spots
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            float angle = 20.0f * i + 10.f * (float)glfwGetTime();
            transformf model(instancePositions[i], quatf{ vecf3(1.0f, 0.3f, 0.5f), to_radian(angle) });
            program.SetMatf4("model", model);
            spot->va->Draw(&program);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    delete spot;

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Movement::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Movement::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Movement::DOWN, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        have_denoise = !have_denoise;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            displacement_lambda += 0.001;
            std::cout << "lambda:" << displacement_lambda << std::endl;
        }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            displacement_lambda -= 0.001;
            std::cout << "lambda:" << displacement_lambda << std::endl;
        }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    gl::Viewport({ 0, 0 }, width, height);
    scr_width = width;
    scr_height = height;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    float xoffset = static_cast<float>(xpos) - lastX;
    float yoffset = lastY - static_cast<float>(ypos); // reversed since y-coordinates go from bottom to top

    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    camera.ProcessMouseMovement(static_cast<float>(xoffset), static_cast<float>(yoffset));
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

gl::Texture2D loadTexture(char const* path)
{
    gl::Texture2D tex;
    tex.SetWrapFilter(gl::WrapMode::Repeat, gl::WrapMode::Repeat, gl::MinFilter::Linear, gl::MagFilter::Linear);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    gl::PixelDataFormat c2f[4] = {
        gl::PixelDataFormat::Red,
        gl::PixelDataFormat::Rg,
        gl::PixelDataFormat::Rgb,
        gl::PixelDataFormat::Rgba
    };
    gl::PixelDataInternalFormat c2if[4] = {
        gl::PixelDataInternalFormat::Red,
        gl::PixelDataInternalFormat::Rg,
        gl::PixelDataInternalFormat::Rgb,
        gl::PixelDataInternalFormat::Rgba
    };
    if (data)
    {
        tex.SetImage(0, c2if[nrChannels - 1], width, height, c2f[nrChannels - 1], gl::PixelDataType::UnsignedByte, data);
        tex.GenerateMipmap();
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return tex;
}

gl::Texture2D genDisplacementmap(const SimpleLoader::OGLResources* resources) {
    //const float* displacementData = new float[1024 * 1024];
    float* displacementData = new float[1024 * 1024];
    // TODO: HW8 - 1_denoise | genDisplacementmap
    // 1. set displacementData with resources's positions, indices, normals, ...
    std::map<size_t, std::vector<size_t>>fix_map;

    for (size_t i = 0; i < resources->positions.size(); i++)
    {
        for (size_t j = 0; j < resources->positions.size(); j++)
        {
            if (resources->positions[i].distance(resources->positions[j]) < 1e-5 && i != j)
            {
                if (!fix_map.count(i))
                {
                    fix_map[i] = std::vector<size_t>();
                }
                fix_map[i].push_back(j);
            }
        }
    }

    std::vector<vecf3> sum(resources->positions.size(), vecf3(0.f)); // Sum of the positions of the points
    std::vector<float> num(resources->positions.size(), 0);  // Numbers of the points
    std::vector<vecf3> delta_list(resources->positions.size());
    std::map<pointi2, float> data_list;

    for (size_t i = 0; i < resources->indices.size(); i += 3)
 {
     size_t idx0 = resources->indices[i];
     size_t idx1 = resources->indices[i + 1];
     size_t idx2 = resources->indices[i + 2];
     num[idx0] += 2;
     num[idx1] += 2;
     num[idx2] += 2;
     sum[idx0] += resources->positions[idx1].cast_to<vecf3>() + resources->positions[idx2].cast_to<vecf3>();
     sum[idx1] += resources->positions[idx0].cast_to<vecf3>() + resources->positions[idx2].cast_to<vecf3>();
     sum[idx2] += resources->positions[idx0].cast_to<vecf3>() + resources->positions[idx1].cast_to<vecf3>();
 }

    for (size_t i = 0; i < resources->positions.size(); i++)
    {
        auto a = resources->positions[i];
        for (size_t j = i + 1; j < resources->positions.size(); j++)
        {
            auto b = resources->positions[j];
            if ((a - b).norm() < 1e-5)
            {
                num[i] += num[j];
                sum[i] += sum[j];
            }
        }
    }

    std::cout << " set displacementData done" << std::endl;
 // 2. change global variable: displacement_bias, displacement_scale, displacement_lambda
    float first_val = (resources->positions[0].cast_to<vecf3>() - sum[0].cast_to<vecf3>() / num[0]).dot(resources->normals[0].cast_to<vecf3>());
    float max_val = first_val;
    float min_val = first_val;
    for (size_t i = 0; i < delta_list.size(); i++)
    {
        delta_list[i] = resources->positions[i].cast_to<vecf3>() - sum[i].cast_to<vecf3>() / num[i];
        float val = delta_list[i].dot(resources->normals[i].cast_to<vecf3>());
        if (fix_map.count(i))
        {
            for (auto x : fix_map[i])
            {
                delta_list[x] = resources->positions[x].cast_to<vecf3>() - sum[x].cast_to<vecf3>() / num[x] ;
                val += delta_list[x].dot(resources->normals[x].cast_to<vecf3>());
            }
            val /= fix_map.size() + 1;
        }
        if (resources->texcoords[i][0] >= 0 && resources->texcoords[i][1] >= 0)
        {
            displacementData[long(resources->texcoords[i][0] * 1024) * 1024 + long(resources->texcoords[i][1] * 1024)] = val;
        }
        data_list[pointi2(int(resources->texcoords[i][0] * 1024), int(resources->texcoords[i][1] * 1024))] = val;
        min_val = min_val > val ? val : min_val;
        max_val = max_val < val ? val : max_val;
    }

    //  参数 取λ= 0.7
    displacement_scale = max_val - min_val;
    displacement_bias = min_val;
    displacement_lambda = 0.7;

    // 用ANN填充
    int count = 1;
    int ARR_n = 0;
    int threshold = 100;
    constexpr int DIM = 2;
    constexpr int K = 1;	//取K个点
    ANNpointArray ptsArr = annAllocPts(1024 * 1024, DIM);
    for (auto i : data_list)
    {
        ptsArr[ARR_n][0] = i.first[0];
        ptsArr[ARR_n][1] = i.first[1];
        ARR_n++;
    }
    ANNkd_tree* kdtree = new ANNkd_tree(ptsArr, ARR_n, DIM);
    ANNpoint queryPt = annAllocPt(2);
    for (int x = 0; x < 1024; x++)
    {
        for (int y = 0; y < 1024; y++)
        {
            ANNidx idxArr[K];
            ANNdist distArr[K];
            queryPt[0] = x;
            queryPt[1] = y;
            float weight_sum = 0;
            kdtree->annkSearch(queryPt, K, idxArr, distArr);
            for (int k = 0; k < K; k++)
            {
                int m = (int)ptsArr[idxArr[k]][0];
                int n = (int)ptsArr[idxArr[k]][1];
                float weight = data_list[pointi2(m, n)];
                weight_sum += weight;
            }
            displacementData[y * 1024 + x] = (weight_sum / K - displacement_bias) / displacement_scale;
        }
    }
     
    annDeallocPts(ptsArr);
    annDeallocPt(queryPt);
 std::cout << " change global variable done" << std::endl;
    // ...

    gl::Texture2D displacementmap;
    displacementmap.SetImage(0, gl::PixelDataInternalFormat::Red, 1024, 1024, gl::PixelDataFormat::Red, gl::PixelDataType::Float, displacementData);
    displacementmap.SetWrapFilter(gl::WrapMode::Repeat, gl::WrapMode::Repeat,
        gl::MinFilter::Linear, gl::MagFilter::Linear);
    stbi_uc* stbi_data = new stbi_uc[1024 * 1024];
    for (size_t i = 0; i < 1024 * 1024; i++)
        stbi_data[i] = static_cast<stbi_uc>(std::clamp(displacementData[i] * 255.f, 0.f, 255.f));
    stbi_write_png("../data/1_denoise_displacement_map.png", 1024, 1024, 1, stbi_data, 1024);
    delete[] stbi_data;
    delete[] displacementData;
    return displacementmap;
}
