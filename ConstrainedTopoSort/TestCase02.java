// Sean Szumlanski
// COP 3503, Spring 2021

// ====================================
// ConstrainedTopoSort: TestCase02.java
// ====================================
// A brief test case to help ensure you've implemented the hoursSpent() method correctly.


import java.io.*;
import java.util.*;

public class TestCase02
{
	private static void failwhale()
	{
		System.out.println("Test Case #2: fail whale :(");
		System.exit(0);
	}

	public static void main(String [] args)
	{
		double hours = ConstrainedTopoSort.hoursSpent();
		if (hours <= 0.0) failwhale();

		System.out.println("Test Case #2: PASS (Hooray!)");
	}
}
