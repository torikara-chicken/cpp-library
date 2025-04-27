#include <algorithm>
#include <cassert>
#include <vector>

/////////////////////////////////////////////////////
// Potentialized Disjoint Set Union
/////////////////////////////////////////////////////

template <class S, auto op, auto e, auto inv>
struct potentialized_dsu {
    static_assert(std::is_convertible_v<decltype(op), std::function<S(S, S)>>,
        "op must work as S(S, S)");
    static_assert(std::is_convertible_v<decltype(e), std::function<S()>>,
        "e must work as S()");
    static_assert(std::is_convertible_v<decltype(inv), std::function<S(S)>>,
        "inv must work as S(S)");

    public:
        potentialized_dsu() : _n(0) {}
        explicit potentialized_dsu(int n) : _n(n), parent_or_size(n, -1), pot(n, e()) {}

        bool merge(int a, int b, S d) {
            //potantial(b) - potential(a) = d としてマージ
            assert(0 <= a && a < _n);
            assert(0 <= b && b < _n);
            d = op(op(potential(a), d), inv(potential(b)));
            int x = leader(a), y = leader(b);
            if (x == y) return d == e();
            if (-parent_or_size[x] < -parent_or_size[y]) {
                std::swap(x, y);
                d = inv(d);
            }
            parent_or_size[x] += parent_or_size[y];
            parent_or_size[y] = x;
            pot[y] = d;
            return true;
        }

        bool same(int a, int b) {
            assert(0 <= a && a < _n);
            assert(0 <= b && b < _n);
            return leader(a) == leader(b);
        }

        int leader(int a) {
            assert(0 <= a && a < _n);
            if (parent_or_size[a] < 0) return a;
            int x = leader(parent_or_size[a]);
            pot[a] = op(pot[parent_or_size[a]], pot[a]);
            return parent_or_size[a] = x;
        }

        S potential(int a) {
            assert(0 <= a && a < _n);
            leader(a);
            return pot[a];
        }

        S diff(int a, int b) {
            assert(0 <= a && a < _n);
            assert(0 <= b && b < _n);
            assert(same(a, b));
            return op(inv(potential(a)), potential(b));
        }

        int size(int a) {
            assert(0 <= a && a < _n);
            return -parent_or_size[leader(a)];
        }

        std::vector<std::vector<int>> groups() {
            std::vector<int> leader_buf(_n), group_size(_n);
            for (int i = 0; i < _n; i++) {
                leader_buf[i] = leader(i);
                group_size[leader_buf[i]]++;
            }
            std::vector<std::vector<int>> result(_n);
            for (int i = 0; i < _n; i++) {
                result[i].reserve(group_size[i]);
            }
            for (int i = 0; i < _n; i++) {
                result[leader_buf[i]].push_back(i);
            }
            result.erase(
                std::remove_if(result.begin(), result.end(),
                    [&](const std::vector<int>& v) { return v.empty(); }),
                result.end());
            return result;
        }

    private:
        int _n;
        // root node: -1 * component size
        // otherwise: parent
        std::vector<int> parent_or_size;
        std::vector<S> pot;
};