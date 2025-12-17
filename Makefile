all:
	gcc $(shell pwd)/main.c -o $(shell pwd)/build/openrepair.elf
clean:
	rm -rf $(shell pwd)/build/openrepair.elf