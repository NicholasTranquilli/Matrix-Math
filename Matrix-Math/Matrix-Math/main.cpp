// Matrix lib

#include "Matrix.h"

int main()
{
	try {
		// Initialize directly
		Matrix<double> m1(3, 3);

		m1[0][0] = 1;
		m1[0][1] = 1;
		m1[0][2] = 0;

		m1[1][0] = 1;
		m1[1][1] = 1;
		m1[1][2] = 0;

		m1[2][0] = 0;
		m1[2][1] = 0;
		m1[2][2] = 1;

		printf("m1 matrix:\n");
		m1.Display();

		// Initializer list
		Matrix<double> m2 =
		{
			{1, 2},
			{3, 4},
			{5, 6}
		};

		printf("m2 matrix:\n");
		m2.Display();

		// Functions:

		printf("Transpose of m2:\n");
		m2.Transpose().Display();

		printf("Augment m1 with m2 to make m1 = [m1 | m2]:\n");
		m1.Augment(m2);
		m1.Display();

		printf("Row Reduce [m1 | m2]:\n");
		m1.RREF().Display();

		printf("Resize m1 to 4x4:\n");
		m1.Resize(4, 4);
		m1.Display();

		printf("Create new values for m1:\n");
		m1 = {
			{1, 0, 0, 0},
			{3, 2, 0, 0},
			{2, 3, 1, 0},
			{0, 0, 0, 1},
		};
		m1.Display();

		printf("Inverse of m1:\n");
		m1.Inverse().Display();

		printf("m1 + m1:\n");
		(m1 + m1).Display();

		printf("m3:\n");
		Matrix<double> m3 = {
			{4},
			{3},
			{2},
			{1},
		};
		m3.Display();

		printf("m1 * m3:\n");
		(m1 * m3).Display();

		printf("m3 - m3:\n");
		(m3 - m3).Display();

		printf("Sub Matrix (2nd row) of m1:\n");
		m1.SubMatrix(1, 2, 0, m1.GetColumns()).Display();
	}
	catch (...)
	{
		printf("ERROR");
	}

	return 0;
}