#include<iostream>
#include<algorithm>
using namespace std;

const int kNsize = 100010;

int n, s;
int a[kNsize], b[kNsize];

void print_a() {
  for (int i = 0; i < n; i++) {
    cout<<a[i];
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
  int copies, machines;
  for (int i = 0; i < s; i++) {
        cin>>copies>>machines;
        for (int j = 0; j < machines; j++) {
          a[j] -= copies;
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
        for (int j = 0; j < n; j++) {
          a[j] = b[j];
        }
  }
  print_a();
}

