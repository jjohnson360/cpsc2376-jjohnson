#include "pch.h"
#include "CppUnitTest.h"
#include "../../practice/practice03/main.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace practice03test
{
	TEST_CLASS(practice03test)
	{
	public:
		
        TEST_METHOD(TestMethodSums)
        {
            Assert::AreEqual(10, MathUtils::sumRange(1, 4)); // 1 + 2 + 3 + 4 = 10

            Assert::AreEqual(0, MathUtils::sumRange(0, 0)); // No elements to sum

            Assert::AreEqual(-3, MathUtils::sumRange(-3, 2)); // -3 + -2 + -1 + 0 + 1 + 2 = -3
        }

        TEST_METHOD(TestMethodNegatives)
        {
            std::vector<int> values1 = { 3, -1, 5, 7 };
            std::vector<int> values2 = { 1, 2, 3 };
            std::vector<int> values3 = {};

            // Negative numbers test
            Assert::IsTrue(MathUtils::containsNegative(values1));

            // No negative numbers test
            Assert::IsFalse(MathUtils::containsNegative(values2));

            // Empty vector
            Assert::IsFalse(MathUtils::containsNegative(values3));
        }

        TEST_METHOD(TestMethodMax)
        {
            std::vector<int> values1 = { 3, -1, 5, 7 };
            std::vector<int> values2 = {};
            std::vector<int> values3 = { 4, 4, 4 };

            // Standard input test
            Assert::AreEqual(7, MathUtils::findMax(values1));

            // Empty vector test
            auto func = [&values2] { MathUtils::findMax(values2); };
            Assert::ExpectException<std::invalid_argument>(func);

            // All equal values test
            Assert::AreEqual(4, MathUtils::findMax(values3));
        }
		
	};
}
