CC = g++
CC_FLAGS = -std=c++11 -Wall -Wextra

main: main.o hashset.o memory_leak_detection.o hero.o log.h
	$(CC) $(CC_FLAGS) $^ -o $@

main.o: main.cpp my_list.h hero.h
	$(CC) $(CC_FLAGS) $< -c -o $@

hero.o: hero.cpp hero.h
	$(CC) $(CC_FLAGS) $< -c -o $@

hashset.o: hashset.cpp hashset.h
	$(CC) $(CC_FLAGS) $< -c -o $@

memory_leak_detection.o: memory_leak_detection.cpp memory_leak_detection.h log.h
	$(CC) $(CC_FLAGS) $< -c -o $@

.PHONY: clean

clean:
	rm -f main main.o hero.o test test.o hashset.o memory_leak_detection.o
