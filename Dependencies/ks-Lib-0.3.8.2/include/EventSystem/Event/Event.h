// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once
#include <string>

#define EVENTID(id) static unsigned int GetEventID() { return id; } unsigned int getEventID() const override { return GetEventID(); }
#define EVENTCATEGORY(category) static unsigned int GetEventCategory() { return category; } unsigned int getEventCategory() const override { return GetEventCategory(); }
#define EVENT(id, category) EVENTID(id) EVENTCATEGORY(category)

static const std::string buildErrorMessage(const char* msg, const char* path) { std::string _msg(msg); _msg.append(path); return _msg; }
#define BUILD_ERROR_MESSAGE(msg, path) buildErrorMessage(msg, path)

//Event category
#define WINDOW_EVENT_CATEGORY                       0
#define INPUT_EVENT_CATEGORY                        1
#define APPLICATION_EVENT_CATEGORY                  2
#define ERROR_EVENT_CATEGORY                        3
#define USER_EVENT_CATEGORY                         10

//Window event
#define WINDOW_CLOSE_EVENT                          0
#define WINDOW_HAS_FOCUS_EVENT                      1
#define WINDOW_LOST_FOCUS_EVENT                     2
#define WINDOW_DROP_EVENT                           3
#define WINDOW_SIZE_EVENT                           4
#define WINDOW_FRAMEBUFFER_EVENT                    5
#define WINDOW_ICONIFY_EVENT                        6
#define WINDOW_RESTORED_EVENT                       7
#define WINDOW_POSITION_EVENT                       8
#define WINDOW_REFRESH_EVENT                        9

//Input event
#define CHAR_EVENT                                  10
#define CHAR_MODS_EVENT                             11
#define CURSOR_ENTERED_EVENT                        12
#define CURSOR_LEAVED_EVENT                         13
#define CURSOR_POSITION_EVENT                       14
#define KEY_PRESSED_EVENT                           15
#define KEY_RELEASED_EVENT                          16
#define KEY_REPEAT_EVENT                            17
#define MOUSE_BUTTON_PRESSED_EVENT                  18
#define MOUSE_BUTTON_RELEASED_EVENT                 19
#define MOUSE_UP_SCROLL_EVENT                       20
#define MOUSE_DOWN_SCROLL_EVENT                     21

//Application event
#define APPLICATION_UPDATE_EVENT                    22
#define APPLICATION_RENDER_EVENT                    23

//Error Event
#define APPLICATION_ERROR_EVENT                     24
#define WINDOW_ERROR_EVENT                          25
#define FILESTREAM_ERROR_EVENT                      26
#define NETWORK_ERROR_EVENT                         27
#define GRAPHICAL_ERROR_EVENT                       28
#define USER_ERROR_EVENT                            29

//Application Error defines
#define NO_SCENE_TO_MANAGE                          0

//Window Error defines
#define UNKNOWN_API_SELECTED                        0
#define FAILED_TO_INIT_GLEW                         1
#define FAILED_TO_INIT_GLFW                         2
#define FAILED_TO_INIT_SDL                          3
#define FAILED_TO_SET_GL_DEBUG_FLAG                 4

//FileStream Error defines
#define FILE_NOT_FOUND                              0
#define FILE_IS_EMPTY                               1
#define FAILED_TO_ALLOCATE_MEMORY_FOR_FILE_DATA     2
#define FAILED_TO_READ_FILE                         3
#define FAILED_TO_WRITE_TO_FILE                     4

//Security level
#define SECURITY_LOW                                0
#define SECURITY_MID                                1
#define SECURITY_HIGHT                              2
#define SECURITY_CRIT                               3

//Data id´s
#define  WINDOW_DATA_ID                             0

//Namespace EventSystem
namespace es {
    class Event {
    public:
        bool handled = false;
        void* userData = nullptr;
        int* dataID = nullptr;

        void setUserData(void* data, int dataID);
        virtual unsigned int getEventID() const = 0;
        virtual unsigned int getEventCategory() const = 0;
    };
} //Namespace EventSystem