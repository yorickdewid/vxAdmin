#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#include "common.h"

void print_usage() {
	printf("Usage: vxadmin [OPTION]\n\n");
	printf(" -c, --config       Specify config\n");
	printf("     --gen-key      Generate new secret key\n");
	printf(" -v  --verify       Verify config\n");
	printf(" -h, --help         This help\n");
}

int main(int argc, char *argv[]) {
	int opt_genkey = 0, opt_config = 0, opt_verify = 0;
	char configname[1024];
	json_value *config = NULL;

	static struct option long_options[] = {
		{"config",    required_argument, 0,  'c' },
		{"gen-key",   no_argument,       0,  'g' },
		{"verify",    no_argument,       0,  'v' },
		{"help",      no_argument,       0,  'h' },
		{0,           0,                 0,  0   }
	};

	int opt, long_index = 0;
	while ((opt = getopt_long(argc, argv,"c:gh", long_options, &long_index)) != -1) {
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

		for (int x = 0; x < config->u.object.length; x++) {
			if (!strcmp(config->u.object.values[x].name, "secret")) {
				free(config->u.object.values[x].value->u.string.ptr);
				config->u.object.values[x].value->u.string.ptr = key;
				config->u.object.values[x].value->u.string.length = 40;
			}
		}

		json_serialize_opts opts;
		opts.mode = json_serialize_mode_multiline;
		opts.opts = json_serialize_opt_use_tabs;
		opts.indent_size = 1;

		char *buf = malloc(json_measure_ex(config, opts));
		json_serialize_ex(buf, config, opts);

		unlink(configname);
		FILE *fp = fopen(configname, "ab");
		if (fp) {
			fputs(buf, fp);
			fclose(fp);
		}

		free(buf);
	}

	if (opt_verify) {
		//
	}

	json_value_free(config);
	return 0;
}
