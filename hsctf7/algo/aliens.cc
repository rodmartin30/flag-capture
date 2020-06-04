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
ll a[N][N];
ll ones[N][N];
ll s[N][N];
ll w[N][N];

ll sum(int i,int j,int k,int h){
  return s[k][h]-s[i-1][h]-s[k][j-1]+s[i-1][j-1];
}
ll q(int i,int j,int k,int h){
  return w[k][h]-w[i-1][h]-w[k][j-1]+w[i-1][j-1];
}

int main(){FIN;
  int n=501;
	fore(i,1,n)fore(j,1,n)cin>>a[i][j],ones[i][j]=(a[i][j]==-1);
	s[0][0]=a[0][0];
	fore(i,1,n){
	  fore(j,1,n){
	    s[i][j]=a[i][j]+s[i-1][j]+s[i][j-1]-s[i-1][j-1];
	    w[i][j]=ones[i][j]+w[i-1][j]+w[i][j-1]-w[i-1][j-1];
	  }
	}
	ll ans=0;
	fore(i,1,n){
	  fore(j,1,n){
	    fore(k,i,n){
	      fore(h,j,n){
	        ll g=sum(i,j,k,h);
	        if(g%13==0){
	          ans += g*(q(i,j,k,h)%2?-1:1);
	        }
	      }
	    }
	  }
	}
	cout<<ans<<"\n";
}

