objects = main.o

yuv: $(objects)
	gcc -o yuv -O2 -s -lm -lpng -Wall -Wextra $(objects)
$(objects) :
.PHONY : clean
clean :
	rm yuv $(objects)
