/* *****************************************************************************
 *  Name:
 *  Date:
 *  Description:
 **************************************************************************** */

import edu.princeton.cs.algs4.Picture;

import java.awt.Color;

public class SeamCarver {
    private Picture picture;

    // create a seam carver object based on the given picture
    public SeamCarver(Picture picture) {
        if (picture == null) {
            throw new IllegalArgumentException("argument cannot be null");
        }
        this.picture = new Picture(picture);
    }

    // current picture
    public Picture picture() {
        return this.picture;
    }

    // width of current picture
    public int width() {
        return this.picture.width();
    }

    // height of current picture
    public int height() {
        return this.picture.height();
    }

    // energy of pixel at column x and row y
    public double energy(int x, int y) {
        if (x < 0 || x > this.picture.width() - 1 || y < 0 || y > this.picture.height() - 1) {
            throw new IllegalArgumentException("coordinates out of bounds");
        }

        return computeEnergy(x, y);
    }

    // Generic operation to find a seam
    private int[] findVertSeam(boolean transpose) {
        int width = transpose ? this.picture.height() : this.picture.width();
        int height = transpose ? this.picture.width() : this.picture.height();

        int[] seam = new int[height];

        double[][] energy = new double[width][height];
        double[][] distTo = new double[width][height];
        int[][] edgeTo = new int[width][height];

        int minCol = -1;
        double minDist = Double.POSITIVE_INFINITY;

        // Compute the min path from each starting vertex
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                energy[x][y] = transpose ? computeEnergy(y, x) : computeEnergy(x, y);

                // First row
                if (y == 0) {
                    distTo[x][y] = energy[x][y];
                    edgeTo[x][y] = -1;

                }
                // First/last col
                else if (x == 0 || x == width - 1) {
                    distTo[x][y] = distTo[x][y - 1] + energy[x][y];
                    edgeTo[x][y] = x;
                }
                else {
                    double a, b, c;
                    a = distTo[x - 1][y - 1] + energy[x][y];
                    b = distTo[x][y - 1] + energy[x][y];
                    c = distTo[x + 1][y - 1] + energy[x][y];
                    if (a < b && a < c) {
                        edgeTo[x][y] = x - 1;
                        distTo[x][y] = a;
                    }
                    else if (b < a && b < c) {
                        edgeTo[x][y] = x;
                        distTo[x][y] = b;
                    }
                    else {
                        edgeTo[x][y] = x + 1;
                        distTo[x][y] = c;
                    }
                }
                // Last row
                if (y == height - 1) {
                    if (distTo[x][y] < minDist) {
                        minDist = distTo[x][y];
                        minCol = x;
                    }
                }
            }
        }

        // Follow the edgeTo array back up from the minCol
        // back up to find the seam.
        for (int i = height - 1; i >= 0; i--) {
            seam[i] = minCol;
            minCol = edgeTo[minCol][i];
        }

        return seam;
    }

    // sequence of indices for horizontal seam
    public int[] findHorizontalSeam() {
        return findVertSeam(true);
    }

    // sequence of indices for vertical seam
    public int[] findVerticalSeam() {
        return findVertSeam(false);
    }

    // remove horizontal seam from current picture
    public void removeHorizontalSeam(int[] seam) {
        if (seam == null) {
            throw new IllegalArgumentException("argument cannot be null");
        }
        if (!checkSeam(seam, this.picture.width())) {
            throw new IllegalArgumentException("invalid seam");
        }
        if (picture.height() <= 1) {
            throw new IllegalArgumentException("picture width is too small");
        }
        Picture pictureNew = new Picture(this.picture.width(), this.picture.height() - 1);
        for (int x = 0; x < pictureNew.width(); x++) {
            int curSeam = seam[x];
            for (int y = 0; y < pictureNew.height(); y++) {
                if (y < curSeam) {
                    pictureNew.setRGB(x, y, this.picture.getRGB(x, y));
                }
                else if (y >= curSeam) {
                    pictureNew.setRGB(x, y, this.picture.getRGB(x, y + 1));
                }
            }
        }
        this.picture = pictureNew;
    }

    // remove vertical seam from current picture
    public void removeVerticalSeam(int[] seam) {
        if (seam == null) {
            throw new IllegalArgumentException("argument cannot be null");
        }
        if (!checkSeam(seam, this.picture.height())) {
            throw new IllegalArgumentException("invalid seam");
        }
        if (picture.width() <= 1) {
            throw new IllegalArgumentException("picture width is too small");
        }
        Picture pictureNew = new Picture(this.picture.width() - 1, this.picture.height());
        for (int y = 0; y < pictureNew.height(); y++) {
            int curSeam = seam[y];
            for (int x = 0; x < pictureNew.width(); x++) {
                if (x < curSeam) {
                    pictureNew.setRGB(x, y, this.picture.getRGB(x, y));
                }
                else if (x >= curSeam) {
                    pictureNew.setRGB(x, y, this.picture.getRGB(x + 1, y));
                }
            }
        }
        this.picture = pictureNew;
    }

    // Checks if a seam is valid
    private boolean checkSeam(int[] seam, int maxCoord) {
        if (seam.length != maxCoord) {
            return false;
        }
        for (int i = 1; i < seam.length; i++) {
            if (Math.abs(seam[i] - seam[i - 1]) > 1) {
                return false;
            }
        }

        return true;
    }

    // Computes the energy at a given pixel coordinate
    private double computeEnergy(int x, int y) {
        if (x == 0 || x == this.picture.width() - 1 || y == 0 || y == this.picture.height() - 1) {
            return 1000;
        }

        Color left = this.picture.get(x - 1, y);
        Color right = this.picture.get(x + 1, y);
        double deltaSqX = Math.pow(left.getRed() - right.getRed(), 2) +
                Math.pow(left.getGreen() - right.getGreen(), 2) +
                Math.pow(left.getBlue() - right.getBlue(), 2);

        Color up = this.picture.get(x, y - 1);
        Color down = this.picture.get(x, y + 1);
        double deltaSqY = Math.pow(up.getRed() - down.getRed(), 2) +
                Math.pow(up.getGreen() - down.getGreen(), 2) +
                Math.pow(up.getBlue() - down.getBlue(), 2);

        return Math.sqrt(deltaSqX + deltaSqY);
    }

    public static void main(String[] args) {

    }
}
