// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once
#include <functional>
#include "../UserNotificationSystem/UserNotificationSystem.h"

namespace cs {
  //return type, callback
  template<typename returnType, typename T, typename ... TT >
  class BasicCallback {
    private:
      using CallbackFunc = std::function<returnType(T t, TT ... tt)>;
      CallbackFunc callback;
      bool isCallBackBind = false;
      bool showPushMsg = true;
    public:
      BasicCallback() {}
      BasicCallback(bool showPushMsg) : showPushMsg(showPushMsg) {}

      void bind(const CallbackFunc& callback) {
        if(!isCallBackBind) {
          this->callback = callback;
          isCallBackBind = true;
        } else
          uns::log<uns::WARN>("$c", "callback is already binded!");
      }

      template<typename F>
      void bindMFunc(const F &func) {
          if(!isCallBackBind) {
              this->callback = func;
              isCallBackBind = true;
          } else
              uns::log<uns::WARN>("$c", "callback is already binded!");
      }

      void unbind() {
          this->callback = nullptr;
          isCallBackBind = false;
      }

      bool isBind() { return isCallBackBind; }

      returnType pushCall(T t, TT ... tt) {
        if(isCallBackBind)  return callback(t, (tt)...);
        else
          if(showPushMsg)
            uns::log<uns::WARN>("$c", "no callback bind!");
      }

      void togglePushMsg() { showPushMsg = false; }
  };
}