/* *****************************************************************************
 *  Name:
 *  Date:
 *  Description:
 **************************************************************************** */

import edu.princeton.cs.algs4.BreadthFirstDirectedPaths;
import edu.princeton.cs.algs4.Digraph;
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

public class SAP {
    private final Digraph g;

    private class AncestorAndDist {
        int ancestor;
        int dist;
    }

    // constructor takes a digraph (not necessarily a DAG)
    public SAP(Digraph G) {
        if (G == null) {
            throw new IllegalArgumentException("null arg was passed");
        }
        g = new Digraph(G); // make a copy so that SAP is immutable
    }

    // Returns the shortest common ancestor of v and w
    private AncestorAndDist findAncestorAndDist(int v, int w) {
        // BFS from each source vertex to every other vertex in the graph
        BreadthFirstDirectedPaths bfsV = new BreadthFirstDirectedPaths(g, v);
        BreadthFirstDirectedPaths bfsW = new BreadthFirstDirectedPaths(g, w);
        int minDist = g.V();    // set to max distance to start
        int currDist = -1;
        int ancestor = -1;
        for (int s = 0; s < g.V(); s++) {
            if (bfsV.hasPathTo(s) && bfsW.hasPathTo(s)) {
                currDist = bfsV.distTo(s) + bfsW.distTo(s);
                if (currDist < minDist) {
                    minDist = currDist;
                    ancestor = s;
                }
            }
        }

        if (ancestor == -1) {
            minDist = -1;
        }
        AncestorAndDist ret = new AncestorAndDist();
        ret.ancestor = ancestor;
        ret.dist = minDist;

        return ret;
    }

    private AncestorAndDist findAncestorAndDist(Iterable<Integer> v, Iterable<Integer> w) {
        // BFS from each source vertex to every other vertex in the graph
        BreadthFirstDirectedPaths bfsV;
        BreadthFirstDirectedPaths bfsW;
        try {
            bfsV = new BreadthFirstDirectedPaths(g, v);
            bfsW = new BreadthFirstDirectedPaths(g, w);
        }
        catch (IllegalArgumentException e) {
            if (e.getMessage().equals("zero vertices")) {
                AncestorAndDist ret = new AncestorAndDist();
                ret.ancestor = -1;
                ret.dist = -1;
                return ret;
            }
            else {
                throw e;
            }
        }
        int minDist = g.V();    // set to max distance to start
        int currDist = -1;
        int ancestor = -1;
        for (int s = 0; s < g.V(); s++) {
            if (bfsV.hasPathTo(s) && bfsW.hasPathTo(s)) {
                currDist = bfsV.distTo(s) + bfsW.distTo(s);
                if (currDist < minDist) {
                    minDist = currDist;
                    ancestor = s;
                }
            }
        }

        if (ancestor == -1) {
            minDist = -1;
        }
        AncestorAndDist ret = new AncestorAndDist();
        ret.ancestor = ancestor;
        ret.dist = minDist;

        return ret;
    }

    // length of shortest ancestral path between v and w; -1 if no such path
    public int length(int v, int w) {
        // Check if vertex is outside prescribed range
        if (v < 0 || w < 0 || v > g.V() - 1 || w > g.V() - 1) {
            throw new IllegalArgumentException("vertex out of bounds");
        }

        AncestorAndDist a = findAncestorAndDist(v, w);
        return a.dist;
    }

    // a common ancestor of v and w that participates in a shortest ancestral path; -1 if no such path
    public int ancestor(int v, int w) {
        // Check if vertex is outside prescribed range
        if (v < 0 || w < 0 || v > g.V() - 1 || w > g.V() - 1) {
            throw new IllegalArgumentException("vertex out of bounds");
        }

        AncestorAndDist a = findAncestorAndDist(v, w);
        return a.ancestor;
    }

    // length of shortest ancestral path between any vertex in v and any vertex in w; -1 if no such path
    public int length(Iterable<Integer> v, Iterable<Integer> w) {
        if (v == null || w == null) {
            throw new IllegalArgumentException("null arg was passed");
        }
        // Check if iterable contains any null items

        AncestorAndDist a = findAncestorAndDist(v, w);
        return a.dist;
    }

    // a common ancestor that participates in shortest ancestral path; -1 if no such path
    public int ancestor(Iterable<Integer> v, Iterable<Integer> w) {
        if (v == null || w == null) {
            throw new IllegalArgumentException("null arg was passed");
        }
        // Check if iterable contains any null items

        AncestorAndDist a = findAncestorAndDist(v, w);
        return a.ancestor;
    }

    // The following test client takes the name of a digraph input file as as a
    // command-line argument, constructs the digraph, reads in vertex pairs from
    // standard input, and prints out the length of the shortest ancestral path
    // between the two vertices and a common ancestor that participates in that
    // path.
    public static void main(String[] args) {
        In in = new In(args[0]);
        Digraph G = new Digraph(in);
        SAP sap = new SAP(G);
        while (!StdIn.isEmpty()) {
            int v = StdIn.readInt();
            int w = StdIn.readInt();
            int length = sap.length(v, w);
            int ancestor = sap.ancestor(v, w);
            StdOut.printf("length = %d, ancestor = %d\n", length, ancestor);
        }
    }
}
