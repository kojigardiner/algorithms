/* *****************************************************************************
 *  Name:
 *  Date:
 *  Description:
 **************************************************************************** */

import edu.princeton.cs.algs4.StdOut;

public class CircularSuffixArray {
    private final int len;                // string length
    private final int[] sortedIdx;        // sorted suffix indices

    // circular suffix array of s
    public CircularSuffixArray(String s) {
        if (s == null) {
            throw new IllegalArgumentException("String s cannot be null");
        }

        // Cache the length for O(1) lookup
        len = s.length();

        sortedIdx = CircularMSD.sort(s);
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
