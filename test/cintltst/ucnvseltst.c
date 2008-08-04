/********************************************************************
 * Copyright (c) 1997-2008, International Business Machines
 * Corporation and others. All Rights Reserved.
 ********************************************************************
 *
 * File UCNVSELTST.C
 *
 * Modification History:
 *        Name                     Description            
 *     MOHAMED ELDAWY               Creation
 ********************************************************************
 */

/* C API AND FUNCTIONALITY TEST FOR CONVERTER SELECTOR (ucnvsel.h)*/

#include "ucnvseltst.h"

#include <stdio.h>

#include "unicode/utypes.h"
#include "unicode/ucnvsel.h"
#include "cmemory.h"
#include "cstring.h"


void addCnvSelTest(TestNode** root)
{
    addTest(root, &TestConversionUTF16, "ucnv/ucnvseltst/TestConversionUTF16");
    addTest(root, &TestConversionUTF8, "ucnv/ucnvseltst/TestConversionUTF8");
    addTest(root, &TestSerializationAndUnserialization, "ucnv/ucnvseltst/TestSerializationAndUnserialization");
}




// warning: this function is retarded!
// there doesn't seem to be a fn in ucnv to get the index of a converter
// given one of its aliases!
int findIndex (const char* converterName) {
  UErrorCode status = U_ZERO_ERROR;
  int i;
  for (i = 0 ; i < ucnv_countAvailable() ; i++) {
    int alias_index;
    const char* convName = ucnv_getAvailableName(i);
    if(ucnv_compareNames(convName, converterName) == 0) {
      return i;
    }

    for (alias_index = 0 ; alias_index < ucnv_countAliases(convName, & status) ; alias_index++) {
      const char* aliasName = ucnv_getAlias(convName, alias_index, & status);
      if(ucnv_compareNames(aliasName, converterName) == 0) {
        return i;
      }
    }
  }
  return -1;
}

// fill a boolean array with whether the conversion succeeded
// or not
void fillBool(UEnumeration* res, UBool* toFill, int toFillLen) {
  UErrorCode status = U_ZERO_ERROR;
  int i;
  for(i = 0 ; i < toFillLen ; i++)
    toFill[i] = FALSE;
  for(i = 0 ; i < uenum_count(res,&status) ; i++) {
    const char* name = uenum_next(res,NULL, &status);
    toFill[findIndex(name)] = TRUE;
  }
}



void verifyResultUTF8(const char* const s, const char** encodings, int num_encodings, UEnumeration* res, const USet* excludedEncodings, const UConverterUnicodeSet   whichSet) {
  UBool* resultsFromSystem;
  UBool* resultsManually;
  int i;
  resultsFromSystem = (UBool*) uprv_malloc(ucnv_countAvailable() * sizeof(UBool));
  resultsManually = (UBool*) uprv_malloc(ucnv_countAvailable() * sizeof(UBool));
  for(i = 0 ; i < ucnv_countAvailable() ; i++)
    resultsFromSystem[i] = resultsManually[i] = FALSE;


  for(i = 0 ; i < num_encodings ; i++) {
    UErrorCode status = U_ZERO_ERROR;
    //get unicode set for that converter
    USet* unicode_point_set;
    unicode_point_set = uset_open(1, 0);  
    UConverter* test_converter = ucnv_open(encodings[i], &status);
    ucnv_getUnicodeSet(test_converter, unicode_point_set,
                       whichSet, &status);


    int offset = 0;
    int length = uprv_strlen(s);

    resultsManually[findIndex(encodings[i])] = TRUE;
    UChar32 next = 0;

    while(offset<length) {
      U8_NEXT(s, offset, length, next)
      if (next >= 0 && uset_contains(excludedEncodings, next)==FALSE && uset_contains(unicode_point_set, next)==FALSE) {
        resultsManually[findIndex(encodings[i])] = FALSE;
        break;
      }
    }
    uset_close(unicode_point_set);

    ucnv_close(test_converter);
  }
  //fill the bool for the selector results!
  fillBool(res, resultsFromSystem, ucnv_countAvailable());
  for(i = 0 ; i < ucnv_countAvailable() ; i++) {
    if(resultsManually[i] != resultsFromSystem[i]) {
      log_err("failure in converter selector converter %s had conflicting results manual: %d, system %d\n",ucnv_getAvailableName(i), resultsManually[i], resultsFromSystem[i]);
      exit(1);
    }
  }
  uprv_free(resultsFromSystem);
  uprv_free(resultsManually);
}


