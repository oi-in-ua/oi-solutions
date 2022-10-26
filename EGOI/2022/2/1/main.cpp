#include<iostream>
#include<algorithm>
using namespace std;

const int kNsize = 1000010;

int n, s;
int a[kNsize], b[kNsize];

void printa() {
	for (int i = 0; i < n; i++) {
		cout<<a[i];
		if (i < n - 1) {
			cout<<" ";
		} else {
			cout<<endl;
		}
	}
}

void printb() {
	for (int i = 0; i < n; i++) {
		cout<<b[i];
		if (i < n - 1) {
			cout<<" ";
		} else {
			cout<<endl;
		}
	}
}

int main()
{
	cin>>n>>s;
	for (int i = 0; i < n; i++) {
		cin>>a[i];
	}
	sort(a, a + n, greater<int>());
	int duplicates, machines;
	for (int i = 0; i < s; i++) {
		if (i % 2 == 0) {
			cin>>duplicates>>machines;
			for (int j = 0; j < machines; j++) {
				a[j] -= duplicates;
			}
			int x = 0;
			int y = machines;
			for (int j = 0; j < n; j++) {
				if ((x < machines) && (a[x] >= a[y])) {
					b[j] = a[x];
					x++;
				} else {
					b[j] = a[y];
					y++;
				}
			}
		} else {
			cin>>duplicates>>machines;
			for (int j = 0; j < machines; j++) {
				b[j] -= duplicates;
			}
			int x = 0;
			int y = machines;
			for (int j = 0; j < n; j++) {
				if ((x < machines) && (b[x] >= b[y])) {
					a[j] = b[x];
					x++;
				} else {
					a[j] = b[y];
					y++;
				}
			}
		}
	}
	if (s % 2 == 0) {
		printa();
	} else {
		printb();
	}
}