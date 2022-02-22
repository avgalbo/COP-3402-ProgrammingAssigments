// Sean Szumlanski
// COP 3503, Spring 2021

// ====================================
// ConstrainedTopoSort: TestCase04.java
// ====================================
// A small test case for ConstrainedTopoSort.


import java.io.*;
import java.util.*;

public class TestCase07
{
	private static void failwhale(String params)
	{
		System.out.println("Test Case #7: hasConstrainedTopoSort(" + params + "): fail whale :(");
		System.exit(0);
	}

	public static void main(String [] args) throws IOException
	{
		ConstrainedTopoSort t = new ConstrainedTopoSort("input_files/g1.txt");

		if (t.hasConstrainedTopoSort(4, 3) == true) failwhale("(1, 4)");
		if (t.hasConstrainedTopoSort(1, 2) == false) failwhale("(1, 2)");
		if (t.hasConstrainedTopoSort(1, 3) == false) failwhale("(1, 3)");
    if (t.hasConstrainedTopoSort(4, 1) == false) failwhale("(4, 1)");
    if (t.hasConstrainedTopoSort(2, 3) == false) failwhale("(2, 3)");
    if (t.hasConstrainedTopoSort(4, 2) == false) failwhale("(4, 2)");

		System.out.println("Test Case #7: PASS (Hooray!)");
	}
}
