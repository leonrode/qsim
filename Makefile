objects = main.o math.o

qsim : $(objects)
	gcc -o qsim $(objects)

clean:
	rm qsim $(objects)