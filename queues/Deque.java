/* *****************************************************************************
 *  Name:
 *  Date:
 *  Description:
 **************************************************************************** */

import edu.princeton.cs.algs4.StdOut;

import java.util.Iterator;
import java.util.NoSuchElementException;


public class Deque<Item> implements Iterable<Item> {

    private Node first;
    private Node last;
    private int size;

    private class Node {
        Item item;
        Node next;
        Node prev;
    }

    // construct an empty deque
    public Deque() {
        this.first = null;
        this.last = null;
        this.size = 0;
    }

    // is the deque empty?
    public boolean isEmpty() {
        return (this.size == 0);
    }

    // return the number of items on the deque
    public int size() {
        return this.size;
    }

    // add the item to the front
    public void addFirst(Item item) {
        if (item == null) {
            throw new IllegalArgumentException("Item must not be null");
        }
        size++;

        Node old_first = first;
        first = new Node();
        first.item = item;
        first.next = old_first;
        first.prev = null;

        if (old_first != null) {
            old_first.prev = first;
        }
        if (size == 1) {
            last = first;
        }
    }

    // add the item to the back
    public void addLast(Item item) {
        if (item == null) {
            throw new IllegalArgumentException("Item must not be null");
        }
        size++;

        Node old_last = last;
        last = new Node();
        last.item = item;
        last.next = null;
        last.prev = old_last;

        if (old_last != null) {
            old_last.next = last;
        }
        if (size == 1) {
            first = last;
        }
    }

    // remove and return the item from the front
    public Item removeFirst() {
        if (isEmpty()) {
            throw new NoSuchElementException("Deque is empty");
        }
        size--;
        Item item = first.item;
        first = first.next;
        if (first != null) {
            first.prev = null;
        }
        return item;
    }

    // remove and return the item from the back
    public Item removeLast() {
        if (isEmpty()) {
            throw new NoSuchElementException("Deque is empty");
        }
        size--;
        Item item = last.item;
        last = last.prev;
        if (last != null) {
            last.next = null;
        }
        return item;
    }

    // return an iterator over items in order from front to back
    public Iterator<Item> iterator() {
        return new ListIterator();
    }

    private class ListIterator implements Iterator<Item> {
        private Node current = first;

        public boolean hasNext() {
            return current != null;
        }

        public void remove() {
            throw new UnsupportedOperationException("Remove not supported by iterator");
        }

        public Item next() {
            Item item = current.item;
            current = current.next;
            return item;
        }

    }

    // unit testing (required)
    public static void main(String[] args) {
        StdOut.println("Constructing new deque");
        Deque<Integer> d = new Deque<Integer>();

        StdOut.println("Adding first/last to deque");
        d.addFirst(5);
        d.addFirst(4);
        d.addFirst(3);
        d.addFirst(2);
        d.addFirst(1);
        d.addLast(6);
        d.addLast(7);
        d.addLast(8);
        d.addLast(9);
        d.addLast(10);

        StdOut.println("Using iterator to print all elements (expecting 1-10)");
        for (Integer i : d) {
            StdOut.println(i);
        }

        StdOut.println("Remove and print first/last elements (expecting 1, 2, 10, 9)");
        // should print 1, 2, 10, 9
        StdOut.println(d.removeFirst());
        StdOut.println(d.removeFirst());
        StdOut.println(d.removeLast());
        StdOut.println(d.removeLast());

        StdOut.println("Printing remaining elements (expecting 3-8)");
        // should print 3-8 in order
        for (Integer i : d) {
            StdOut.println(i);
        }

        StdOut.println("Removing all remaining elements");
        while (!d.isEmpty()) {
            d.removeFirst();
        }

        StdOut.println("Adding and removing one element");
        d.addLast(5);
        d.removeFirst();

        System.out.print("Deque size is ");
        System.out.println(d.size());

        StdOut.println("Attempting to remove an element (should throw an exception)");
        d.removeLast();
    }
}
