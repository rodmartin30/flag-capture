#include <bits/stdc++.h>
#define fore(x,a,b) for(int x=a, qwerty=b; x<qwerty; x++)
#define ALL(a) (a).begin(),(a).end()
#define SZ(a) ((int)(a).size())
#define pb push_back
#define FIN ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define fst first
#define snd second
#define mset(a,v) memset((a),(v),sizeof(a))
#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")
using namespace std;
typedef long long ll;
typedef pair<int,int> ii;
const int N=505;

int n,W;
int school[N][N],student[N][N];
string name[N];

int st_match[N];
int sc_match[N];
int step;
int pos[N][N];
bool ok(){
  // student
  int r=1;
  fore(i,0,n){
    // school
    fore(j,0,n){
      int sc_current = student[i][j];
      // If we got the best possible result
      if(st_match[i]==sc_current)break;
      int st_current = sc_match[sc_current];
      if(pos[sc_current][i]<pos[sc_current][sc_match[sc_current]]){
          swap(st_match[i],st_match[st_current]);
          swap(sc_match[st_match[i]],sc_match[st_match[st_current]]);
          step++;
          cout<<"ste\n";
          r=0;
          break;
      }
    }
  }
  return r;
}
typedef pair<int,int> ii;
queue<int> q[N];
int g[N],dp[N][N];
set<ii> s[N];
bool vis[N];
int main(){
  cin>>n>>W;
  fore(i,0,n)fore(j,0,n){
    cin>>school[i][j];
    pos[i][school[i][j]]=j;
  }
  fore(i,0,n)fore(j,0,n){
    cin>>student[i][j];
    q[i].push(student[i][j]);
  }
  cin.ignore();
  fore(i,0,n)getline(cin,name[i]);
  vector<int> u;fore(i,0,n)u.pb(i);
  while(SZ(u)){
    for(int v:u){
      int sc=q[v].front();
      q[v].pop();
      s[sc].insert({pos[sc][v],v});
    }
    u.clear();
    mset(st_match,-1);
    mset(sc_match,-1);
    fore(i,0,n){
      if(SZ(s[i])==0)continue;
      ii p=*s[i].begin();
      s[i].clear();
      s[i].insert(p);
      sc_match[i]=p.snd;
      st_match[p.snd]=i;
    }
    fore(i,0,n)if(st_match[i]==-1)u.pb(i);
  }
  while(!ok()){}
  cout<<name[sc_match[W]]<<endl;
}

