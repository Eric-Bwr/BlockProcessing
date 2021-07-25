#include "Chat.h"

static std::vector<UIText*> texts;
static bool shouldDisplay = false;
static UIImage *background;
static int revertStep = 0;
const int Chat::height;

void Chat::init() {
    background = new UIImage(0, Interface::UI.getHeight() - ChatInterface::height, ChatInterface::width, 0);
    auto color = UIColor(000000, 0.3);
    background->setColor(color);
}

void Chat::append(const std::string &input, const UIColor &textColor) {
    if(texts.size() >= 20) {
        if(shouldDisplay)
            Interface::UI.remove(texts.front());
        delete texts.front();
        texts.erase(texts.begin());
    }
    auto text = new UIText((char *) input.data(), Interface::font, 30, 0, Interface::UI.getHeight() - ChatInterface::height - height, ChatInterface::width, height, UITextMode::LEFT);
    text->r = textColor.r;
    text->g = textColor.g;
    text->b = textColor.b;
    text->a = textColor.a;
    for (auto oldText : texts)
        oldText->setPosition(0, oldText->getY() - height);
    texts.emplace_back(text);
    if (shouldDisplay)
        Interface::UI.add(text, 1);
    background->setPosition(0, Interface::UI.getHeight() - texts.size() * height - ChatInterface::height);
    background->setSize(ChatInterface::width, texts.size() * height);
    revertStep = texts.size() - 1;
}
//TODO: FIXME
void Chat::revertUp(){
    if(!texts.empty()) {
        if (shouldDisplay && revertStep > 0) {
            revertStep--;
            ChatInterface::textField->setText(texts.at(revertStep)->getText());
        } else if (revertStep == 0)
            ChatInterface::textField->setText("");
    }
}

void Chat::revertDown(){
    if(!texts.empty()) {
        if (shouldDisplay && revertStep <= texts.size() - 1) {
            ChatInterface::textField->setText(texts.at(revertStep)->getText());
            revertStep++;
        } else if (revertStep > texts.size() - 1)
            ChatInterface::textField->setText("");
    }
}

void Chat::display(bool display) {
    shouldDisplay = display;
    if (display) {
        for (auto text : texts)
            Interface::UI.add(text, 1);
        Interface::UI.add(background);
        revertStep = texts.size() - 1;
    } else {
        revertStep = 0;
        for (auto text : texts)
            Interface::UI.remove(text);
        Interface::UI.remove(background);
    }
}

Chat::~Chat() {
    for (auto text : texts) {
        Interface::UI.remove(text);
        delete text;
    }
    Interface::UI.remove(background);
    delete background;
}
