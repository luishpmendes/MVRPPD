CPP=g++
CARGS=-std=c++14 -Wall -Werror -O0 -g3 -m64
GRBINC=/opt/gurobi901/linux64/include/
GRBLIB=-L/opt/gurobi901/linux64/lib -lgurobi_c++ -lgurobi90 -lm
MKDIR=mkdir -p
RM=rm -rf
SRC=$(PWD)/src
BIN=$(PWD)/bin

clean:
	$(RM) $(BIN)

$(BIN)/%.o: $(SRC)/%.cpp
	$(MKDIR) $(@D)
	$(CPP) $(CARGS) -c $< -o $@ -I$(GRBINC) $(GRBLIB)

$(BIN)/test/InstanceTest: $(BIN)/instance/Instance.o \
                          $(BIN)/test/InstanceTest.o
	$(MKDIR) $(@D)
	$(CPP) -o $@ $^ $(CARGS)

InstanceTest: $(BIN)/test/InstanceTest

$(BIN)/test/SolutionTest: $(BIN)/instance/Instance.o \
                          $(BIN)/solution/Solution.o \
                          $(BIN)/test/SolutionTest.o
	$(MKDIR) $(@D)
	$(CPP) -o $@ $^ $(CARGS)

SolutionTest: $(BIN)/test/SolutionTest

$(BIN)/test/BnBSolverTest: $(BIN)/instance/Instance.o \
                           $(BIN)/solution/Solution.o \
                           $(BIN)/solution/SolutionSet.o \
                           $(BIN)/solver/Solver.o \
                           $(BIN)/solver/branch-and-bound/BnBSolverCallback.o \
                           $(BIN)/solver/branch-and-bound/BnBSolver.o \
                           $(BIN)/test/BnBSolverTest.o
	$(MKDIR) $(@D)
	$(CPP) -o $@ $^ $(CARGS) -I$(GRBINC) $(GRBLIB)

BnBSolverTest: $(BIN)/test/BnBSolverTest

Tests: InstanceTest SolutionTest BnBSolverTest

