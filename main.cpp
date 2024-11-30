#include <iostream>
#include "Set.h"
template <typename T>
void SearchRepeats(const T* arr, size_t n) {
	Set<T> uniqs;
	Set<T> repeats;

	for (size_t i = 0; i < n; ++i) {
		if (uniqs.SearchValue(arr[i])) {
			repeats.push_back(arr[i]);
		}
		else {
			uniqs.push_back(arr[i]);
		}
	}

	std::cout << "Repeats: " << repeats << std::endl;
}

int main()
{
	Set<double> s1(5, -54, 32);
	std::cout << s1;

	Set<std::string> s2(5, "fadSFD", "fdbafsdfaheqa");
	std::cout << s2;

	std::pair<int, double> p1(10, 256);
	std::pair<int, double> p2(-43, 260.4);
	Set<std::pair<int, double>> s3(5, p1, p2);
	std::cout << s3;

	std::cout << std::endl << std::endl;
	double arr[5]{ 13,45,-32.4,0,40 };
	Set<double> set1(arr, 5);
	std::cout << set1;
	set1 += 22;
	set1 += -32.4;
	std::cout << set1;
	set1 -= 13;
	set1 -= 50;
	std::cout << set1;
	std::cout << std::endl << std::endl;
	double arr2[5]{ 12, 22, 40};
	Set<double> set2(arr2, 5);
	Set<double> s;
	s += 64;
	std::cout << s;
	Set<double> set_sum(set1 + set2);
	std::cout << set_sum;
	std::cout << std::endl << set1 << std::endl << set2 << std::endl;

	std::cout << set1.IntersecSets(set2) << std::endl;
	int array[12] { 1, 2, 3, 2, 4, 5, 1, 6, 7, 8, 9, 5 };
	SearchRepeats(array, 12);
}