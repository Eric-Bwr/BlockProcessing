#include "ServerMenuInterface.h"
#include "BlockProcessing/Game/BlockProcessing.h"

static SceneManager *sceneManagerPtr;
static NetworkManager *networkManagerPtr;
static Parameters *parametersPtr;

const float connectionIcons[6][2] = {
        {263, 197},

        {263, 189},
        {263, 181},
        {263, 173},
        {263, 165},
        {263, 157}
};

void ServerMenuInterface::init(SceneManager *sceneManager) {
    sceneManagerPtr = sceneManager;
    parametersPtr = &parameters;
    networkManagerPtr = sceneManager->blockProcessing->networkManager;
    networkManagerPtr->setServerMenuInterface(this);
    serverTexture = new UITexture(TEXTURE_SERVER);
    serverTexture->minNear();
    serverTexture->magNear();
    serverTexture->repeat();
    titleText = new UIText("Server", font, 60, 0, 40, width, 80, UITextMode::CENTERED);
    nameText = new UIText("Your name:", font, 40, width / 2 - fieldWidth / 2, 160, fieldWidth, 40, UITextMode::LEFT);
    nameText->setRGBA(0.7, 0.7, 0.7, 1.0);
    nameField = new UITextField("", font, 40, nameText->getX(), nameText->getY() + nameText->getHeight(), fieldWidth, fieldHeight, 10);
    nameField->setBackgroundTexture(guiTexture, 0, 131, 202, 22);
    nameField->setFontSize(50);
    nameField->setText(parameters.getString("Server#PlayerName", "Elly").data());
    nameField->setMaxCharacter(20);
    nameField->setContentCallback([](std::string content, std::string passwordContent) {
        parametersPtr->getString("Server#PlayerName") = content;
    });
    serverText = new UIText("Server address:", font, 40, width / 2 - fieldWidth / 2, 320, fieldWidth, 40, UITextMode::LEFT);
    serverText->setRGBA(0.7, 0.7, 0.7, 1.0);
    serverField = new UITextField("", font, 40, serverText->getX(), serverText->getY() + serverText->getHeight(), fieldWidth, fieldHeight, 10);
    serverField->setBackgroundTexture(guiTexture, 0, 131, 202, 22);
    serverField->setFontSize(50);
    serverField->setMaxCharacter(20);
    serverField->setText(parameters.getString("Server#Address", "127.0.0.1:25566").data());
    serverField->setContentCallback([](std::string content, std::string passwordContent) {
        std::string strippedContent;
        for (auto c : content) {
            if (std::isdigit(c) || c == '.' || c == ':')
                strippedContent += c;
        }
        if(!strippedContent.empty())
            parametersPtr->getString("Server#Address") = strippedContent;
    });
    backButton = new UIButton(width / 2 - 200 * 4 / 2, height - 140, 200 * 4, 20 * 4);
    backButton->setBackgroundTexture(guiTexture, 0, 20, 200, 20, 0, 40, 200, 20, 0, 40, 200, 20);
    backButton->setText("Back", font, 40);
    backButton->setCallback([](bool hovered, bool pressed) {
        if (hovered && pressed)
            sceneManagerPtr->setCurrent(ID_MAIN_MENU);
    });
    serverInfo = new UIText("", font, 50, width / 2 - 250 * 3.5 / 2 + 30, height - 300, 250 * 3.5, 50, UITextMode::LEFT);
    refreshButton = new UIButton(width / 2 - 200 * 3.25 / 2, height - 510, 200 * 3.25, 20 * 3.25);
    refreshButton->setBackgroundTexture(guiTexture, 0, 20, 200, 20, 0, 40, 200, 20, 0, 40, 200, 20);
    refreshButton->setText("Refresh", font, 40);
    refreshButton->setCallback([](bool hovered, bool pressed) {
        if (hovered && pressed) {
            auto option = parametersPtr->getString("Server#Address");
            if(!option.empty())
                networkManagerPtr->connect(option);
        }
    });
    connectionInfo = new UIImage(width / 2 + 250 * 3.5 / 2 - 11 * 8, height - 400 + 8, 10 * 8, 7 * 8);
    connectionInfo->setTexture(guiTexture, connectionIcons[0][0], connectionIcons[0][1], 10, 7);
    serverBackground = new UIImage(width / 2 - 250 * 3.5 / 2, height - 400, 250 * 3.5, 200);
    serverBackground->setTexture(guiTexture, 0, 155, 250, 55);
    background = new UIImage(0, 0, width, height);
    background->setTexture(serverTexture, 0, 0, width / 8, height / 8);
    background->setColor({0.3, 0.3, 0.3, 1.0});
}

void ServerMenuInterface::load() {
    networkManagerPtr->connect(parameters.getString("Server#Address"));
    UI->add(titleText, 1);
    UI->add(nameText, 1);
    UI->add(nameField, 1);
    UI->add(serverText, 1);
    UI->add(serverField, 1);
    UI->add(backButton, 1);
    UI->add(refreshButton, 1);
    UI->add(serverInfo, 2);
    UI->add(connectionInfo, 2);
    UI->add(serverBackground, 1);
    UI->add(background, 0);
}

void ServerMenuInterface::unload() {
    UI->remove(titleText);
    UI->remove(nameText);
    UI->remove(nameField);
    UI->remove(serverText);
    UI->remove(serverField);
    UI->remove(backButton);
    UI->remove(refreshButton);
    UI->remove(serverInfo);
    UI->remove(connectionInfo);
    UI->remove(serverBackground);
    UI->remove(background);
}

void ServerMenuInterface::update(double frameDeltaTime) {
    networkManagerPtr->update();
    auto status = networkManagerPtr->status;
    auto info = networkManagerPtr->info;
    if(oldStatus != status) {
        oldStatus = status;
        if (status == STATUS_UNKNOWN_HOST) {
            connectionInfo->setTextureCoords(connectionIcons[0][0], connectionIcons[0][1], 10, 7);
            serverInfo->setText("Unknown Host");
            serverInfo->setHex(0xDDDDDD);
        } else if (status == STATUS_CONNECTING) {
            connectionInfo->setTextureCoords(connectionIcons[0][0], connectionIcons[0][1], 10, 7);
            serverInfo->setText("Connecting...");
            serverInfo->setHex(0xDDDDDD);
        } else if (status == STATUS_CONNECTED) {
            connectionInfo->setTextureCoords(connectionIcons[1][0], connectionIcons[1][1], 10, 7);
            serverInfo->setText("Connected");
            serverInfo->setHex(0xFFFFFF);
        }
    }
}

void ServerMenuInterface::setInfo(const char *name, const char *motd) {
    serverInfo->setText(name);
}

ServerMenuInterface::~ServerMenuInterface() {
    UI->remove(titleText);
    UI->remove(nameText);
    UI->remove(nameField);
    UI->remove(serverText);
    UI->remove(serverField);
    UI->remove(backButton);
    UI->remove(refreshButton);
    UI->remove(serverInfo);
    UI->remove(connectionInfo);
    UI->remove(serverBackground);
    UI->remove(background);
    delete serverTexture;
    delete titleText;
    delete nameText;
    delete nameField;
    delete serverText;
    delete serverField;
    delete backButton;
    delete refreshButton;
    delete serverInfo;
    delete connectionInfo;
    delete serverBackground;
    delete background;
}
