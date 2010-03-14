/**
 * @(#)UnaryTreeNode.java
 *
 *
 * @author Wolfgang Keller
 * @version 1.00 2010/3/14
 */
import java.util.*;

public class NotTreeNode<L extends List<Boolean>> implements TreeNode<L> {
	public TreeNode<L> child;
	
	NotTreeNode(int childrenCount, int variablesCount) {
		if (childrenCount == 0) {
			child = new LeafTreeNode<L>(variablesCount);
		} else {
			child = new NotTreeNode<L>(childrenCount-1, variablesCount);
		}
		
	}
    
    public boolean computeValue(L list)
    {
    	return !child.computeValue(list);
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
	
	public String toString() {
		return "!"+child.toString();
	}
}