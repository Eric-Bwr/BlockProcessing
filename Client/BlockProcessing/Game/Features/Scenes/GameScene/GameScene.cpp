#include "GameScene.h"
#include "BlockProcessing/Game/BlockProcessing.h"

void GameScene::init() {
    this->window = blockProcessing->window;
    this->width = blockProcessing->width;
    this->height = blockProcessing->height;
    projectionView.identity();

    player = new Player();
    chatInterface = new ChatInterface();
    commandManager = new CommandManager();
    debugInterface = new DebugInterface();
    crosshairInterface = new CrosshairInterface();
    hotbarInterface = new HotbarInterface();
    gameMenuInterface = new GameMenuInterface();
    optionsMenuInterface = new OptionsMenuInterface();
    
    terrainManager.init(rand(), FastNoise::PerlinFractal, 0.009, 6);
    clouds.init();

    blockProcessing->interfaceManager->add(chatInterface);
    blockProcessing->interfaceManager->add(debugInterface);
    blockProcessing->interfaceManager->add(crosshairInterface);
    blockProcessing->interfaceManager->add(hotbarInterface);
    blockProcessing->interfaceManager->add(gameMenuInterface);
    blockProcessing->interfaceManager->add(optionsMenuInterface);
    chatInterface->init(commandManager);
    commandManager->init(chatInterface);
    debugInterface->init();
    crosshairInterface->init();
    hotbarInterface->init();
    gameMenuInterface->init(sceneManager, optionsMenuInterface, this);
    optionsMenuInterface->init(gameMenuInterface, this);

    chunkBorderVisualizer.init();
    linePointVisualizer.init();

    player->init(terrainManager.getWorldManager(), 0, 0, 0, 90, 0);

    commandManager->add(new CommandTP(player));
    commandManager->add(new CommandSpeed(player));
    commandManager->add(new CommandHelp());

    updateProjection(parameters.getInt("Rendering#FOV", 90));
}

void GameScene::load() {
    player->position.y = ((terrainManager.getWorldManager()->fastNoise->GetNoise(0, 0) + 1.0f) / 2.0f) * 200 + 5;
    player->updatePlayerPosition();
    if(!optionsMenuInterface->shouldVsync)
        glfwSwapInterval(0);
    if(!crosshair)
        crosshairInterface->load();
    if(!hotbar)
        hotbarInterface->load();
    crosshair = true;
    hotbar = true;
    player->updatePlayerPosition();
    glfwSetCursorPos(window, width / 2, height / 2);
    glfwSetInputMode(window, CURSOR, CURSOR_DISABLED);
}

void GameScene::unload() {
    terrainManager.getWorldManager()->reset();
    if (crosshair)
        crosshairInterface->unload();
    if (hotbar)
        hotbarInterface->unload();
    chatInterface->unload(true);
    if (debug)
        debugInterface->unload();
    if (gameMenu)
        gameMenuInterface->unload();
    crosshair = false;
    hotbar = false;
    chat = false;
    debug = false;
    gameMenu = false;
    inInterface = false;
}

void GameScene::update(double deltaFrameTime) {
    chatInterface->update(deltaFrameTime);
    player->update(deltaFrameTime);
    terrainManager.setLightPosition(player->position.x, player->position.y + 1000, player->position.z);
    terrainManager.generate(player->chunk, player->octree);
    clouds.update(deltaFrameTime);
    if (wireFrame)
        chunkBorderVisualizer.generate(player->chunk);
    if (debug)
        debugInterface->update(player);
}

void GameScene::render(double deltaFrameTime) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.8, 0.9, 0.9, 1.0);
    view = player->getViewMatrix();
    viewf = toFloat(view);
    projectionView = projectionView.multiply(toDouble(blockProcessing->projection), view);
    terrainManager.render(projectionView, view);
    if(optionsMenuInterface->shouldCloud)
        clouds.render(viewf, player->position);
    player->render(view);
    if (collision)
        terrainManager.visualizeOctree(view, leftControl);
    if (wireFrame)
        chunkBorderVisualizer.render(viewf);
    linePointVisualizer.setView(viewf);
}

void GameScene::updateProjection(float fov) {
    blockProcessing->projection.perspective(fov, (float) blockProcessing->width, (float) blockProcessing->height, 0.1, 20000.0f);
    terrainManager.setProjection(blockProcessing->projection);
    clouds.setProjection(blockProcessing->projection);
    linePointVisualizer.setProjection(blockProcessing->projection);
    chunkBorderVisualizer.setProjection(blockProcessing->projection);
    player->setProjection(blockProcessing->projection);
    blockProcessing->skyBoxManager->setProjection(blockProcessing->projection);
}

