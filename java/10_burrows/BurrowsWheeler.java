/* *****************************************************************************
 *  Name:
 *  Date:
 *  Description:
 **************************************************************************** */

import edu.princeton.cs.algs4.BinaryStdIn;
import edu.princeton.cs.algs4.BinaryStdOut;
import edu.princeton.cs.algs4.StdOut;

public class BurrowsWheeler {
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

    // apply Burrows-Wheeler inverse transform,
    // reading from standard input and writing to standard output
    public static void inverseTransform() {

    }

    private static void printUsage() {
        StdOut.println("Usage (transform): BurrowsWheeler - < <filename>");
        StdOut.println("Usage (inverse transform): BurrowsWheeler + < <filename>");
    }

    // if args[0] is "-", apply Burrows-Wheeler transform
    // if args[0] is "+", apply Burrows-Wheeler inverse transform
    public static void main(String[] args) {
        if (args.length < 1) {
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
