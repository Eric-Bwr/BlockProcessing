// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once
#include <string>
#include "../EventSystem/EventSystem.h"
#include <STB_IMAGE.h>
#include "../Graphics/Image.h"
#include "../AudioSystem/AudioBuffer.h"
#include "../CallbackSystem/Callback.h"


namespace fios {

  void linkCallback(cs::BasicCallback<void, es::Event&>* callback);

  char* readFile(const char* filePath);
  bool writeFile(char* buffer, const char* filePath);
  bool writeToFile(char* buffer, const char* filePath);

  gfx::Image loadImage(const char* filePath);
  as::AudioBuffer loadAudio(const char* filePath);

  //bool exportModel(const char* filePath);

  namespace binary {
    void readFileb(void* buffer, const char* filePath);
    bool writeFileb(void* buffer, const char* filePath, int size);
  }
}