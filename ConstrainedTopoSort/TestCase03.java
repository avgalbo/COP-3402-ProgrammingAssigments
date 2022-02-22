// Sean Szumlanski
// COP 3503, Spring 2021

// ====================================
// ConstrainedTopoSort: TestCase03.java
// ====================================
// A small test case for ConstrainedTopoSort.


import java.io.*;
import java.util.*;

public class TestCase03
{
	private static void failwhale(String params)
	{
		System.out.println("Test Case #3: hasConstrainedTopoSort(" + params + "): fail whale :(");
		System.exit(0);
	}

	public static void main(String [] args) throws IOException
	{
		ConstrainedTopoSort t = new ConstrainedTopoSort("input_files/g1.txt");

		if (t.hasConstrainedTopoSort(1, 2) != true) failwhale("1, 2");
		if (t.hasConstrainedTopoSort(1, 3) != true) failwhale("1, 3");
		if (t.hasConstrainedTopoSort(1, 4) != true) failwhale("1, 4");

		System.out.println("Test Case #3: PASS (Hooray!)");
	}
}