static void TestConversionUTF8()
{
  //test cases are separated by a -1
  //each line is one test case including encodings to check for
  //I'd like to generate this array randomly but not sure if this is an allowed practice in ICU
  int encodingsTestCases[] = {  90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, -1,
                                1, 3, 7, 9, 11, 13, 12, 15, 19, 20, 22, 24, -1,
                                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1,
                                0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, -1,
                                1, 5, 9, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, -1,
                                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 
                                30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
                                60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
                                90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 
                                130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
                                160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
                                190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, -1, 1, -1};



  USet* excluded_sets[3];
  int i;
  excluded_sets[0] = uset_open(1,0);
  for(i = 1 ; i < 3 ; i++)
    excluded_sets[i] = uset_open(i*30, i*30+500);

  int prev, testCaseIdx;
  int excluded_set_id;
  int curCase = 0;

 for(excluded_set_id = 0 ; excluded_set_id < 3 ; excluded_set_id++)
  for(testCaseIdx = 0, prev=0, curCase=0 ; testCaseIdx < sizeof(encodingsTestCases) / sizeof(int) ; testCaseIdx++)
  {
    if(encodingsTestCases[testCaseIdx] != -1) continue;
    curCase++;

    if(QUICK && curCase > 2)
      break;

    UErrorCode status = U_ZERO_ERROR;
    UEnumeration* res1;
    int i;
    USet* partial_set = NULL;
    UConverterSelector* sel;
    char** encodings = (char**) uprv_malloc((testCaseIdx - prev) * sizeof(char*));
    int num_rndm_encodings = testCaseIdx - prev;
    int totalStrLen = 0;
    for(i = prev ; i < testCaseIdx ; i++) {
      totalStrLen += uprv_strlen(ucnv_getAvailableName(encodingsTestCases[i]))+1;
    }
    char* names = (char*)uprv_malloc(totalStrLen);
    uprv_memset(names, 0, totalStrLen);

    for(i = prev ; i < testCaseIdx ; i++) {
      uprv_memcpy(names, ucnv_getAvailableName(encodingsTestCases[i]), uprv_strlen(ucnv_getAvailableName(encodingsTestCases[i])));
      encodings[i-prev] = names;
      names+=uprv_strlen(ucnv_getAvailableName(encodingsTestCases[i]))+1;
    }

    status = U_ZERO_ERROR;
    sel = ucnvsel_open((const char**)encodings, testCaseIdx-prev, excluded_sets[excluded_set_id], UCNV_ROUNDTRIP_SET, &status);
    //count how many bytes (Is there a portable function that is more efficient than this?)
    FILE* f1 = fopen("../testdata/ConverterSelectorTestUTF8.txt","rb");
      if(!f1) log_err("Couldn't find ConverterSelectorTestUTF8.txt");
      int counter = 0;
      char c;
      while(fread(&c, 1, 1, f1) > 0) counter++;
    fclose(f1);
    char* text = (char*)uprv_malloc((counter+1));
    f1 = fopen("../testdata/ConverterSelectorTestUTF8.txt","rb");
      if(!f1) log_err("Couldn't find ConverterSelectorTestUTF8.txt");
      fread(text,1, counter,f1);
    fclose(f1);


    for (i = 0 ; i < counter ; i++) {
      if(text[i] == '#')
        text[i] = 0;
    }
    text[counter] = 0;

    int curTestCase=0;
    for (i = 0 ; i < counter ; i++) {
      if(i==0 || text[i-1] == 0) {
        curTestCase++;
        if(curTestCase > 2 && QUICK)
          break;
        //test, both with length, and NULL terminated
        res1 = ucnvsel_selectForUTF8(sel, text+i, -1, &status);
        //make sure result is correct!
        verifyResultUTF8(text+i, (const char**) encodings, num_rndm_encodings, res1, excluded_sets[excluded_set_id], UCNV_ROUNDTRIP_SET);

        res1 = ucnvsel_selectForUTF8(sel, text+i, uprv_strlen(text+i), &status);
        //make sure result is correct!
        verifyResultUTF8(text+i, (const char**)encodings, num_rndm_encodings, res1, excluded_sets[excluded_set_id], UCNV_ROUNDTRIP_SET);
      }
    }
    uprv_free(text);
    uprv_free(encodings[0]);
    uprv_free(encodings);
    ucnvsel_close(sel);
    prev = testCaseIdx + 1;
  }

//////////////////////////////////////////////////////////////////////////

 //try fallback mapping!
 curCase = 0;
 for(excluded_set_id = 0 ; excluded_set_id < 3 ; excluded_set_id++)
  for(testCaseIdx = 0, prev=0, curCase=0 ; testCaseIdx < sizeof(encodingsTestCases) / sizeof(int) ; testCaseIdx++)
  {
    if(encodingsTestCases[testCaseIdx] != -1) continue;

    curCase++;

    if(QUICK && curCase > 2)
      break;

    UErrorCode status = U_ZERO_ERROR;
    UEnumeration* res1;
    int i;
    USet* partial_set = NULL;
    UConverterSelector* sel;
    char** encodings = (char**)uprv_malloc((testCaseIdx - prev) * sizeof(char*));
    int num_rndm_encodings = testCaseIdx - prev;
    int totalStrLen = 0;
    for(i = prev ; i < testCaseIdx ; i++) {
      totalStrLen += uprv_strlen(ucnv_getAvailableName(encodingsTestCases[i]))+1;
    }
    char* names = (char*)uprv_malloc(totalStrLen);
    uprv_memset(names, 0, totalStrLen);

    for(i = prev ; i < testCaseIdx ; i++) {
      uprv_memcpy(names, ucnv_getAvailableName(encodingsTestCases[i]), uprv_strlen(ucnv_getAvailableName(encodingsTestCases[i])));
      encodings[i-prev] = names;
      names+=uprv_strlen(ucnv_getAvailableName(encodingsTestCases[i]))+1;
    }


    status = U_ZERO_ERROR;
    sel = ucnvsel_open((const char**)encodings, testCaseIdx-prev, excluded_sets[excluded_set_id], UCNV_ROUNDTRIP_AND_FALLBACK_SET, &status);
    //count how many bytes (Is there a portable function that is more efficient than this?)
    FILE* f1 = fopen("../testdata/ConverterSelectorTestUTF8.txt","rb");
      if(!f1) log_err("Couldn't find ConverterSelectorTestUTF8.txt"); 
      int counter = 0;
      char c;
      while(fread(&c, 1, 1, f1) > 0) counter++;
    fclose(f1);
    char* text = (char*)uprv_malloc(counter+1);
    f1 = fopen("../testdata/ConverterSelectorTestUTF8.txt","rb");
      if(!f1) log_err("Couldn't find ConverterSelectorTestUTF8.txt");
      fread(text,1, counter,f1);
    fclose(f1);


    for (i = 0 ; i < counter ; i++) {
      if(text[i] == '#')
        text[i] = 0;
    }
    text[counter] = 0;

    int curTestCase=0;
    for (i = 0 ; i < counter ; i++) {
      if(i==0 || text[i-1] == 0) {
        curTestCase++;
        if(curTestCase > 2 && QUICK)
          break;
        //test, both with length, and NULL terminated
        res1 = ucnvsel_selectForUTF8(sel, text+i, -1, &status);
        //make sure result is correct!
        verifyResultUTF8(text+i, (const char**)encodings, num_rndm_encodings, res1,excluded_sets[excluded_set_id],  UCNV_ROUNDTRIP_AND_FALLBACK_SET);

        res1 = ucnvsel_selectForUTF8(sel, text+i, uprv_strlen(text+i), &status);
        //make sure result is correct!
        verifyResultUTF8(text+i, (const char**)encodings, num_rndm_encodings, res1,excluded_sets[excluded_set_id],  UCNV_ROUNDTRIP_AND_FALLBACK_SET);
      }
    }
    uprv_free(text);
    ucnvsel_close(sel);
    prev = testCaseIdx + 1;
  }



  for(i = 0 ; i < 3 ; i++)
    uset_close(excluded_sets[i]);

}

