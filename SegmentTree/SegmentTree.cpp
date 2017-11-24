// SegmentTree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>

using namespace std;

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
	
	while (1)
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
	}
    return 0;
}

