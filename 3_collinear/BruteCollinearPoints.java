import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.StdOut;

import java.util.ArrayList;
import java.util.Arrays;

public class BruteCollinearPoints {
    private static final int SEG_LENGTH = 4;

    private ArrayList<LineSegment> segments;

    public BruteCollinearPoints(Point[] points) {   // finds all line segments containing 4 points
        if (points == null) {
            throw new IllegalArgumentException("points cannot be null");
        }
        Arrays.sort(points, 0, points.length);

        segments = new ArrayList<LineSegment>();

        for (int i = 0; i <= (points.length - SEG_LENGTH); i++) {
            for (int j = i + 1; j <= (points.length - SEG_LENGTH + 1); j++) {
                for (int k = j + 1; k <= (points.length - SEG_LENGTH + 2); k++) {
                    for (int m = k + 1; m <= (points.length - SEG_LENGTH + 3); m++) {
                        Point p = points[i];
                        Point q = points[j];
                        Point r = points[k];
                        Point s = points[m];

                        if (p == q || p == r || p == s || q == r || q == s || r == s) {
                            throw new IllegalArgumentException("equal points detected");
                        }
                        if (p == null || q == null || r == null || s == null) {
                            throw new IllegalArgumentException("null point detected");
                        }

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
