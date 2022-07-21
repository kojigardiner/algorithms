import edu.princeton.cs.algs4.StdOut;

import java.util.ArrayList;
import java.util.Arrays;

public class Board {
    private final int[][] tiles;    // current set of tiles
    private final int n;            // length of one edge of nxn board

    // create a board from an n-by-n array of tiles,
    // where tiles[row][col] = tile at (row, col)
    public Board(int[][] tiles) {
        if (tiles == null) throw new IllegalArgumentException("tiles cannot be null");

        this.n = tiles.length;
        this.tiles = new int[n][n];

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                this.tiles[i][j] = tiles[i][j];
            }
        }
    }

    // string representation of this board
    public String toString() {
        StringBuilder s = new StringBuilder();
        s.append(this.n + "\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                s.append(String.format("%2d ", this.tiles[i][j]));
            }
            s.append("\n");
        }
        return s.toString();
    }

    // board dimension n
    public int dimension() {
        return this.n;
    }

    // number of tiles out of place
    public int hamming() {
        int sum = 0;

        for (int i = 0; i < this.n; i++) {
            for (int j = 0; j < this.n; j++) {
                if (this.tiles[i][j] != goal(i, j)) sum++;
            }
        }

        return sum;
    }

    // sum of Manhattan distances between tiles and goal
    public int manhattan() {
        int sum = 0;

        for (int i = 0; i < this.n; i++) {
            for (int j = 0; j < this.n; j++) {
                int val = this.tiles[i][j];

                // -1 to account for starting tiles at 1
                int goal_i = (val - 1) / n;
                int goal_j = (val - 1) % n;
                sum += (abs(i - goal_i) + abs(j - goal_j));
            }
        }

        return sum;
    }

    // get goal value for coordinate
    private int goal(int i, int j) {
        if (i == n - 1 && j == n - 1) return 0;
        return i * n + j + 1;    // +1 to start at 1
    }

    // absolute value
    private int abs(int val) {
        if (val < 0) return -1 * val;
        return val;
    }

    // is this board the goal board?
    public boolean isGoal() {
        return (this.hamming() == 0);
    }

    // does this board equal y?
    public boolean equals(Object y) {
        if (y == null) return false;
        if (y.getClass() != this.getClass()) return false;

        // if we've gotten this far, we can safely cast the object to Board
        Board that = (Board) y;
        if (this.dimension() != that.dimension()) return false;

        // if we've gotten this far, we can safely check equality
        return Arrays.deepEquals(this.tiles, that.tiles);
    }

    // all neighboring boards
    public Iterable<Board> neighbors() {
        ArrayList<Board> boards = new ArrayList<Board>();

        for (int i = 0; i < this.n; i++) {
            for (int j = 0; j < this.n; j++) {
                if (this.tiles[i][j] == 0) {    // blank space
                    // left swap
                    if (j > 0) {
                        boards.add(this.swap(i, j, i, j - 1));
                    }
                    // right swap
                    if (j < this.n - 1) {
                        boards.add(this.swap(i, j, i, j + 1));
                    }
                    // add above swap
                    if (i > 0) {
                        boards.add(this.swap(i, j, i - 1, j));
                    }
                    // add below swap
                    if (i < n - 1) {
                        boards.add(this.swap(i, j, i + 1, j));
                    }

                    return boards;
                }
            }
        }
        return null;    // shouldn't get here
    }

    // return a board with tiles swapped
    private Board swap(int i, int j, int inew, int jnew) {
        Board b = new Board(this.tiles);
        int tmp = b.tiles[i][j];
        b.tiles[i][j] = b.tiles[inew][jnew];
        b.tiles[inew][jnew] = tmp;
        return b;
    }

    // a board that is obtained by exchanging any pair of tiles
    public Board twin() {
        // search for a left-right swap
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n - 1; j++) {
                if (this.tiles[i][j] != 0 && this.tiles[i][j + 1] != 0) {
                    return this.swap(i, j, i, j + 1);
                }
            }
        }
        return null;    // should not get here
    }

    // unit testing (not graded)
    public static void main(String[] args) {
        // Constructor
        int n = 3;
        int[][] tiles1 = new int[n][n];
        int[][] tiles2 = new int[n][n];

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                tiles1[i][j] = i * n + j;
                tiles2[i][j] = i * n + j + 1;
            }
        }
        tiles2[n - 1][n - 1] = 0;
        Board b1 = new Board(tiles1);
        Board b2 = new Board(tiles2);
        StdOut.println("First board is off by 1");
        StdOut.println("Second board is in goal state");

        // toString
        StdOut.println("Testing toString()");
        StdOut.println(b1.toString());
        StdOut.println(b2.toString());

        // hamming
        StdOut.println("Testing hamming()");
        StdOut.println(b1.hamming());
        StdOut.println(b2.hamming());

        // manhattan
        StdOut.println("Testing manhattan()");
        StdOut.println(b1.manhattan());
        StdOut.println(b2.manhattan());

        // isGoal
        StdOut.println("Testing isGoal()");
        StdOut.println(b1.isGoal());
        StdOut.println(b2.isGoal());

        // equals
        StdOut.println("Testing equals()");
        StdOut.println(b1.equals(b2));
        StdOut.println(b1.equals(b1));

        // neighbors
        StdOut.println("Testing neighbors()");
        for (Board board : b1.neighbors()) {
            StdOut.println(board.toString());
        }
        for (Board board : b2.neighbors()) {
            StdOut.println(board.toString());
        }

        // twin
        StdOut.println("Testing twin()");
        StdOut.println(b1.twin().toString());
        StdOut.println(b2.twin().toString());
    }
}
