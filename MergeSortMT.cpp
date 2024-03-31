#include <iostream>
#include <random>
#include <future>
#include <chrono>

using namespace std;

void print(int* arr, int n)
{
	for (int i = 0; i < n; i++)
	{
		cout << arr[i] << " ";
	}
	cout << endl;
}

bool multithread = false;

void merge(int* arr, int l, int m, int r)
{
	int nl = m - l + 1;
	int nr = r - m;

	int* left = new int[nl];
	int* right = new int[nr];

	for (int i = 0; i < nl; i++)
		left[i] = arr[l + i];
	for (int j = 0; j < nr; j++)
		right[j] = arr[m + j + 1];

	int i = 0, j = 0;
	int k = l;

	while (i < nl && j < nr)
	{
		if (left[i] < right[j])
		{
			arr[k++] = left[i++];
		}
		else
		{
			arr[k++] = right[j++];
		}
	}

	while (i < nl)
		arr[k++] = left[i++];

	while (j < nr)
		arr[k++] = right[j++];

	delete[] left;
	delete[] right;
}

void mergeSort(int* arr, int l, int r)
{
	if (l >= r)
		return;

	int m = (l + r - 1) / 2;
	if (multithread && (m - l > 100000))
	{

		future<void> f = async(launch::async, [&]() {
			mergeSort(arr, l, m);
			});
		mergeSort(arr, m + 1, r);
	}
	else
	{
		mergeSort(arr, l, m);
		mergeSort(arr, m + 1, r);
	}
	merge(arr, l, m, r);
}

int main()
{
	srand(0);
	cout << "Allocate memory for array..." << endl;
	int arr_size = 100000000;
	int* arr = new int[arr_size];


	cout << "Single thread\nGenerating random numbers and set in array..." << endl;
	for (size_t i = 0; i < arr_size; i++)
	{
		arr[i] = rand() % 50000;
	}
	time_t start, end;
	cout << "Sorting in single thread started..." << endl;
	time(&start);
	mergeSort(arr, 0, arr_size - 1);
	time(&end);
	double seconds = difftime(end, start);

	printf("Elapsed time: %f seconds\n", seconds);
	cout << "Checking sorted array..." << endl;
	for (long i = 0; i < arr_size - 1; i++)
	{
        if (arr[i] > arr[i + 1]) {
            cout << "Unsorted" << endl;
            break;
        }
    }

	cout << "\nMultithread\nGenerating random numbers and set in array..." << endl;
	multithread = true;
	for (size_t i = 0; i < arr_size; i++)
	{
		arr[i] = rand() % 50000;
	}
	cout << "Sorting in multithread started..." << endl;
	time(&start);
	mergeSort(arr, 0, arr_size - 1);
	time(&end);
	seconds = difftime(end, start);
	printf("Elapsed time: %f seconds\n", seconds);
	cout << "Checking sorted array..." << endl;
	for (long i = 0; i < arr_size - 1; i++)
	{
        if (arr[i] > arr[i + 1]) {
            cout << "Unsorted" << endl;
            break;
        }
    }
	cout << "Deallocate memory from array..." << endl;
	delete[] arr;
	return 0;
}