void verifyResultUTF16(const UChar* const s, const char** encodings, int num_encodings, UEnumeration* res, const USet* excludedEncodings, const UConverterUnicodeSet   whichSet) {
  UBool* resultsFromSystem;
  UBool* resultsManually;
  int i;
  resultsFromSystem = (UBool*) uprv_malloc(ucnv_countAvailable() * sizeof(UBool));
  resultsManually = (UBool*) uprv_malloc(ucnv_countAvailable() * sizeof(UBool));
  for(i = 0 ; i < ucnv_countAvailable() ; i++)
    resultsFromSystem[i] = resultsManually[i] = FALSE;



  for(i = 0 ; i < num_encodings ; i++) {
    UErrorCode status = U_ZERO_ERROR;

    //get unicode set for that converter
    USet* unicode_point_set;
    unicode_point_set = uset_open(1, 0);  
    UConverter* test_converter = ucnv_open(encodings[i], &status);
    ucnv_getUnicodeSet(test_converter, unicode_point_set,
                       whichSet, &status);

    int offset = 0;
    int length = u_strlen(s);

    resultsManually[findIndex(encodings[i])] = TRUE;
    UChar32 next = 0;

    while(offset<length) {
    //loop over string
      uint16_t result = 0;
      U16_NEXT(s, offset, length, next)
      if (uset_contains(excludedEncodings, next)==FALSE && uset_contains(unicode_point_set, next)==FALSE) {
        resultsManually[findIndex(encodings[i])] = FALSE;
        break;
      }
    }
    uset_close(unicode_point_set);

    ucnv_close(test_converter);
  }

  //fill the bool for the selector results!
  fillBool(res, resultsFromSystem, ucnv_countAvailable());
  for(i = 0 ; i < ucnv_countAvailable() ; i++) {
    if(resultsManually[i] != resultsFromSystem[i]) {
      log_err("failure in converter selector converter %s had conflicting results manual: %d, system %d\n",ucnv_getAvailableName(i), resultsManually[i], resultsFromSystem[i]);
    }
  }

  uprv_free(resultsFromSystem);
  uprv_free(resultsManually);
}

