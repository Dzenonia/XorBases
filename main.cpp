#include <bits/stdc++.h>

using namespace std;

struct event;
struct base;

int n;
const int BITS = 20;

vector<int> values;
vector<int> answers;
vector<vector<event>> quarry;
vector<int> position;
vector<base> prefix;


struct event {
    int r;
    int id;

    bool operator<(const event &rhs) const {
        return (this->r == rhs.r ? this->id < rhs.id : this->r < rhs.r);
    }
};

int checkBit(int value, int pos) {
    return (value >> pos) & 1;
}

struct base {
    vector<int> data;

    base() : data(BITS, 0) {}

    void add(int value) {
        for (int i = BITS - 1; i >= 0 && value > 0; --i) {
            if (data[i] == 0) {
                data[i] = value;
                return;
            }
            if ((value ^ data[i]) < value) {
                value ^= data[i];
            }
        }
    }
    int getAns() {
        int ans = 0;
        for (int i = BITS - 1; i >= 0; --i) {
            if ((ans ^ data[i]) > ans) {
                ans ^= data[i];
            }
        }
        return ans;
    }

    static base merge(const base &lhs, const base &rhs) {
        base result = lhs;
        for (int value: rhs.data) {
            result.add(value);
        }
        return result;
    }

    void print() {
        for (int value: data) {
            cout << value << " ";
        }
        cout << endl;
    }

    void clear() {
        for (auto& vec: data) {
            vec = 0;
        }
    }
};

base suffix;

void getAns(int l, int r) {
    if (l + 1 == r) {
        while (position[l] < quarry[l].size() && quarry[l][position[l]].r < r) {
            answers[quarry[l][position[l]].id] = values[l];
            ++position[l];
        }
        return;
    }
    int m = (l + r) >> 1;
    getAns(l, m);
    getAns(m, r);
    prefix[m - 1] = {};
    for (int i = m; i < r; ++i) {
        prefix[i] = prefix[i - 1];
        prefix[i].add(values[i]);
    }
    suffix.clear();
    for (int i = m - 1; i >= l; --i) {
        suffix.add(values[i]);
        while (position[i] < quarry[i].size() && quarry[i][position[i]].r < r) {
            assert(quarry[i][position[i]].r >= m);
            answers[quarry[i][position[i]].id] = base::merge(suffix, prefix[quarry[i][position[i]].r]).getAns();
            ++position[i];
        }
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cin >> n;

    values.resize(n);
    quarry.resize(n);
    prefix.resize(n + 1);
    position.resize(n, 0);

    for (int i = 0; i < n; ++i) {
        cin >> values[i];
    }
    int q;
    cin >> q;

    answers.resize(q);
    for (int i = 0; i < q; ++i) {
        int l;
        int r;
        cin >> l >> r;
        --l;
        --r;
        quarry[l].push_back({r, i});
    }
    for (int i = 0; i < n; ++i) {
        sort(quarry[i].begin(), quarry[i].end());
    }
    getAns(0, n);
    cout << endl;
    for (int i = 0; i < q; ++i) {
        cout << answers[i] << endl;
    }
    return 0;
}
