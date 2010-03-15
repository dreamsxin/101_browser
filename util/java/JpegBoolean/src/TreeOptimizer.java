/**
 * @(#)TreeOptimizer.java
 *
 *
 * @author Wolfgang Keller
 * @version 1.00 2010/3/14
 */
 import java.util.*;

public class TreeOptimizer {
	
	public static void optimizeTree(ArrayList<ArrayList<Boolean>> variables, ArrayList<Boolean> values) {
		BooleanTree<ArrayList<Boolean>> tree=new BooleanTree<ArrayList<Boolean>>();
		
		int bestApproximation = 0;
		long maximumIterations = -1;
		long currentIteration = 0;
		
		while (true) {
			Iterator<ArrayList<Boolean>> varIt = variables.iterator();
			Iterator<Boolean> valIt = values.iterator();
			
			//System.out.println("Tree = "+tree.toString());
			
			int approximationCorrectness = 0;
			int computed = 0;
			
			while (varIt.hasNext() && valIt.hasNext()) {
				if (tree.computeValue(varIt.next()) == valIt.next()) {
					approximationCorrectness++;
				}
				
				computed++;
				
				if (computed-approximationCorrectness>16-bestApproximation)
				{
					approximationCorrectness=0;
					break;
				}
			}
			
			if (approximationCorrectness>bestApproximation) {
				bestApproximation = approximationCorrectness;
				System.out.println(approximationCorrectness);
				System.out.println(tree);
			}
			
			if (approximationCorrectness==16) {
				break;
			}
			
			if (!tree.increment()) {
				System.out.println("Breaking since tree could not be incremented");
				break;
			}
			
			currentIteration++;
			if (currentIteration == maximumIterations) {
				System.out.println("Breaking because of iterations count");
				break;
			}
		}
	}
}
