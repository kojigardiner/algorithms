import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.StdOut;

import java.util.ArrayList;
import java.util.Arrays;

public class FastCollinearPoints {
    private static final int SEG_LENGTH = 4;

    private ArrayList<LineSegment> segments;

    public FastCollinearPoints(
            Point[] points) {     // finds all line segments containing 4 or more points
        if (points == null) {
            throw new IllegalArgumentException("points cannot be null");
        }
        Arrays.sort(points, 0, points.length);
        segments = new ArrayList<LineSegment>();

        for (int i = 0; i < points.length - 1; i++) {
            Point p = points[i];
            Arrays.sort(points, i + 1, points.length, p.slopeOrder());

            for (int j = i + 1; j <= points.length - SEG_LENGTH + 1; j++) {
                if (p.slopeTo(points[j]) == p.slopeTo(points[j + 1])
                        && p.slopeTo(points[j]) == p.slopeTo(points[j + 2])) {
                    // assumes the initial sort and this sort are stable
                    segments.add(new LineSegment(p, points[j + 2]));
                }
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
