#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "json.h"
#include "builder.h"

json_value *parse_config(char *filename) {
	struct stat filestatus;
	FILE *fp = NULL;

	if (stat(filename, &filestatus) != 0) {
		fprintf(stderr, "File %s not found\n", filename);
		return NULL;
	}

	size_t file_size = filestatus.st_size;
	char *file_contents = (char *)malloc(filestatus.st_size);
	if (!file_contents) {
		fprintf(stderr, "Memory error: unable to allocate %d bytes\n", file_size);
		return NULL;
	}

	fp = fopen(filename, "rt");
	if (!fp) {
		fprintf(stderr, "Unable to open config %s\n", filename);
		fclose(fp);
		free(file_contents);
		return NULL;
	}
	if (fread(file_contents, file_size, 1, fp) < 0 ) {
		fprintf(stderr, "Unable t read content of config %s\n", filename);
		fclose(fp);
		free(file_contents);
		return NULL;
	}
	fclose(fp);

	json_settings settings = {};
	settings.value_extra = json_builder_extra;

	char error[128];
	json_value *config = json_parse_ex(&settings, (json_char *)file_contents, file_size, error);
	if (!config) {
		fprintf(stderr, "Unable to parse config\n");
		free(file_contents);
		return NULL;
	}
	free(file_contents);

	if (config->type != json_object) {
		fprintf(stderr, "Unable to parse config\n");
		json_value_free(config);
		return NULL;
	}

	return config;
}

void set_key_config(char *configname, json_value *config, char *key) {
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