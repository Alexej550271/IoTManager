#include "items/vSensorDht.h"

#include <Arduino.h>

#include "BufferExecute.h"
#include "Class/LineParsing.h"
#include "Global.h"

DHTesp* dht = nullptr;

SensorDht::SensorDht(const params& paramsTmp, const params& paramsHum) {
    _paramsTmp = params(paramsTmp);
    _paramsHum = params(paramsHum);

    if (!dht) {
        dht = new DHTesp();
    }

    dht->setup(_paramsTmp.pin, DHTesp::DHT11);

    interval = _paramsTmp.interval < _paramsHum.interval ? _paramsTmp.interval : _paramsHum.interval;
    interval = interval + dht->getMinimumSamplingPeriod();
}

SensorDht::~SensorDht() {}

void SensorDht::loop() {
    difference = millis() - prevMillis;
    if (difference >= interval) {
        prevMillis = millis();
        readTmpHum();
    }
}

void SensorDht::readTmpHum() {
    float tmp;
    float hum;

    tmp = dht->getTemperature();
    hum = dht->getHumidity();

    if (String(tmp) != "nan" && String(hum) != "nan") {
        tmp = tmp * _paramsTmp.c;
        hum = hum * _paramsHum.c;

        eventGen2(_paramsTmp.key, String(tmp));
        jsonWriteStr(configLiveJson, _paramsTmp.key, String(tmp));
        publishStatus(_paramsTmp.key, String(tmp));
        SerialPrint("I", "Sensor", "'" + _paramsTmp.key + "' data: " + String(tmp));

        eventGen2(_paramsHum.key, String(hum));
        jsonWriteStr(configLiveJson, _paramsHum.key, String(hum));
        publishStatus(_paramsHum.key, String(hum));
        SerialPrint("I", "Sensor", "'" + _paramsHum.key + "' data: " + String(hum));

    } else {
        SerialPrint("E", "Sensor DHT", "Error");
    }
}

MySensorDhtVector* mySensorDht = nullptr;

void dhtSensor() {
    myLineParsing.update();
    String type = myLineParsing.gtype();
    String interval = myLineParsing.gint();
    String pin = myLineParsing.gpin();
    String key = myLineParsing.gkey();
    String c = myLineParsing.gc();
    myLineParsing.clear();

    static int enterCnt = -1;
    enterCnt++;

    static params paramsTmp;
    static params paramsHum;

    if (enterCnt == 0) {
        paramsTmp.type = type;
        paramsTmp.key = key;
        paramsTmp.interval = interval.toInt() * 1000;
        paramsTmp.pin = pin.toInt();
        paramsTmp.c = c.toFloat();
    }

    if (enterCnt == 1) {
        paramsHum.type = type;
        paramsHum.key = key;
        paramsHum.interval = interval.toInt() * 1000;
        paramsHum.pin = pin.toInt();
        paramsHum.c = c.toFloat();

        static bool firstTime = true;
        if (firstTime) mySensorDht = new MySensorDhtVector();
        firstTime = false;
        mySensorDht->push_back(SensorDht(paramsTmp, paramsHum));
    }
}
