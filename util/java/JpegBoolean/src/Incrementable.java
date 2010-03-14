/**
 * @(#)Incrementable.java
 *
 *
 * @author Wolfgang
 * @version 1.00 2010/3/14
 */


public interface Incrementable {
    
    /**
     * Tries to increment the subtree with root this.
     * @return true if sucessful
     *         false otherwise
     */
    public boolean increment();
    
}