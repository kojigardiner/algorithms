import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.StdOut;

import java.util.ArrayList;
import java.util.Arrays;

public class FastCollinearPoints {
    private static final int SEG_LENGTH = 4;
    private static final boolean DEBUG = false;

    private ArrayList<LineSegment> segments;
    private Point[] my_points_natural, my_points_slope;

    public FastCollinearPoints(
            Point[] points) {     // finds all line segments containing 4 or more points
        if (points == null) {
            throw new IllegalArgumentException("points cannot be null");
        }

        // create a new array that we will modify, checking for nulls as we go
        my_points_natural = dupe_array_and_check_nulls(points);
        Arrays.sort(my_points_natural, 0, my_points_natural.length);    // sort by natural order
        check_dupes(my_points_natural);

        segments = new ArrayList<LineSegment>();        // storage for segments
        ArrayList<Point> seg_points = new ArrayList<Point>();   // temp storage for seg creation

        for (int i = 0; i < my_points_natural.length - 1; i++) {
            Point p = my_points_natural[i];

            // sort by slope with p, start at i+1 to skip comparison with self
            int j = 0;

            // always take the original natural-sorted array as our base
            // this ensures stability after the slopeorder sort
            my_points_slope = Arrays.copyOf(my_points_natural, my_points_natural.length);
            Arrays.sort(my_points_slope, 0, my_points_slope.length, p.slopeOrder());

            if (DEBUG) {
                StdOut.println("pivot = " + p);
                for (int z = j; z < my_points_slope.length; z++) {
                    StdOut.println(my_points_slope[z] + " slope " + p.slopeTo(my_points_slope[z]));
                }
            }

            // look at each remaining point and check slope
            while (j < my_points_slope.length) {
                // only proceed if the pivot point is the smallest
                // otherwise we may be looking at a subsegment
                if (p.compareTo(my_points_slope[j]) < 0) {
                    seg_points.clear();     // reset the points we are considering

                    // count how many points have equal slope to current index
                    int count = 1;
                    while (j + count < my_points_slope.length &&
                            p.slopeTo(my_points_slope[j]) == p.slopeTo(
                                    my_points_slope[j + count])) {
                        count++;
                    }
                    if (count >= SEG_LENGTH - 1) {
                        if (DEBUG) StdOut.println("segment found, length " + (count + 1));

                        segments.add(new LineSegment(p, my_points_slope[j + count - 1]));
                    }
                    j = j + count;  // skip ahead since there were no matches
                }
                else {
                    j++;
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
        FastCollinearPoints collinear = new FastCollinearPoints(points);
        for (LineSegment segment : collinear.segments()) {
            StdOut.println(segment);
            segment.draw();
        }
        StdDraw.show();
    }
}
