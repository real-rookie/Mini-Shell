CC = g++
C_FLAGS = -c -std=c++11 -g -Wall -O0
file_main = shell379
file1 = shell
file2 = process

$(file_main): $(file_main).o $(file1).o $(file2).o
	$(CC) -o $@ $^
$(file_main).o: $(file_main).cpp
	$(CC) $(C_FLAGS) $<
$(file1).o: $(file1).cpp
	$(CC) $(C_FLAGS) $<
$(file2).o: $(file2).cpp
	$(CC) $(C_FLAGS) $<

run:
	./$(file_main)
memcheck:
	valgrind --tool=memcheck --leak-check=full ./$(file_main)
clean:
	rm -f $(file_main) *.o 