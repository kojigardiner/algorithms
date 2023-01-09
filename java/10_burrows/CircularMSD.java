/******************************************************************************
 *  Compilation: javac CircularMSD.java
 *  Execution:   java CircularMSD < input.txt
 *  Dependencies: StdIn.java StdOut.java
 *  Data files:   https://algs4.cs.princeton.edu/51radix/words3.txt
 *                https://algs4.cs.princeton.edu/51radix/shells.txt
 *
 *  Sort an array of strings or integers using CircularMSD radix sort.
 *
 *  % java CircularMSD < shells.txt
 *  are
 *  by
 *  sea
 *  seashells
 *  seashells
 *  sells
 *  sells
 *  she
 *  she
 *  shells
 *  shore
 *  surely
 *  the
 *  the
 *
 ******************************************************************************/


/**
 * The {@code CircularMSD} class provides static methods for sorting an
 * array of extended ASCII strings or integers using CircularMSD radix sort.
 * <p>
 * For additional documentation,
 * see <a href="https://algs4.cs.princeton.edu/51radix">Section 5.1</a> of
 * <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 * @author Robert Sedgewick
 * @author Kevin Wayne
 */
public class CircularMSD {
    private static final int BITS_PER_BYTE = 8;
    private static final int BITS_PER_INT = 32;   // each Java int is 32 bits
    private static final int R = 256;   // extended ASCII alphabet size
    private static final int CUTOFF = 15;   // cutoff to insertion sort

    // do not instantiate
    private CircularMSD() {
    }

    /**
     * Rearranges the array of extended ASCII strings in ascending order.
     *
     * @param a the array to be sorted
     */
    public static int[] sort(String a) {
        int n = a.length();
        int[] aux = new int[n];
        // idx array that will eventually be sorted
        int[] idx = new int[n];
        for (int i = 0; i < n; i++) {
            idx[i] = i;
        }
        sort(a, 0, n - 1, 0, aux, idx);
        return idx;
    }

    // return dth character of s, -1 if d = length of string
    private static int charAt(String s, int d) {
        assert d >= 0 && d <= s.length();
        if (d == s.length()) return -1;
        return s.charAt(d);
    }

    // sort from a[lo] to a[hi], starting at the dth character
    private static void sort(String a, int lo, int hi, int d, int[] aux, int[] idx) {

        // cutoff to insertion sort for small subarrays
        if (hi <= lo + CUTOFF) {
            insertion(a, lo, hi, d, idx);
            return;
        }

        // compute frequency counts
        int[] count = new int[R + 2];
        for (int i = lo; i <= hi; i++) {
            int c;
            if (d == a.length()) {
                c = -1;
            }
            else {
                c = a.charAt((idx[i] + d) % a.length());
            }
            count[c + 2]++;
        }

        // transform counts to indicies
        for (int r = 0; r < R + 1; r++)
            count[r + 1] += count[r];

        // distribute
        for (int i = lo; i <= hi; i++) {
            int c;
            if (d == a.length()) {
                c = -1;
            }
            else {
                c = a.charAt((idx[i] + d) % a.length());
            }
            aux[count[c + 1]++] = idx[i];
        }

        // copy back
        for (int i = lo; i <= hi; i++)
            idx[i] = aux[i - lo];


        // recursively sort for each character (excludes sentinel -1)
        for (int r = 0; r < R; r++) {
            sort(a, lo + count[r], lo + count[r + 1] - 1, d + 1, aux, idx);
        }
    }


    // insertion sort a[lo..hi], starting at dth character
    private static void insertion(String a, int lo, int hi, int d, int[] idx) {
        for (int i = lo; i <= hi; i++)
            for (int j = i; j > lo; j--) {
                if (less(a, idx[j], idx[j - 1], d)) {
                    exch(idx, j, j - 1);
                }
                else {
                    break;
                }
            }
    }

    // exchange a[i] and a[j]
    private static void exch(int[] a, int i, int j) {
        int temp = a[i];
        a[i] = a[j];
        a[j] = temp;
    }

    // is v less than w, starting at character d
    private static boolean less(String s, int v, int w, int d) {
        // assert v.substring(0, d).equals(w.substring(0, d));
        for (int i = d; i < s.length(); i++) {
            if (s.charAt((v + i) % s.length()) < s.charAt((w + i) % s.length()))
                return true;
            if (s.charAt((v + i) % s.length()) > s.charAt((w + i) % s.length()))
                return false;
        }
        return true;
    }

    /**
     * Reads in a sequence of extended ASCII strings from standard input;
     * CircularMSD radix sorts them;
     * and prints them to standard output in ascending order.
     *
     * @param args the command-line arguments
     */
    public static void main(String[] args) {
        // String[] a = StdIn.readAllStrings();
        // int n = a.length;
        // sort(a);
        // for (int i = 0; i < n; i++)
        //     StdOut.println(a[i]);
    }
}

/******************************************************************************
 *  Copyright 2002-2022, Robert Sedgewick and Kevin Wayne.
 *
 *  This file is part of algs4.jar, which accompanies the textbook
 *
 *      Algorithms, 4th edition by Robert Sedgewick and Kevin Wayne,
 *      Addison-Wesley Professional, 2011, ISBN 0-321-57351-X.
 *      http://algs4.cs.princeton.edu
 *
 *
 *  algs4.jar is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  algs4.jar is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with algs4.jar.  If not, see http://www.gnu.org/licenses.
 ******************************************************************************/
