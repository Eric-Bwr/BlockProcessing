#include "UIManager.h"
#include "UIShader.h"

#define LERP(a, b, c) (a+(b-a)*c)

void UIManager::init(int width, int height, bool scaleOnResize) {
    DataManager::init();
    ortho = orthographicMatrix<float>(0.0f, width, height, 0.0, -1.0, 1.0);
    textShader = new Shader();
    textShader->addVertexShader(UITextShaderVertex);
    textShader->addFragmentShader(UITextShaderFragment);
    textShader->finish();
    textShader->addUniforms({"ortho", "textureCoords", "level", "image", "color"});
    textShader->setUniformMatrix4f("ortho", ortho.getBuffer());
    quadShader = new Shader();
    quadShader->addVertexShader(UIQuadShaderVertex);
    quadShader->addFragmentShader(UIQuadShaderFragment);
    quadShader->finish();
    quadShader->addUniforms({"info", "textureCoords", "roundingCoords", "corners", "ortho", "smoothness", "color", "image"});
    quadShader->setUniformMatrix4f("ortho", ortho.getBuffer());
    quadShader->setUniform1f("smoothness", CORNER_SMOOTHNESS);
    start = std::chrono::system_clock::now();
    this->width = width;
    this->height = height;
    this->scaleOnResize = scaleOnResize;
}

void UIManager::add(UIComponent *component, int order) {
    std::vector<UIComponent *> *componentBatch;
    if (!components.count(order)) {
        componentBatch = new std::vector<UIComponent *>;
        components.insert(std::pair<int, std::vector<UIComponent *> *>(order, componentBatch));
    } else {
        componentBatch = components.find(order)->second;
    }
    for(auto compare : *components.at(order))
        if(compare == component)
            return;
    componentBatch->emplace_back(component);
}

void UIManager::remove(UIComponent *component) {
    for (auto compPair : components) {
        std::vector<UIComponent *> *compList = compPair.second;
        for (int i = 0; i < compList->size(); i++)
            if (compList->at(i) == component)
                compList->erase(compList->begin() + i);
    }
}

void UIManager::setSize(int width, int height) {
    ortho.orthographic(0.0f, width, height, 0.0, -1.0, 1.0);
    textShader->bind();
    textShader->setUniformMatrix4f("ortho", ortho.getBuffer());
    quadShader->bind();
    quadShader->setUniformMatrix4f("ortho", ortho.getBuffer());
    if(width == 0 || height == 0)
        return;
    if (scaleOnResize) {
        double factorX = (double) width / (double) this->width;
        double factorY = (double) height / (double) this->height;
        for (auto const &componentList : components)
            for (auto component : *componentList.second)
                component->setBounds(component->positionX * factorX, component->positionY * factorY,component->width * factorX, component->height * factorY);
    }
    this->width = width;
    this->height = height;
}

void UIManager::keyInput(int key, int action, int mods) {
    for (auto const &componentList : components)
        for (auto component : *componentList.second)
            component->keyInput(key, action, mods);
}

void UIManager::charInput(unsigned int key) {
    for (auto const &componentList : components)
        for (auto component : *componentList.second)
            component->charInput(key);
}

void UIManager::mousePositionInput(double x, double y) {
    this->mouseX = x;
    this->mouseY = y;
    for (auto const &componentList : components)
        for (auto component : *componentList.second)
            component->mousePositionInput(x, y);
}

void UIManager::mouseButtonInput(int button, int action) {
    if (button == MOUSE_BUTTON_PRESSED) {
        for (auto const &componentList : components) {
            auto i = componentList.first;
            for (auto component : *componentList.second) {
                component->mouseButtonInput(action);
                component->mousePositionInput(mouseX, mouseY);
            }
        }
    }
}

void UIManager::render() {
    if ((std::chrono::system_clock::now() - start).count() >= CURSOR_INTERVAL) {
        cursor = !cursor;
        start = std::chrono::system_clock::now();
    }
    for (auto const &componentList : components)
        for (auto component : *componentList.second)
            renderComponent(component);
}

