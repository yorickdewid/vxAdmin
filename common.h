#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include "sha1.h"
#include "json.h"
#include "builder.h"

json_value *parse_config(char *filename);
void set_key_config(char *configname, json_value *config, char *key);
void *get_random_data(size_t len);
char *genkey();

#endif // COMMON_H_INCLUDED
