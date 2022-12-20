/* *****************************************************************************
 *  Name:
 *  Date:
 *  Description:
 **************************************************************************** */

import edu.princeton.cs.algs4.FlowEdge;
import edu.princeton.cs.algs4.FlowNetwork;
import edu.princeton.cs.algs4.FordFulkerson;
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;

import java.util.ArrayList;
import java.util.HashMap;

public class BaseballElimination {
    private int numTeams;
    private HashMap<String, Integer> teams;
    private int[] wins;
    private int[] losses;
    private int[] remaining;
    private int[][] remainingAgainst;
    private boolean[] eliminated;
    private int totalRemaining;

    // create a baseball division from given filename in format specified below
    public BaseballElimination(String filename) {
        In in = new In(filename);
        numTeams = in.readInt();

        teams = new HashMap<String, Integer>();
        wins = new int[numTeams];
        losses = new int[numTeams];
        remaining = new int[numTeams];
        remainingAgainst = new int[numTeams][numTeams];
        eliminated = new boolean[numTeams];
        totalRemaining = 0;

        for (int i = 0; i < numTeams; i++) {
            String team = in.readString();
            // StdOut.printf("%d. %s\n", i, team);
            teams.put(team, i);
            wins[i] = in.readInt();
            losses[i] = in.readInt();
            remaining[i] = in.readInt();
            totalRemaining += remaining[i];
            for (int j = 0; j < numTeams; j++) {
                remainingAgainst[i][j] = in.readInt();
            }
            eliminated[i] = false;
        }

        calcTrivialElimination();

        for (int i = 0; i < numTeams; i++) {
            // Skip if we have already trivially eliminated a team
            if (!eliminated[i]) {
                calcElimination(i);
            }
        }
    }

    // Determines teams that are trivially eliminated. This consists of teams
    // who do not have sufficient total games remaining to catch up to first
    // place.
    private void calcTrivialElimination() {
        int curMax = -1;
        int curFirst = -1;

        // Find the team in first and their number of wins
        for (int i = 0; i < numTeams; i++) {
            if (wins[i] > curMax) {
                curMax = wins[i];
                curFirst = i;
            }
        }

        // Check which remaining teams have insufficient remaining games to
        // catch up
        for (int i = 0; i < numTeams; i++) {
            if (i != curFirst) {
                if (wins[i] + remaining[i] < curMax) {
                    eliminated[i] = true;
                    StdOut.printf("%d eliminated by %d\n", i, curFirst);
                }
            }
        }
    }

    // Determines non-trivial elimination by setting up the maxflow problem for
    // a given team x.
    private void calcElimination(int x) {
        // StdOut.printf("Calc elimination of team %d\n", x);
        // Create a new flow network with 2 artificial source/sink vertices,
        // vertices for each game, and vertices for each team.
        FlowNetwork network = new FlowNetwork(2 + totalRemaining + numTeams);

        // Team indices are 0 through numTeams-1
        // Source and sink indices come next
        // Remaining are game indices
        int source = numTeams;
        int sink = numTeams + 1;
        int gameIdx = numTeams + 2;

        // Use a double loop to iterate over the remainingAgainst[][] matrix,
        // only look at the unique upper half of games in the matrix
        for (int i = 0; i < numTeams; i++) {
            if (i != x) {
                // Add an edge from the team to the sink, with a capacity equal
                // to that of the total possible wins for team x minus the wins
                // for team i.
                FlowEdge e = new FlowEdge(i, sink, wins[x] + remaining[x] - wins[i]);
                // StdOut.println("team to sink: " + e);
                network.addEdge(e);
                for (int j = i + 1; j < numTeams; j++) {
                    if (j != x) {
                        // Add three edges for each game, one from the source to the
                        // game, and one from the game to each possible winner.
                        e = new FlowEdge(source, gameIdx, remainingAgainst[i][j]);
                        // StdOut.println("source to game: " + e);
                        network.addEdge(e);

                        e = new FlowEdge(gameIdx, i, Double.POSITIVE_INFINITY);
                        // StdOut.println("game to team1: " + e);
                        network.addEdge(e);

                        e = new FlowEdge(gameIdx, j, Double.POSITIVE_INFINITY);
                        // StdOut.println("game to team2: " + e);
                        network.addEdge(e);

                        gameIdx++;
                    }
                }
            }
        }
        // StdOut.println(network);
        FordFulkerson ff = new FordFulkerson(network, source, sink);
        // StdOut.printf("%d: maxflow = %f\n", x, ff.value());
        for (int i = 0; i < numTeams; i++) {
            if (i != x) {
                if (ff.inCut(i)) {
                    StdOut.printf("%d eliminated by %d\n", x, i);
                    eliminated[x] = true;
                }
            }
        }
    }

    // number of teams
    public int numberOfTeams() {
        return numTeams;
    }

    // all teams
    public Iterable<String> teams() {
        ArrayList<String> teamsCopy = new ArrayList<String>(teams.keySet());
        return teamsCopy;
    }

    // number of wins for given team
    public int wins(String team) {
        if (!teams.containsKey(team)) {
            throw new IllegalArgumentException("team does not exist");
        }
        return wins[teams.get(team)];
    }

    // number of losses for given team
    public int losses(String team) {
        if (!teams.containsKey(team)) {
            throw new IllegalArgumentException("team does not exist");
        }
        return losses[teams.get(team)];
    }

    // number of remaining games for given team
    public int remaining(String team) {
        if (!teams.containsKey(team)) {
            throw new IllegalArgumentException("team does not exist");
        }
        return remaining[teams.get(team)];
    }

    // number of remaining games between team1 and team2
    public int against(String team1, String team2) {
        if (!teams.containsKey(team1) || !teams.containsKey(team2)) {
            throw new IllegalArgumentException("team does not exist");
        }
        return remainingAgainst[teams.get(team1)][teams.get(team2)];
    }

    // is given team eliminated?
    public boolean isEliminated(String team) {
        if (!teams.containsKey(team)) {
            throw new IllegalArgumentException("team does not exist");
        }
        return eliminated[teams.get(team)];
    }

    // subset R of teams that eliminates given team; null if not eliminated
    public Iterable<String> certificateOfElimination(String team) {
        if (!teams.containsKey(team)) {
            throw new IllegalArgumentException("team does not exist");
        }
        ArrayList<String> cert = new ArrayList<String>();
        // int idx = teams.get(team);

        return cert;
    }

    public static void main(String[] args) {
        BaseballElimination division = new BaseballElimination(args[0]);
        for (String team : division.teams()) {
            if (division.isEliminated(team)) {
                StdOut.print(team + " is eliminated by the subset R = { ");
                for (String t : division.certificateOfElimination(team)) {
                    StdOut.print(t + " ");
                }
                StdOut.println("}");
            }
            else {
                StdOut.println(team + " is not eliminated");
            }
        }
    }
}
