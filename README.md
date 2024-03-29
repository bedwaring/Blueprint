a build file generator like cmake but bad

the syntax is kinda weird, CPP_VERSION sets the cpp version, for example CPP_VERSION c++17 would set the cpp version to 17
TARGET sets the project name so TARGET hello would set the project name to hello
and SRC is where the source code of your project is located
like SRC src/main.cpp would make the source src/main.cpp

this is the format

CPP_VERSION c++17

TARGET hello SRC src/helloworld.cpp

u can save the code into a file called Blueprint.bp and do blueprint Ninja/Makefile (you choose either Ninja or Makefile, we only support them for now)

it will save the output to a Makefile or a build.ninja file depending on what you selected. it reads a file called Blueprint.bp so make sure to name the file with the blueprint code to Blueprint.bp (blueprint code = CPP_VERSION c++17 TARGET hello SRC src/helloworld.cpp, etc, ive decided to name the custom lang blueprint)
