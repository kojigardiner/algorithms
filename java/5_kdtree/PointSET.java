import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.RectHV;
import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.StdOut;

import java.util.ArrayList;
import java.util.TreeSet;

public class PointSET {
    private TreeSet<Point2D> set;

    public PointSET() {                             // construct an empty set of points
        set = new TreeSet<Point2D>();
    }

    public boolean isEmpty() {                      // is the set empty?
        return set.isEmpty();
    }

    public int size() {                             // number of points in the set
        return set.size();
    }

    public void insert(
            Point2D p) {                 // add the point to the set (if it is not already in the set)
        if (p == null) throw new IllegalArgumentException("p cannot be null");
        set.add(p);
    }

    public boolean contains(Point2D p) {            // does the set contain point p?
        if (p == null) throw new IllegalArgumentException("p cannot be null");
        return set.contains(p);
    }

    public void draw() {                            // draw all points to standard draw
        if (isEmpty()) return;

        StdDraw.setPenColor(StdDraw.BLACK);
        StdDraw.setPenRadius(0.01);

        for (Point2D p : set) {
            StdDraw.point(p.x(), p.y());
        }
    }

    public Iterable<Point2D> range(
            RectHV rect) {                          // all points that are inside the rectangle (or on the boundary)
        if (rect == null) throw new IllegalArgumentException("p cannot be null");
        if (isEmpty()) return null;

        ArrayList<Point2D> points = new ArrayList<Point2D>();

        for (Point2D d : set) {
            if (rect.contains(d)) points.add(d);
        }

        return points;
    }

    public Point2D nearest(
            Point2D p) {                            // a nearest neighbor in the set to point p; null if the set is empty
        if (p == null) throw new IllegalArgumentException("p cannot be null");
        if (isEmpty()) return null;

        Point2D closest = null;
        double closest_dist = Double.POSITIVE_INFINITY;
        for (Point2D d : set) {
            double dist = p.distanceSquaredTo(d);
            if (closest == null || dist < closest_dist) {
                closest_dist = dist;
                closest = d;
            }
        }
        return closest;
    }

    public static void main(
            String[] args) {                        // unit testing of the methods (optional)

        PointSET ps = new PointSET();

        ps.insert(new Point2D(0.5, 0.5));
        ps.insert(new Point2D(0.25, 0.25));
        ps.insert(new Point2D(0.1, 0.5));
        ps.draw();

        StdOut.printf("Size = %d\n", ps.size());

        Point2D p = new Point2D(0.6, 0.8);
        StdOut.printf("Nearest to %s is %s\n", p.toString(), ps.nearest(p).toString());

        RectHV rect = new RectHV(0.8, 0.8, 0.9, 0.9);
        StdOut.printf("Points in %s\n", rect.toString());
        for (Point2D q : ps.range(rect)) {
            StdOut.printf("%s\n", q.toString());
        }

        rect = new RectHV(0, 0, 1, 1);
        StdOut.printf("Points in %s\n", rect.toString());
        for (Point2D q : ps.range(rect)) {
            StdOut.printf("%s\n", q.toString());
        }
    }
}
