/* *****************************************************************************
 *  Name:
 *  Date:
 *  Description:
 **************************************************************************** */

import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;

import java.util.Iterator;
import java.util.NoSuchElementException;

public class RandomizedQueue<Item> implements Iterable<Item> {
    private int count;
    private Item[] q;

    // construct an empty randomized queue
    public RandomizedQueue() {
        count = 0;
        q = (Item[]) new Object[1];
    }

    // is the randomized queue empty?
    public boolean isEmpty() {
        return (count == 0);
    }

    // return the number of items on the randomized queue
    public int size() {
        return count;
    }

    // add the item
    public void enqueue(Item item) {
        if (item == null) {
            throw new IllegalArgumentException("Item cannot be null");
        }
        q[count] = item;
        count++;
        if (count == q.length) resize(q.length * 2);
    }

    // remove and return a random item
    public Item dequeue() {
        if (isEmpty()) {
            throw new NoSuchElementException("Queue is empty");
        }
        int idx = get_random_idx();

        Item item = q[idx];
        count--;

        q[idx] = q[count];  // fill the old idx with the last element
        q[count] = null;    // change the last element to null

        if (count > 0 && count == q.length / 4) resize(q.length / 2);

        return item;
    }

    // return a random item (but do not remove it)
    public Item sample() {
        if (isEmpty()) {
            throw new NoSuchElementException("Queue is empty");
        }
        int idx = get_random_idx();

        return q[idx];
    }

    // return a valid random idx (one that has not already been removed)
    private int get_random_idx() {
        return StdRandom.uniform(count);
    }

    // resize the arrays (Item[] and removed[])
    private void resize(int capacity) {
        Item[] copy_q = (Item[]) new Object[capacity];

        int copy_count = 0;
        for (int i = 0; i < count; i++) {
            copy_q[i] = q[i];
        }
        // update pointer
        q = copy_q;
    }

    // return an independent iterator over items in random order
    public Iterator<Item> iterator() {
        return new RandomListIterator();
    }

    private class RandomListIterator implements Iterator<Item> {
        private int[] random_idx_array;
        private int idx_into_random_idx_array;

        private RandomListIterator() {
            random_idx_array = new int[count];
            idx_into_random_idx_array = 0;

            // generate list of valid idxs
            for (int i = 0; i < count; i++) {
                random_idx_array[i] = i;
            }
            // make that list of idxs random
            StdRandom.shuffle(random_idx_array);
        }

        public boolean hasNext() {
            return (idx_into_random_idx_array < random_idx_array.length);
        }

        public void remove() {
            throw new UnsupportedOperationException("Remove not supported by iterator");
        }

        public Item next() {
            if (isEmpty() || idx_into_random_idx_array >= random_idx_array.length) {
                throw new NoSuchElementException("Queue is empty");
            }
            return q[random_idx_array[idx_into_random_idx_array++]];
        }

    }

    // unit testing (required)
    public static void main(String[] args) {
        StdOut.println("Constructing RandomizedQueue");
        RandomizedQueue<Integer> q = new RandomizedQueue<Integer>();

        StdOut.println("Adding 1-10 to queue");
        for (int i = 1; i <= 10; i++) {
            q.enqueue(i);
        }

        StdOut.print(q.size());
        StdOut.println(" elements in queue");

        StdOut.println("Sampling 5 random elements");
        for (int i = 0; i < 5; i++) {
            StdOut.println(q.sample());
        }

        StdOut.print(q.size());
        StdOut.println(" elements in queue");

        StdOut.println("Removing 5 random elements");
        for (int i = 0; i < 5; i++) {
            StdOut.println(q.dequeue());
        }

        StdOut.print(q.size());
        StdOut.println(" elements in queue");

        StdOut.println("Randomly iterating through remainder of queue");
        for (Integer i : q) {
            StdOut.println(i);
        }

        StdOut.println("Emptying queue");
        while (!q.isEmpty()) {
            q.dequeue();
        }

        StdOut.println("Adding more elements");
        q.enqueue(10);
        q.enqueue(14);
        q.enqueue(-4);

        StdOut.println("Emptying queue");
        while (!q.isEmpty()) {
            q.dequeue();
        }

        StdOut.println("Attempting to dequeue an empty queue, should throw an error");
        q.dequeue();
    }

}

