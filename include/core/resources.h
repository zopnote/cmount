#pragma once

#include <stdlib.h>

#include "klib/kstring.h"
#include "klib/klist.h"

static void stringListFree(void* str) {
    kstring_t* kstr = (kstring_t *)str;
    ks_release(kstr);
    free(kstr->s);
    free(kstr);
}

kstring_t resources_getString(const kstring_t path, const char* executablePath);

float resources_getFloat(const kstring_t path, const char* executablePath);

int resources_getInt(const kstring_t path, const char* executablePath);

KLIST_INIT(stringList, kstring_t, stringListFree)
klist_t(stringList)* resources_getStringList(const kstring_t path, const char* executablePath);

KLIST_INIT(intList, int, free)
klist_t(intList)* resources_getIntList(const kstring_t path, const char* executablePath);

KLIST_INIT(floatList, float, free)
klist_t(floatList)* resources_getFloatList(const kstring_t path, const char* executablePath);