//does selectForUTF16() work well?
static void TestConversionUTF16()
{
  //test cases are separated by a -1
  //each line is one test case including encodings to check for
  //I'd like to generate this array randomly but not sure if this is an allowed practice in ICU
  int encodingsTestCases[] = {  90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, -1,
                                1, 3, 7, 9, 11, 13, 12, 15, 19, 20, 22, 24, -1,
                                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1,
                                0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, -1,
                                1, 5, 9, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, -1,
                                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 
                                30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
                                60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
                                90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 
                                130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
                                160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
                                190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, -1, 1, -1};



  USet* excluded_sets[3];
  int i;
  excluded_sets[0] = uset_open(1,0);
  for(i = 1 ; i < 3 ; i++)
    excluded_sets[i] = uset_open(i*30, i*30+500);


  int prev, testCaseIdx;
  //try roundtrip mapping
  int excluded_set_id;

 int curCase = 0;
 for(excluded_set_id = 0 ; excluded_set_id < 3 ; excluded_set_id++)
  for(testCaseIdx = 0, prev=0, curCase=0 ; testCaseIdx < sizeof(encodingsTestCases) / sizeof(int) ; testCaseIdx++)
  {

    if(encodingsTestCases[testCaseIdx] != -1) continue;
    curCase++;

    if(QUICK && curCase > 2)
      break;

    UErrorCode status = U_ZERO_ERROR;
    UEnumeration* res1;
    int i;
    USet* partial_set = NULL;
    UConverterSelector* sel;
    char** encodings = (char**) uprv_malloc((testCaseIdx - prev) * sizeof(char*));
    int num_rndm_encodings = testCaseIdx - prev;
    int totalStrLen = 0;
    for(i = prev ; i < testCaseIdx ; i++) {
      totalStrLen += uprv_strlen(ucnv_getAvailableName(encodingsTestCases[i]))+1;
    }
    char* names = (char*)uprv_malloc(totalStrLen);
    uprv_memset(names, 0, totalStrLen);

    for(i = prev ; i < testCaseIdx ; i++) {
      uprv_memcpy(names, ucnv_getAvailableName(encodingsTestCases[i]), uprv_strlen(ucnv_getAvailableName(encodingsTestCases[i])));
      encodings[i-prev] = names;
      names+=uprv_strlen(ucnv_getAvailableName(encodingsTestCases[i]))+1;
    }

    status = U_ZERO_ERROR;
    sel = ucnvsel_open((const char**)encodings, testCaseIdx-prev, excluded_sets[excluded_set_id], UCNV_ROUNDTRIP_SET, &status);
    //count how many bytes (Is there a portable function that is more efficient than this?)
    FILE* f1 = fopen("../testdata/ConverterSelectorTestUTF16.txt","rb");
      if(!f1) log_err("Couldn't find ConverterSelectorTestUTF16.txt");
      int counter = 0;
      UChar c;
      while(fread(&c, sizeof(UChar), 1, f1) > 0) counter++;
    fclose(f1);
    UChar* text = (UChar*)uprv_malloc((counter+1)*sizeof(UChar));
    f1 = fopen("../testdata/ConverterSelectorTestUTF16.txt","rb");
      if(!f1) log_err("Couldn't find ConverterSelectorTestUTF16.txt");
      fread(text,sizeof(UChar), counter,f1);
    fclose(f1);


    for (i = 0 ; i < counter ; i++) {
      if(text[i] == (UChar)'#')
        text[i] = 0;
    }
    text[counter] = 0;

    int curTestCase=0;
    for (i = 0 ; i < counter ; i++) {
      if(i==0 || text[i-1] == 0) {
        curTestCase++;
        if(curTestCase > 2 && QUICK)
          break;
        //test, both with length, and NULL terminated
        res1 = ucnvsel_selectForString(sel, text+i, -1, &status);
        //make sure result is correct!
        verifyResultUTF16(text+i, (const char**) encodings, num_rndm_encodings, res1, excluded_sets[excluded_set_id], UCNV_ROUNDTRIP_SET);

        res1 = ucnvsel_selectForString(sel, text+i, u_strlen(text+i), &status);
        //make sure result is correct!
        verifyResultUTF16(text+i, (const char**)encodings, num_rndm_encodings, res1, excluded_sets[excluded_set_id], UCNV_ROUNDTRIP_SET);
      }
    }
    uprv_free(text);
    uprv_free(encodings[0]);
    uprv_free(encodings);
    ucnvsel_close(sel);
    prev = testCaseIdx + 1;
  }

//////////////////////////////////////////////////////////////////////////

 //try fallback mapping!

 for(excluded_set_id = 0 ; excluded_set_id < 3 ; excluded_set_id++)
  for(testCaseIdx = 0, prev=0, curCase=0 ; testCaseIdx < sizeof(encodingsTestCases) / sizeof(int) ; testCaseIdx++)
  {
    if(encodingsTestCases[testCaseIdx] != -1) continue;

    curCase++;

    if(QUICK && curCase > 2)
      break;

    UErrorCode status = U_ZERO_ERROR;
    UEnumeration* res1;
    int i;
    USet* partial_set = NULL;
    UConverterSelector* sel;
    char** encodings = (char**)uprv_malloc((testCaseIdx - prev) * sizeof(char*));
    int num_rndm_encodings = testCaseIdx - prev;
    int totalStrLen = 0;
    for(i = prev ; i < testCaseIdx ; i++) {
      totalStrLen += uprv_strlen(ucnv_getAvailableName(encodingsTestCases[i]))+1;
    }
    char* names = (char*)uprv_malloc(totalStrLen);
    uprv_memset(names, 0, totalStrLen);

    for(i = prev ; i < testCaseIdx ; i++) {
      uprv_memcpy(names, ucnv_getAvailableName(encodingsTestCases[i]), uprv_strlen(ucnv_getAvailableName(encodingsTestCases[i])));
      encodings[i-prev] = names;
      names+=uprv_strlen(ucnv_getAvailableName(encodingsTestCases[i]))+1;
    }


    //first time
    status = U_ZERO_ERROR;
    sel = ucnvsel_open((const char**)encodings, testCaseIdx-prev, excluded_sets[excluded_set_id], UCNV_ROUNDTRIP_AND_FALLBACK_SET, &status);
    //count how many bytes (Is there a portable function that is more efficient than this?)
    FILE* f1 = fopen("../testdata/ConverterSelectorTestUTF16.txt","rb");
      if(!f1) log_err("Couldn't find ConverterSelectorTestUTF16.txt"); 
      int counter = 0;
      UChar c;
      while(fread(&c, sizeof(UChar), 1, f1) > 0) counter++;
    fclose(f1);
    UChar* text = (UChar*)uprv_malloc((counter+1)*sizeof(UChar));
    f1 = fopen("../testdata/ConverterSelectorTestUTF16.txt","rb");
      if(!f1) log_err("Couldn't find ConverterSelectorTestUTF16.txt");
      fread(text,sizeof(UChar), counter,f1);
    fclose(f1);


    for (i = 0 ; i < counter ; i++) {
      if(text[i] == (UChar)'#')
        text[i] = 0;
    }
    text[counter] = 0;

    int curTestCase=0;
    for (i = 0 ; i < counter ; i++) {
      if(i==0 || text[i-1] == 0) {
        curTestCase++;
        if(curTestCase > 2 && QUICK)
          break;
        //test, both with length, and NULL terminated
        res1 = ucnvsel_selectForString(sel, text+i, -1, &status);
        //make sure result is correct!
        verifyResultUTF16(text+i, (const char**)encodings, num_rndm_encodings, res1,excluded_sets[excluded_set_id],  UCNV_ROUNDTRIP_AND_FALLBACK_SET);

        res1 = ucnvsel_selectForString(sel, text+i, u_strlen(text+i), &status);
        //make sure result is correct!
        verifyResultUTF16(text+i, (const char**)encodings, num_rndm_encodings, res1,excluded_sets[excluded_set_id],  UCNV_ROUNDTRIP_AND_FALLBACK_SET);
      }
    }
    uprv_free(text);
    ucnvsel_close(sel);
    prev = testCaseIdx + 1;
  }
  for(i = 0 ; i < 3 ; i++)
    uset_close(excluded_sets[i]);
}





