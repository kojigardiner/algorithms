import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.StdOut;

import java.util.ArrayList;
import java.util.Arrays;

public class BruteCollinearPoints {
    private static final int SEG_LENGTH = 4;

    private ArrayList<LineSegment> segments;
    private Point[] my_points;

    public BruteCollinearPoints(Point[] points) {   // finds all line segments containing 4 points
        if (points == null) {
            throw new IllegalArgumentException("points cannot be null");
        }
        my_points = dupe_array_and_check_nulls(points);
        Arrays.sort(my_points, 0, my_points.length);
        check_dupes(my_points);

        segments = new ArrayList<LineSegment>();

        for (int i = 0; i <= (my_points.length - SEG_LENGTH); i++) {
            for (int j = i + 1; j <= (my_points.length - SEG_LENGTH + 1); j++) {
                for (int k = j + 1; k <= (my_points.length - SEG_LENGTH + 2); k++) {
                    for (int m = k + 1; m <= (my_points.length - SEG_LENGTH + 3); m++) {
                        Point p = my_points[i];
                        Point q = my_points[j];
                        Point r = my_points[k];
                        Point s = my_points[m];

                        //                        StdOut.println(p + "->" + q + "->" + r + "->" + s);
                        //                        StdOut.println(p.slopeTo(q) + "/" + p.slopeTo(r) + "/" + p.slopeTo(s));
                        if (p.slopeTo(q) == p.slopeTo(r) && p.slopeTo(q) == p.slopeTo(s)) {
                            //                            StdOut.println("collinear");
                            segments.add(new LineSegment(p, s));
                        }
                    }
                }
            }
        }
    }

    // duplicate an array and check all elements for nulls
    private Point[] dupe_array_and_check_nulls(Point[] points) {
        Point[] dupe = new Point[points.length];
        for (int i = 0; i < points.length; i++) {
            if (points[i] == null) {
                throw new IllegalArgumentException("null point detected");
            }
            dupe[i] = points[i];
        }
        return dupe;
    }

    // check if there are any duplicate points. assumes points have been sorted.
    private void check_dupes(Point[] sorted_points) {
        for (int i = 1; i < sorted_points.length; i++) {
            if (sorted_points[i - 1].compareTo(sorted_points[i]) == 0) {
                throw new IllegalArgumentException("duplicate point detected");
            }
        }
    }

    public int numberOfSegments() {       // the number of line segments
        return segments.size();
    }

    public LineSegment[] segments() {                // the line segments
        LineSegment[] seg_array = new LineSegment[segments.size()];
        for (int i = 0; i < segments.size(); i++) {
            seg_array[i] = segments.get(i);
        }
        return seg_array;
    }

    public static void main(String[] args) {

        // read the n points from a file
        In in = new In(args[0]);
        int n = in.readInt();
        Point[] points = new Point[n];
        for (int i = 0; i < n; i++) {
            int x = in.readInt();
            int y = in.readInt();
            points[i] = new Point(x, y);
        }

        // draw the points
        StdDraw.enableDoubleBuffering();
        StdDraw.setXscale(0, 32768);
        StdDraw.setYscale(0, 32768);
        for (Point p : points) {
            p.draw();
        }
        StdDraw.show();

        // print and draw the line segments
        BruteCollinearPoints collinear = new BruteCollinearPoints(points);
        for (LineSegment segment : collinear.segments()) {
            StdOut.println(segment);
            segment.draw();
        }
        StdDraw.show();
    }
}
