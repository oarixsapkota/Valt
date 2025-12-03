C_SRC := $(wildcard src/*.c) $(wildcard src/*.h) $(wildcard src/**/*.c) $(wildcard src/**/*.h)

format:
	clang-format -i $(C_SRC)

.PHONY: format