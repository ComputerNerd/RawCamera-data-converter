objects = main.o

yuv: $(objects)
	gcc -o convert -O2 -s -lm -lpng -Wall -Wextra $(objects)
%.o: %.c
	gcc -o convert -O2 -s -lm -lpng -Wall -Wextra -c $< -o $@
$(objects) :
.PHONY : clean
clean :
	rm convert $(objects)
