import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.StdOut;

import java.util.ArrayList;
import java.util.Arrays;

public class FastCollinearPoints {
    private static final int SEG_LENGTH = 4;

    private ArrayList<LineSegment> segments;
    private Point[] my_points;

    public FastCollinearPoints(
            Point[] points) {     // finds all line segments containing 4 or more points
        if (points == null) {
            throw new IllegalArgumentException("points cannot be null");
        }
        my_points = new Point[points.length];
        for (int i = 0; i < points.length; i++) {
            if (points[i] == null) {
                throw new IllegalArgumentException("null point detected");
            }
            my_points[i] = points[i];
        }
        Arrays.sort(my_points, 0, my_points.length);
        segments = new ArrayList<LineSegment>();

        for (int i = 0; i < my_points.length - 1; i++) {
            Point p = my_points[i];

            if (p == null) {
                throw new IllegalArgumentException("null point detected");
            }

            // start at i+1 to skip comparison with self
            Arrays.sort(my_points, i + 1, my_points.length, p.slopeOrder());

            for (int j = i + 1; j <= my_points.length - SEG_LENGTH + 1; j++) {
                Point q1 = my_points[j];
                Point q2 = my_points[j + 1];
                Point q3 = my_points[j + 2];

                check_nulls_and_dupes(p, q1, q2, q3);

                if (p.slopeTo(q1) == p.slopeTo(q2)
                        && p.slopeTo(q1) == p.slopeTo(q3)) {
                    // assumes the initial sort and this sort are stable
                    segments.add(new LineSegment(p, q3));
                }
            }
        }
    }

    // check if there are any nulls or duplicates within a set of 4 points
    private void check_nulls_and_dupes(Point p1, Point p2, Point p3, Point p4) {
        if (p1 == null || p2 == null || p3 == null || p4 == null) {
            throw new IllegalArgumentException("null point detected");
        }

        if (p1.compareTo(p2) == 0 || p1.compareTo(p3) == 0 || p1.compareTo(p4) == 0
                || p2.compareTo(p3) == 0
                || p2.compareTo(p4) == 0
                || p3.compareTo(p4) == 0) {
            throw new IllegalArgumentException("duplicate point detected");
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
        FastCollinearPoints collinear = new FastCollinearPoints(points);
        for (LineSegment segment : collinear.segments()) {
            StdOut.println(segment);
            segment.draw();
        }
        StdDraw.show();
    }
}
