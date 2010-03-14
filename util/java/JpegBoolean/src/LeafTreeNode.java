/**
 * @(#)TreeLeaf.java
 *
 *
 * @author Wolfgang Keller
 * @version 1.00 2010/3/14
 */
import java.util.*;

public class LeafTreeNode<L extends List<Boolean>> implements TreeNode<L> {
	int current, max;
	
	LeafTreeNode(int max) {
		this.current = 0;
		this.max = max;
	}

	/**
	 * Method computeValue
	 *
	 *
	 * @param list
	 *
	 * @return
	 *
	 */
	public boolean computeValue(L list) {
		return (list.get(current));
	}

	/**
	 * Method increment
	 *
	 *
	 * @return
	 *
	 */
	public boolean increment() {
		if (current+1<max) {
			current++;
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
		current = 0;
	}

	/**
	 * Method size
	 *
	 *
	 * @return
	 *
	 */
	public int size() {
		return 0;
	}
    
    public String toString() {
    	return "x"+current;
    }
}