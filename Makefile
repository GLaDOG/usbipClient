src = $(wildcard ./src/*.c)
obj = $(patsubst ./src/%.c, ./obj/%.o, $(src))
usbip_src = $(wildcard ./usbip_src/*.c)
usbip_obj = $(patsubst ./usbip_src/%.c, ./obj/%.o, $(usbip_src))

target = server
inc_path = ./include/
usbip_inc_path = ./usbip_include/

CC = gcc
all : $(target)

server : $(obj) $(usbip_obj)
	$(CC) $^ -lsysfs -o $@

./obj/%.o : ./src/%.c
	$(CC) -c $< -o $@ -I $(inc_path) -I $(usbip_inc_path)

./obj/%.o : ./usbip_src/%.c
	$(CC) -c $< -o $@ -I $(usbip_inc_path)


clean:
	@echo cleaning ...
	rm -rf $(obj) $(usbip_obj) $(target)

.PHONY : clean
