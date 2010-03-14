/**
 * @(#)UnaryTreeNode.java
 *
 *
 * @author Wolfgang Keller
 * @version 1.00 2010/3/14
 */


public class NotTreeNode<State> implements TreeNode<State> {
	public TreeNode<State> child;
	
	NotTreeNode(int childrenCount, int variablesCount) {
		if (childrenCount == 0) {
			child = new LeafTreeNode(variablesCount);
		} else {
			child = new NotTreeNode(childrenCount-1, variablesCount);
		}
		
	}
    
    public boolean computeValue(State s)
    {
    	return !child.computeValue(s);
    }

	/**
	 * Method increment
	 *
	 *
	 * @return
	 *
	 */
	public boolean increment() {
		return child.increment();
	}

	/**
	 * Method reset
	 *
	 *
	 */
	public void reset() {
		child.reset();
	}

	/**
	 * Method size
	 *
	 *
	 * @return
	 *
	 */
	public int size() {
		return 1+child.size();
	}
}