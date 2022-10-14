/******************************************************************************
 *  Compilation:  javac KdTreeGenerator.java
 *  Execution:    java KdTreeGenerator n
 *  Dependencies:
 *
 *  Creates n random points in the unit square and print to standard output.
 *
 *  % java KdTreeGenerator 5
 *  0.195080 0.938777
 *  0.351415 0.017802
 *  0.556719 0.841373
 *  0.183384 0.636701
 *  0.649952 0.237188
 *
 ******************************************************************************/

import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;

public class KdTreeGenerator {

    public static void main(String[] args) {
        KdTree kd = new KdTree();

        long start, end;
        long duration = 0;
        int n = Integer.parseInt(args[0]);
        for (int i = 0; i < n; i++) {
            double x = StdRandom.uniform(0.0, 1.0);
            double y = StdRandom.uniform(0.0, 1.0);
            StdOut.printf("%8.6f %8.6f\n", x, y);
            Point2D p = new Point2D(x, y);
            start = System.nanoTime();
            kd.insert(p);
            end = System.nanoTime();
            duration += end - start;
        }
        StdOut.printf("Total time: %f ms\nTime per insertion: %f ms\n",
                      (double) duration / 1000000.0, (double) duration / 1000000.0 / n);
    }
}
