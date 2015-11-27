all:
	$(CC) json.c config.c main.c -o bin/vxadmin -std=c11 -lm

clean:
	$(RM) bin/vxadmin