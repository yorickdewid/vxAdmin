#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sha1.h"
#include "common.h"

static int _seed = 67482363;

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

char *genpasswd(size_t length) {
	static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!$^&*";
	char *crand = NULL;

	srand(time(NULL) * length + ++_seed);

	if (length) {
		crand = malloc(sizeof(char) * (length + 1));

		if (crand) {
			for (unsigned int n = 0; n < length; ++n) {
				int key = rand() % (int)(sizeof(charset) -1);
				crand[n] = charset[key];
			}

			crand[length] = '\0';
		}
	}

	return crand;
}
