/* *****************************************************************************
 *  Name:
 *  Date:
 *  Description:
 **************************************************************************** */

import edu.princeton.cs.algs4.BinaryStdIn;
import edu.princeton.cs.algs4.BinaryStdOut;
import edu.princeton.cs.algs4.StdOut;

public class BurrowsWheeler {
    private static final int R = 256;

    // apply Burrows-Wheeler transform,
    // reading from standard input and writing to standard output
    public static void transform() {
        // Read the input as a string and form its CircularSuffixArray
        String s = BinaryStdIn.readString();
        CircularSuffixArray csa = new CircularSuffixArray(s);
        int len = csa.length();

        // Declare variables to store the transform
        int first = -1;  // the index of the original string in the sorted CSA
        char[] t = new char[len];  // the last characters of each string in the sorted CSA

        for (int i = 0; i < len; i++) {
            int csaIdx = csa.index(i);
            if (csaIdx == 0) {
                first = i;
                t[i] = s.charAt(len - 1);
            }
            else {
                t[i] = s.charAt(csaIdx - 1);
            }
        }
        // StdOut.printf("%d\n%s\n", first, new String(t));
        BinaryStdOut.write(first);
        for (int i = 0; i < len; i++) {
            BinaryStdOut.write(t[i]);
        }
    }

    // Returns a sorted copy of the given character array. Uses key-indexed
    // counting.
    private static char[] sortChars(char[] t) {
        int len = t.length;
        char[] tSorted = new char[len];
        int[] counts = new int[R + 1];

        // Count frequency of each char
        for (int i = 0; i < len; i++) {
            counts[t[i] + 1]++;
        }
        // Calculate cumulates
        for (int i = 0; i < R; i++) {
            counts[i + 1] += counts[i];
        }
        // Sort
        for (int i = 0; i < len; i++) {
            tSorted[counts[t[i]]] = t[i];
            counts[t[i]]++;
        }

        return tSorted;
    }

    // apply Burrows-Wheeler inverse transform,
    // reading from standard input and writing to standard output
    public static void inverseTransform() {
        // Read the transform values
        int first = BinaryStdIn.readInt();
        String s = BinaryStdIn.readString();

        // Cache the length for O(1) lookup
        int len = s.length();

        // Convert to a char array
        char[] t = s.toCharArray();

        // Sort the char array
        char[] tSorted = new char[len];
        int[] counts = new int[R + 1];

        // Count frequency of each char
        for (int i = 0; i < len; i++) {
            counts[t[i] + 1]++;
        }
        // Calculate cumulates
        for (int i = 0; i < R; i++) {
            counts[i + 1] += counts[i];
        }

        // StdOut.printf("%s\n", new String(t));
        // StdOut.printf("%s\n", new String(tSorted));

        // Declare the next array
        int[] next = new int[len];

        // Iterate over the last characters in the sorted suffix array, and
        // find their position in the cumulate counts of the sorted character
        // array. This tells us the next index at which to place the current
        // iteration count i. Increment the cumulate counts for that
        // character to ensure the next call goes to the next position.
        // While in this loop, also create the sorted first character array
        for (int i = 0; i < len; i++) {
            char c = t[i];
            next[counts[c]] = i;
            tSorted[counts[c]] = t[i];
            counts[c]++;
        }
        // for (int i = 0; i < len; i++) {
        //     StdOut.printf("%d\n", next[i]);
        // }

        // Reconstruct the original string using first and next[]
        int idx = first;
        for (int i = 0; i < len; i++) {
            BinaryStdOut.write(tSorted[idx]);
            idx = next[idx];
        }
    }

    private static void printUsage() {
        StdOut.println("Usage (transform): BurrowsWheeler - < <filename>");
        StdOut.println("Usage (inverse transform): BurrowsWheeler + < <filename>");
    }

    // if args[0] is "-", apply Burrows-Wheeler transform
    // if args[0] is "+", apply Burrows-Wheeler inverse transform
    public static void main(String[] args) {
        if (args.length != 1) {
            printUsage();
        }
        else if (args[0].equals("-")) {
            transform();
        }
        else if (args[0].equals("+")) {
            inverseTransform();
        }
        else {
            printUsage();
        }
        BinaryStdOut.close();
    }
}
