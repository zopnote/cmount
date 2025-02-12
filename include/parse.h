#pragma once

#include <stdio.h>

#ifdef _WIN64
    #ifdef PARSE_EXPORTS
        #define EXTERN __declspec(dllexport)
    #else
        #define EXTERN __declspec(dllimport)
    #endif
#else
    #define EXTERN __attribute__((visibility("default")))
#endif


/**
 * @brief Available configuration types of the parse collection.
 */
enum {
    yaml,
    json
} typedef ParseFormat;

/**
 * @brief The type the buffer of an entry has.
 *
 * Maps are further ParseEntry arrays which must be
 * already defined and set as buffer of an entry to store
 * the recursive values in it.
 *
 * Lists are an array of unparsed strings of the parsed document.
 * Their buffer should not be set at first definition.
 */
enum {
    map,
    integer,
    string,
    floating,
    list
} typedef ParseType;


/**
 * @brief Representation of an entry.
 *
 * Define a tree by create an array of entries to load
 * configuration values from a file or write them to a string.
 *
 * All value buffers except the map should not be set.
 * Maps are further ParseEntry arrays. The size value represent
 * the length of an array in case the buffer is an array
 * or the actual size of the value behind the buffer.
 *
 * The parser scans for the key and decide based on the ParseType
 * what the buffer should be and set it. Buffers can be null even after
 * the parse process, if the value was not found.
 */
struct {
    char* key;
    ParseType type;
    void* buffer;
    size_t size;
} typedef ParseEntry;

/**
 * @brief Initialize the yaml parser with the input string and continues with the scan.
 *
 * The entries array will be filled with the found values.
 */
EXTERN void parse_resolveYamlString(
    const char* const string,
    ParseEntry* const entries,
    const size_t entriesLength,
    const bool verbose
);

/**
 * @brief Initialize the json parser with the input string and continues with the scan.
 *
 * The entries array will be filled with the found values.
 */
EXTERN void parse_resolveJsonString(
    const char* const string,
    ParseEntry* const entries,
    const size_t entriesLength
);

/**
 * @brief Writes the entries to the string buffer with the desired configuration format.
 */
EXTERN void parse_emitString(
    char* stringBuffer,
    const ParseFormat format,
    const ParseEntry* const entries,
    const size_t entriesLength
);