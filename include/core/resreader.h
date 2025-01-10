#pragma once
/**
 * @title Resource manager
 *
 * @brief Get different data types out of a ´.yaml´ or ´.json´.
 */

#include <stdlib.h>
#include "klib/kstring.h"
#include "klib/klist.h"


inline static void stringListFree(void* str) {
    kstring_t* kstr = (kstring_t *)str;
    ks_release(kstr);
    free(kstr->s);
    free(kstr);
}

/**
 * @brief Gets the string located in file described by the path.
 *
 *
 * @return Returns a ´kstring_t´ type.
 */
kstring_t resread_getString(const kstring_t path);

/**
 * @brief Gets the float located in file described by the path.
 *
 * @return Returns a ´float´ type.
 */
float resread_getFloat(const kstring_t path);

/**
 * @brief Gets the int located in file described by the path.
 *
 * @return Returns a ´int´ type.
 */
int resread_getInt(const kstring_t path);

/**
 * @meta[makro]
 *
 * @brief Returns a list with all found strings at location and file described by the path.
 *
 * @return Returns a ´klist_t(kstring_t)´ type.
 */
KLIST_INIT(stringList, kstring_t, stringListFree)
klist_t(stringList)* resread_getStringList(const kstring_t path);

/**
 * @meta[makro]
 *
 * @brief Returns a list with all found ints at location and file described by the path.
 *
 * @return Returns a ´klist_t(int)´ type.
 */
KLIST_INIT(intList, int, free)
klist_t(intList)* resread_getIntList(const kstring_t path);

/**
 * @meta[makro]
 *
 * @brief Returns a list with all found floats at location and file described by the path.
 *
 * @return Returns a ´klist_t(float)´ type.
 */
KLIST_INIT(floatList, float, free)
klist_t(floatList)* resread_getFloatList(const kstring_t path);