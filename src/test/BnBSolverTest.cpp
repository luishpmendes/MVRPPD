#include "../solver/branch-and-bound/BnBSolver.hpp"
#include <cassert>

int main () {
    unsigned timeLimit = 100;
    unsigned seed = 0;
    unsigned maxNumSolutions = 10;
    Instance instance;
    BnBSolver solver;

    instance = Instance("instances/test.in");
    solver = BnBSolver(instance, timeLimit, seed, maxNumSolutions);

    assert(solver.getMaxNumSolutions() == maxNumSolutions);
    assert(solver.getSolvingTime() == 0);
    assert(solver.getSolutionSet().getSolutions().size() == 0);

    solver.solve();

    assert(solver.getSolvingTime() <= timeLimit);

    std::vector<Solution> solutions = solver.getSolutionSet().getFront(0);

    assert(solutions.size() > 1);
    assert(solutions.size() <= maxNumSolutions);

    for(auto solutionA : solutions) {
        assert(solutionA.isFeasible());
        assert(solutionA.areConstraintsSatisfied());
        for(auto solutionB : solutions) {
            assert(!solutionA.dominates(solutionB));
            assert(!solutionB.dominates(solutionA));
        }
    }

    return 0;
}

