// Sean Szumlanski
// COP 3503, Spring 2021

// ====================================
// ConstrainedTopoSort: TestCase04.java
// ====================================
// A small test case for ConstrainedTopoSort.


import java.io.*;
import java.util.*;

public class TestCase04
{
	private static void failwhale(String params)
	{
		System.out.println("Test Case #4: hasConstrainedTopoSort(" + params + "): fail whale :(");
		System.exit(0);
	}

	public static void main(String [] args) throws IOException
	{
		ConstrainedTopoSort t = new ConstrainedTopoSort("input_files/g1.txt");

		if (t.hasConstrainedTopoSort(2, 1) != true) failwhale("2, 1");
		if (t.hasConstrainedTopoSort(2, 3) != true) failwhale("2, 3");
		if (t.hasConstrainedTopoSort(2, 4) != true) failwhale("2, 4");

		System.out.println("Test Case #4: PASS (Hooray!)");
	}
}
