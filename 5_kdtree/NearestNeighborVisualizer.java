/******************************************************************************
 *  Compilation:  javac NearestNeighborVisualizer.java
 *  Execution:    java NearestNeighborVisualizer input.txt
 *  Dependencies: PointSET.java KdTree.java
 *
 *  Read points from a file (specified as a command-line argument) and
 *  draw to standard draw. Highlight the closest point to the mouse.
 *
 *  The nearest neighbor according to the brute-force algorithm is drawn
 *  in red; the nearest neighbor using the kd-tree algorithm is drawn in blue.
 *
 ******************************************************************************/

import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;

public class NearestNeighborVisualizer {

    public static void main(String[] args) {

        // initialize the two data structures with point from file
        String filename = args[0];
        boolean timing = false;
        if (args.length > 1) timing = true;

        In in = new In(filename);
        PointSET brute = new PointSET();
        KdTree kdtree = new KdTree();
        while (!in.isEmpty()) {
            double x = in.readDouble();
            double y = in.readDouble();
            Point2D p = new Point2D(x, y);
            kdtree.insert(p);
            brute.insert(p);
        }

        long startTime, endTime;
        long durationBrute, durationKd;

        // process nearest neighbor queries
        StdDraw.enableDoubleBuffering();
        while (true) {

            if (!timing) {
                // the location (x, y) of the mouse
                double x = StdDraw.mouseX();
                double y = StdDraw.mouseY();
                Point2D query = new Point2D(x, y);

                // draw all of the points
                StdDraw.clear();
                StdDraw.setPenColor(StdDraw.BLACK);
                StdDraw.setPenRadius(0.01);
                brute.draw();

                // draw in red the nearest neighbor (using brute-force algorithm)
                StdDraw.setPenRadius(0.03);
                StdDraw.setPenColor(StdDraw.RED);
                startTime = System.nanoTime();
                brute.nearest(query).draw();
                endTime = System.nanoTime();
                durationBrute = (endTime - startTime);  // divide by 1000000 to get milliseconds.
                StdDraw.setPenRadius(0.02);

                // draw in blue the nearest neighbor (using kd-tree algorithm)
                StdDraw.setPenColor(StdDraw.BLUE);
                startTime = System.nanoTime();
                kdtree.nearest(query).draw();
                endTime = System.nanoTime();
                durationKd = (endTime - startTime);  // divide by 1000000 to get milliseconds.

                StdOut.printf("Brute:\t%f ms\n", durationBrute / 1000000.0);
                StdOut.printf("Kd:\t%f ms\n", durationKd / 1000000.0);
                StdOut.printf("Delta:\t%f\n", (double) durationBrute / durationKd);

                StdDraw.show();
                StdDraw.pause(40);
            }
            else {
                durationBrute = 0;
                durationKd = 0;
                int iterations = 100;
                for (int i = 0; i < iterations; i++) {
                    double x = StdRandom.uniform(0.0, 1.0);
                    double y = StdRandom.uniform(0.0, 1.0);
                    Point2D query = new Point2D(x, y);

                    startTime = System.nanoTime();
                    kdtree.nearest(query);
                    endTime = System.nanoTime();
                    durationKd += (endTime - startTime);

                    startTime = System.nanoTime();
                    brute.nearest(query);
                    endTime = System.nanoTime();
                    durationBrute += (endTime - startTime);
                }
                StdOut.printf("Brute:\t%f queries per second\n",
                              1.0 / ((double) durationBrute / 1.0e9 / iterations));
                StdOut.printf("Kd:\t%f queries per second\n",
                              1.0 / ((double) durationKd / 1.0e9 / iterations));
            }
        }
    }
}