void UIManager::renderComponent(UIComponent *component) {
    quadShader->bind();
    if (component->type == UIComponentType::UITEXT) {
        auto ui = (UIText *) component;
        textShader->bind();
        textShader->setUniform4f(SHADER_COLOR_NAME, ui->r, ui->g, ui->b, ui->a);
        ui->textMesh.render();
        quadShader->bind();
    } else if (component->type == UIComponentType::UITEXTFIELD) {
        auto ui = ((UITextField *) component);
        auto bgc = ui->bgColor.standard;
        if (ui->pressed)
            bgc = ui->bgColor.pressed;
        else if (ui->hovered)
            bgc = ui->bgColor.hover;
        quadShader->setUniform4f(SHADER_COLOR_NAME, bgc.r, bgc.g, bgc.b, bgc.a);
        if (ui->texture != nullptr)
            ui->texture->bind();
        if (ui->pressed)
            ui->mesh.render(2);
        else if (ui->hovered)
            ui->mesh.render(1);
        else
            ui->mesh.render(0);
        if (ui->pressed && cursor) {
            auto cc = ui->cursorColor;
            quadShader->setUniform4f(SHADER_COLOR_NAME, cc.r, cc.g, cc.b, cc.a);
            ui->cursorMesh.render();
        }
        auto fgc = ui->fgColor;
        textShader->bind();
        textShader->setUniform4f(SHADER_COLOR_NAME, fgc.r, fgc.g, fgc.b, fgc.a);
        ui->text.textMesh.render();
        quadShader->bind();
    } else if (component->type == UIComponentType::UITEXTAREA) {
        auto ui = ((UITextArea *) component);
        auto bgc = ui->bgColor.standard;
        if (ui->pressed)
            bgc = ui->bgColor.pressed;
        else if (ui->hovered)
            bgc = ui->bgColor.hover;
        quadShader->setUniform4f(SHADER_COLOR_NAME, bgc.r, bgc.g, bgc.b, bgc.a);
        if (ui->texture != nullptr)
            ui->texture->bind();
        if (ui->pressed)
            ui->mesh.render(2);
        else if (ui->hovered)
            ui->mesh.render(1);
        else
            ui->mesh.render(0);
        if (ui->pressed && cursor) {
            auto cc = ui->cursorColor;
            quadShader->setUniform4f(SHADER_COLOR_NAME, cc.r, cc.g, cc.b, cc.a);
            ui->cursorMesh.render();
        }
        auto fgc = ui->fgColor;
        textShader->bind();
        textShader->setUniform4f(SHADER_COLOR_NAME, fgc.r, fgc.g, fgc.b, fgc.a);
        ui->text.textMesh.render();
        quadShader->bind();
    } else if (component->type == UIComponentType::UIBUTTON) {
        auto ui = (UIButton *) component;
        auto bgc = ui->bgColor.standard;
        if (ui->pressed)
            bgc = ui->bgColor.pressed;
        else if (ui->hovered)
            bgc = ui->bgColor.hover;
        quadShader->setUniform4f(SHADER_COLOR_NAME, bgc.r, bgc.g, bgc.b, bgc.a);
        if (ui->texture != nullptr)
            ui->texture->bind();
        if (ui->pressed)
            ui->mesh.render(2);
        else if (ui->hovered)
            ui->mesh.render(1);
        else
            ui->mesh.render(0);
        auto fgc = ui->fgColor;
        textShader->bind();
        textShader->setUniform4f(SHADER_COLOR_NAME, fgc.r, fgc.g, fgc.b, fgc.a);
        ui->text.textMesh.render();
        quadShader->bind();
    } else if (component->type == UIComponentType::UIIMAGE) {
        auto ui = (UIImage*) component;
        auto bgc = ui->color;
        quadShader->setUniform4f(SHADER_COLOR_NAME, bgc.r, bgc.g, bgc.b, bgc.a);
        if (ui->texture != nullptr)
            ui->texture->bind();
        ui->mesh.render();
    } else if (component->type == UIComponentType::UILAYOUT) {
        auto layout = (Layout *) component;
        for (auto ui : layout->components)
            renderComponent(ui);
    } else if (component->type == UIComponentType::UISLIDER) {
        auto ui = (UISlider *) component;
        auto bgc = ui->bgColor;
        auto dc = ui->dragColor;
        auto sc = ui->slideColor;
        if (ui->texture != nullptr)
            ui->texture->bind();
        if (ui->dragging) {
            quadShader->setUniform4f(SHADER_COLOR_NAME, bgc.pressed.r, bgc.pressed.g, bgc.pressed.b, bgc.pressed.a);
            ui->bgMesh.render(2);
            quadShader->setUniform4f(SHADER_COLOR_NAME, sc.pressed.r, sc.pressed.g, sc.pressed.b, sc.pressed.a);
            ui->slideMesh.render(2);
            quadShader->setUniform4f(SHADER_COLOR_NAME, dc.pressed.r, dc.pressed.g, dc.pressed.b, dc.pressed.a);
            ui->dragMesh.render(2);
        } else if (ui->hovered) {
            quadShader->setUniform4f(SHADER_COLOR_NAME, bgc.hover.r, bgc.hover.g, bgc.hover.b, bgc.hover.a);
            ui->bgMesh.render(1);
            quadShader->setUniform4f(SHADER_COLOR_NAME, sc.hover.r, sc.hover.g, sc.hover.b, sc.hover.a);
            ui->slideMesh.render(1);
            quadShader->setUniform4f(SHADER_COLOR_NAME, dc.hover.r, dc.hover.g, dc.hover.b, dc.hover.a);
            ui->dragMesh.render(1);
        } else {
            quadShader->setUniform4f(SHADER_COLOR_NAME, bgc.standard.r, bgc.standard.g, bgc.standard.b, bgc.standard.a);
            ui->bgMesh.render(0);
            quadShader->setUniform4f(SHADER_COLOR_NAME, sc.standard.r, sc.standard.g, sc.standard.b, sc.standard.a);
            ui->slideMesh.render(0);
            quadShader->setUniform4f(SHADER_COLOR_NAME, dc.standard.r, dc.standard.g, dc.standard.b, dc.standard.a);
            ui->dragMesh.render(0);
        }
    } else if (component->type == UIComponentType::UISWITCH) {
        auto sw = (UISwitch *) component;
        auto bgc = sw->bgColor;
        auto sc = sw->switchColor;

        float ass = sw->getSwitchSize() * sw->height;
        float targetSwitchX = sw->isEnabled() ? sw->width - ass : 0;
        float oldSwitchX = sw->getSwitchX();
        sw->setSwitchX(LERP(oldSwitchX, targetSwitchX, 0.05));

        quadShader->setUniform4f(SHADER_COLOR_NAME, bgc.r, bgc.g, bgc.b, bgc.a);
        sw->bgMesh.render();
        quadShader->setUniform4f(SHADER_COLOR_NAME, sc.r, sc.g, sc.b, sc.a);
        sw->switchMesh.render();
    } else if (component->type == UIComponentType::UISPLITPANE) {
        auto ui = (UISplitPane *) component;
        if (ui->texture != nullptr)
            ui->texture->bind();
        auto bgc = ui->dividerColor.standard;
        if (ui->dragging) {
            bgc = ui->dividerColor.pressed;
            quadShader->setUniform4f(SHADER_COLOR_NAME, bgc.r, bgc.g, bgc.b, bgc.a);
            ui->mesh.render(2);
        } else if (ui->hovered) {
            bgc = ui->dividerColor.hover;
            quadShader->setUniform4f(SHADER_COLOR_NAME, bgc.r, bgc.g, bgc.b, bgc.a);
            ui->mesh.render(1);
        } else {
            quadShader->setUniform4f(SHADER_COLOR_NAME, bgc.r, bgc.g, bgc.b, bgc.a);
            ui->mesh.render(0);
        }
        renderComponent(ui->getLeft());
        renderComponent(ui->getRight());
    } else if (component->type == UIComponentType::UISCROLLBAR) {
        auto ui = (UIScrollbar *) component;
        auto bgc = ui->barBgColor;
        quadShader->setUniform4f(SHADER_COLOR_NAME, bgc.r, bgc.g, bgc.b, bgc.a);
        ui->barBgMesh.render();
        auto fgc = ui->barFgColor;
        quadShader->setUniform4f(SHADER_COLOR_NAME, fgc.r, fgc.g, fgc.b, fgc.a);
        ui->barFgMesh.render();
        glEnable(GL_SCISSOR_TEST);
        if (ui->orientation == Orientation::VERTICAL)
            glScissor(ui->positionX, height - (ui->positionY + ui->height), ui->width - ui->barWidth, ui->height);
        else
            glScissor(ui->positionX, height - (ui->positionY + ui->height - ui->barWidth), ui->width,
                      ui->height - ui->barWidth);
        renderComponent(ui->target);
        glDisable(GL_SCISSOR_TEST);
    }
}

UIManager::~UIManager() {
    for (auto compPair : components)
        delete compPair.second;
    delete quadShader;
    delete textShader;
}