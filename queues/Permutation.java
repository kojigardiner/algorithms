/* *****************************************************************************
 *  Name:
 *  Date:
 *  Description:
 **************************************************************************** */

import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;

public class Permutation {
    public static void main(String[] args) {
        if (args.length != 1) {
            throw new IllegalArgumentException("Must supply a single command-line argument");
        }

        int k = Integer.parseInt(args[0]);
        RandomizedQueue<String> q = new RandomizedQueue<String>();

        int count = 0;
        String s;

        // Use Knuth's method to keep the queue size to k and replace elements at random
        while (!StdIn.isEmpty()) {
            s = StdIn.readString();
            if (count < k) {
                q.enqueue(s);
            }
            else {
                if (StdRandom.bernoulli((double) k / count)) {
                    q.dequeue();
                    q.enqueue(s);
                }
            }
            count++;
        }

        for (int i = 0; i < k; i++) {
            StdOut.println(q.dequeue());
        }
    }
}
