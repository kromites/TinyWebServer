#include <vector>

using namespace std;

void quicksort(vector<int> &nums, int l, int r) {
	if (l + 1 >= r)
		return;

	// key is the pivot element 
	auto first = l, last = r - 1, key = nums[first];
	while(first < last) {
		// deal with the element from the last
		while (first < last && nums[last] >= key) {
			--last;
		}

		// swap and pivot become to the nums[last]
		nums[first] = nums[last];

		// deal wiht the element from the first

		while(first < last && nums[first] <= key) {
			++first;
		}

		// swap 
		nums[last] = nums[first];
	}

	// put the pivot on the correct position, sun first is the division line
	nums[first] = key;

	// recursion
	quicksort(nums, l, first);
	quicksort(nums, first + 1, r);
}

void mergesort(vector<int> &nums, int l, int r, vector<int> &temp) {
	if(l + 1 >= r)
		return;
	// divide, recursion first
	int m = l + (r - l) / 2;
	mergesort(nums, l, m, temp);
	mergesort(nums, m, r, temp);

	// conquer

	int p = l, q = m, i = l;
	while(p < m || q < r) {
		if (q >= r || (p < m && nums[p] <= nums[q]))
			temp[i++] = nums[p++];
		else
			temp[i++] = nums[q++];
	}

	for (i = l; i < r; ++i) {
		nums[i] = temp[i];
	}
}

void insertsort(vector<int> &nums, int n) {
	for(int i=0; i<n; ++i) {
		for(int j=i; j>0&& nums[j] < nums[j-1]; --j) {
			swap(nums[j], nums[j - 1]);
		}
	}
}

void bubblesort(vector<int> &nums, int n) {
	bool swapped;
	for(int i = 1; i<n; ++i) {
		swapped = false;
		for(int j=i; j < n-i+1; ++j) {
			if(nums[j] < nums[j-1]) {
				swap(nums[j], nums[j - 1]);
				swapped = true;
			}
		}
		if(!swapped)
			break;
	}
}


void selection_sort(vector<int>& nums, int n) {
	int mid;
	for (int i = 0; i < n - 1; ++i) {
		mid = i;
		for (int j = i + 1; j < n; ++j) {
			if (nums[j] < nums[mid]) {
				mid = j;
			}
		}
		swap(nums[mid], nums[i]);
	}
}



void heapadjust(vector<int> &nums, int len, int index) {
	auto left = index * 2 + 1;
	auto right = index * 2 + 1;

	auto maxIdx = index;

	if (left < len && nums[left] > nums[maxIdx]) maxIdx = left;
	if (right< len && nums[right] > nums[maxIdx]) maxIdx = right;

	if(maxIdx != index) {
		swap(nums[maxIdx], nums[index]);
		heapadjust(nums, len, maxIdx);
	}
}

void heapsort(vector<int> &nums, int size) {
	// build a heap
	for(int i = size / 2 -1; i >= 0; --i) {
		heapadjust(nums, size, i);
	}

	// adjust the heap
	for(int i=size-1; i>0; i--) {
		swap(nums[0], nums[i]);
		heapadjust(nums, i, 0);
	}
}