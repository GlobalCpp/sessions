PROGRAMS = investigate

% : %.cpp
	g++ -fopenmp -O2 -std=c++23 -o $@ $<

clean :
	@rm -f a.out *~ ${PROGRAMS}
