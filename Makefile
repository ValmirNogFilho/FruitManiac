# Variáveis
obj-m += kernelgpudriver.o
MODULE_NAME := gpu123
KERNEL_DIR := /lib/modules/$(shell uname -r)/build
SCRIPT := $(CURDIR)/create_device.sh

# Regra padrão
all: load_module create_device

load_module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules CFLAGS="-std=c99"
	
	sudo insmod kernelgpudriver.ko; 

	rm -f /dev/gpu123
	
create_device:
	@echo "Criando dispositivo /dev/$(MODULE_NAME)..."
	chmod +x create_device.sh
	sudo $(SCRIPT) $(MODULE_NAME)
	
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
