objects = main.o math.o

t_objects = tests.o math.o

qsim : $(objects)
	gcc -o qsim $(objects)

tests : $(t_objects) 
	gcc -o tests $(t_objects)

clean:
	rm qsim $(objects)

clean_tests:
	rm tests $(t_objects)