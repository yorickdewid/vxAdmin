#include <stdio.h>
#include <stdlib.h>

#include "sha1.h"
#include "common.h"

char *genkey() {
	sha1nfo s;

	char *crand = (char *)get_random_data(20);

	sha1_init(&s);
	sha1_write(&s, crand, 20);
	uint8_t *rs = sha1_result(&s);

	for (int i = 0; i < 20; ++i) {
		printf("%02x", rs[i]);
	}
	printf("\n");

	free(crand);
}