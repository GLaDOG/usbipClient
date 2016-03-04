all:
	gcc -o uc *.c -std=c99 -lsysfs -lpthread
