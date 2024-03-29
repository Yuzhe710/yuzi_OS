
GPPPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings #for the 32 bit mode, compile with 32 bit gcc
ASPARAMS = --32 
LDPARAMS = -melf_i386

objects = obj/loader.o \
		  obj/gdt.o \
		  obj/drivers/driver.o \
		  obj/hardwarecommunication/port.o \
		  obj/hardwarecommunication/interruptstubs.o \
		  obj/hardwarecommunication/interrupts.o \
		  obj/multitasking.o \
		  obj/hardwarecommunication/pci.o \
		  obj/drivers/keyboard.o \
		  obj/drivers/mouse.o \
		  obj/drivers/vga.o \
		  obj/gui/widget.o \
		  obj/gui/window.o \
		  obj/gui/desktop.o \
		  obj/kernel.o

# the output is the target file, which is %.o, $< means the first prerequisite
# which is %.cpp
obj/%.o: src/%.cpp
		mkdir -p $(@D)
		g++ $(GPPPARAMS) -o $@ -c $<

obj/%.o: src/%.s
		mkdir -p $(@D)
		as $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
		ld $(LDPARAMS) -T $< -o $@ $(objects)

install: mykernel.bin
		sudo cp $< /boot/mykernel.bin

mykernel.iso: mykernel.bin
		mkdir iso
		mkdir iso/boot
		mkdir iso/boot/grub
		cp $< iso/boot/
		echo 'set timeout=0' > iso/boot/grub/grub.cfg
		echo 'set default=0' >> iso/boot/grub/grub.cfg
		echo '' >> iso/boot/grub/grub.cfg
		echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
		echo '  multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
		echo '  boot' >> iso/boot/grub/grub.cfg
		echo '}' >> iso/boot/grub/grub.cfg
		grub-mkrescue --output=$@ iso
		rm -rf iso

run: mykernel.iso
		# (killall VBoxManage && sleep 1) || true 
		VBoxManage startvm "My Operating System"

.PHONY:clean
clean:
		rm -rf obj mykernel.bin mykernel.iso
