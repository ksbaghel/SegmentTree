// SegmentTree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>

using namespace std;

int min(int a, int b) { return a < b ? a : b;  }

void updateValueUtil(int idx, int diff, vector<int>& segT, vector<int>& RMQSegT, int segIdx, int start, int end)
{
	//Now if idx is within range of start and end then update the segT[segIdx]
	if (idx < start || idx > end) return;

	segT[segIdx] += diff;
	RMQSegT[segIdx] += diff;

	if (start != end)
	{
		int mid = start + (end - start) / 2;
		updateValueUtil(idx, diff, segT, RMQSegT, 2 * segIdx + 1, start, mid);
		updateValueUtil(idx, diff, segT, RMQSegT, 2 * segIdx + 2, mid + 1, end);
		RMQSegT[segIdx] = min(RMQSegT[2*segIdx+1], RMQSegT[2*segIdx+2]);
	}
}

void updateValue(int idx, int diff, vector<int>& input, vector<int>& segT, vector<int>& RMQSegT)
{
	input[idx] += diff;
	updateValueUtil(idx, diff, segT, RMQSegT, 0, 0, input.size() - 1);
}

int getQuerySum(int qs, int qe, vector<int>& segT, int start, int end, int stIdx)
{
	//Here start and end are idx from  original input array for which the stIdx in ST contains sum.
	// ST[stIdx] = sum(A[start]....A[end]);

	//if curr segment node is fully part of the query range
	if (qs <= start && qe >= end)
	{
		return segT[stIdx];
	}
	
	//if curr segment is outside the query range
	if (end < qs || start > qe)
	{
		return 0;
	}
	//if curr segment partially covers the query range
	int mid = start + (end - start) / 2;
	return getQuerySum(qs, qe, segT, start, mid, 2 * stIdx + 1) + getQuerySum(qs, qe, segT, mid + 1, end, 2 * stIdx + 2);
}

int constructSegT_Util(vector<int>& ST, int stIdx, int start, int end, vector<int>& A)
{
	if (start == end)
	{
		ST[stIdx] = A[start];
		return ST[stIdx];
	}

	int mid = start + (end - start) / 2;

	ST[stIdx] = constructSegT_Util(ST, 2 * stIdx + 1, start, mid, A) + constructSegT_Util(ST, 2 * stIdx + 2, mid + 1, end, A);
	return ST[stIdx];
}

vector<int> constructSegmentTree(vector<int>& A)
{
	//first calculate the size of segtree
	int n = A.size();
	int N = n;
	for (int i = 0; i < 32; i++)
	{
		if ((1 << i) >= n)
		{
			N = (1 << i); 
			break;
		}
	}

	vector<int> SegT(2*N-1);

	int sum = constructSegT_Util(SegT, 0, 0, n-1, A);

	return SegT;
}

int getQueryMin(int qs, int qe, vector<int>& RMQSegT, int idx, int start, int end)
{
	if (qe < start || qs > end) return INT_MAX;
	if (qs <= start && end <= qe) return RMQSegT[idx];

	int mid = start + (end - start) / 2;
	return min(getQueryMin(qs, qe, RMQSegT, 2 * idx + 1, start, mid), getQueryMin(qs, qe, RMQSegT, 2 * idx + 2, mid + 1, end));
}

int constructRMQSegTreeUtil(vector<int>& RMQSegT, int idx, vector<int>& input, int start, int end)
{
	if (start == end)
	{
		RMQSegT[idx] = input[start];
		return RMQSegT[idx];
	}

	int mid = start + (end - start) / 2;

	RMQSegT[idx] = min(constructRMQSegTreeUtil(RMQSegT, 2 * idx + 1, input, start, mid), constructRMQSegTreeUtil(RMQSegT, 2 * idx + 2, input, mid + 1, end));
	return RMQSegT[idx];
}

vector<int> constructRMQSegTree(vector<int>& A)
{
	//identify the size of segment tree
	int N = A.size();
	for (int i = 0; i < 32; i++)
	{
		if ((1 << i) >= A.size())
		{
			N = (1 << i);
			break;
		}
	}

	vector<int> RMQSegT(2 * N - 1);

	constructRMQSegTreeUtil(RMQSegT, 0, A, 0, A.size() - 1);


	return RMQSegT;
}

int main()
{
	int n;
	cin >> n;

	vector<int> A(n);
	for (int i = 0; i < n; i++)
	{
		cin >> A[i];
	}

	//Construct segment tree
	vector<int> SegTree = constructSegmentTree(A);
	vector<int> RMQSegTree = constructRMQSegTree(A);
	
	while (1)
	{
		cout << endl;
		cout << "Enter options: " << endl;
		cout << "1. Print Input array" << endl;
		cout << "2. Print sum of range" << endl;
		cout << "3. Update value" << endl;
		cout << "4. Print min of range" << endl;
		
		char x;
		cin >> x;
		switch (x)
		{
			case '1': 
			{
				cout << "Input array is: " << endl;
				for (int i = 0; i < A.size(); i++)
				{
					cout << A[i] << " ";
				}
				cout << endl;
				break;
			}
			case '2':
			{
				cout << "Enter query range: " << endl;
				int qs, qe;
				cin >> qs >> qe;
				//check if query is valid or not
				if ((qs < 0 || qe >= n) || (qs > qe))
				{
					cout << "Please enter valid query in range 0 - " << n - 1 << endl;
				}
				else
				{
					cout << "Sum of range " << qs << "-" << qe << " is : " << getQuerySum(qs, qe, SegTree, 0, n - 1, 0) << endl;
				}
				break;
			}
			case '3':
			{
				cout << "Please enter the index and new value to be updated" << endl;
				int idx, newValue;
				cin >> idx >> newValue;
				if (idx < 0 || idx >= A.size())
				{
					cout << "Invalid idx. Please provide correct idx." << endl;
				}
				else {
					updateValue(idx, newValue - A[idx], A, SegTree, RMQSegTree);
				}
				break;
			}
			case '4':
			{
				cout << "Enter query range: " << endl;
				int qs, qe;
				cin >> qs >> qe;
				//check if query is valid or not
				if ((qs < 0 || qe >= n) || (qs > qe))
				{
					cout << "Please enter valid query in range 0 - " << n - 1 << endl;
				}
				else
				{
					cout << "min of range " << qs << "-" << qe << " is : " << getQueryMin(qs, qe, RMQSegTree, 0, 0,n - 1) << endl;
				}
				break;
			}
			default:
				cout << "Please provide valid input. 1 to 3 only" << endl;
		}
		
	}
    return 0;
}

