/********************************************************************
 * COPYRIGHT: 
 * Copyright (c) 1997-2002, International Business Machines Corporation and
 * others. All Rights Reserved.
 ********************************************************************/
/*   file name:  strtest.cpp
*   encoding:   US-ASCII
*   tab size:   8 (not used)
*   indentation:4
*
*   created on: 1999nov22
*   created by: Markus W. Scherer
*/

#include "unicode/utypes.h"
#include "unicode/putil.h"
#include "intltest.h"
#include "strtest.h"
#include "unicode/ustring.h"

void StringTest::TestEndian(void) {
    union {
        uint8_t byte;
        uint16_t word;
    } u;
    u.word=0x0100;
    if(U_IS_BIG_ENDIAN!=u.byte) {
        errln("TestEndian: U_IS_BIG_ENDIAN needs to be fixed in platform.h");
    }
}

void StringTest::TestSizeofWCharT(void) {
    if(U_SIZEOF_WCHAR_T!=sizeof(wchar_t)) {
        errln("TestSizeofWCharT: U_SIZEOF_WCHAR_T!=sizeof(wchar_t) - U_SIZEOF_WCHAR_T needs to be fixed in platform.h");
    }
}

void StringTest::TestSizeofInt64(void) {
#ifdef U_INT64_T_UNAVAILABLE
    errln("int64_t and uint64_t are undefined.");
#else
    if(8!=sizeof(int64_t)) {
        errln("TestSizeofInt64: 8!=sizeof(int64_t) - int64_t needs to be fixed in platform.h");
    }
    if(8!=sizeof(uint64_t)) {
        errln("TestSizeofInt64: 8!=sizeof(uint64_t) - uint64_t needs to be fixed in platform.h");
    }
#endif
}

void StringTest::TestCharsetFamily(void) {
    unsigned char c='A';
    if( U_CHARSET_FAMILY==U_ASCII_FAMILY && c!=0x41 ||
        U_CHARSET_FAMILY==U_EBCDIC_FAMILY && c!=0xc1
    ) {
        errln("TestCharsetFamily: U_CHARSET_FAMILY needs to be fixed in platform.h");
    }
}

U_STRING_DECL(ustringVar, "aZ0 -", 5);

void StringTest::runIndexedTest(int32_t index, UBool exec, const char *&name, char * /*par*/) {
    if(exec) {
        logln("TestSuite Character and String Test: ");
    }
    switch(index) {
    case 0:
        name="TestEndian";
        if(exec) {
            TestEndian();
        }
        break;
    case 1:
        name="TestSizeofWCharT";
        if(exec) {
            TestSizeofWCharT();
        }
        break;
    case 2:
        name="TestSizeofInt64";
        if(exec) {
            TestSizeofInt64();
        }
        break;
    case 3:
        name="TestCharsetFamily";
        if(exec) {
            TestCharsetFamily();
        }
        break;
    case 4:
        name="Test_U_STRING";
        if(exec) {
            U_STRING_INIT(ustringVar, "aZ0 -", 5);
            if( sizeof(ustringVar)/sizeof(*ustringVar)!=6 ||
                ustringVar[0]!=0x61 ||
                ustringVar[1]!=0x5a ||
                ustringVar[2]!=0x30 ||
                ustringVar[3]!=0x20 ||
                ustringVar[4]!=0x2d ||
                ustringVar[5]!=0
            ) {
                errln("Test_U_STRING: U_STRING_DECL with U_STRING_INIT does not work right! "
                      "See putil.h and utypes.h with platform.h.");
            }
        }
        break;
    case 5:
        name="Test_UNICODE_STRING";
        if(exec) {
            UnicodeString ustringVar=UNICODE_STRING("aZ0 -", 5);
            if( ustringVar.length()!=5 ||
                ustringVar[0]!=0x61 ||
                ustringVar[1]!=0x5a ||
                ustringVar[2]!=0x30 ||
                ustringVar[3]!=0x20 ||
                ustringVar[4]!=0x2d
            ) {
                errln("Test_UNICODE_STRING: UNICODE_STRING does not work right! "
                      "See unistr.h and utypes.h with platform.h.");
            }
        }
        break;
    default:
        name="";
        break;
    }
}
