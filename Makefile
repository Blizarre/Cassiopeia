TEST_DIR=.


all: ${TEST_DIR}/result.tiff

main: main.cpp
	clang++-6.0 $^ `pkg-config --libs opencv` -l raw -ggdb -O3 -flto -Wall -Wextra -Werror -o $@

${TEST_DIR}/result.tiff: main
	./$< ${TEST_DIR}/*.ARW ${TEST_DIR}/result.tiff

clean:
	rm -f ${TEST_DIR}/result.tiff main
