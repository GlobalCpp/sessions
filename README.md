# sessions
Repository space for sharing code during a GlobalCpp session

Each session is a branch of this repository. Naming convention should 
be `session_YYYY_MM_DD` to dissambiguate. Presenter may be pushing 
changes to either a fork of the repository or to a branch in this repo. 
Not all presenters may choose to utilize this process. Please take note 
at the start of the session where they will be operating from that week.


```
micromamba env create -f environment_gcc_15_2.yml
micromamba env create -f environment_gcc_14_2.yml
micromamba env create -f environment_clang.yml
micromamba activate global-cpp-15-2
micromamba activate global-cpp-clang
```
