#include <stdio.h>
#include <stdlib.h>

#include "sha1.h"
#include "common.h"

char *genkey() {
	sha1nfo s;

	char *crand = (char *)get_random_data(20);
	char *key = (char *)malloc((HASH_LENGTH*2)+1);

	sha1_init(&s);
	sha1_write(&s, crand, 20);
	uint8_t *rs = sha1_result(&s);

	for (int i = 0; i < 20; ++i) {
		sprintf(key+(i*2), "%02x", rs[i]);
	}

	free(crand);
	return key;
}