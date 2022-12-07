/* *****************************************************************************
 *  Name:
 *  Date:
 *  Description:
 **************************************************************************** */

import edu.princeton.cs.algs4.Digraph;
import edu.princeton.cs.algs4.DirectedCycle;
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.ST;
import edu.princeton.cs.algs4.StdOut;

import java.util.ArrayList;

public class WordNet {
    private final ST<String, ArrayList<Integer>> st; // key = noun, value = synset ids
    private final ArrayList<String> synsetList;    // id indexed synsets
    private SAP sapObj;

    // constructor takes the name of the two input files
    public WordNet(String synsets, String hypernyms) {
        if (synsets == null || hypernyms == null) {
            throw new IllegalArgumentException("null arg was passed");
        }

        st = new ST<String, ArrayList<Integer>>();
        synsetList = new ArrayList<String>();

        In in = new In(synsets);
        int id = 0;
        while (in.hasNextLine()) {
            String line = in.readLine();
            String[] elems = line.split(",");
            id = Integer.parseInt(elems[0]);
            synsetList.add(elems[1]);
            String[] currSynset = elems[1].split(" ");
            for (int i = 0; i < currSynset.length; i++) {
                ArrayList<Integer> ids = st.get(currSynset[i]);
                if (ids == null) {
                    ids = new ArrayList<Integer>();
                }
                ids.add(id);
                st.put(currSynset[i], ids);
            }
        }

        Digraph g = new Digraph(id + 1);

        in = new In(hypernyms);
        while (in.hasNextLine()) {
            String line = in.readLine();
            String[] elems = line.split(",");
            id = Integer.parseInt(elems[0]);
            for (int i = 1; i < elems.length; i++) {
                g.addEdge(id, Integer.parseInt(elems[i]));
            }
        }

        // Check if graph is a DAG
        DirectedCycle cycle = new DirectedCycle(g);
        if (cycle.hasCycle()) {
            throw new IllegalArgumentException("graph has a cycle");
        }

        // Check if graph is rooted
        int rootsFound = 0;
        for (int v = 0; v < g.V(); v++) {
            if (g.outdegree(v) == 0) {
                rootsFound++;
            }
        }
        if (rootsFound != 1) {
            throw new IllegalArgumentException("graph does not have a single root");
        }

        sapObj = new SAP(g);
    }

    // returns all WordNet nouns
    public Iterable<String> nouns() {
        ArrayList<String> allNouns = new ArrayList<String>();
        for (String n : st) {
            allNouns.add(n);
        }
        return allNouns;
    }

    // is the word a WordNet noun?
    public boolean isNoun(String word) {
        if (word == null) {
            throw new IllegalArgumentException("null arg was passed");
        }
        return (st.contains(word));
    }

    // distance between nounA and nounB (defined below)
    public int distance(String nounA, String nounB) {
        if (nounA == null || nounB == null) {
            throw new IllegalArgumentException("null arg was passed");
        }

        // Check if noun is in WordNet
        if (!(st.contains(nounA) && st.contains(nounB))) {
            throw new IllegalArgumentException("input noun not found in WordNet");
        }

        return sapObj.length(st.get(nounA), st.get(nounB));
    }

    // a synset (second field of synsets.txt) that is the common ancestor of nounA and nounB
    // in a shortest ancestral path (defined below)
    public String sap(String nounA, String nounB) {
        if (nounA == null || nounB == null) {
            throw new IllegalArgumentException("null arg was passed");
        }
        // Check if noun is in WordNet
        if (!(st.contains(nounA) && st.contains(nounB))) {
            throw new IllegalArgumentException("input noun not found in WordNet");
        }

        int ancestorId = sapObj.ancestor(st.get(nounA), st.get(nounB));

        return synsetList.get(ancestorId);
    }

    // do unit testing of this class
    public static void main(String[] args) {
        WordNet wn = new WordNet("synsets3.txt", "hypernyms3InvalidTwoRoots.txt");
        for (String s : wn.nouns()) {
            StdOut.println(s);
        }
        if (wn.isNoun("j")) {
            StdOut.println("j is a WordNet noun");
        }
        else {
            StdOut.println("Failed!");
        }
        if (!wn.isNoun("z")) {
            StdOut.println("z is not a WordNet noun");
        }
        else {
            StdOut.println("Failed!");
        }
    }
}
