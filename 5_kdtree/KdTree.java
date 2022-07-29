import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.RectHV;
import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.StdOut;

import java.util.ArrayList;

public class KdTree {
    private static class Node {
        private Point2D p;      // the point
        private RectHV rect;    // the axis-aligned rectangle corresponding to this node
        private Node lb;        // the left/bottom subtree
        private Node rt;        // the right/top subtree

        private Node(Point2D p, RectHV rect, Node lb, Node rt) {
            this.p = p;
            this.rect = rect;
            this.lb = lb;
            this.rt = rt;
        }
    }

    private Node root = null;
    private int n = 0;
    private int dim = 2;  // dimensions of tree (2d)

    public KdTree() {                             // construct an empty set of points
    }

    public boolean isEmpty() {                      // is the set empty?
        return size() == 0;
    }

    public int size() {                             // number of points in the set
        return n;
    }

    public void insert(
            Point2D p) {                 // add the point to the set (if it is not already in the set)
        if (p == null) throw new IllegalArgumentException("p cannot be null");

        root = put(root, 0, 1, 0, 1, p, 0);
    }

    // Recursive call to place a new node on the tree rooted at r
    // xmin/xmax/ymin/ymax are the rect bounds for the parent node
    // level = depth of tree
    private Node put(Node r, double xmin, double xmax, double ymin, double ymax, Point2D p,
                     int level) {
        if (r == null) {    // reached bottom of tree, add a node
            n++;
            RectHV rect = new RectHV(xmin, ymin, xmax, ymax);
            return new Node(p, rect, null, null);
        }
        if (p.equals(r.p)) return r;     // point already exists in tree

        // logic for rects:
        // if we are a vertical split, the x-coord will be the split position
        //      for the left side, the split position is xmax, for the right side it is xmin
        // if we are on a horizontal split, the y-coord will be the split position
        //      for the bottom side, the split position is ymax, for the top side it is ymin
        // all other rects coords carry over from the parent node
        if (level == 0) { // vertical split
            if (p.x() < r.p.x()) r.lb = put(r.lb, xmin, r.p.x(), ymin, ymax, p, (level + 1) % dim);
            else r.rt = put(r.rt, r.p.x(), xmax, ymin, ymax, p, (level + 1) % dim);
        }
        else {  // horizontal split
            if (p.y() < r.p.y()) r.lb = put(r.lb, xmin, xmax, ymin, r.p.y(), p, (level + 1) % dim);
            else r.rt = put(r.rt, xmin, xmax, r.p.y(), ymax, p, (level + 1) % dim);
        }
        return r;
    }

    public boolean contains(Point2D p) {            // does the set contain point p?
        if (p == null) throw new IllegalArgumentException("p cannot be null");

        return get(root, p, 0) != null;
    }

    private Node get(Node r, Point2D p, int level) {
        if (r == null) return null; // point not found
        if (p.equals(r.p)) return r;    // point found
        if (level == 0) { // vertical compare (x)
            if (p.x() < r.p.x()) return get(r.lb, p, (level + 1) % dim);
            else return get(r.rt, p, (level + 1) % dim);
        }
        else {  // horizontal compare (y)
            if (p.y() < r.p.y()) return get(r.lb, p, (level + 1) % dim);
            else return get(r.rt, p, (level + 1) % dim);
        }
    }

    public void draw() {                            // draw all points to standard draw
        if (isEmpty()) return;

        draw(root, 0);
    }

    private void draw(Node r, int level) {
        if (r == null) return;

        StdDraw.setPenColor(StdDraw.BLACK);
        StdDraw.setPenRadius(0.01);
        StdDraw.point(r.p.x(), r.p.y());

        StdDraw.setPenRadius();
        if (level == 0) {   // vertical split
            StdDraw.setPenColor(StdDraw.RED);
            StdDraw.line(r.p.x(), r.rect.ymin(), r.p.x(), r.rect.ymax());
        }
        else {  // horizontal split
            StdDraw.setPenColor(StdDraw.BLUE);
            StdDraw.line(r.rect.xmin(), r.p.y(), r.rect.xmax(), r.p.y());
        }
        draw(r.lb, (level + 1) % dim);
        draw(r.rt, (level + 1) % dim);
    }

    public Iterable<Point2D> range(
            RectHV rect) {                          // all points that are inside the rectangle (or on the boundary)
        if (rect == null) throw new IllegalArgumentException("p cannot be null");
        if (isEmpty()) return null;

        ArrayList<Point2D> points = new ArrayList<Point2D>();

        range(root, rect, points);

        return points;
    }

    // recursively search for points in the query rect
    private void range(Node r, RectHV rect, ArrayList<Point2D> points) {
        if (r == null) return;
        if (!r.rect.intersects(rect)) return;
        if (rect.contains(r.p)) points.add(r.p);
        range(r.lb, rect, points);
        range(r.rt, rect, points);
    }

