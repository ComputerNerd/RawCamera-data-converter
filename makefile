objects = main.o tables.o savePng.o yuv.o bilinear.o adaptive.o pixels.o

convert: $(objects)
	gcc -o convert -O2 -s -lm -lpng -Wall -Wextra $(objects)
%.o: %.c
	gcc -O2 -s -lm -lpng -Wall -Wextra -c $< -o $@
$(objects) :
.PHONY : clean
clean :
	rm convert $(objects)
