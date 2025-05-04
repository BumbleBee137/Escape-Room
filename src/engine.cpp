#include "engine.h"

enum state {north, east, west, south};
state screen;

// Colors
color originalFill, hoverFill, pressFill;


Engine::Engine() : keys() {
    this->initWindow();
    this->initShaders();
    this->initShapes();

    originalFill = {1, 0, 0, 1};
    hoverFill.vec = originalFill.vec + vec4{0.5, 0.5, 0.5, 0};
    pressFill.vec = originalFill.vec - vec4{0.5, 0.5, 0.5, 0};
}

Engine::~Engine() {}

unsigned int Engine::initWindow(bool debug) {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    window = glfwCreateWindow(width, height, "engine", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // OpenGL configuration
    glViewport(0, 0, width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSwapInterval(1);

    return 0;
}

void Engine::initShaders() {
    // load shader manager
    shaderManager = make_unique<ShaderManager>();

    // Load shader into shader manager and retrieve it
    shapeShader = this->shaderManager->loadShader("../res/shaders/shape.vert", "../res/shaders/shape.frag",  nullptr, "shape");

    // Configure text shader and renderer
    textShader = shaderManager->loadShader("../res/shaders/text.vert", "../res/shaders/text.frag", nullptr, "text");
    fontRenderer = make_unique<FontRenderer>(shaderManager->getShader("text"), "../res/fonts/MxPlus_IBM_BIOS.ttf", 24);

    // Set uniforms
    textShader.setVector2f("vertex", vec4(100, 100, .5, .5));
    shapeShader.use();
    shapeShader.setMatrix4("projection", this->PROJECTION);
}

void Engine::initShapes() {
    //testing that all shapes work correctly
    triangleTest = make_unique<Triangle>(shapeShader, vec2{width/2,height/2}, vec2{100, 50}, color{1, 0, 0, 1});
    circleTest = make_unique<Circle>(shapeShader, vec2{width/2,height/2}, vec2{10, 5}, color{1, 0, 0, 1});
    door = make_unique<Item>("This is a door");
    door->pushShape(make_shared<Rect>(shapeShader, vec2(600, 200), vec2(500,1200), color(150/255.0, 75/255.0, 0, 1)));
    door->pushShape(make_shared<Circle>(shapeShader, vec2(750, 350), 3, color(1, 1, 1, 1)));
    inventory = make_unique<Inventory>(shapeShader);
    square = make_shared<Hold>("Square", vec2(width/2, height/2));
    square->pushShape(make_shared<Rect>(shapeShader, vec2{width/2, height/2}, vec2{50, 50}, color(1, 1, 1, 1)));

}

void Engine::processInput() {
    glfwPollEvents();

    // Set keys to true if pressed, false if released
    for (int key = 0; key < 1024; ++key) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            keys[key] = true;
        }
        else if (glfwGetKey(window, key) == GLFW_RELEASE) {
            keys[key] = false;
        }
    }


    //checking for numbers
    if (keys[GLFW_KEY_1]) message = inventory->select(0);
    if (keys[GLFW_KEY_2]) message = inventory->select(1);
    if (keys[GLFW_KEY_3]) message = inventory->select(2);
    if (keys[GLFW_KEY_4]) message = inventory->select(3);
    if (keys[GLFW_KEY_5]) message = inventory->select(4);
    if (keys[GLFW_KEY_6]) message = inventory->select(5);
    if (keys[GLFW_KEY_7]) message = inventory->select(6);
    if (keys[GLFW_KEY_8]) message = inventory->select(7);
    if (keys[GLFW_KEY_9]) message = inventory->select(8);

    // Close window if escape key is pressed
    if (keys[GLFW_KEY_ESCAPE])
        glfwSetWindowShouldClose(window, true);

    // Mouse position saved to check for collisions
    glfwGetCursorPos(window, &MouseX, &MouseY);

    // Moving between windows
    if (screen == east) {
        if (keys[GLFW_KEY_LEFT] && !leftLastFrame)
            screen = south;
        else if (keys[GLFW_KEY_RIGHT] && !rightLastFrame)
            screen = north;
    }else if (screen == south) {
        if (keys[GLFW_KEY_LEFT] && !leftLastFrame)
            screen = west;
        else if (keys[GLFW_KEY_RIGHT] && !rightLastFrame)
            screen = east;
    }else if (screen == west) {
        if (keys[GLFW_KEY_LEFT] && !leftLastFrame)
            screen = north;
        else if (keys[GLFW_KEY_RIGHT] && !rightLastFrame)
            screen = south;
    }else if (screen == north) {
        if (keys[GLFW_KEY_LEFT] && !leftLastFrame)
            screen = east;
        else if (keys[GLFW_KEY_RIGHT] && !rightLastFrame)
            screen = west;
    }


    // Mouse position is inverted because the origin of the window is in the top left corner
    MouseY = height - MouseY; // Invert y-axis of mouse position
    bool mousePressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    if (mousePressed && !mousePressedLastFrame) {
        click = true;
        hold = false;
    }else {
        click = false;
        hold = true;
    }


    // Save mousePressed & left and right arrow for next frame
    mousePressedLastFrame = mousePressed;
    leftLastFrame = keys[GLFW_KEY_LEFT];
    rightLastFrame = keys[GLFW_KEY_RIGHT];

}

void Engine::update() {
    // Calculate delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;


}

void Engine::render() {
    glClearColor(234.0/255.0, 210.0/255.0, 168.0/255.0f, 1.0f); // Set background color
    glClear(GL_COLOR_BUFFER_BIT);

    // Set shader to draw shapes
    shapeShader.use();

    // Render differently depending on screen
    switch (screen) {
        case west: {
            this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height-50, projection, 1, vec3{1, 1, 1});
            shapeShader.use();
            if (triangleTest->isOverlapping({MouseX, MouseY})) triangleTest->setColor(pressFill);
            else triangleTest->setColor(originalFill);
            triangleTest->setUniforms();
            triangleTest->draw();
            break;
        }
        case south: {
            this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height - 50, projection, 1, vec3{1, 1, 1});
            shapeShader.use();
            if (circleTest->isOverlapping({MouseX, MouseY})) circleTest->setColor(pressFill);
            circleTest->setUniforms();
            circleTest->draw();
            break;
        }
        case east: {
            if (door->isOverlapping({MouseX, MouseY}) && click) {
                if (inventory->current() != square) message = door->getText();
                else {
                    message = "You did it!";
                    inventory->remove();
                }
            }
            this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height - 50, projection, 1, vec3{1, 1, 1});
            shapeShader.use();
            door->setUniformsAndDraw();
            break;
        }
        case north: {
            this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height-50, projection, 1, vec3{1, 1, 1});
            shapeShader.use();
            if (!square->getGrabbed())
                square->setUniformsAndDraw();
            if (square->isOverlapping({MouseX, MouseY}) && click) message = inventory->grab(square);
            break;
        }
    }
    shapeShader.use();
    inventory->setUniformsAndDraw();

    glfwSwapBuffers(window);
}

bool Engine::shouldClose() {
    return glfwWindowShouldClose(window);
}

GLenum Engine::glCheckError_(const char *file, int line) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        cout << error << " | " << file << " (" << line << ")" << endl;
    }
    return errorCode;
}