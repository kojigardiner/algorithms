/* *****************************************************************************
 *  Name:
 *  Date:
 *  Description:
 **************************************************************************** */

import edu.princeton.cs.algs4.BinaryStdIn;
import edu.princeton.cs.algs4.BinaryStdOut;
import edu.princeton.cs.algs4.StdOut;

public class MoveToFront {
    private static final int R = 256;

    // apply move-to-front encoding, reading from standard input and writing to standard output
    public static void encode() {
        // Initialize our sequence array. Index = ASCII char value, value =
        // ordered position
        int[] seq = new int[R];
        for (int i = 0; i < R; i++) {
            seq[i] = i;
        }

        // Read the input file byte by byte
        while (!BinaryStdIn.isEmpty()) {
            char c = BinaryStdIn.readChar();

            // Initialize position to be the current position of the character
            int position = seq[c];

            // Write the position
            BinaryStdOut.write((char) position);

            // Iterate over the full alphabet, incrementing the position of any
            // characters ahead of the char we read.
            for (int i = 0; i < R; i++) {
                if (seq[i] < position) {
                    seq[i]++;
                }
            }
            // Move to front by setting position to 0.
            seq[c] = 0;
        }
    }

    // apply move-to-front decoding, reading from standard input and writing to standard output
    public static void decode() {
        // Initialize our sequence array. Index = ASCII char value, value =
        // ordered position
        int[] seq = new int[R];
        for (int i = 0; i < R; i++) {
            seq[i] = i;
        }

        while (!BinaryStdIn.isEmpty()) {
            // Read in the current position
            int position = (int) BinaryStdIn.readChar();

            // Initialize c be the character currently at that position
            int c = seq[position];

            // Write the character
            BinaryStdOut.write((char) c);

            // Iterate over the sequence, up to the position of the char we
            // just found. Swap each character with the one before it in the
            // order.
            int prev = c;
            for (int i = 0; i <= position; i++) {
                // Swap seq[i] with seq[i-1].
                int tmp = seq[i];
                seq[i] = prev;
                prev = tmp;
            }
        }
    }

    private static void printUsage() {
        StdOut.println("Usage (encode): MoveToFront - < <filename>");
        StdOut.println("Usage (decode): MoveToFront + < <filename>");
    }

    // if args[0] is "-", apply move-to-front encoding
    // if args[0] is "+", apply move-to-front decoding
    public static void main(String[] args) {
        if (args.length != 1) {
            printUsage();
        }
        else if (args[0].equals("-")) {
            encode();
        }
        else if (args[0].equals("+")) {
            decode();
        }
        else {
            printUsage();
        }
        BinaryStdOut.close();
    }
}
