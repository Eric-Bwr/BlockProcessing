#include "ServerMenuInterface.h"
#include "BlockProcessing/Game/BlockProcessing.h"

static SceneManager *sceneManagerPtr;
static NetworkManager *networkManagerPtr;
static Parameters *parametersPtr;

const float connectionIcons[11][2] = {
        {263, 197},

        {263, 189},
        {263, 181},
        {263, 173},
        {263, 165},
        {263, 157},

        {252, 189},
        {252, 181},
        {252, 173},
        {252, 165},
        {252, 157}
};

void ServerMenuInterface::init(SceneManager *sceneManager) {
    sceneManagerPtr = sceneManager;
    parametersPtr = &parameters;
    networkManagerPtr = sceneManager->blockProcessing->networkManager;
    networkManagerPtr->setServerMenuInterface(this);
    serverTexture = new Texture(TEXTURE_SERVER);
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
            parametersPtr->getString("Server#Address", "127.0.0.1:25566") = strippedContent;
    });
    backButton = new UIButton(width / 2 - 200 * 4 / 2, height - 120, 200 * 4, 20 * 4);
    backButton->setBackgroundTexture(guiTexture, 0, 20, 200, 20, 0, 40, 200, 20, 0, 40, 200, 20);
    backButton->setText("Back", font, 40);
    backButton->setCallback([](bool hovered, bool pressed) {
        if (hovered && pressed)
            sceneManagerPtr->setCurrent(ID_MAIN_MENU);
    });
    serverName = new UIText("", font, 50, width / 2 - serverFieldWidth / 2 + 20, height - 400 + 20, serverFieldWidth, 50, UITextMode::LEFT);
    serverMotd = new UIText("", font, 50, width / 2 - serverFieldWidth / 2 + 20, height - 400 + 145, serverFieldWidth, 50, UITextMode::LEFT);
    serverPing = new UIText("", font, 50, width / 2 + serverFieldWidth / 2 - 10 * 8 - 50 - 200, height - 400 + 12, 200, 50, UITextMode::RIGHT);
    refreshButton = new UIButton(width / 2 - 200 * 3.25 / 2, height - 510, 200 * 3.25, 20 * 3.25);
    refreshButton->setBackgroundTexture(guiTexture, 0, 20, 200, 20, 0, 40, 200, 20, 0, 40, 200, 20);
    refreshButton->setText("Refresh", font, 40);
    refreshButton->setCallback([](bool hovered, bool pressed) {
        if (hovered && pressed)
            sceneManagerPtr->onKey(KEY_ENTER, 0, PRESS, 0);
    });
    joinButton = new UIButton(width / 2 - 200 * 3.25 / 2, height - 220, 200 * 3.25, 20 * 3.25);
    joinButton->setBackgroundTexture(guiTexture, 0, 20, 200, 20, 0, 40, 200, 20, 0, 40, 200, 20);
    joinButton->setText("Join", font, 40);
    joinButton->setCallback([](bool hovered, bool pressed) {
        if (hovered && pressed) {

        }
    });
    connectionInfo = new UIImage(width / 2 + serverFieldWidth / 2 - 11 * 8, height - 400 + 8, 10 * 8, 7 * 8);
    connectionInfo->setTexture(guiTexture, connectionIcons[0][0], connectionIcons[0][1], 10, 7);
    serverBackground = new UIImage(width / 2 - serverFieldWidth / 2, height - 450, serverFieldWidth, serverFieldHeight);
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
    UI->add(joinButton, 1);
    UI->add(refreshButton, 1);
    UI->add(serverName, 2);
    UI->add(serverMotd, 2);
    UI->add(serverPing, 2);
    UI->add(connectionInfo, 2);
    UI->add(serverBackground, 1);
    UI->add(background, 0);
    animation = 0.0f;
}

void ServerMenuInterface::unload() {
    networkManagerPtr->disconnect();
    UI->remove(titleText);
    UI->remove(nameText);
    UI->remove(nameField);
    UI->remove(serverText);
    UI->remove(serverField);
    UI->remove(serverPing);
    UI->remove(backButton);
    UI->remove(joinButton);
    UI->remove(refreshButton);
    UI->remove(serverName);
    UI->remove(serverMotd);
    UI->remove(connectionInfo);
    UI->remove(serverBackground);
    UI->remove(background);
}

void ServerMenuInterface::update(double frameDeltaTime) {
    networkManagerPtr->update();
    auto status = networkManagerPtr->status;
    int i = 0;
    if (status == STATUS_CONNECTING) {
        if (animation > 8) {
            animation = 0.0f;
            i = 1;
        } else if (animation > 7)
            i = 2;
        else if (animation > 6)
            i = 3;
        else if (animation > 5)
            i = 4;
        else if (animation > 4)
            i = 5;
        else if (animation > 3)
            i = 4;
        else if (animation > 2)
            i = 3;
        else if (animation > 1)
            i = 2;
        else if (animation >= 0)
            i = 1;
        connectionInfo->setTextureCoords(connectionIcons[i][0], connectionIcons[i][1], 10, 7);
        animation += animationSpeed * frameDeltaTime;
        serverName->setText("Connecting...");
        serverName->setHex(0xDDDDDD);
        serverPing->setText("");
    } else if (status == STATUS_CONNECTED) {
        auto delay = networkManagerPtr->delay;
        if (delay <= 30)
            i = 10;
        else if (delay <= 40)
            i = 9;
        else if (delay <= 50)
            i = 8;
        else if (delay <= 60)
            i = 7;
        else if (delay <= 70)
            i = 6;
        connectionInfo->setTextureCoords(connectionIcons[i][0], connectionIcons[i][1], 10, 7);
        serverName->setHex(0xFFFFFF);
        serverPing->setText((std::to_string(delay) + "ms").c_str());
    } else if (status == STATUS_UNKNOWN_HOST) {
        connectionInfo->setTextureCoords(connectionIcons[0][0], connectionIcons[0][1], 10, 7);
        serverName->setText("Unknown Host");
        serverName->setHex(0xDDDDDD);
        serverPing->setText("");
    }
}

void ServerMenuInterface::setInfo(const char *name, const char *motd) {
    serverName->setText(name);
    serverMotd->setText(motd);
}

void ServerMenuInterface::keyInput(int key, int action, int mods) {
    if(action == PRESS){
        if(key == KEY_ENTER || key == KEY_KP_ENTER) {
            animation = 0.0f;
            auto option = parameters.getString("Server#Address", "127.0.0.1:25566");
            if (!option.empty())
                networkManagerPtr->connect(option);
        } else if(key == KEY_ESCAPE)
            sceneManagerPtr->setCurrent(ID_MAIN_MENU);
    }
}

ServerMenuInterface::~ServerMenuInterface() {
    UI->remove(titleText);
    UI->remove(nameText);
    UI->remove(nameField);
    UI->remove(serverText);
    UI->remove(serverField);
    UI->remove(backButton);
    UI->remove(joinButton);
    UI->remove(serverName);
    UI->remove(serverMotd);
    UI->remove(serverPing);
    UI->remove(refreshButton);
    UI->remove(connectionInfo);
    UI->remove(serverBackground);
    UI->remove(background);
    delete serverTexture;
    delete titleText;
    delete nameText;
    delete nameField;
    delete serverText;
    delete serverField;
    delete serverPing;
    delete backButton;
    delete joinButton;
    delete refreshButton;
    delete serverName;
    delete serverMotd;
    delete connectionInfo;
    delete serverBackground;
    delete background;
}
