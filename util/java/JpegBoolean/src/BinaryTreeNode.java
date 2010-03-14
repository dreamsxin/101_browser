/**
 * @(#)BinaryTreeNode.java
 *
 *
 * @author Wolfgang Keller
 * @version 1.00 2010/3/2
 */


public class BinaryTreeNode<State> implements TreeNode<State> {
	public BinaryFunction function;
	public TreeNode<State> child0, child1;
	
	BinaryTreeNode(int childrenCount, int variablesCount) {
		if (childrenCount == 0) {
			child0 = new LeafTreeNode(variablesCount);
		} else {
			child0 = new NotTreeNode(childrenCount-1, variablesCount);
		}
		child1 = new LeafTreeNode(variablesCount);
	}

    public boolean computeValue(State s) {
    	return combineChildren(child0.computeValue(s), child1.computeValue(s));
    }
    
    public boolean combineChildren(boolean b0, boolean b1) {
    	switch (function) {
    		case AND:
    			return b0 && b1;
    		case OR:
    			return b0 || b1;
    		case XOR:
    			return b0 ^ b1;
    	}
    	
    	// should never happen
    	return false;
    }

	/**
	 * Method increment
	 *
	 *
	 * @return
	 *
	 */
	public boolean increment() {
		if (child0.increment())
			return true;
		
		if (child1.increment())
		{
			child0.reset();
			return true;
		}
		
		if (function != BinaryFunction.XOR)
		{
			child0.reset();
			child1.reset();
			
			if (function == BinaryFunction.AND)
				function = BinaryFunction.OR;
			else if (function == BinaryFunction.OR)
				function = BinaryFunction.XOR;
			
			return true;
		}
		
		return false;
	}

	/**
	 * Method reset
	 *
	 *
	 */
	public void reset() {
		function = BinaryFunction.AND;
		child0.reset();
		child1.reset();
	}

	/**
	 * Method size
	 *
	 *
	 * @return
	 *
	 */
	public int size() {
		return 1+child0.size()+child1.size();
	}
}