import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;
import edu.princeton.cs.algs4.StdStats;
import edu.princeton.cs.algs4.QuickFindUF;
import edu.princeton.cs.algs4.WeightedQuickUnionUF;
import edu.princeton.cs.algs4.Stopwatch;

public class PercolationStats {

    private final double[] trial_results;
    private final int trial_count;
    private final double CONFIDENCE_95 = 1.96;

    // perform independent trials on an n-by-n grid
    public PercolationStats(int n, int trials) {
        if (n < 1 || trials < 1) {
            throw new IllegalArgumentException("n and trials must be greater than 0");
        }

        trial_count = trials;

        trial_results = new double[trials];

        for (int i = 0; i < trials; i++) {
            Percolation p = new Percolation(n);
            while (true) {  // run until we percolate
                while (true) {  // loop until we are able to open a random site
                    int row = StdRandom.uniform(1, n + 1);
                    int col = StdRandom.uniform(1, n + 1);
                    if (!p.isOpen(row, col)) {
                        p.open(row, col);
                        break;
                    }
                }
                if (p.percolates()) {
                    break;
                }
            }
            trial_results[i] = (double) p.numberOfOpenSites() / (n * n);
        }
    }

    // sample mean of percolation threshold
    public double mean() {
        return StdStats.mean(trial_results);
    }

    // sample standard deviation of percolation threshold
    public double stddev() {
        return StdStats.stddev(trial_results);
    }

    // low endpoint of 95% confidence interval
    public double confidenceLo() {
        return (mean() - CONFIDENCE_95 * stddev() / Math.sqrt(trial_count));
    }

    // high endpoint of 95% confidence interval
    public double confidenceHi() {
        return (mean() + CONFIDENCE_95 * stddev() / Math.sqrt(trial_count));
    }

    // test client (see below)
    public static void main(String[] args) {
        if (args.length < 2) {
            throw new IllegalArgumentException("must provide two arguments, n and T");
        }

        //Stopwatch sw = new Stopwatch();
        PercolationStats ps = new PercolationStats(Integer.parseInt(args[0]), Integer.parseInt(args[1]));
        //StdOut.println("elapsed time = " + sw.elapsedTime());
        StdOut.println("mean = " + ps.mean());
        StdOut.println("stddev = " + ps.stddev());
        StdOut.println("95% confidence interval = [" + ps.confidenceLo() + ", " + ps.confidenceHi() + "]");
    }

}

