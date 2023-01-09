/* *****************************************************************************
 *  Name:
 *  Date:
 *  Description:
 **************************************************************************** */

import edu.princeton.cs.algs4.IndexMinPQ;
import edu.princeton.cs.algs4.StdOut;

public class CircularSuffixArray {
    private int len;        // string length
    private int[] sortedIdx;      // sorted suffix indices

    // circular suffix array of s
    public CircularSuffixArray(String s) {
        if (s == null) {
            throw new IllegalArgumentException("String s cannot be null");
        }

        // Cache the length for O(1) lookup
        len = s.length();

        // Create a StringBuilder for O(1) charAt and append
        StringBuilder sb = new StringBuilder(s);

        // Create a priority queue to implicitly sort the suffix strings
        IndexMinPQ<String> pq = new IndexMinPQ<String>(len);

        // Insert into the priority queue each shifted version of the string
        // with a new index
        pq.insert(0, s);
        for (int i = 1; i < len; i++) {
            char c = sb.charAt(0);
            sb.deleteCharAt(0);
            sb.append(c);
            pq.insert(i, sb.toString());
        }

        // Initial the sorted index array
        sortedIdx = new int[len];
        int count = 0;

        // Iterate over the priority queue and store the indices
        for (int i : pq) {
            sortedIdx[count] = i;
            count++;
        }
    }

    // length of s
    public int length() {
        return len;
    }

    // returns index of ith sorted suffix
    public int index(int i) {
        if (i < 0 || i > len - 1) {
            throw new IllegalArgumentException("Index must be in range [0, n-1]");
        }

        return sortedIdx[i];
    }

    // unit testing (required)
    public static void main(String[] args) {
        CircularSuffixArray csa = new CircularSuffixArray("ABRACADABRA!");

        StdOut.printf("length = %d\n", csa.length());

        StdOut.printf("indices:\n");
        for (int i = 0; i < csa.length(); i++) {
            StdOut.printf("%d\n", csa.index(i));
        }
    }
}
