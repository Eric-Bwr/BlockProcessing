#include "TextMesh.h"
#include "FontType.h"
#include "../UIText.h"

void TextMesh::init(UIText* uiText) {
    this->uiText = uiText;
    vao.init();
    vbo.init(nullptr, 0, GL_DYNAMIC_DRAW);
    vao.addBuffer(vbo, DataManager::textLayout);
}

void TextMesh::loadTextStructure(){
    auto length = uiText->text.size();
    Word word;
    auto spaceWidth = uiText->fontType->characters[' '].advance;
    std::vector<Word> words;
    for (int i = 0; i < length; i++) {
        char currentChar = uiText->text[i];
        if(currentChar == '\r')
            currentChar = '\n';
        if (currentChar == ' ') {
            if (!word.characters.empty())
                words.emplace_back(word);
            word.characters.clear();
            word.width = 0.0f;
            word.spaceWidth = spaceWidth;
            words.emplace_back(word);
            word.width = 0.0f;
            word.spaceWidth = 0.0f;
        } else if (currentChar == '\n') {
            word.characters.emplace_back(currentChar);
            words.emplace_back(word);
            word.width = 0.0f;
            word.spaceWidth = 0.0f;
            word.characters.clear();
        } else {
            if(word.width + uiText->fontType->characters[currentChar].advance > uiText->width){
                words.emplace_back(word);
                word.characters.clear();
                word.width = uiText->fontType->characters[currentChar].advance;
                word.spaceWidth = 0.0f;
                word.characters.emplace_back(currentChar);
            }else {
                word.width += uiText->fontType->characters[currentChar].advance;
                word.characters.emplace_back(currentChar);
            }
            if (i == length - 1)
                words.emplace_back(word);
        }
    }
    Line line;
    lines.clear();
    for (auto &word : words) {
        if (word.characters.empty()) {
            if(line.lineWidth + word.width + word.spaceWidth > uiText->width){
                lines.emplace_back(line);
                line.words.clear();
                line.words.emplace_back(word);
                line.lineWidth = 0.0f;
                line.lineWidth += word.width + word.spaceWidth;
            }else {
                line.lineWidth += word.width + word.spaceWidth;
                line.words.emplace_back(word);
            }
        } else {
            if (word.characters.back() == '\n') {
                if (line.lineWidth + word.width + word.spaceWidth <= uiText->width) {
                    line.words.emplace_back(word);
                    line.lineWidth += word.width + word.spaceWidth;
                } else {
                    lines.emplace_back(line);
                    line.words.clear();
                    line.words.emplace_back(word);
                    line.lineWidth = 0.0f;
                    line.lineWidth += word.width + word.spaceWidth;
                }
                lines.emplace_back(line);
                line.words.clear();
                line.lineWidth = 0.0f;
            } else if (line.lineWidth + word.width + word.spaceWidth <= uiText->width) {
                line.words.emplace_back(word);
                line.lineWidth += word.width + word.spaceWidth;
            } else {
                lines.emplace_back(line);
                line.words.clear();
                line.words.emplace_back(word);
                line.lineWidth = 0.0f;
                line.lineWidth += word.width + word.spaceWidth;
            }
        }
    }
    lines.emplace_back(line);
}

void TextMesh::loadText() {
    texture = uiText->fontType->texture;
    vertices.clear();
    auto spaceWidth = uiText->fontType->characters[' '].advance;
    auto bearing = uiText->fontType->bearing + 2;
    auto height = uiText->fontType->height;
    auto cutoff = height - (bearing - 2);
    float cursorX, cursorY;
    if (uiText->mode == UITextMode::CENTERED_VERTICAL_RIGHT || uiText->mode == UITextMode::CENTERED_VERTICAL_LEFT || uiText->mode == UITextMode::CENTERED)
        if (lines.size() * uiText->fontSize >= uiText->height)
            cursorY = uiText->positionY + bearing;
        else
            cursorY = uiText->positionY + ((uiText->height / 2) - ((lines.size() * uiText->fontSize) / 2) + height - cutoff / 2);
    else
        cursorY = uiText->positionY + bearing;
    for (const auto &line : lines) {
        if (!line.words.empty() && line.words.at(0).width > uiText->width)
            break;
        if (uiText->mode == UITextMode::CENTERED_HORIZONTAL || uiText->mode == UITextMode::CENTERED)
            if (line.words.empty() || line.words.back().characters.empty())
                cursorX = uiText->positionX + ((uiText->width / 2) - (line.lineWidth / 2)) + spaceWidth / 2;
            else
                cursorX = uiText->positionX + ((uiText->width / 2) - (line.lineWidth / 2));
        else if (uiText->mode == UITextMode::RIGHT || uiText->mode == UITextMode::CENTERED_VERTICAL_RIGHT)
            if (line.words.empty() || line.words.back().characters.empty())
                cursorX = uiText->positionX + uiText->width - line.lineWidth + spaceWidth;
            else
                cursorX = uiText->positionX + uiText->width - line.lineWidth;
        else
            cursorX = uiText->positionX;
        if (cursorY - uiText->positionY + cutoff > uiText->height)
            break;
        for (const auto &word : line.words) {
            cursorX += word.spaceWidth;
            for (auto character : word.characters) {
                if(character != '\n') {
                    Character c = uiText->fontType->characters[character];
                    auto x = cursorX + c.bearingX;
                    auto y = cursorY - c.bearingY;

                    vertices.emplace_back(x);
                    vertices.emplace_back(y + c.height);
                    vertices.emplace_back(0);
                    vertices.emplace_back(c.textureY);
                    vertices.emplace_back(c.ascii);

                    vertices.emplace_back(x);
                    vertices.emplace_back(y);
                    vertices.emplace_back(0);
                    vertices.emplace_back(0);
                    vertices.emplace_back(c.ascii);

                    vertices.emplace_back(x + c.width);
                    vertices.emplace_back(y);
                    vertices.emplace_back(c.textureX);
                    vertices.emplace_back(0);
                    vertices.emplace_back(c.ascii);

                    vertices.emplace_back(x);
                    vertices.emplace_back(y + c.height);
                    vertices.emplace_back(0);
                    vertices.emplace_back(c.textureY);
                    vertices.emplace_back(c.ascii);

                    vertices.emplace_back(x + c.width);
                    vertices.emplace_back(y);
                    vertices.emplace_back(c.textureX);
                    vertices.emplace_back(0);
                    vertices.emplace_back(c.ascii);

                    vertices.emplace_back(x + c.width);
                    vertices.emplace_back(y + c.height);
                    vertices.emplace_back(c.textureX);
                    vertices.emplace_back(c.textureY);
                    vertices.emplace_back(c.ascii);
                    cursorX += c.advance;
                }
            }
        }
        cursorY += uiText->fontType->fontSize + uiText->lineAdvance;
    }
    vertexCount = vertices.size() / 5;
    vbo.subData(vertices.data(), vertexCount * DataManager::textLayout.getStride(), 0, GL_DYNAMIC_DRAW);
}

void TextMesh::render() {
    vao.bind();
    texture->bind();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

TextMesh::~TextMesh() {
    vertices.clear();
}
