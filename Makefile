objects = main.o qmath.o utils.o gate.o state.o qc.o

t_objects = tests.o qmath.o utils.o gate.o state.o

include_dir = "/include"

qsim : $(objects)
	gcc -o qsim $(objects) -I ${include_dir}

tests : $(t_objects) 
	gcc -o tests $(t_objects) -I ${include_dir}

clean:
	rm qsim $(objects)

clean_tests:
	rm tests $(t_objects)