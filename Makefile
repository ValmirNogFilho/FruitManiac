obj-m += kernelgpudriver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules CFLAGS="-std=c99"
	
	sudo insmod kernelgpudriver.ko; \

	rm -f /dev/gpu123
	
run: gputest
	sudo ./gputest

program: gputest.o instructions.o gpulib.o
	gcc -o gputest gputest.o gpulib.o instructions.o
	rm -f gpulib.o instructions.o

instructions.o: lib/instructions.c
	gcc -c lib/instructions.c -o instructions.o

gpulib.o: lib/gpulib.c
	gcc -c lib/gpulib.c

gputest.o: gputest.c
	gcc -c gputest.c -o gputest.o

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f gpulib.o instructions.o gputest
	rm -f kernelgpudriver.mod.o kernelgpudriver.mod kernelgpudriver.ko.cmd kernelgpudriver.o kernelgpudriver.mod.o.cmd kernelgpudriver.o.cmd
