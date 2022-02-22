// Anthony Galbo
// COP 3503, Spring 2021
// an577845

import java.util.*;
import java.io.*;

public class ConstrainedTopoSort
{
  boolean [][] matrix;

  // Constructor that opens a file and reads a graph into an adjacency matrix.
  public ConstrainedTopoSort(String filename) throws IOException
  {
    int numVerticies, numEdges, temp;

    Scanner in = new Scanner(new File(filename));
    numVerticies = in.nextInt();

    matrix = new boolean[numVerticies][numVerticies];

    for (int i = 0; i < numVerticies; i++)
    {
      numEdges = in.nextInt();

      for (int j = 0; j < numEdges; j++)
      {
        temp = in.nextInt() - 1; // (adjust to range 0 to n - 1)
        matrix[i][temp] = true;
      }
    }
  }

  // Method that calls the dfsIter() wrapper method that returns true if
  // vertex x precedes vertex y, otherwise this method returns false.
  public boolean hasConstrainedTopoSort(int x, int y)
  {
    return dfsIter(x, y);
  }

  // Method that returns the private overloaded dfsIter() method.
  // Note: DFS traversal algorithm can be applied to solve this problem because
  // an alternative algorithm for tropological sort is based on DFS.
  private boolean dfsIter(int start, int end)
  {
    boolean [] visited = new boolean[matrix.length];
    return dfsIter(start - 1, end - 1, visited); // (adjust to range 0 to n - 1)
  }

  // This method returns true if the graph has a valid DFS, otherwise, this
  // method returns false. The skeleton of this code was written by
  // Dr. Szumlanski with the adjustments of using a stack data structure to
  // satisfy LIFO requirements for DFS, and hashset to track start to end vertex.
  private boolean dfsIter(int start, int end, boolean [] visited)
  {
    Stack<Integer> stack = new Stack<>();
    HashSet<Integer> tracker = new HashSet<>();

    // Add end vertex to the stack and hashset. Mark end vertex as visited.
    stack.push(end);
    tracker.add(end);
    visited[end] = true;

    while (!stack.empty())
    {
      // Remove a node from the stack and process it.
      int node = stack.pop();

      // Add all neighbors of 'node' to the stack (as long as they haven't
      // been visited already).
      for (int i = 0; i < matrix.length; i++)
      {
        if (matrix[node][i] && !visited[i])
        {
          visited[i] = true;
          stack.push(i);
          tracker.add(i);
        }
      }
    }

    // Case where start vertex is in the hashSet, this means there is no
    // valid DFS, so return false.
    if (tracker.contains(start))
      return false;

    // We have a valid DFS!
    return true;
  }

  public static double difficultyRating()
  {
    return 3.0;
  }

  public static double hoursSpent()
  {
    return 7.0;
  }
}