//does selectForUTF16() work well?
static void TestSerializationAndUnserialization()
{
  //test cases are separated by a -1
  //each line is one test case including encodings to check for
  //I'd like to generate this array randomly but not sure if this is an allowed practice in ICU
  int encodingsTestCases[] = {  90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, -1,
                                1, 3, 7, 9, 11, 13, 12, 15, 19, 20, 22, 24, -1,
                                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1,
                                0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, -1,
                                1, 5, 9, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, -1,
                                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 
                                30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
                                60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
                                90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 
                                130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
                                160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
                                190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, -1, 1, -1};



  USet* excluded_sets[3];
  int i;
  excluded_sets[0] = uset_open(1,0);
  for(i = 1 ; i < 3 ; i++)
    excluded_sets[i] = uset_open(i*30, i*30+500);


  int prev, testCaseIdx;
  //try roundtrip mapping
  int excluded_set_id;
 int curCase = 0;
 for(excluded_set_id = 0 ; excluded_set_id < 3 ; excluded_set_id++) 
  for(testCaseIdx = 0, prev=0, curCase =0 ; testCaseIdx < sizeof(encodingsTestCases) / sizeof(int) ; testCaseIdx++)
  {
    if(encodingsTestCases[testCaseIdx] != -1) continue;

    curCase++;

    if(QUICK && curCase > 2)
      break;

    UErrorCode status = U_ZERO_ERROR;
    UEnumeration* res1;
    int i;
    USet* partial_set = NULL;
    UConverterSelector* sel;
    char** encodings = (char**) uprv_malloc((testCaseIdx - prev) * sizeof(char*));
    int num_rndm_encodings = testCaseIdx - prev;
    int totalStrLen = 0;
    for(i = prev ; i < testCaseIdx ; i++) {
      totalStrLen += uprv_strlen(ucnv_getAvailableName(encodingsTestCases[i]))+1;
    }
    char* names = (char*)uprv_malloc(totalStrLen);
    uprv_memset(names, 0, totalStrLen);

    for(i = prev ; i < testCaseIdx ; i++) {
      uprv_memcpy(names, ucnv_getAvailableName(encodingsTestCases[i]), uprv_strlen(ucnv_getAvailableName(encodingsTestCases[i])));
      encodings[i-prev] = names;
      names+=uprv_strlen(ucnv_getAvailableName(encodingsTestCases[i]))+1;
    }

    //first time
    status = U_ZERO_ERROR;
    sel = ucnvsel_open((const char**)encodings, testCaseIdx-prev, excluded_sets[excluded_set_id], UCNV_ROUNDTRIP_SET, &status);

    char *buffer = NULL;
    unsigned int ser_len = ucnvsel_serialize(sel, NULL, 0, &status);
    status = U_ZERO_ERROR;
    buffer = uprv_malloc(ser_len);
    ucnvsel_serialize(sel, buffer, ser_len, &status);

    ucnvsel_close(sel);
    sel = ucnvsel_unserialize( buffer,  ser_len,&status);

    //count how many bytes (Is there a portable function that is more efficient than this?)
    FILE* f1 = fopen("../testdata/ConverterSelectorTestUTF16.txt","rb");
      if(!f1) log_err("Couldn't find ConverterSelectorTestUTF16.txt");
      int counter = 0;
      UChar c;
      while(fread(&c, sizeof(UChar), 1, f1) > 0) counter++;
    fclose(f1);
    UChar* text = (UChar*)uprv_malloc((counter+1)*sizeof(UChar));
    f1 = fopen("../testdata/ConverterSelectorTestUTF16.txt","rb");
      if(!f1) log_err("Couldn't find ConverterSelectorTestUTF16.txt");
      fread(text,sizeof(UChar), counter,f1);
    fclose(f1);

    for (i = 0 ; i < counter ; i++) {
      if(text[i] == (UChar)'#')
        text[i] = 0;
    }
    text[counter] = 0;

    int curTestCase=0;
    for (i = 0 ; i < counter ; i++) {
      if(i==0 || text[i-1] == 0) {
        curTestCase++;
        if(curTestCase > 2 && QUICK)
          break;
        //test, both with length, and NULL terminated
        res1 = ucnvsel_selectForString(sel, text+i, -1, &status);
        //make sure result is correct!
        verifyResultUTF16(text+i, (const char**) encodings, num_rndm_encodings, res1, excluded_sets[excluded_set_id], UCNV_ROUNDTRIP_SET);

        res1 = ucnvsel_selectForString(sel, text+i, u_strlen(text+i), &status);
        //make sure result is correct!
        verifyResultUTF16(text+i, (const char**)encodings, num_rndm_encodings, res1, excluded_sets[excluded_set_id], UCNV_ROUNDTRIP_SET);
      }
    }
    uprv_free(text);
    uprv_free(encodings[0]);
    uprv_free(encodings);
    ucnvsel_close(sel);
    prev = testCaseIdx + 1;
    uprv_free(buffer);
  }

//////////////////////////////////////////////////////////////////////////

 //try fallback mapping!
 for(excluded_set_id = 0 ; excluded_set_id < 3 ; excluded_set_id++) 
  for(testCaseIdx = 0, prev=0, curCase=0 ; testCaseIdx < sizeof(encodingsTestCases) / sizeof(int) ; testCaseIdx++)
  {
    if(encodingsTestCases[testCaseIdx] != -1) continue;

    curCase++;

    if(QUICK && curCase > 2)
      break;

    UErrorCode status = U_ZERO_ERROR;
    UEnumeration* res1;
    int i;
    USet* partial_set = NULL;
    UConverterSelector* sel;
    char** encodings = (char**)uprv_malloc((testCaseIdx - prev) * sizeof(char*));
    int num_rndm_encodings = testCaseIdx - prev;
    int totalStrLen = 0;
    for(i = prev ; i < testCaseIdx ; i++) {
      totalStrLen += uprv_strlen(ucnv_getAvailableName(encodingsTestCases[i]))+1;
    }
    char* names = (char*)uprv_malloc(totalStrLen);
    uprv_memset(names, 0, totalStrLen);

    for(i = prev ; i < testCaseIdx ; i++) {
      uprv_memcpy(names, ucnv_getAvailableName(encodingsTestCases[i]), uprv_strlen(ucnv_getAvailableName(encodingsTestCases[i])));
      encodings[i-prev] = names;
      names+=uprv_strlen(ucnv_getAvailableName(encodingsTestCases[i]))+1;
    }

    //first time
    status = U_ZERO_ERROR;
    sel = ucnvsel_open((const char**)encodings, testCaseIdx-prev, excluded_sets[excluded_set_id], UCNV_ROUNDTRIP_AND_FALLBACK_SET, &status);

    char *buffer = NULL;
    unsigned int ser_len = ucnvsel_serialize(sel, NULL, 0, &status);
    buffer = uprv_malloc(ser_len);
    status = U_ZERO_ERROR;
    ucnvsel_serialize(sel, buffer, ser_len, &status);

    ucnvsel_close(sel);
    sel = ucnvsel_unserialize( buffer,  ser_len,&status);

    //count how many bytes (Is there a portable function that is more efficient than this?)
    FILE* f1 = fopen("../testdata/ConverterSelectorTestUTF16.txt","rb");
      if(!f1) log_err("Couldn't find ConverterSelectorTestUTF16.txt"); 
      int counter = 0;
      char c;
      while(fread(&c, 2, 1, f1) > 0) counter++;
    fclose(f1);
    UChar* text = (UChar*)uprv_malloc((counter+1)*sizeof(UChar));
    f1 = fopen("../testdata/ConverterSelectorTestUTF16.txt","rb");
      if(!f1) log_err("Couldn't find ConverterSelectorTestUTF16.txt");
      fread(text,2, counter,f1);
    fclose(f1);

    for (i = 0 ; i < counter ; i++) {
      if(text[i] == (UChar)'#')
        text[i] = 0;
    }
    text[counter] = 0;

    int curTestCase=0;
    for (i = 0 ; i < counter ; i++) {
      if(i==0 || text[i-1] == 0) {
        curTestCase++;
        if(curTestCase > 2 && QUICK)
          break;
        //test, both with length, and NULL terminated
        res1 = ucnvsel_selectForString(sel, text+i, -1, &status);
        //make sure result is correct!
        verifyResultUTF16(text+i, (const char**)encodings, num_rndm_encodings, res1,excluded_sets[excluded_set_id],  UCNV_ROUNDTRIP_AND_FALLBACK_SET);

        res1 = ucnvsel_selectForString(sel, text+i, u_strlen(text+i), &status);
        //make sure result is correct!
        verifyResultUTF16(text+i, (const char**)encodings, num_rndm_encodings, res1,excluded_sets[excluded_set_id],  UCNV_ROUNDTRIP_AND_FALLBACK_SET);
      }
    }
    uprv_free(text);
    ucnvsel_close(sel);
    prev = testCaseIdx + 1;
    uprv_free(buffer);
  }

  for(i = 0 ; i < 3 ; i++)
    uset_close(excluded_sets[i]);

}