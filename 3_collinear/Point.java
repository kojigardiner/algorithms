/******************************************************************************
 *  Compilation:  javac Point.java
 *  Execution:    java Point
 *  Dependencies: none
 *
 *  An immutable data type for points in the plane.
 *  For use on Coursera, Algorithms Part I programming assignment.
 *
 ******************************************************************************/

import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.StdOut;

import java.util.Comparator;

public class Point implements Comparable<Point> {

    private final int x;     // x-coordinate of this point
    private final int y;     // y-coordinate of this point

    /**
     * Initializes a new point.
     *
     * @param x the <em>x</em>-coordinate of the point
     * @param y the <em>y</em>-coordinate of the point
     */
    public Point(int x, int y) {
        /* DO NOT MODIFY */
        this.x = x;
        this.y = y;
    }

    /**
     * Draws this point to standard draw.
     */
    public void draw() {
        /* DO NOT MODIFY */
        StdDraw.point(x, y);
    }

    /**
     * Draws the line segment between this point and the specified point
     * to standard draw.
     *
     * @param that the other point
     */
    public void drawTo(Point that) {
        /* DO NOT MODIFY */
        StdDraw.line(this.x, this.y, that.x, that.y);
    }

    /**
     * Returns the slope between this point and the specified point.
     * Formally, if the two points are (x0, y0) and (x1, y1), then the slope
     * is (y1 - y0) / (x1 - x0). For completeness, the slope is defined to be
     * +0.0 if the line segment connecting the two points is horizontal;
     * Double.POSITIVE_INFINITY if the line segment is vertical;
     * and Double.NEGATIVE_INFINITY if (x0, y0) and (x1, y1) are equal.
     *
     * @param that the other point
     * @return the slope between this point and the specified point
     */
    public double slopeTo(Point that) {
        double y_delta = (that.y - this.y);
        double x_delta = (that.x - this.x);
        double slope;

        if (x_delta == 0.0 && y_delta == 0.0) {     // points are equal
            slope = Double.NEGATIVE_INFINITY;
        }
        else if (x_delta == 0) {                  // vertical
            slope = Double.POSITIVE_INFINITY;
        }
        else if (y_delta == 0) {                  // horizontal
            slope = +0.0;
        }
        else {
            slope = y_delta / x_delta;
        }

        return slope;
    }

    /**
     * Compares two points by y-coordinate, breaking ties by x-coordinate.
     * Formally, the invoking point (x0, y0) is less than the argument point
     * (x1, y1) if and only if either y0 < y1 or if y0 = y1 and x0 < x1.
     *
     * @param that the other point
     * @return the value <tt>0</tt> if this point is equal to the argument
     * point (x0 = x1 and y0 = y1);
     * a negative integer if this point is less than the argument
     * point; and a positive integer if this point is greater than the
     * argument point
     */
    public int compareTo(Point that) {
        double y_delta = (that.y - this.y);
        double x_delta = (that.x - this.x);
        int ret;

        if (x_delta == 0.0 && y_delta == 0.0) {     // points are equal
            ret = 0;
        }
        else if (y_delta == 0.0) {     // need to break the tie with x-coord
            if (x_delta > 0.0) {         // that > this
                ret = -1;
            }
            else {
                ret = +1;
            }
        }
        else if (y_delta > 0.0) {      // that > this
            ret = -1;
        }
        else {
            ret = +1;
        }

        return ret;
    }

    /**
     * Compares two points by the slope they make with this point.
     * The slope is defined as in the slopeTo() method.
     *
     * @return the Comparator that defines this ordering on points
     */
    public Comparator<Point> slopeOrder() {
        return new BySlope(this);
    }

    private static class BySlope implements Comparator<Point> {
        private final Point p;

        private BySlope(Point p) {
            this.p = p;
        }

        public int compare(Point q1, Point q2) {
            double slope1 = p.slopeTo(q1);
            double slope2 = p.slopeTo(q2);
            int ret;

            if (slope1 == slope2) {     // should cover the degenerate case of both slopes being 0
                ret = 0;
            }
            else if (slope1 < slope2) {
                ret = -1;
            }
            else {
                ret = +1;
            }

            return ret;
        }
    }


    /**
     * Returns a string representation of this point.
     * This method is provide for debugging;
     * your program should not rely on the format of the string representation.
     *
     * @return a string representation of this point
     */
    public String toString() {
        /* DO NOT MODIFY */
        return "(" + x + ", " + y + ")";
    }

    /**
     * Unit tests the Point data type.
     */
    public static void main(String[] args) {
        StdOut.println(
                "Testing slope for equal points (0,0) to (0,0), should be negative infinity");
        Point p = new Point(0, 0);
        Point q = new Point(0, 0);
        StdOut.println(p.slopeTo(q));

        StdOut.println("Testing slope for vertical (5,0) to (5,10), should be positive infinity");
        p = new Point(5, 0);
        q = new Point(5, 10);
        StdOut.println(p.slopeTo(q));

        StdOut.println("Testing slope for horizontal (5,10) to (10,10), should be +0.0");
        p = new Point(5, 10);
        q = new Point(10, 10);
        StdOut.println(p.slopeTo(q));

        StdOut.println("Testing negative slope (5,5) to (7,3)");
        p = new Point(5, 5);
        q = new Point(7, 3);
        StdOut.println(p.slopeTo(q));

        StdOut.println("Testing equal compare (2,4) to (2,4), should be 0");
        p = new Point(2, 4);
        q = new Point(2, 4);
        StdOut.println(p.compareTo(q));

        StdOut.println("Testing less than compare (2,1) to (2,3), should be -1");
        p = new Point(2, 1);
        q = new Point(2, 3);
        StdOut.println(p.compareTo(q));

        StdOut.println("Testing less than compare (1,3) to (2,3) with x tie-break, should be -1");
        p = new Point(1, 3);
        q = new Point(2, 3);
        StdOut.println(p.compareTo(q));

        StdOut.println("Testing greater than compare (2,5) to (2,3), should be +1");
        p = new Point(2, 5);
        q = new Point(2, 3);
        StdOut.println(p.compareTo(q));


    }
}
