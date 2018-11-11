all: result.tiff

main: main.cpp
	clang++-6.0 $^ -l raw -ggdb -O3 -flto -Wall -Wextra -Werror -o $@

result.tiff: main
	./$< *.ARW result.tiff
