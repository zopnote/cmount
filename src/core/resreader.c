
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "yaml.h"
#include "cjson/cJson.h"
#include "klib/kstring.h"
#include "klib/klist.h"

// Define a free function for klist
#define klib_list_free(x) free(x)
KLIST_INIT(value_list, kstring_t*, klib_list_free)

// Parse a path into tokens separated by "."
klist_t(value_list) *parse_path(const char *path) {
    klist_t(value_list) *list = kl_init(value_list);
    char *path_copy = strdup(path); // Create a modifiable copy of the path
    char *token = strtok(path_copy, ".");

    while (token) {
        kstring_t *kstr = malloc(sizeof(kstring_t));
        kstr->s = NULL;
        kstr->l = 0;
        kstr->m = 0;
        ksprintf(kstr, "%s", token);
        *kl_pushp(value_list, list) = kstr;
        token = strtok(NULL, ".");
    }

    free(path_copy);
    return list;
}

// Retrieve a value from a JSON object based on a dot-separated path
void *getJsonValue(const char *json_str, const char *path) {
    cJSON *root = cJSON_Parse(json_str);
    if (!root) {
        fprintf(stderr, "Error parsing JSON.\n");
        return NULL;
    }

    klist_t(value_list) *tokens = parse_path(path);
    cJSON *current = root;

    // Traverse the JSON object based on the parsed tokens
    while (current && kl_begin(tokens)) {
        kstring_t *token = *kl_val(kl_begin(tokens));
        current = cJSON_GetObjectItemCaseSensitive(current, token->s);
        kl_shift(value_list, tokens, free); // Remove the processed token
    }

    void *result = NULL;

    // Determine the type of the final JSON value and allocate memory accordingly
    if (current) {
        if (cJSON_IsNumber(current)) {
            if (cJSON_IsDouble(current)) {
                float *fval = malloc(sizeof(float));
                *fval = (float)cJSON_GetNumberValue(current);
                result = fval;
            } else {
                int *ival = malloc(sizeof(int));
                *ival = (int)cJSON_GetNumberValue(current);
                result = ival;
            }
        } else if (cJSON_IsString(current)) {
            kstring_t *kstr = malloc(sizeof(kstring_t));
            kstr->s = NULL;
            kstr->l = 0;
            kstr->m = 0;
            ksprintf(kstr, "%s", cJSON_GetStringValue(current));
            result = kstr;
        } else if (cJSON_IsBool(current)) {
            int *bval = malloc(sizeof(int));
            *bval = cJSON_IsTrue(current);
            result = bval;
        } else if (cJSON_IsArray(current)) {
            klist_t(value_list) *list = kl_init(value_list);
            cJSON *item;
            cJSON_ArrayForEach(item, current) {
                kstring_t *kstr = malloc(sizeof(kstring_t));
                kstr->s = NULL;
                kstr->l = 0;
                kstr->m = 0;
                ksprintf(kstr, "%s", cJSON_Print(item));
                *kl_pushp(value_list, list) = kstr;
            }
            result = list;
        }
    }

    cJSON_Delete(root);
    return result;
}

// Free allocated memory for the retrieved result based on its type
static void free_result(void *result, const char *type) {
    if (strcmp(type, "int") == 0 || strcmp(type, "bool") == 0) {
        free(result);
    } else if (strcmp(type, "float") == 0) {
        free(result);
    } else if (strcmp(type, "string") == 0) {
        kstring_t *str = (kstring_t *)result;
        free(str->s);
        free(str);
    } else if (strcmp(type, "list") == 0) {
        klist_t(value_list) *list = (klist_t(value_list) *)result;
        kstring_t *kstr;
        while ((kstr = *kl_shift(value_list, list, free)) != NULL) {
            free(kstr->s);
            free(kstr);
        }
        kl_destroy(value_list, list);
    }
}
