# sessions
Repository space for sharing code during a GlobalCpp session

Each session is a branch of this repository. Naming convention should 
be `session_YYYY_MM_DD` to dissambiguate. Presenter may be pushing 
changes to either a fork of the repository or to a branch in this repo. 
Not all presenters may choose to utilize this process. Please take note 
at the start of the session where they will be operating from that week.


Todays session may use the following tools:
```
perf
hotspot
claude
git
```

To get the branch in one line, use
```
git clone --single-branch --branch session_2026_03_21 git@github.com:GlobalCpp/sessions.git
```

environment setup
```
micromamba env create -f environment_gcc_15_2.yml
micromamba activate global-cpp-15-2
```

to build the programs, see `src/Makefile`

to run the programs, use
```
./ts.out cities_1.txt
./tsp.out cities_1.txt
./scratch.out cities_1.txt
```

to time the programs, use
```
	time ./ts.out cities_1.txt
	time ./tsp.out cities_1.txt
	time ./scratch.out cities_1.txt
```

to analyze the programs, use
```
	perf record ./ts.out cities_1.txt
	perf record ./tsp.out cities_1.txt
	perf record ./scratch.out cities_1.txt
```
and then
```
	perf report ./ts.out cities_1.txt
	perf report ./tsp.out cities_1.txt
	perf report ./scratch.out cities_1.txt
```
or
```
	hotspot
```
