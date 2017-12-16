INCLUDEDIR = headers/
CXXFLAGS   = -std=c++11 -I $(INCLUDEDIR) -Wall -Wfatal-errors 
EXES  = correctnessTests timingTests main
vpath %.h $(INCLUDEDIR)



all: $(EXES)

$(EXES): %: src/%.cpp src/primeFactors.cpp
	g++ $(CXXFLAGS) -o bin/$@.out $^ -lboost_system  -lboost_unit_test_framework
#It's not the prettiest, but lboost wants to load after everything else
clean:
	rm -f $(EXES) 



