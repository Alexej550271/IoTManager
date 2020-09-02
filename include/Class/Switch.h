#pragma once

#include <Arduino.h>

#include "Class/LineParsing.h"
#include "Global.h"

class Switch : public LineParsing {
   protected:
    int numberEntering = 0;
    int state = _state.toInt();

   public:
    Switch() : LineParsing(){};

    void init() {
        if (_pin != "") {
            int number = numberEntering++;
            buttons[number].attach(_pin.toInt());
            buttons[number].interval(_db.toInt());
            but[number] = true;
            jsonWriteStr(configOptionJson, "switch_num_" + String(number), _key);
        }
    }

    void loop() {
        static uint8_t switch_number = 1;
        if (but[switch_number]) {
            buttons[switch_number].update();
            if (buttons[switch_number].fell()) {
            }
            if (buttons[switch_number].rose()) {
                String key = jsonReadStr(configOptionJson, "switch_num_" + String(switch_number));
                state = !state;
                switchChangeVirtual(key, String(state));
            }
        }
        switch_number++;
        if (switch_number == NUM_BUTTONS) {
            switch_number = 0;
        }
    }
    void switchStateSetDefault() {
        if (_state != "") {
            switchChangeVirtual(_key, _state);
        }
    }

    void switchChangeVirtual(String key, String state) {
        eventGen(key, "");
        jsonWriteInt(configLiveJson, key, state.toInt());
        MqttClient::publishStatus(key, state);
    }
};

extern Switch* mySwitch;