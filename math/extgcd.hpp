
/////////////////////////////////////////////////////
// 拡張ユークリッドの互除法(Extended-Euclidean-Algorithm)
// T extgcd(T a, T b, T &x, T &y)
// gcd(a, b)を返す
// (x,y)にはax+by=gcd(a,b)を満たす、|x|+|y|が最小のものが入る
/////////////////////////////////////////////////////

template <typename T>
T extgcd(T a, T b, T &x, T &y) {
    T d = a;
    if (b != 0) {
        d = extgcd(b, a % b, y, x);
        y -= (a / b) * x;
    } else {
        x = 1;
        y = 0;
    }
        return d;
}
