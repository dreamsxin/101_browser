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
		function = BinaryFunction.AND;
		
		if (childrenCount == 0) {
			child0 = new LeafTreeNode<L>(variablesCount);
		} else {
			child0 = new NotTreeNode<L>(childrenCount-1, variablesCount);
		}
		child1 = new LeafTreeNode<L>(variablesCount);
	}

    public boolean computeValue(L list) {
    	return combineChildren(child0.computeValue(list), child1.computeValue(list));
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
	
	public String toString() {
		StringBuilder b = new StringBuilder();
		b.append("(");
		b.append(child0.toString());
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
		b.append("(");
		b.append(child1.toString());
		b.append(")");
		return b.toString();
	}
}