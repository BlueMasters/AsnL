#include <Arduino.h>

#include "AsnLWriter.h"
#include "AsnLParser.h"


void setup() {
    Serial.begin(9600);
    Serial.println("TESTING");

    AsnLWriter a = AsnLWriter(32);
    a.Init();
    a.Struct();
    a.Int(1,1);
    a.Struct();
    a.Str("1234");
    a.Int(2, 2000);
    a.Int(2,   -1);
    a.Int(4,   -1);
    a.EndStruct();
    a.EndStruct();

    Serial.print("Check: ");
    Serial.println(a.FixOk() ? "OK" : "ERROR");

    a.Dump();

    AsnLParser b = AsnLParser(32);
    b.FromAsnL(a);
    b.Init();
    while (1) {
        int t = b.NextToken();
        Serial.print((char)t);
        Serial.print(":");
        if (t == ASNL_INT) {
            int x;
            b.ReadInt(&x);
            Serial.println(x);
        } else if (t == ASNL_STRING) {
            char buffer[32];
            b.ReadString(buffer, 32);
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
    Serial.println(a.FixOk() ? "OK" : "ERROR");
    b.Dump();

    Serial.println("DONE");
}

void loop() {

}
