import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.StdOut;

import java.util.ArrayList;
import java.util.Arrays;

public class FastCollinearPoints {
    private static final int SEG_LENGTH = 4;
    private static final boolean DEBUG = false;

    private ArrayList<LineSegment> segments;
    private Point[] my_points;

    public FastCollinearPoints(
            Point[] points) {     // finds all line segments containing 4 or more points
        if (points == null) {
            throw new IllegalArgumentException("points cannot be null");
        }

        // create a new array that we will modify, checking for nulls as we go
        my_points = new Point[points.length];
        for (int i = 0; i < points.length; i++) {
            if (points[i] == null) {
                throw new IllegalArgumentException("null point detected");
            }
            my_points[i] = points[i];
        }
        Arrays.sort(my_points, 0, my_points.length);    // sort by natural order

        segments = new ArrayList<LineSegment>();        // storage for segments
        ArrayList<Point> seg_points = new ArrayList<Point>();   // temp storage for seg creation

        for (int i = 0; i < my_points.length - 1; i++) {
            Point p = my_points[i];
            // check for dupes here since we are sorted in natural order
            if (p.compareTo(my_points[i + 1]) == 0) {
                throw new IllegalArgumentException("duplicate point detected");
            }

            // sort by slope with p, start at i+1 to skip comparison with self
            int j = i + 1;
            Arrays.sort(my_points, j, my_points.length, p.slopeOrder());

            if (DEBUG) {
                StdOut.println("pivot = " + p);
                for (int z = j; z < my_points.length; z++) {
                    StdOut.println(my_points[z] + " slope " + p.slopeTo(my_points[z]));
                }
            }

            // look at each remaining point and check slope
            while (j < my_points.length) {
                seg_points.clear();     // reset the points we are considering

                // count how many points have equal slope to current index
                int count = 1;
                while (j + count < my_points.length &&
                        p.slopeTo(my_points[j]) == p.slopeTo(my_points[j + count])) {
                    seg_points.add(my_points[j + count]);
                    count++;
                }
                if (count >= SEG_LENGTH - 1) {
                    if (DEBUG) StdOut.println("segment found, length " + (count + 1));
                    // add the first comparison points here
                    seg_points.add(p);
                    seg_points.add(my_points[j]);
                    segments.add(create_segment(seg_points));
                }
                j = j + count;  // skip ahead since there were no matches
            }
        }
    }

    private LineSegment create_segment(ArrayList<Point> points) {
        Point[] points_array = points.toArray(new Point[0]);
        Arrays.sort(points_array);

        return new LineSegment(points_array[0], points_array[points_array.length - 1]);
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
