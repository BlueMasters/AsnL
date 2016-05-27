/****************************************************************************
 * Copyright 2016 BlueMasters
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#ifndef ASN_L_WRITER__H
#define ASN_L_WRITER__H

#include "AsnL.h"
#include <Arduino.h>

class AsnLWriter : public AsnL {

public:
    AsnLWriter(int bufferSize) : AsnL(bufferSize) {};

    void Init();
    int Int(int len, int value);
    int Uint(int len, unsigned int value);
    int Str(String txt);
    int Struct();
    int EndStruct();
    int Close();

};

#endif