void GameScene::onKey(int key, int scancode, int action, int mods) {
    if(hotbar && !chat && !gameMenu && !optionsMenu)
        hotbarInterface->onKey(key, scancode, action, mods);
    if (key == KEY_T && action == RELEASE) {
        if (!inInterface) {
            glfwSetInputMode(window, CURSOR, CURSOR_NORMAL);
            glfwSetCursorPos(window, width / 2, height / 2);
            player->hasLastPos = false;
            chat = true;
            chatInterface->load();
            crosshair = false;
            crosshairInterface->unload();
            player->shouldMoveUp = player->shouldMoveDown = player->shouldMoveForward = player->shouldMoveBackward = player->shouldMoveLeft = player->shouldMoveRight = false;
        }
    }
    if (key == KEY_ESCAPE && action == PRESS) {
        if (chat) {
            glfwSetInputMode(window, CURSOR, CURSOR_DISABLED);
            glfwSetCursorPos(window, width / 2, height / 2);
            player->hasLastPos = false;
            chat = false;
            chatInterface->unload(false);
            crosshairInterface->load();
            crosshair = true;
        } else if (optionsMenu) {
            optionsMenuInterface->updateInput();
            optionsMenuInterface->unload();
            gameMenuInterface->load();
            gameMenuInterface->updateInput();
            gameMenu = true;
            optionsMenu = false;
        } else if (gameMenu) {
            glfwSetInputMode(window, CURSOR, CURSOR_DISABLED);
            glfwSetCursorPos(window, width / 2, height / 2);
            player->hasLastPos = false;
            gameMenu = false;
            gameMenuInterface->unload();
            crosshairInterface->load();
            crosshair = true;
        } else {
            glfwSetInputMode(window, CURSOR, CURSOR_NORMAL);
            glfwSetCursorPos(window, width / 2, height / 2);
            player->hasLastPos = false;
            gameMenu = true;
            gameMenuInterface->load();
            gameMenuInterface->updateInput(width / 2, height / 2);
            crosshair = false;
            crosshairInterface->unload();
            player->shouldMoveUp = player->shouldMoveDown = player->shouldMoveForward = player->shouldMoveBackward = player->shouldMoveLeft = player->shouldMoveRight = false;
        }
    }
    if (key == KEY_ENTER && action == PRESS)
        if (chat)
            chatInterface->enter();
    if ((action == PRESS || action == GLFW_REPEAT) && chat) {
        if (key == KEY_UP)
            chatInterface->revertUp();
        else if (key == KEY_DOWN)
            chatInterface->revertDown();
    }
    if (chat || gameMenu || optionsMenu) {
        inInterface = true;
        return;
    } else
        inInterface = false;
    if (key == KEY_C && action == PRESS)
        collision = !collision;
    if (key == KEY_F3 && action == PRESS) {
        debug = !debug;
        if (debug)
            debugInterface->load();
        else
            debugInterface->unload();
    }
    if (key == KEY_RIGHT_SHIFT && action == PRESS) {
        mode = !mode;
        if (mode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (key == KEY_M && action == PRESS) {
        modeGui = !modeGui;
        modeCursor = modeGui;
        if (modeGui)
            glfwSetInputMode(window, CURSOR, CURSOR_NORMAL);
        else
            glfwSetInputMode(window, CURSOR, CURSOR_DISABLED);
    }
    if (key == KEY_X && action == PRESS) {
        modeCursor = !modeCursor;
        if (modeCursor)
            glfwSetInputMode(window, CURSOR, CURSOR_NORMAL);
        else
            glfwSetInputMode(window, CURSOR, CURSOR_DISABLED);
    }
    if (key == KEY_LEFT_CONTROL && action == PRESS)
        leftControl = !leftControl;
    if (key == KEY_N && action == PRESS)
        wireFrame = !wireFrame;
    if (key == KEY_Y && action == PRESS)
        zoom = !zoom;
    if (action == PRESS || action == REPEAT) {
        if (key == KEY_SPACE)
            player->shouldMoveUp = true;
        if (key == KEY_LEFT_SHIFT)
            player->shouldMoveDown = true;
        if (key == KEY_W)
            player->shouldMoveForward = true;
        if (key == KEY_S)
            player->shouldMoveBackward = true;
        if (key == KEY_A)
            player->shouldMoveLeft = true;
        if (key == KEY_D)
            player->shouldMoveRight = true;
    } else if (action == RELEASE) {
        if (key == KEY_SPACE)
            player->shouldMoveUp = false;
        if (key == KEY_LEFT_SHIFT)
            player->shouldMoveDown = false;
        if (key == KEY_W)
            player->shouldMoveForward = false;
        if (key == KEY_S)
            player->shouldMoveBackward = false;
        if (key == KEY_A)
            player->shouldMoveLeft = false;
        if (key == KEY_D)
            player->shouldMoveRight = false;
    }
}

void GameScene::onMousePosition(double x, double y) {
    if (!inInterface)
        player->moveMouse(x, y);
    else
        chatInterface->onMousePosition(x, y);
}

void GameScene::onMouseButton(int button, int action, int mods) {
    if (!inInterface) {
        if (action == PRESS) {
            if (button == MOUSE_BUTTON_2) {
                player->dig();
            }
            if (button == MOUSE_BUTTON_1) {
                player->place();
            }
        }
    } else
        chatInterface->onMouseButton(button, action);
}

void GameScene::onMouseScroll(double x, double y) {
    if(hotbar)
        hotbarInterface->onMouseScroll(x, y);
}

void GameScene::onResize(bool show, int width, int height) {
    if (show) {
        if (crosshair)
            crosshairInterface->unload();
        if(!hotbar)
            hotbarInterface->load();
        if(!chat)
            chatInterface->load();
        if (debug)
            debugInterface->unload();
        if (!gameMenu)
            gameMenuInterface->load();
        if (!optionsMenu)
            optionsMenuInterface->load();
    } else {
        if (crosshair)
            crosshairInterface->load();
        if(!hotbar)
            hotbarInterface->unload();
        if(!chat)
            chatInterface->unload(true);
        if (debug)
            debugInterface->load();
        if (!gameMenu)
            gameMenuInterface->unload();
        if (!optionsMenu)
            optionsMenuInterface->unload();
    }
}

int GameScene::getID() {
    return ID_GAME;
}

GameScene::~GameScene() {
    delete player;
    delete commandManager;
}
