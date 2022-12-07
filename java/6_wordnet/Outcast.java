/* *****************************************************************************
 *  Name:
 *  Date:
 *  Description:
 **************************************************************************** */

import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;

public class Outcast {
    private final WordNet wn;

    // constructor takes a WordNet object
    public Outcast(WordNet wordnet) {
        if (wordnet == null) {
            throw new IllegalArgumentException("null arg was passed");
        }

        wn = wordnet;
    }

    // given an array of WordNet nouns, return an outcast
    public String outcast(String[] nouns) {
        int maxDist = -1;
        int currDist;
        String outcast = "";
        for (String n1 : nouns) {
            currDist = 0;
            for (String n2 : nouns) {
                currDist += wn.distance(n1, n2);
            }
            if (currDist > maxDist) {
                maxDist = currDist;
                outcast = n1;
            }
        }

        return outcast;
    }

    // The following test client takes from the command line the name of a
    // synset file, the name of a hypernym file, followed by the names of
    // outcast files, and prints out an outcast in each file:
    public static void main(String[] args) {
        WordNet wordnet = new WordNet(args[0], args[1]);
        Outcast outcast = new Outcast(wordnet);
        for (int t = 2; t < args.length; t++) {
            In in = new In(args[t]);
            String[] nouns = in.readAllStrings();
            StdOut.println(args[t] + ": " + outcast.outcast(nouns));
        }

    }
}
