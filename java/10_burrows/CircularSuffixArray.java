/* *****************************************************************************
 *  Name:
 *  Date:
 *  Description:
 **************************************************************************** */

import edu.princeton.cs.algs4.StdOut;

public class CircularSuffixArray {
    private final String str;
    private final int len;                // string length
    private final int[] sortedIdx;        // sorted suffix indices
    
    // circular suffix array of s
    public CircularSuffixArray(String s) {
        if (s == null) {
            throw new IllegalArgumentException("String s cannot be null");
        }

        // Cache the length for O(1) lookup
        str = s;
        len = s.length();

        sortedIdx = sortCircularSuffixes(s);
    }

    // Returns an integer list of sorted circular suffix indices. Uses LSD
    // string sort with modular indexing.
    private int[] sortCircularSuffixes(String s) {
        int w = s.length();
        int n = s.length();
        int R = 256;   // extend ASCII alphabet size
        int[] aux = new int[n];

        // Initialize idx array. Idx array entries will be used to calculate
        // the circular shift for each string.
        int[] idx = new int[n];
        for (int i = 0; i < n; i++) {
            idx[i] = i;
        }

        for (int d = w - 1; d >= 0; d--) {
            // sort by key-indexed counting on dth character

            // compute frequency counts
            int[] count = new int[R + 1];
            for (int i = 0; i < n; i++)
                // Extract starting index of the current string with idx[i]
                // then calculate + d modulo w for the circular wrap.
                count[s.charAt((idx[i] + d) % w) + 1]++;

            // compute cumulates
            for (int r = 0; r < R; r++)
                count[r + 1] += count[r];

            // move data
            for (int i = 0; i < n; i++)
                // Extract starting index of the current string with idx[i]
                // then calculate + d modulo w for the circular wrap.
                aux[count[s.charAt((idx[i] + d) % w)]++] = idx[i];

            // copy back
            for (int i = 0; i < n; i++)
                idx[i] = aux[i];
        }
        return idx;
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
