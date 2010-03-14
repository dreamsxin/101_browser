/**
 * @(#)TreeNode.java
 *
 *
 * @author 
 * @version 1.00 2010/3/2
 */


public interface TreeNode<State> {
    public boolean computeValue(State s);
    
    /**
     * Tries to increment the subtree with root this.
     * @return true if sucessful
     *         false otherwise
     */
    public boolean increment();
    
    /**
     * Resets the subtree to the original value
     */
    public void reset();
}