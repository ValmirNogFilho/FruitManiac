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

game: game.o instructions.o gpulib.o displayprinter.o drawer.o inputbehaviour.o
	gcc -o game game.o lib/gpulib.o lib/instructions.o gameFolder/displayprinter.o gameFolder/drawer.o gameFolder/inputbehaviour.o -pthread
	rm -f gpulib.o instructions.o

instructions.o: lib/instructions.c
	gcc -c lib/instructions.c -o lib/instructions.o

gpulib.o: lib/gpulib.c
	gcc -c lib/gpulib.c -o lib/gpulib.o

displayprinter.o: gameFolder/displayprinter.c
	gcc -c gameFolder/displayprinter.c -o gameFolder/displayprinter.o

drawer.o: gameFolder/drawer.c
	gcc -c gameFolder/drawer.c -o gameFolder/drawer.o

inputbehaviour.o: gameFolder/inputbehaviour.c
	gcc -c gameFolder/inputbehaviour.c -o gameFolder/inputbehaviour.o

game.o: game.c
	gcc -c game.c -o game.o

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f game gpulib.o instructions.o gameFolder/displayprinter.o gameFolder/drawer.o gameFolder/inputbehaviour.o
	rm -f kernelgpudriver.mod.o kernelgpudriver.mod kernelgpudriver.ko.cmd kernelgpudriver.o kernelgpudriver.mod.o.cmd kernelgpudriver.o.cmd
