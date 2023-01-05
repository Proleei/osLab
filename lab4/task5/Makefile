obj-m += task5.o
all:
	make -C /home/os/Desktop/linux-6.0.6 M=$(PWD) modules
	gcc tt.c -o tt.out
clean:
	make -C /home/os/Desktop/linux-6.0.6 M=$(PWD) clean
	rm tt.out
test:
	gcc tt.c -o tt.out
