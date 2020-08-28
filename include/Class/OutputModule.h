#pragma once

#include <Arduino.h>
#include "Class/LineParsing.h"
#include "Global.h"

class OutputModule : public LineParsing {
   public:
    OutputModule() : LineParsing(){};

    void OutputModuleStateSetDefault() {
        if (_state != "") { 
            OutputModuleChange(_key, _state);
        }
    }

    void OutputModuleChange(String key, String state) {
        state.replace("#", " ");
        eventGen(key, "");
        jsonWriteStr(configLiveJson, key, state);
        MqttClient::publishStatus(key, state);
    }
};
extern OutputModule* myText;