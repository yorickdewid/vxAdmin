#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include "sha1.h"
#include "json.h"
#include "builder.h"

json_value *parse_config(char *filename);
void *get_random_data(size_t len);
char *genkey();

#endif // COMMON_H_INCLUDED
