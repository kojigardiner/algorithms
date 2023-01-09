/* *****************************************************************************
 *  Name:
 *  Date:
 *  Description:
 **************************************************************************** */

import edu.princeton.cs.algs4.IndexMinPQ;
import edu.princeton.cs.algs4.StdOut;

public class CircularSuffixArray {
    private final int len;                // string length
    private final int[] sortedIdx;        // sorted suffix indices

    // Private nested class that enables memory-efficient construction of the
    // circular suffix array. This class stores the string length, a reference
    // to the string object, and the index where the suffix starts. It
    // implements the Comparable interface by performing a
    // character-by-character comparison between two CircularSuffix objects
    // with different starting indices.
    private class CircularSuffix implements Comparable<CircularSuffix> {
        private final String s;
        private final int len;
        private final int idx;

        public CircularSuffix(String str, int i) {
            s = str;
            idx = i;
            len = s.length();
        }

        public int compareTo(CircularSuffix cs) {
            int minLen = Math.min(len, cs.len);
            for (int i = 0; i < minLen; i++) {
                char c1 = s.charAt((idx + i) % len);
                char c2 = cs.s.charAt((cs.idx + i) % cs.len);
                if (c1 < c2) {
                    return -1;
                }
                if (c1 > c2) {
                    return 1;
                }
            }
            return 0;
        }
    }

    // circular suffix array of s
    public CircularSuffixArray(String s) {
        if (s == null) {
            throw new IllegalArgumentException("String s cannot be null");
        }

        // Cache the length for O(1) lookup
        len = s.length();

        // Create a priority queue to implicitly sort the suffix strings
        IndexMinPQ<CircularSuffix> pq = new IndexMinPQ<CircularSuffix>(len);

        // Insert into the priority queue each shifted version of the string
        // with a new index
        for (int i = 0; i < len; i++) {
            pq.insert(i, new CircularSuffix(s, i));
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
