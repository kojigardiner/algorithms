import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.MinPQ;
import edu.princeton.cs.algs4.Stack;
import edu.princeton.cs.algs4.StdOut;

public class Solver {
    private Node final_node;
    private boolean solvable;
    private Stack<Board> solution_stack;

    // find a solution to the initial board (using the A* algorithm)
    public Solver(Board initial) {
        solvable = false;
        final_node = null;
        solution_stack = null;

        SolverInstance s = new SolverInstance(initial);
        SolverInstance s_twin = new SolverInstance(initial.twin());

        while (true) {
            s.step();
            s_twin.step();

            if (s.solved()) {
                solvable = true;
                final_node = s.get_node();
                break;
            }
            if (s_twin.solved()) {
                solvable = false;
                break;
            }
        }
    }

    private class SolverInstance {
        private MinPQ<Node> pq;
        private Node curr;
        private boolean solved;

        private SolverInstance(Board initial) {
            if (initial == null) throw new IllegalArgumentException("initial board cannot be null");

            solved = false;
            Node n = new Node(initial, null, 0);
            pq = new MinPQ<Node>();
            pq.insert(n);

            curr = null;
        }

        public void step() {
            if (pq.isEmpty()) return;

            curr = pq.delMin();
            if (curr.b.isGoal()) {
                solved = true;
                return;
            }
            for (Board neighbor : curr.b.neighbors()) {
                if (curr.prev == null || !neighbor.equals(curr.prev.b)) {
                    pq.insert(new Node(neighbor, curr, curr.moves + 1));
                }
            }
        }

        public boolean solved() {
            return solved;
        }

        public Node get_node() {
            return curr;
        }
    }

    private class Node implements Comparable<Node> {
        private Board b;
        private Node prev;
        private int moves;
        private int prio;

        private Node(Board b, Node prev, int moves) {
            this.b = b;
            this.prev = prev;
            this.moves = moves;
            this.prio = this.moves + this.b.manhattan();
        }

        public int compareTo(Node that) {
            return this.prio - that.prio;
        }
    }

    // is the initial board solvable? (see below)
    public boolean isSolvable() {
        return solvable;
    }

    // min number of moves to solve initial board; -1 if unsolvable
    public int moves() {
        if (this.isSolvable()) return final_node.moves;
        return -1;
    }

    // sequence of boards in a shortest solution; null if unsolvable
    public Iterable<Board> solution() {
        if (!this.isSolvable()) return null;
        if (solution_stack != null) return solution_stack;

        Stack<Board> solution_stack = new Stack<Board>();

        Node n = final_node;
        while (n != null) {
            solution_stack.push(n.b);
            n = n.prev;
        }
        return solution_stack;
    }

    // test client (see below)
    public static void main(String[] args) {
        // create initial board from file
        In in = new In(args[0]);
        int n = in.readInt();
        int[][] tiles = new int[n][n];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                tiles[i][j] = in.readInt();
        Board initial = new Board(tiles);

        // solve the puzzle
        Solver solver = new Solver(initial);

        // print solution to standard output
        if (!solver.isSolvable())
            StdOut.println("No solution possible");
        else {
            StdOut.println("Minimum number of moves = " + solver.moves());
            for (Board board : solver.solution())
                StdOut.println(board);
        }
    }
}
