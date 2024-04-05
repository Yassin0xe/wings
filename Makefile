build:wings.c
	gcc wings.c -fno-stack-protector -z execstack -o wings
install:wings.c
	gcc wings.c -fno-stack-protector -z execstack -o wings
	mv wings /usr/bin/wings
