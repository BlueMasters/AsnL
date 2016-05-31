#include <Arduino.h>

#include "AsnLMsg.h"
#include "AsnLWriter.h"
#include "AsnLParser.h"


void setup() {
    Serial.begin(9600);
    Serial.println("TESTING");

    AsnLMsg msg = AsnLMsg(32);
    AsnLParser p = AsnLParser(msg);
    AsnLWriter w = AsnLWriter(msg);

    w.init();
    w.structure();
    w.integer(1,1);
    w.structure();
    w.string("1234");
    w.integer(2, 2000);
    w.integer(2,   -1);
    w.integer(4,   -1);
    w.endStructure();
    w.endStructure();

    Serial.print("Check: ");
    Serial.println(w.fixOk() ? "OK" : "ERROR");

    msg.dump();

    p.init();
    while (1) {
        int t = p.nextToken();
        Serial.print((char)t);
        Serial.print(":");
        if (t == ASNL_INT) {
            int x;
            p.readInt(&x);
            Serial.println(x);
        } else if (t == ASNL_STRING) {
            char buffer[32];
            p.readString(buffer, 32);
            Serial.print("'");
            Serial.print(buffer);
            Serial.println("'");
        } else {
            Serial.println();
        }
        if (t == ASNL_NIL) break;
    }
    Serial.println();
    Serial.print("Check: ");
    Serial.println(p.fixOk() ? "OK" : "ERROR");
    msg.dump();

    Serial.println("DONE");
}

void loop() {

}
