/* *****************************************************************************
 *  Name:
 *  Date:
 *  Description:
 **************************************************************************** */

import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.Stopwatch;

import java.util.HashSet;

public class BoggleSolver {
    // Use a customized TrieST with radix 26 and "memory" as to the last node visited in the Trie.
    // This makes the prefix & contains methods faster.
    private final MyTrieST<Integer> dict;
    private BoggleBoard myBoard;

    // Initializes the data structure using the given array of strings as the dictionary.
    // (You can assume each word in the dictionary contains only the uppercase letters A through Z.)
    public BoggleSolver(String[] dictionary) {
        // Create a trie of dictionary words. Uses MyTrieSet, which is a modified version of TrieSET
        // that fixes the radix at 26.
        dict = new MyTrieST<Integer>();
        for (String s : dictionary) {
            dict.put(s, calcScore(s));
        }
    }

    // Uses DFS to recursively build a string with neighboring characters and adds words found
    // in the dictionary to the words HashSet
    private void addWordsRecursive(int row, int col, StringBuilder s, boolean[][] marked,
                                   HashSet<String> words) {
        // StdOut.printf("visiting %d %d\n", row, col);
        marked[row][col] = true;
        char letter = myBoard.getLetter(row, col);
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
                    if (rowTo >= 0 && rowTo < myBoard.rows()
                            && colTo >= 0 && colTo < myBoard.cols()) {
                        if (!marked[rowTo][colTo]) {
                            addWordsRecursive(rowTo, colTo, s, marked, words);
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
        dict.reset();
    }

    // Sets marked matrix values to false
    private void clearMarked(boolean[][] marked) {
        for (int i = 0; i < myBoard.rows(); i++) {
            for (int j = 0; j < myBoard.cols(); j++) {
                marked[i][j] = false;
            }
        }
    }

    // Returns the set of all valid words in the given Boggle board, as an Iterable.
    public Iterable<String> getAllValidWords(BoggleBoard board) {
        this.myBoard = board;
        HashSet<String> words = new HashSet<String>();

        // Do DFS from each tile
        StringBuilder s = new StringBuilder();
        boolean[][] marked = new boolean[myBoard.rows()][myBoard.cols()];
        for (int r = 0; r < myBoard.rows(); r++) {
            for (int c = 0; c < myBoard.cols(); c++) {
                dict.reset();
                clearMarked(marked);
                s.delete(0, s.length());
                addWordsRecursive(r, c, s, marked, words);
            }
        }

        return words;
    }

    // Returns the score of the given word if it is in the dictionary, zero otherwise.
    // (You can assume the word contains only the uppercase letters A through Z.)
    public int scoreOf(String word) {
        if (!dict.contains(word)) return 0;
        return (Integer) dict.get(word);
    }

    // Calculates the score for a given word, assuming it is in the dictionary.
    private int calcScore(String word) {
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

        BoggleSolver solver = new BoggleSolver(dictionary);

        Stopwatch timer = new Stopwatch();
        double start, end;
        start = timer.elapsedTime();
        int iterations = 100;
        for (int i = 0; i < iterations; i++) {
            BoggleBoard board = new BoggleBoard(4, 4);
            solver.getAllValidWords(board);
        }
        end = timer.elapsedTime();
        StdOut.printf("%f solves per second\n", 1 / ((end - start) / iterations));

        // BoggleBoard board = new BoggleBoard(args[1]);
        // int score = 0;
        // for (String word : solver.getAllValidWords(board)) {
        //     StdOut.println(word);
        //     score += solver.scoreOf(word);
        // }
        // StdOut.println("Score = " + score);
    }
}


