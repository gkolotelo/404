# IAS_assembler Makefile
# Default name:
NAME = ra135964_ra137943
# Default compiler:
DC = g++
# Default flags for compile process:
CFLAGS = -c -std=c++11
# Default echo text:
COMPILING = "Creating object files:"
LINKING = "Linking object files:"
CLEANING = "Cleaning up object files:"
FINAL = "Created file: "$(NAME)


all: compiling_text compiling_process linking_text IAS_assembler cleaning_text clean final_text

compiling_text:
	@echo $(COMPILING)
linking_text:
	@echo $(LINKING)
cleaning_text:
	@echo $(CLEANING)
final_text:
	@echo $(FINAL)

IAS_assembler: compiling_process
	$(DC) main.o assembler.o reading.o types.o errors.o -o $(NAME)

compiling_process: main.o assembler.o reading.o types.o errors.o 

main.o: main.cpp
	$(DC) $(CFLAGS) main.cpp

assembler.o: assembler.cpp
	$(DC) $(CFLAGS) assembler.cpp

reading.o: reading.cpp
	$(DC) $(CFLAGS) reading.cpp

types.o: types.cpp
	$(DC) $(CFLAGS) types.cpp

errors.o: errors.cpp
	$(DC) $(CFLAGS) errors.cpp

clean:
	rm main.o assembler.o reading.o types.o errors.o
