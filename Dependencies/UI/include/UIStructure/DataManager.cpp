#include "DataManager.h"
#include "UIComponents/Text/Font.h"
#include "UIComponents/Text/Structure/FontType.h"
#include "UIComponents/Text/UIText.h"

FT_Library DataManager::library;
Font *DataManager::defaultFont;
VertexBufferObjectLayout DataManager::textLayout;
VertexBufferObjectLayout DataManager::quadLayout;
static std::map<const char*, std::vector<FontType*>*> fonts;

static bool shouldInit = true;

int DataManager::init() {
    if (shouldInit) {
        shouldInit = false;
        int errorCode = FT_Init_FreeType(&library);
        defaultFont = new Font(DEFAULT_FONT_PATH);
        textLayout.pushFloat(2);
        textLayout.pushFloat(2);
        textLayout.pushFloat(1);
        quadLayout.pushFloat(2);
        quadLayout.pushFloat(2);
        quadLayout.pushFloat(3);
        quadLayout.pushFloat(4);
        return errorCode;
    } else
        return -1;
}

FontType* DataManager::getFontType(UIText *uiText) {
    return getFontType(uiText->font, uiText->fontSize);
}

FontType *DataManager::getFontType(Font *font, int fontSize) {
    std::vector<FontType *>* fontTypeBatch;
    FontType* fontType = nullptr;
    if (!fonts.count(font->path)) {
        fontTypeBatch = new std::vector<FontType *>;
        fonts.insert(std::pair<const char*, std::vector<FontType *>*>(font->path, fontTypeBatch));
        fontType = new FontType(font, fontSize);
        fontTypeBatch->emplace_back(fontType);
    } else {
        fontTypeBatch = fonts.find(font->path)->second;
        for(auto type : *fontTypeBatch){
            if(type->fontSize == fontSize){
                fontType = type;
                break;
            }
        }
        if(fontType == nullptr) {
            fontType = new FontType(font, fontSize);
            fontTypeBatch->emplace_back(fontType);
        }
    }
    return fontType;
}

void DataManager::loadText(UIText *uiText) {
    uiText->textMesh.loadTextStructure();
    uiText->textMesh.loadText();
}

DataManager::~DataManager() {
    FT_Done_FreeType(library);
    delete defaultFont;
    for (auto elementPair : fonts) {
        for (auto element : *elementPair.second)
            delete element;
        delete elementPair.second;
    }
}