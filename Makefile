run : clean compile
ifneq ($(INFILE),)
		./codeGen $(INFILE)
else
		./codeGen ./input/input1.c
endif
		./variableRename
		mv out ./output/Output.c
compile : codeGen.cpp variableRename.cpp
		g++ -o codeGen codeGen.cpp
		g++ -o variableRename variableRename.cpp
clean :
		rm -f info.txt out codeGen variableRename intermediate.txt