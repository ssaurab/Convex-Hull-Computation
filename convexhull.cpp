#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;

const long double EPS = 1e-10;
const int INF = 1e9 + 41;
const int N = 1e3 + 20;

long double d[N][N];
bool u[N][N];
int sc[N][N];
long double ed[N][N];

struct pt {
    long double x, y;
    pt() {}
    pt(long double a, long double b){
        x = a;
        y = b;
    }
    pt operator + (const pt &a) const{
        return pt(x + a.x, y + a.y);
    }
    pt operator - (const pt &a) const{
        return pt(x - a.x, y - a.y);
    }
    long double operator * (const pt &a) const{
        return x * a.y - y * a.x;
    }
    long double operator % (const pt &a) const{
        return x * a.x + y * a.y;
    }
    pt operator * (const long double &a) const{
        return pt(x * a, y * a);
    }
    long double len() const{
        return sqrtl(x * x + y * y);
    }
    bool operator == (const pt &a) const{
        return (fabsl(x - a.x) < EPS && fabsl(y - a.y) < EPS);
    }
};

int dir(const pt &a, const pt &b, const pt &c){
    long double kek = (b - a) * (c - a);
    if (kek > EPS) return 1;
    if (kek < -EPS) return -1;
    return 0;
}

bool does_int(const pt &a, const pt &b, const pt &c, const pt &d){
    if (a == c || a == d || b == c || b == d) return false;
    if (dir(a, b, c) * dir(a, b, d) == 1 || dir(c, d, a) * dir(c, d, b) == 1)
        return false;

    if (dir(a, b, c) == 0){
      if(dir(a,b,d) == 0)
      {
        if ((a - c) % (b - c) > EPS) return false;
        if ((a - d) % (b - d) > EPS) return false;
        if ((c - a) % (d - a) > EPS) return false;
        if ((c - b) % (d - b) > EPS) return false;        
      }
    }

    return true;
}

bool is_on(int n, pt a[], const pt &p){
    a[n] = a[0];
    for (int i = 0; i <= n-1; i++) {
        if (fabsl((a[i + 1] - p) * (a[i + 1] - a[i])) > EPS) continue;
        if ((a[i] - p) % (a[i + 1] - p) < EPS) return true;
    }
    return false;
}

bool is_in(int n, pt a[], const pt &p){
    a[n] = a[0];
    int inum = 0;
    pt A, B;
    for (int i = 0; i < n; i++){
        A = a[i];
        B = a[i + 1];
        if (!(fabsl(A.y - B.y) > EPS)) continue;
        if (B.y < A.y){
            swap(A, B);
        }
        if (A.y - EPS > p.y) continue;
        if (B.y - EPS < p.y) continue;
        if ((B - A) * (A - p) > 0) continue;

        inum++;
    }
    return (inum % 2);
}

bool is_int(int n, pt a[], const pt &A, const pt &B){
    a[n] = a[0];
    for (int i = 0; i < n; i++){
        if (does_int(a[i], a[i + 1], A, B))
            return true;
    }
    return false;
}

vector <int> getPath(int x, int y){
    vector <int> p = {x};
    while (x != y){
        x = sc[x][y];
        p.push_back(x);
    }
    return p;
}

int n, m;
pt a[N], b[N], c[N];
pt path[N];
int plen;

int main(){

    cin >> n;
    for (int i = 0; i <= n-1; i++){
        cin >> a[i].x >> a[i].y;
        c[i] = a[i];
    }

    pt KEK;

    for (int i = 0; i < n; i++)
    for (int j = n - 1; j > i + 1; j--){
        pt c = (a[i] + a[j]) * 0.5;
        if (!is_on(n, a, c) && is_in(n, a, c)){
            KEK = c;
        }
    }

    if (n == 3){
        KEK = (a[0] + a[1] + a[2]) * (1.0 / 3.0);
    }

    cin >> m;
    for (int i = 0; i <= m-1; i++){
        cin >> b[i].x >> b[i].y;
        c[i + n] = b[i];
    }

    int tot2 = m+n;
    for (int i = 0; i < tot2; i++)
    {
        for (int j = 0; j < tot2; j++){
            d[i][j] = 1e16;
            sc[i][j] = -1;
        }
    }

    int tot3 = m+n;
    for (int i = 0; i <tot2; i++)
    for (int j = i + 1; j <tot2; j++){
        pt mid = (c[i] + c[j]) * 0.5;

        if (!is_on(m, b, mid)) if(!is_in(m, b, mid)) continue;
        if (!is_on(n, a, mid)) if(is_in(n, a, mid)) continue;

        if (is_int(n, a, c[i], c[j])) continue;
        if (is_int(m, b, c[i], c[j])) continue;

        ed[i][j] = ed[j][i] = d[i][j] = d[j][i] = (c[i] - c[j]).len();
        u[i][j] = true;
        sc[j][i] = i;
        sc[i][j] = j;
        
    }

    int tot1 = m+n;
    for (int k = 0; k < tot1; k++)
    for (int i = 0; i < tot1; i++)
    for (int j = 0; j < tot1; j++){
        if (d[i][j] > d[i][k] + d[k][j] + EPS){
            d[i][j] = d[i][k] + d[k][j];
            sc[i][j] = sc[i][k];
        }
    }

    int tot = 0;
    for (int i = 1; i < n; i++){
        if (a[i].x + EPS < a[tot].x || (fabsl(a[i].x - a[tot1].x) < EPS && a[i].y + EPS < a[tot1].y))
            tot = i;
    }

    vector <int> p1, p2;
    long double dist, ans = 1e17;
    for (int i = 0; i < n + m; i++)
    for (int j = 0; j < n + m; j++){
        if (!u[i][j]) continue;
        p1 = getPath(tot, i);
        p2 = getPath(tot, j);

        dist = d[tot][i] + ed[i][j] + d[j][tot];

        plen = 0;
        for (int i = 0; i <= (int)p1.size()-1; i++)
            path[plen++] = c[p1[i]];
        for (int i = (int)p2.size() - 1; i >= 1; i--)
            path[plen++] = c[p2[i]];

        if (is_in(plen, path, KEK) && (dist <= ans)){
                ans = dist;   
        }
    }

   
    cout << fixed << setprecision(4)<<ans;

    return 0;
}
