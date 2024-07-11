obj-m += kernelgpudriver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules CFLAGS="-std=c99"
	
	sudo insmod kernelgpudriver.ko;

	rm -f /dev/gpu123
	
run: game
	sudo ./game

game: game.o instructions.o gpulib.o
	gcc -o game game.o gpulib.o instructions.o -pthread
	rm -f gpulib.o instructions.o

program: botao.o instructions.o gpulib.o
	gcc -o botao botao.o gpulib.o instructions.o -pthread

instructions.o: lib/instructions.c
	gcc -c lib/instructions.c -o instructions.o

gpulib.o: lib/gpulib.c
	gcc -c lib/gpulib.c

game.o: game.c
	gcc -c game.c -o game.o

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f gpulib.o instructions.o game
	rm -f kernelgpudriver.mod.o kernelgpudriver.mod kernelgpudriver.ko.cmd kernelgpudriver.o kernelgpudriver.mod.o.cmd kernelgpudriver.o.cmd
