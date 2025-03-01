DIR 	?= PATH_TO_TARGET_DIR
ELF 	= main.elf

build:
	$(MAKE) -BC $(DIR) $(ELF)

STDIO_CONFIG := -nographic -serial mon:stdio -serial pty
run: build
	qemu-system-arm -M mps2-an505 $(STDIO_CONFIG) -kernel $(DIR)/$(ELF)

debug: build
	qemu-system-arm -M mps2-an505 $(STDIO_CONFIG) -kernel $(DIR)/$(ELF) -S -gdb tcp::1234

gdb: build
	gdb-multiarch -ex "set architecture arm" -ex "dir $(DIR)" -q $(DIR)/$(ELF) -ex "target remote tcp::1234"

format-check: DIR = .
format-check:
	find $(DIR) -type f -regextype egrep -regex ".*\.(c|h)" -exec bash -c "clang-format -style=file {} | diff --color --unified=0 {} -" \;

format-apply: DIR = .
format-apply:
	find $(DIR) -type f -regextype egrep -regex ".*\.(c|h)" -exec clang-format -i -style=file {} \;

clean: DIR = .
clean:
	find $(DIR) -type f -regextype egrep -regex ".*\.(o|elf|map)" -exec $(RM) -vr {} \;
