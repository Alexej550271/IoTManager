#pragma once

#include <Arduino.h>

#include "Class/LineParsing.h"
#include "Global.h"

class Input : public LineParsing {
   public:
    Input() : LineParsing(){};

    void inputSetDefaultFloat() {
        inputSetFloat(_key, _state);
    }

    void inputSetDefaultStr() {
        inputSetStr(_key, _state);
    }

    void inputSetFloat(String key, String state) {
        eventGen(key, "");
        jsonWriteFloat(configLiveJson, key, state.toFloat());
        MqttClient::publishStatus(key, state);
    }

    void inputSetStr(String key, String state) {
        eventGen(key, "");
        jsonWriteStr(configLiveJson, key, state);
        MqttClient::publishStatus(key, state);
    }
};

extern Input* myInput;