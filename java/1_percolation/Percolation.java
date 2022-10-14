import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;
import edu.princeton.cs.algs4.StdStats;
import edu.princeton.cs.algs4.QuickFindUF;
import edu.princeton.cs.algs4.WeightedQuickUnionUF;

public class Percolation {
    private final int OPEN = 1 << 1;
    private final int TOP_CONNECT = 1 << 2;
    private final int BOTTOM_CONNECT = 1 << 3;

    private int[] flat_grid;
    private int open_sites = 0;
    private boolean does_percolate = false;

    private final int grid_width;
    private final WeightedQuickUnionUF uf;

    // creates n-by-n grid, with all sites initially blocked
    public Percolation(int n) {
        if (n <= 0) {
            throw new IllegalArgumentException("n must be greater than 0");
        }
        grid_width = n;
        flat_grid = new int[grid_width * grid_width + 2];
        uf = new WeightedQuickUnionUF(grid_width * grid_width + 2);  // include two extra indices for the virtual top/bottom

        // close all sites
        for (int i = 0; i < grid_width * grid_width; i++) {
            flat_grid[i] = 0;
        }
    }

    // opens the site (row, col) if it is not open already
    public void open(int row, int col) {
        check_range(row, col);

        int idx = row_col_to_idx(row, col);

        if ((flat_grid[idx] & OPEN) == 0) {    // if site is not already open
            flat_grid[idx] |= OPEN;
            open_sites++;

            // handle top/bottom rows
            if (row == 1) {
                flat_grid[idx] |= TOP_CONNECT;
            }
            if (row == grid_width) {
                flat_grid[idx] |= BOTTOM_CONNECT;
            }

            // create new unions with neighboring open cells
            // left
            if (col > 1) {    // check corner case of leftmost column
                int left_idx = idx - 1;
                update_states(idx, left_idx);
            }
            // right
            if (col < grid_width) {  // check corner case of rightmost column
                int right_idx = idx + 1;
                update_states(idx, right_idx);
            }
            // up
            if (row > 1) {    // check corner case of top row
                int up_idx = idx - grid_width;
                update_states(idx, up_idx);
            }
            // down
            if (row < grid_width) {  // check corner case of bottom row
                int down_idx = idx + grid_width;
                update_states(idx, down_idx);
            }

            if (flat_grid[idx] == (OPEN | TOP_CONNECT | BOTTOM_CONNECT)) {
                does_percolate = true;
            }
        }
    }

    // is the site (row, col) open?
    public boolean isOpen(int row, int col) {
        check_range(row, col);

        int idx = row_col_to_idx(row, col);
        return ((flat_grid[idx] & OPEN) > 0);
    }

    // is the site (row, col) full?
    public boolean isFull(int row, int col) {
        check_range(row, col);

        int idx = row_col_to_idx(row, col);
        return ((flat_grid[uf.find(idx)] & TOP_CONNECT) > 0);
    }

    // returns the number of open sites
    public int numberOfOpenSites() {
        return open_sites;
    }

    // does the system percolate?
    public boolean percolates() {
        return does_percolate;
    }

    private void check_range(int row, int col) {
        if (row < 1 || row > grid_width || col < 1 || col > grid_width) {
            throw new IllegalArgumentException("row/col must be in range [1, n]");
        }
    }

    // Returns the index into the "flattened" grid for a given row/col (1-indexed)
    private int row_col_to_idx(int row, int col) {
        return (row - 1) * grid_width + (col - 1);
    }

    private void update_states(int curr_idx, int neighbor_idx) {
        if ((flat_grid[neighbor_idx] & OPEN) > 0) { // if not blocked
            int curr_root = uf.find(curr_idx);
            int neighbor_root = uf.find(neighbor_idx);
            uf.union(curr_idx, neighbor_idx);
            int new_root = uf.find(curr_idx);
            int new_state = flat_grid[curr_root] | flat_grid[neighbor_root] | flat_grid[new_root] | flat_grid[curr_idx] | flat_grid[neighbor_idx];

            flat_grid[curr_idx] |= new_state;
            flat_grid[neighbor_idx] |= new_state;
            flat_grid[curr_root] |= new_state;
            flat_grid[neighbor_root] |= new_state;
            flat_grid[new_root] |= new_state;
        }
    }

    // test client (optional)
    public static void main(String[] args) {
        int length = 20;
        Percolation p = new Percolation(length);

        for (int row = 1; row <= length; row++) {
            p.open(row, 1);
        }
        for (int row = 1; row <= length; row++) {
            p.open(row, 2);
        }

        StdOut.println("open sites = " + p.numberOfOpenSites());
        if (p.percolates()) {
            StdOut.println("percolates!");
        } else {
            StdOut.println("does not percolate :(");
        }
    }
}
