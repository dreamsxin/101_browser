/**
 * @(#)BinaryTreeNode.java
 *
 *
 * @author Wolfgang Keller
 * @version 1.00 2010/3/2
 */
import java.util.*;

public class BinaryTreeNode<L extends List<Boolean>> implements TreeNode<L> {
	public BinaryFunction function;
	public TreeNode<L> child0, child1;
	
	BinaryTreeNode(int childrenCount, int variablesCount) {
		function = BinaryFunction.NOT;
		
		if (childrenCount == 0) {
			child0 = new LeafTreeNode<L>(variablesCount);
		} else {
			child0 = new BinaryTreeNode<L>(childrenCount-1, variablesCount);
		}
		child1 = null;
	}

    public boolean computeValue(L list) {
    	switch (function) {
    		case NOT:
    			return !child0.computeValue(list);
    		case AND:
    			return child0.computeValue(list) && child1.computeValue(list);
    		case OR:
    			return child0.computeValue(list) || child1.computeValue(list);
    		case XOR:
    			return child0.computeValue(list) ^ child1.computeValue(list);
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
		else if (child1 != null && child1.increment()) {
			child0.reset();
			return true;
		} else if (function != BinaryFunction.XOR) {
			BinaryFunction funcBak = function;
			
			this.reset();
			child1 = new LeafTreeNode<L>(TreeUtil.VARIABLES_COUNT);
			
			if (funcBak == BinaryFunction.NOT)	{
				function = BinaryFunction.AND;
			} else if (funcBak == BinaryFunction.AND)
				function = BinaryFunction.OR;
			else if (funcBak == BinaryFunction.OR)
				function = BinaryFunction.XOR;
			
			return true;
		} else if (child0.size() != 0) {
			int child0size = child0.size();
			int child1size = child1.size();
			
			if (child0size == 1) {
				child0 = new LeafTreeNode<L>(TreeUtil.VARIABLES_COUNT);	
			} else {
				child0 = new BinaryTreeNode<L>(child0size-2, TreeUtil.VARIABLES_COUNT);
			}
			// Note that this decreases the number of nodes in child1 by 1
			child1 = new BinaryTreeNode<L>(child1size, TreeUtil.VARIABLES_COUNT);
			
			return true;
		} else {
			return false;
		}
	}

	/**
	 * Method reset
	 *
	 *
	 */
	public void reset() {
		function = BinaryFunction.NOT;
		int size = this.size();
		if (size == 1) {
			child0 = new LeafTreeNode<L>(TreeUtil.VARIABLES_COUNT);	
		} else {
			child0 = new BinaryTreeNode<L>(size-2, TreeUtil.VARIABLES_COUNT);
		}
		
		child1 = null;
	}

	/**
	 * Method size
	 *
	 *
	 * @return
	 *
	 */
	public int size() {
		int size = 1+child0.size();
		if (child1 != null)
			size += child1.size();
		
		return size;
	}
	
	public String toString() {
		StringBuilder b = new StringBuilder();
		
		if (function == BinaryFunction.NOT) {
			b.append("!");
			b.append(child0.toString());
			return b.toString();
		} else {
			if (!(child0 instanceof LeafTreeNode))
				b.append("(");
			b.append(child0.toString());
			if (!(child0 instanceof LeafTreeNode))
				b.append(")");
			switch (function) {
				case AND:
					b.append(" && ");
					break;
				case OR:
					b.append(" || ");
					break;
				case XOR:
					b.append(" ^ ");
					break;
			}
			if (!(child1 instanceof LeafTreeNode))
				b.append("(");
			b.append(child1.toString());
			if (!(child1 instanceof LeafTreeNode))
				b.append(")");
			return b.toString();
		}
	}
}