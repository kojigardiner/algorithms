/* *****************************************************************************
 *  Name:
 *  Date:
 *  Description:
 **************************************************************************** */

import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;

import java.util.TreeSet;

public class BoggleSolver {
    private MyTrieSET dict;
    private BoggleBoard board;

    // Initializes the data structure using the given array of strings as the dictionary.
    // (You can assume each word in the dictionary contains only the uppercase letters A through Z.)
    public BoggleSolver(String[] dictionary) {
        int count = 0;

        // Create a trie of dictionary words. Uses MyTrieSet, which is a modified version of TrieSET
        // that fixes the radix at 26.
        dict = new MyTrieSET();
        for (String s : dictionary) {
            dict.add(s);
            count++;
        }
    }

    private void getWordsRecursive(int row, int col, StringBuilder s, boolean marked[][],
                                   TreeSet<String> words) {
        // StdOut.printf("visiting %d %d\n", row, col);
        marked[row][col] = true;
        char letter = board.getLetter(row, col);
        if (letter == 'Q') {
            s.append("QU");
        }
        else {
            s.append(letter);
        }

        // Check if the prefix exists
        boolean prefixExists = dict.doesPrefixExist(s.toString());

        if (prefixExists) {
            // Check if current word is valid
            if (scoreOf(s.toString()) > 0) {
                words.add(s.toString());
            }

            // Visit adjacent tiles
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    int rowTo = row + i;
                    int colTo = col + j;
                    if (rowTo >= 0 && rowTo < board.rows()
                            && colTo >= 0 && colTo < board.cols()) {
                        if (!marked[rowTo][colTo]) {
                            getWordsRecursive(rowTo, colTo, s, marked, words);
                        }
                    }
                }
            }
        }

        // Restore previous state before returning, so we can continue to explore
        // paths that utilize this character
        if (letter == 'Q') {
            s.delete(s.length() - 2, s.length());
        }
        else {
            s.deleteCharAt(s.length() - 1);
        }
        marked[row][col] = false;
    }

    // Returns the set of all valid words in the given Boggle board, as an Iterable.
    public Iterable<String> getAllValidWords(BoggleBoard board) {
        this.board = board;
        TreeSet<String> words = new TreeSet<String>();

        // Do DFS from each tile
        StringBuilder s = new StringBuilder();
        for (int r = 0; r < board.rows(); r++) {
            for (int c = 0; c < board.cols(); c++) {
                s.delete(0, s.length());
                boolean[][] marked = new boolean[board.rows()][board.cols()];
                getWordsRecursive(r, c, s, marked, words);
            }
        }

        return words;
    }

    // Returns the score of the given word if it is in the dictionary, zero otherwise.
    // (You can assume the word contains only the uppercase letters A through Z.)
    public int scoreOf(String word) {
        if (!dict.contains(word)) return 0;
        int len = word.length();
        if (len < 3) return 0;
        if (len <= 4) return 1;
        if (len == 5) return 2;
        if (len == 6) return 3;
        if (len == 7) return 5;
        else return 11;
    }

    public static void main(String[] args) {
        In in = new In(args[0]);
        String[] dictionary = in.readAllStrings();

        BoggleBoard board = new BoggleBoard(args[1]);
        BoggleSolver solver = new BoggleSolver(dictionary);

        // Stopwatch timer = new Stopwatch();
        // double start, end;
        // start = timer.elapsedTime();
        // int iterations = 10;
        // for (int i = 0; i < iterations; i++) {
        //     BoggleBoard board = new BoggleBoard(4, 4);
        //     solver.getAllValidWords(board);
        // }
        // end = timer.elapsedTime();
        // StdOut.printf("%f solves per second\n", 1 / ((end - start) / iterations));

        int score = 0;
        for (String word : solver.getAllValidWords(board)) {
            StdOut.println(word);
            score += solver.scoreOf(word);
        }
        StdOut.println("Score = " + score);
    }
}
