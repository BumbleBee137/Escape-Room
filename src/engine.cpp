#include "engine.h"

enum state {north, east, west, south, win};
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
    screen = east;


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
    //background objects
    door = make_unique<Item>("It's locked. I should find the key");
    door->pushShape(make_shared<Rect>(shapeShader, vec2(500, 200), vec2(500,1200), color(150/255.0, 75/255.0, 0, 1)));
    door->pushShape(make_shared<Circle>(shapeShader, vec2(650, 350), 3, color(1, 1, 1, 1)));

    wind = make_unique<Item>("What a pretty view");
    wind->pushShape(make_shared<Rect>(shapeShader, vec2(width/2, 600), vec2(500,500), color(229/255.0, 243/255.0, 253/255.0, 1)));
    wind->pushShape(make_shared<Rect>(shapeShader, vec2(width/2,600), vec2(10,500), color(1,1, 1, 1)));
    wind->pushShape(make_shared<Rect>(shapeShader, vec2(width/2,600), vec2(500,10), color(1,1,1, 1)));
    wind->pushShape(make_shared<Rect>(shapeShader, vec2(width/2,350), vec2(500,30), color(1,1,1, 1)));
    wind->pushShape(make_shared<Rect>(shapeShader, vec2(width/2,850), vec2(500,30), color(1,1,1, 1)));

    frame1 = make_unique<Item>("What a lovely painting");
    frame1->pushShape(make_shared<Rect>(shapeShader, vec2(1300, 800), vec2(200,200), color(150/255.0, 75/255.0, 0, 1)));
    frame1->pushShape(make_shared<Rect>(shapeShader, vec2(1300, 800), vec2(170,170), color(1, .75, .75, 1)));

    frame2 = make_unique<Item>("This reminds me of my childhood");
    frame2->pushShape(make_shared<Rect>(shapeShader, vec2(1300, 500), vec2(200,200), color(150/255.0, 75/255.0, 0, 1)));
    frame2->pushShape(make_shared<Rect>(shapeShader, vec2(1300, 500), vec2(170,170), color(.5, .75, .75, 1)));

    couch = make_unique<Item>("This looks comfy");
    couch->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 - 250, 150), vec2(800,200), color(255/255.0, 160/255.0, 122/255.0, 1))); //bottom cushion
    couch->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 - 250, 300), vec2(800,300), color(255/255.0, 138/255.0, 100/255.0, 1))); //back cushion
    couch->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 + 150, 150), vec2(100,300), color(255/255.0, 160/255.0, 122/255.0, 1)));//right arm
    couch->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 - 650, 150), vec2(100,300), color(255/255.0, 160/255.0, 122/255.0, 1)));//left arm
    couch->pushShape(make_shared<Circle>(shapeShader, vec2(width/2+25,225), 6, color(1,.9,.9, 1)));

    bookshelf = make_unique<Item>("Looks like there's a book missing");
    //shelving
    bookshelf->pushShape(make_shared<Rect>(shapeShader, vec2(width/2, height/2 - 100), vec2(600,1000), color(150/255.0, 75/255.0, 0, 1))); //back
    bookshelf->pushShape(make_shared<Rect>(shapeShader, vec2(width/2, height/2 + 393), vec2(600,20), color(101/255.0, 67/255.0, 33/255.0, 1)));//top
    bookshelf->pushShape(make_shared<Rect>(shapeShader, vec2(width/2, height/2 - 495), vec2(600,20), color(101/255.0, 67/255.0, 33/255.0, 1)));//bottom
    bookshelf->pushShape(make_shared<Rect>(shapeShader, vec2(width/2, height/2 + 200), vec2(600,20), color(101/255.0, 67/255.0, 33/255.0, 1)));//shelf
    bookshelf->pushShape(make_shared<Rect>(shapeShader, vec2(width/2, height/2), vec2(600,20), color(101/255.0, 67/255.0, 33/255.0, 1)));//shelf
    bookshelf->pushShape(make_shared<Rect>(shapeShader, vec2(width/2, height/2 - 200), vec2(600,20), color(101/255.0, 67/255.0, 33/255.0, 1)));//shelf
    bookshelf->pushShape(make_shared<Rect>(shapeShader, vec2(width/2, height/2 + 200), vec2(600,20), color(101/255.0, 67/255.0, 33/255.0, 1)));//shelf
    bookshelf->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 + 300, height/2 - 100), vec2(20,1005), color(101/255.0, 67/255.0, 33/255.0, 1)));//side
    bookshelf->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 - 300, height/2 - 100), vec2(20,1005), color(101/255.0, 67/255.0, 33/255.0, 1)));//side
    //books on shelf 1
    for (int i = 0; i < 11; i++) {
        bookshelf->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 + 278 - i*50, height/2 +273), vec2(25,125), color(.25,.25,.5, 1)));
        bookshelf->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 + 253 - i*50, height/2 +273), vec2(25,125), color(.25,.5,.25, 1)));
    }
    bookshelf->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 + 282 - 11*50, height/2 +273), vec2(29,125), color(.25,.25,.5, 1)));
    //shelf 2
    bookshelf->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 - 200, height/2 + 25), vec2(175,30), color(.25,.25,.5, 1)));
    bookshelf->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 - 200, height/2 + 55), vec2(150,30), color(.25,.5,.25, 1)));
    for (int i = 0; i < 4; i++) {
        bookshelf->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 + 278 - i*50, height/2 +72), vec2(25,125), color(.25,.25,.5, 1)));
        bookshelf->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 + 253 - i*50, height/2 +72), vec2(25,125), color(.25,.5,.25, 1)));
    }
    bookshelf->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 + 78, height/2 +72), vec2(25,125), color(125/255.0, 70/255.0, 15/255.0, 1)));
    bookshelf->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 + 53, height/2 +72), vec2(25,125), color(.25,.5,.25, 1)));
    for (int i = 5; i < 8; i++) {
        bookshelf->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 + 278 - i*50, height/2 +72), vec2(25,125), color(.25,.25,.5, 1)));
        bookshelf->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 + 253 - i*50, height/2 +72), vec2(25,125), color(.25,.5,.25, 1)));
    }
    //shelf 3
    for (int i = 0; i < 8; i++) {
        bookshelf->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 + 278 - i*50, height/2 -128), vec2(25,125), color(.25,.25,.5, 1)));
        bookshelf->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 + 253 - i*50, height/2 -128), vec2(25,125), color(.25,.5,.25, 1)));
    }


    table = make_unique<Item>("This sure is a table");
    table->pushShape(make_shared<Rect>(shapeShader, vec2(1300, 350), vec2(300,50), color(150/255.0, 75/255.0, 0, 1)));
    table->pushShape(make_shared<Rect>(shapeShader, vec2(1175, 150), vec2(50,400), color(150/255.0, 75/255.0, 0, 1)));
    table->pushShape(make_shared<Rect>(shapeShader, vec2(1425, 150), vec2(50,400), color(150/255.0, 75/255.0, 0, 1)));

    box = make_unique<Move>("This safe needs a password");
    box->pushShape(make_shared<Rect>(shapeShader, vec2(600, 370), vec2(125,125), color (.25,.25,.25,1)));
    //inventory objects
    inventory = make_unique<Inventory>(shapeShader);

    treat = make_unique<Hold>("A cat treat", vec2(width/2 + 100, height/2 - 130));
    treat->pushShape(make_shared<Rect>(shapeShader, vec2{width/2 + 100, height/2 - 130}, vec2{10, 10}, color(.5, .25, 0, 1)));

    drawerKey = make_unique<Hold>("A gold key", vec2(1080,575));
    drawerKey->pushShape(make_shared<Rect>(shapeShader, vec2(1080, 575), vec2(50,20), color(1,1, 0, 1)));

    lighter = make_unique<Hold>("A lighter", vec2(width/2-520,160));
    lighter->pushShape(make_shared<Rect>(shapeShader, vec2(width/2-520,160), vec2(50,20), color(0,0, 0, 1)));

    book = make_unique<Hold>("A book", vec2(0,0));
    book->pushShape(make_shared<Rect>(shapeShader, vec2(0,0), vec2(50,20), color(0,0, 1, 1)));

    blank = make_unique<Hold>("This paper is blank and smells like lemons", vec2(0,0));
    blank->pushShape(make_shared<Rect>(shapeShader, vec2(0,0), vec2(50,20), color(1,1, 1, 1)));

    paper = make_unique<Hold>("'fish'", vec2(0,0));
    paper->pushShape(make_shared<Rect>(shapeShader, vec2(0,0), vec2(50,20), color(1,1, 1, 1)));

    fish = make_unique<Hold>("Ew it's a fish", vec2(600, 370));
    fish->pushShape(make_shared<Rect>(shapeShader, vec2(600, 370), vec2(50,20), color(1,0.5, 0.5, 1)));

    catKey = make_shared<Hold>("A slimy key shaped fish bone", vec2(width/2, height/2));
    catKey->pushShape(make_shared<Rect>(shapeShader, vec2{width/2, height/2}, vec2{50, 50}, color(1, 1, 1, 1)));


    //moveable objects
    curtains = make_unique<Move>("I've already opened these");
    curtains->pushShape(make_shared<Rect>(shapeShader, vec2(width/2, 880),vec2(500,10), color(0,0,0,1)));
    curtains->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 -300, 525),vec2(200,720), color(.5,0,0,1)));
    curtains->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 +300, 525),vec2(200,720), color(.5,0,0,1)));
    curtains->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 + 125, 525),vec2(200,720), color(.5,0,0,1)));
    curtains->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 - 125, 525),vec2(200,720), color(.5,0,0,1)));

    cushion = make_unique<Move>("What a soft cushion");
    cushion->pushShape(make_shared<Rect>(shapeShader, vec2(width/2-500,225), vec2(75,75), color(1,.9,.9, 1)));
    cushion->pushShape(make_shared<Circle>(shapeShader, vec2(width/2-525,225), 6, color(1,.9,.9, 1)));

    frame = make_unique<Move>("Much better");
    frame->pushShape(make_shared<Rect>(shapeShader, vec2(1000, 650), vec2(200,200), color(150/255.0, 75/255.0, 0, 1)));
    frame->pushShape(make_shared<Rect>(shapeShader, vec2(1000, 650), vec2(170,170), color(.5, .75, .5, 1)));
    frame->rotate(30);

    drawer = make_unique<Move>("It's empty now");
    drawer->pushShape(make_shared<Rect>(shapeShader, vec2(1300, 275), vec2(200,100), color(101/255.0, 67/255.0, 33/255.0, 1)));
    drawer->pushShape(make_shared<Circle>(shapeShader, vec2(1300, 275), 3, color(1, 1, 0, 1)));

    candle = make_unique<Move>("FIRE!!!");
    candle->pushShape(make_shared<Rect>(shapeShader, vec2(1350, 425), vec2(50, 100), color(1,1,1,1)));

    cat = make_unique<Move>("I don't think this cat likes me very much");
    cat->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 - 100, 480), vec2(200, 60), color(0,0,0,1)));//body
    cat->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 + 60, 460), vec2(150, 20), color(0,0,0,1)));//tail
    cat->pushShape(make_shared<Triangle>(shapeShader, vec2(width/2 -180,520),vec2(25,50),color(0,0,0,1)));
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
            //background
            wind->setUniformsAndDraw();
            if (wind->isOverlapping({MouseX, MouseY}) && click) message = "What a nice view";

            //items
            if (!treat->getGrabbed())
                treat->setUniformsAndDraw();
            if (treat->isOverlapping({MouseX, MouseY}) && click && !treat->getGrabbed()) message = inventory->grab(treat);

            curtains->setUniformsAndDraw();
            if (curtains->isOverlapping({MouseX, MouseY}) && click) {
                if (curtains->clicked()) message = "I prefer these open";
                else {
                    message = "Let's get some light in here";
                    curtains->pop();
                    curtains->pop();
                    curtains->click();
                }
            }
            //text
            this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height-50, projection, 1, vec3{1, 1, 1});
            break;
        }
        case south: {
            //background
            bookshelf->setUniformsAndDraw();
            if (bookshelf->isOverlapping({MouseX, MouseY}) && click){
                if (inventory->current() == book) {
                    inventory->remove();
                    inventory->grab(blank);
                    bookshelf->pushShape(make_shared<Rect>(shapeShader, vec2(width/2 + 78, height/2 +72), vec2(25,125), color(.25,.25,.5, 1)));
                    bookshelf->setText("A paper fell out");
                }
                message = bookshelf->getText();
            }

            //drawing safe
            box->setUniformsAndDraw();
            if (box->isOverlapping(vec2(MouseX, MouseY)) && click && inventory->current() == paper) {
                box->pushShape(make_shared<Rect>(shapeShader, vec2(600, 370), vec2(100,100), color (0,0,0,1)));
                box->pushShape(make_shared<Rect>(shapeShader, vec2(550, 370), vec2(20,125), color (.25,.25,.25,1)));
                inventory->remove();
                box->click();
            } else if (box->isOverlapping(vec2(MouseX, MouseY)) && click) message = box->getText();
            if (box->clicked()) fish->setUniformsAndDraw();
            if (fish->isOverlapping(vec2(MouseX, MouseY)) && click && box->clicked()) message = inventory->grab(fish);

            //items

            //text
            this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height - 50, projection, 1, vec3{1, 1, 1});
            break;
        }
        case east: {
            //background
            door->setUniformsAndDraw();
            if (door->isOverlapping({MouseX, MouseY}) && click) {
                if (inventory->current() == drawerKey) message = "This is the wrong key";
                else if (inventory->current() != catKey) message = door->getText();
                else {
                    screen = win;
                    inventory->remove();
                }
            }
            //drawing some background stuff
            frame1->setUniformsAndDraw();
            if (frame1->isOverlapping({MouseX, MouseY}) && click) message = frame1->getText();
            frame2->setUniformsAndDraw();
            if (frame2->isOverlapping({MouseX, MouseY}) && click) message = frame2->getText();
            //items
            if (!drawerKey->getGrabbed()) drawerKey->setUniformsAndDraw();
            frame->setUniformsAndDraw();
            if (drawerKey->isOverlapping({MouseX, MouseY}) && click && !frame->isOverlapping({MouseX, MouseY})
                && !drawerKey->getGrabbed()) {
                message = inventory->grab(drawerKey);

            }//fixing the crooked frame
            if (frame->isOverlapping({MouseX, MouseY}) && click) {
                if (frame->clicked()) message = frame->getText();
                else {
                    message = "Let me fix that";
                    frame->rotate(-30);
                    frame->click();
                }
            }


            //text
            this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height - 50, projection, 1, vec3{1, 1, 1});
            break;
        }
        case north: {
            //background
            couch->setUniformsAndDraw();
            if (couch->isOverlapping({MouseX, MouseY}) && click) message = couch->getText();
            table->setUniformsAndDraw();
            if (table->isOverlapping({MouseX, MouseY}) && click) message = table->getText();
            //items
            if(cushion->clicked() && !lighter->getGrabbed()) lighter->setUniformsAndDraw();
            if (lighter->isOverlapping({MouseX, MouseY}) && click && !cushion->isOverlapping({MouseX, MouseY})
                && !lighter->getGrabbed()) {
                message = inventory->grab(lighter);
            }
            //moving cushion over
            cushion->setUniformsAndDraw();
            if (cushion->isOverlapping({MouseX, MouseY}) && click) {
                if (!cushion->clicked()){
                    message = "I'm just gonna scooch that over";
                    cushion->click();
                    cushion->move(vec2(100,0));
                } else message = cushion->getText();
            }
            //drawing cat & checking responses based on inventory item selected
            cat->setUniformsAndDraw();
            if (cat->isOverlapping({MouseX, MouseY}) && click){
                if (inventory->current() == treat) {
                    message = "Awww it liked that. I should find some more treats";
                    inventory->remove();
                }
                else if (inventory->current() == fish) {
                    message = "Woah slow down kitty, no one's taking your fish";
                    inventory->remove();
                    cat->click();
                }
                else if (cat->clicked() && !catKey->getGrabbed()) {
                    message = "EWWWW it barfed in my hand";
                    inventory->grab(catKey);
                }
                else if (cat->clicked()) message = "What a weird cat";
                else message = cat->getText();
            }

            drawer->setUniformsAndDraw();
            if (drawer->isOverlapping({MouseX, MouseY}) && click) {
                if (drawer->clicked() && book->getGrabbed()) message = drawer->getText();
                else if (drawer->clicked()) message = inventory->grab(book);
                else if (inventory->current() == drawerKey){
                    message = "Yay this key works";
                    drawer->click();
                    drawer->resize(1.25);
                    inventory->remove();
                }
                else message = "Hmm it's locked. I should find a key";
            }
            candle->setUniformsAndDraw();
            if (candle->isOverlapping({MouseX, MouseY}) && click) {
                if (candle->clicked()) {
                    if (inventory->current() != blank) message = candle->getText();
                    else {
                        message = "Ooh a secret message";
                        inventory->remove();
                        inventory->grab(paper);
                    }
                }
                else if (inventory->current() == lighter) {
                    candle->click();
                    candle->pushShape(make_shared<Triangle>(shapeShader, vec2(1350,525),vec2(25,50),color(1,1,0,1)));
                    message = "*Click*";
                    inventory->remove();
                }
                else {
                    message = "It's a candle";
                }
            }

            //text
            this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height-50, projection, 1, vec3{1, 1, 1});
            break;
        }
        case win: {
            this->fontRenderer->renderText("You escaped!", width/2 - 264, height/2, projection, 2, vec3{1, 1, 1});
        }
    }
    shapeShader.use();
    if (screen != win) inventory->setUniformsAndDraw();
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