    public Point2D nearest(
            Point2D p) {                            // a nearest neighbor in the set to point p; null if the set is empty
        if (p == null) throw new IllegalArgumentException("p cannot be null");
        if (isEmpty()) return null;

        // StdOut.println("Starting search");
        Closest closest = nearest(root, p, new Closest(), 0);

        return closest.p;
    }

    // Store the closest point along with its distance to avoid recalculating each time
    private static class Closest {
        private Point2D p = null;
        private double dist = Double.POSITIVE_INFINITY;
    }

    // Recursively search for closest point
    private Closest nearest(Node r, Point2D p, Closest closest, int level) {
        if (r == null) return closest;  // we've reached the end of the tree

        // initialize first time
        if (closest.p == null) {
            closest.p = r.p;
            closest.dist = r.p.distanceSquaredTo(p);
        }

        // short circuit if this subtree could have no points in it closer than what we've already found
        if (closest.dist < r.rect.distanceSquaredTo(p)) {
            return closest;
        }
        // StdOut.printf("Curr: %s\n", r.p.toString());
        double curr_dist = r.p.distanceSquaredTo(p);

        if (curr_dist < closest.dist) {
            closest.p = r.p;
            closest.dist = curr_dist;
            // StdOut.printf("Closest! dist: %f\n", curr_dist);
        }

        // optimize order of search based on where the search point is wrt current split line
        if (level == 0) {   // vertical split line
            if (p.x() < r.p.x()) {
                closest = nearest(r.lb, p, closest, (level + 1) % dim);
                closest = nearest(r.rt, p, closest, (level + 1) % dim);
            }
            else {
                closest = nearest(r.rt, p, closest, (level + 1) % dim);
                closest = nearest(r.lb, p, closest, (level + 1) % dim);
            }
        }
        else {  // horizontal split line
            if (p.y() < r.p.y()) {
                closest = nearest(r.lb, p, closest, (level + 1) % dim);
                closest = nearest(r.rt, p, closest, (level + 1) % dim);
            }
            else {
                closest = nearest(r.rt, p, closest, (level + 1) % dim);
                closest = nearest(r.lb, p, closest, (level + 1) % dim);
            }

        }

        return closest;
    }

    public static void main(
            String[] args) {                        // unit testing of the methods (optional)

        KdTree kd = new KdTree();

        kd.insert(new Point2D(0.206107, 0.095492));
        kd.insert(new Point2D(0.975528, 0.654508));
        kd.insert(new Point2D(0.024472, 0.345492));
        kd.insert(new Point2D(0.793893, 0.095492));
        kd.insert(new Point2D(0.793893, 0.904508));
        kd.insert(new Point2D(0.975528, 0.345492));
        kd.insert(new Point2D(0.206107, 0.904508));
        kd.insert(new Point2D(0.500000, 0.000000));
        kd.insert(new Point2D(0.024472, 0.654508));
        kd.insert(new Point2D(0.500000, 1.000000));
        kd.draw();

        // kd.insert(new Point2D(0.7, 0.2));
        // kd.insert(new Point2D(0.5, 0.4));
        // kd.insert(new Point2D(0.2, 0.3));
        // kd.insert(new Point2D(0.4, 0.7));
        // kd.insert(new Point2D(0.9, 0.6));
        // kd.insert(new Point2D(0.5, 0.4));   // duplicate
        // kd.draw();
        //
        // StdOut.printf("Size = %d\n", kd.size());    // should be 5
        //
        // StdOut.println(kd.contains(new Point2D(0.7, 0.2))); // true
        // StdOut.println(kd.contains(new Point2D(0.4, 0.7))); // true
        // StdOut.println(kd.contains(new Point2D(0.5, 0.5))); // false
        // StdOut.println(kd.contains(new Point2D(0.1, 0.5))); // false

        RectHV rect = new RectHV(0.8, 0.8, 0.9, 0.9);
        StdOut.printf("Points in %s\n", rect.toString());
        for (Point2D q : kd.range(rect)) {
            StdOut.printf("%s\n", q.toString());
        }

        rect = new RectHV(0, 0, 1, 1);
        StdOut.printf("Points in %s\n", rect.toString());
        for (Point2D q : kd.range(rect)) {
            StdOut.printf("%s\n", q.toString());
        }

        rect = new RectHV(0, 0.5, 1, 1);
        StdOut.printf("Points in %s\n", rect.toString());
        for (Point2D q : kd.range(rect)) {
            StdOut.printf("%s\n", q.toString());
        }

        Point2D p = new Point2D(0.0, 0.0);
        StdOut.printf("Nearest to %s is %s\n", p.toString(), kd.nearest(p).toString());
    }
}
