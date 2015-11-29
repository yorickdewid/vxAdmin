all:
	$(CC) json.c builder.c sha1.c random.c config.c genkey.c main.c -o bin/vxadmin -std=c11 -lm

clean:
	$(RM) bin/vxadmin