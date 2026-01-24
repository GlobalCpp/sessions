setup-env:
	micromamba env create -f environment_gcc_15_2.yml

activate:
	micromamba activate global-cpp-15-2

adl:
	g++ --std=c++23 src/adl.cpp

ns1:
	g++ --std=c++23 src/namespaces1.cpp

ns2:
	g++ --std=c++23 src/namespaces2.cpp

ctad:
	g++ --std=c++23 src/ctad.cpp

assembled:
	g++ --std=c++23 src/assembled.cpp

