#pragma once

#include <Buffer.h>

class UIText;
class FontType;
class Texture;
class UIColor;

struct Word {
    std::vector<char> characters;
    float width = 0.0f;
    float spaceWidth = 0.0f;
};

struct Line {
    std::vector<Word> words;
    float lineWidth = 0.0f;
};

class TextMesh {
public:
    void init(UIText* uiText);
    void loadTextStructure();
    void loadText();
    void render();
    ~TextMesh();
    std::vector<Line> lines;
private:
    UIText* uiText;
    std::vector<float> vertices;
    unsigned int vertexCount;
    VertexArrayObject vao;
    VertexBufferObject vbo;
    Texture* texture;
};
