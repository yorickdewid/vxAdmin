#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <getopt.h>

#include "common.h"

#define RESTRICT_ROOT	1
#define ADMIN_PASSWD_LENGTH	16

void print_usage() {
	printf("Usage: vxadmin [OPTION]\n\n");
	printf(" -c, --config       Specify config\n");
	printf(" -g, --gen-key      Generate new secret key\n");
	printf(" -v, --verify       Verify config\n");
	printf(" -a, --admin-reset  Reset admin password\n");
	printf(" -h, --help         This help\n");
}

int main(int argc, char *argv[]) {
	int opt_genkey = 0, opt_config = 0, opt_verify = 0, opt_admin_reset = 0;
	char configname[1024];
	json_value *config = NULL;

#ifdef RESTRICT_ROOT
	uid_t uid = getuid(), euid = geteuid();
	if (uid != 0 || uid != euid) {
		fprintf(stderr, "User must have root privileges\n");
		return 1;
	}
#endif

	static struct option long_options[] = {
		{"config",       required_argument, 0,  'c' },
		{"gen-key",      no_argument,       0,  'g' },
		{"verify",       no_argument,       0,  'v' },
		{"admin-reset",  no_argument,       0,  'a' },
		{"help",         no_argument,       0,  'h' },
		{0,              0,                 0,  0   }
	};

	int opt, long_index = 0;
	while ((opt = getopt_long(argc, argv,"c:gvha", long_options, &long_index)) != -1) {
		switch (opt) {
			case 'c' :
				strncpy(configname, optarg, 1024);
				opt_config = 1;
				break;
			case 'g' :
				opt_genkey = 1;
				break;
			case 'v':
				opt_verify = 1;
				break;
			case 'a':
				opt_admin_reset = 1;
				break;
			default:
				print_usage(); 
				return 1;
		}
	}

	if (!opt_config) {
		fprintf(stderr, "No config specified, for help see --help\n");
		return 1;
	}

	config = parse_config(configname);
	if (!config)
		return 1;


	if (opt_genkey) {
		char *key = genkey();

		set_key_config(configname, config, key);
		puts("Secret set");
	}

	if (opt_verify) {
		if (verify_config(config)) {
			puts("Config OK");
		} else {
			puts("Config NOT OK");
		}
	}

	if (opt_admin_reset) {
		char *passwd = genpasswd(ADMIN_PASSWD_LENGTH);

		if (reset_admin_password(config, passwd)) {
			printf("Admin password: %s\n", passwd);
		} else {
			puts("Admin reset failed");
		}

		free(passwd);
	}

	json_value_free(config);
	return 0;
}
