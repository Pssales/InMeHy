package dependencias_class;



import dependencias_interfaces.Problem;
import dependencias_interfaces.ConstrainedProblem;
import dependencias_interfaces.SolutionListEvaluator;

import java.util.List;

/**
 * @author Antonio J. Nebro
 */
@SuppressWarnings("serial")
public class SequentialSolutionListEvaluator<S> implements SolutionListEvaluator<S> {

  @Override
  public List<S> evaluate(List<S> solutionList, Problem<S> problem) throws JMetalException {
      if (problem instanceof ConstrainedProblem) {
        solutionList.stream().forEach(s -> {
          problem.evaluate(s);
          ((ConstrainedProblem<S>) problem).evaluateConstraints(s);
        });
      } else {
        solutionList.stream().forEach(s -> problem.evaluate(s));
      }

    return solutionList;
  }

  @Override public void shutdown() {
    ;
  }
}