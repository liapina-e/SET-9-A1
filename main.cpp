#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <functional>
#include <cmath>

static long long charCount = 0;

inline int cmpChars(char a, char b) {
    ++charCount;
    if (a < b) {
        return -1;
    }

    if (a > b) {
        return  1;
    }
    return 0;
}

inline int strCmpFrom(const std::string& a, const std::string& b, int d) {
    int la = (int)a.size();
    int lb = (int)b.size();

    for (int i = d; i < la || i < lb; ++i) {
        char ca = (i < la) ? a[i] : '\0';
        char cb = (i < lb) ? b[i] : '\0';
        int r = cmpChars(ca, cb);

        if (r != 0) {
            return r;
        }
    }
    return 0;
}

inline bool strLess(const std::string& a, const std::string& b) {
    return strCmpFrom(a, b, 0) < 0;
}

class StringGenerator {
public:
    static const std::string alf;

    explicit StringGenerator(unsigned seed = 42) : rng(seed) {}

    std::string randomString(int minLen = 10, int maxLen = 200) {
        std::uniform_int_distribution<int> lenDist(minLen, maxLen);
        std::uniform_int_distribution<int> charDist(0, (int)alf.size() - 1);
        int len = lenDist(rng);
        std::string s(len, ' ');

        for (char& c : s) {
            c = alf[charDist(rng)];
        }
        return s;
    }

    std::vector<std::string> randomArray(int n) {
        std::vector<std::string> arr(n);
        for (std::string &s: arr) {
            s = randomString();
        }
        return arr;
    }

    std::vector<std::string> reverseSortedArray(int n) {
        std::vector<std::string> arr = randomArray(n);
        std::sort(arr.begin(), arr.end());
        std::reverse(arr.begin(), arr.end());
        return arr;
    }

    std::vector<std::string> nearlySortedArray(int n) {
        auto arr = randomArray(n);
        std::sort(arr.begin(), arr.end());

        int swaps = std::max(1, (int)std::sqrt(n));
        std::uniform_int_distribution<int> idx(0, n - 1);

        for (int i = 0; i < swaps; ++i) {
            int a = idx(rng), b = idx(rng);
            std::swap(arr[a], arr[b]);
        }
        return arr;
    }

    std::vector<std::string> commonPrefixArray(int n, int prefixLen = 50) {
        std::uniform_int_distribution<int> charDist(0, (int)alf.size() - 1);
        std::string prefix(prefixLen, ' ');

        for (char& c : prefix) {
            c = alf[charDist(rng)];
        }

        std::vector<std::string> arr = randomArray(n);
        for (std::string &s: arr) {
            if ((int)s.size() > prefixLen) {
                s = prefix + s.substr(prefixLen);
            } else {
                s = prefix + s;
            }
        }
        return arr;
    }

private:
    std::mt19937 rng;
};

const std::string StringGenerator::alf =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#%:;^&*()-.";


void mergeSortStd(std::vector<std::string>& arr, int l, int r) {
    if (r - l <= 0) {
        return;
    }

    int m = (l + r) / 2;

    mergeSortStd(arr, l, m);
    mergeSortStd(arr, m + 1, r);
    std::vector<std::string> tmp;
    tmp.reserve(r - l + 1);

    int i = l, j = m + 1;

    while (i <= m && j <= r) {
        if (strLess(arr[i], arr[j])) {
            tmp.push_back(std::move(arr[i++]));
        } else {
            tmp.push_back(std::move(arr[j++]));
        }
    }

    while (i <= m) {
        tmp.push_back(std::move(arr[i++]));
    }

    while (j <= r) {
        tmp.push_back(std::move(arr[j++]));
    }

    for (int k = l; k <= r; ++k) {
        arr[k] = std::move(tmp[k - l]);
    }
}

int partitionStd(std::vector<std::string>& arr, int l, int r) {
    int m = (l + r) / 2;

    if (strLess(arr[r], arr[l])) {
        std::swap(arr[l], arr[r]);
    }

    if (strLess(arr[m], arr[l])) {
        std::swap(arr[m], arr[l]);
    }

    if (strLess(arr[r], arr[m])) {
        std::swap(arr[m], arr[r]);
    }

    std::swap(arr[m], arr[r - 1]);
    std::string& pivot = arr[r - 1];

    int i = l, j = r - 1;
    while (true) {
        while (strLess(arr[++i], pivot)) {}
        while (strLess(pivot, arr[--j])) {}

        if (i >= j) {
            break;
        }
        std::swap(arr[i], arr[j]);
    }
    std::swap(arr[i], arr[r - 1]);
    return i;
}

void quickSortStd(std::vector<std::string>& arr, int l, int r) {
    if (r - l < 2) {
        if (r - l == 1 && strLess(arr[r], arr[l])) {
            std::swap(arr[l], arr[r]);
        }
        return;
    }
    int p = partitionStd(arr, l, r);
    quickSortStd(arr, l, p - 1);
    quickSortStd(arr, p + 1, r);
}

void ternaryStringQuickSort(std::vector<std::string>& arr, int l, int r, int d) {
    if (l >= r) {
        return;
    }

    int m = (l + r) / 2;
    std::swap(arr[m], arr[l]);

    auto charAt = [&](int i) -> char {
        return (d < (int)arr[i].size()) ? arr[i][d] : '\0';
    };

    char pivot = charAt(l);

    int lt = l, gt = r, i = l + 1;
    while (i <= gt) {
        ++charCount;
        char c = charAt(i);
        if (c < pivot) {
            std::swap(arr[lt++], arr[i++]);
        } else if (c > pivot) {
            std::swap(arr[i], arr[gt--]);
        } else {
            ++i;
        }
    }

    ternaryStringQuickSort(arr, l, lt - 1, d);
    if (pivot != '\0') {
        ternaryStringQuickSort(arr, lt, gt, d + 1);
    }

    ternaryStringQuickSort(arr, gt + 1, r, d);
}

int computeLCP(const std::string& a, const std::string& b, int start = 0) {
    int la = (int)a.size(), lb = (int)b.size();
    int i = start;
    while (i < la && i < lb) {
        ++charCount;
        if (a[i] != b[i]) {
            return i;
        }

        ++i;
    }

    ++charCount;
    return i;
}

std::pair<int,int> strCmpAndLCP(const std::string& a, const std::string& b, int knownLcp) {
    int la = (int)a.size(), lb = (int)b.size();
    int i = knownLcp;
    while (i < la && i < lb) {
        ++charCount;
        if (a[i] != b[i]) {
            return { (unsigned char)a[i] - (unsigned char)b[i], i };
        }
        ++i;
    }
    ++charCount;
    return { la - lb, i };
}

void stringMergeSort(std::vector<std::string>& arr, std::vector<int>& lcp, int l, int r) {
    if (r - l <= 0) {
        return;
    }

    int m = (l + r) / 2;
    stringMergeSort(arr, lcp, l, m);
    stringMergeSort(arr, lcp, m + 1, r);

    auto [cmp0, hlcp] = strCmpAndLCP(arr[l], arr[m + 1], 0);

    std::vector<std::string> tmpArr;
    std::vector<int> tmpLcp;
    tmpArr.reserve(r - l + 1);
    tmpLcp.reserve(r - l);

    int i = l, j = m + 1;

    int curCmp = cmp0, curLcp = hlcp;

    while (i <= m && j <= r) {
        if (curCmp <= 0) {
            tmpArr.push_back(std::move(arr[i]));

            if (i < m) {
                tmpLcp.push_back(lcp[i]);
                hlcp = std::min(lcp[i], curLcp);
            }

            ++i;

            if (i <= m && j <= r) {
                auto [c, l2] = strCmpAndLCP(arr[i], arr[j], hlcp);
                curCmp = c; curLcp = l2;
            }
        } else {
            tmpArr.push_back(std::move(arr[j]));

            if (j < r) {
                tmpLcp.push_back(lcp[j]);
                hlcp = std::min(lcp[j], curLcp);
            }

            ++j;
            if (i <= m && j <= r) {
                auto [c, l2] = strCmpAndLCP(arr[i], arr[j], hlcp);
                curCmp = c; curLcp = l2;
            }
        }
    }

    if (i <= m) {
        tmpLcp.push_back(curLcp);
        tmpArr.push_back(std::move(arr[i++]));
        while (i <= m) {
            tmpLcp.push_back(lcp[i - 1]);
            tmpArr.push_back(std::move(arr[i++]));
        }
    } else if (j <= r) {
        tmpLcp.push_back(curLcp);
        tmpArr.push_back(std::move(arr[j++]));
        while (j <= r) {
            tmpLcp.push_back(lcp[j - 1]);
            tmpArr.push_back(std::move(arr[j++]));
        }
    }

    for (int k = l; k <= r; ++k) {
        arr[k] = std::move(tmpArr[k - l]);
    }

    for (int k = l; k < r;  ++k) {
        lcp[k] = tmpLcp[k - l];
    }
}

inline int charAtMSD(const std::string& s, int d) {
    return (d < (int)s.size()) ? (unsigned char)s[d] : -1;
}

void msdRadixSort(std::vector<std::string>& arr, int l, int r, int d) {
    if (l >= r) {
        return;
    }

    std::vector<int> count(128 + 2, 0);
    for (int i = l; i <= r; ++i) {
        ++charCount;
        count[charAtMSD(arr[i], d) + 2]++;
    }

    for (int c = 0; c < 128 + 1; ++c) {
        count[c + 1] += count[c];
    }
    std::vector<std::string> tmp(r - l + 1);

    for (int i = l; i <= r; ++i) {
        int c = charAtMSD(arr[i], d) + 1;
        tmp[count[c]++] = std::move(arr[i]);
    }

    for (int i = l; i <= r; ++i) {
        arr[i] = std::move(tmp[i - l]);
    }

    for (int c = 0; c < 128; ++c) {
        int lo = l + count[c];
        int hi = l + count[c + 1] - 1;
        if (lo < hi) {
            msdRadixSort(arr, lo, hi, d + 1);
        }
    }
}

void msdRadixSortHybrid(std::vector<std::string>& arr, int l, int r, int d) {
    if (l >= r) {
        return;
    }

    if (r - l + 1 < 128) {
        ternaryStringQuickSort(arr, l, r, d);
        return;
    }

    std::vector<int> count(128 + 2, 0);
    for (int i = l; i <= r; ++i) {
        ++charCount;
        count[charAtMSD(arr[i], d) + 2]++;
    }

    for (int c = 0; c < 128 + 1; ++c) {
        count[c + 1] += count[c];
    }

    std::vector<std::string> tmp(r - l + 1);
    for (int i = l; i <= r; ++i) {
        int c = charAtMSD(arr[i], d) + 1;
        tmp[count[c]++] = std::move(arr[i]);
    }

    for (int i = l; i <= r; ++i) {
        arr[i] = std::move(tmp[i - l]);
    }

    for (int c = 0; c < 128; ++c) {
        int lo = l + count[c];
        int hi = l + count[c + 1] - 1;
        if (lo < hi) {
            msdRadixSortHybrid(arr, lo, hi, d + 1);
        }
    }
}

struct BenchResult {
    std::string name;
    int arraySize;
    std::string arrayType;
    double timeMs;
    long long charCmps;
};

class StringSortTester {
public:
    template<typename SortFn>
    BenchResult measure(const std::string& name,
                        const std::string& arrayType,
                        const std::vector<std::string>& arr,
                        SortFn sortFn) {
        double totalTime = 0;
        long long totalCmps = 0;

        for (int rep = 0; rep < 5; ++rep) {
            std::vector<std::string> copy = arr;
            charCount = 0;

            std::chrono::time_point<std::chrono::steady_clock> t0 = std::chrono::high_resolution_clock::now();
            sortFn(copy);
            std::chrono::time_point<std::chrono::steady_clock> t1 = std::chrono::high_resolution_clock::now();

            totalTime += std::chrono::duration<double, std::milli>(t1 - t0).count();
            totalCmps += charCount;
        }

        return BenchResult{
            name,
            (int)arr.size(),
            arrayType,
            totalTime / 5,
            totalCmps / 5
        };
    }

    std::vector<BenchResult> runAll(const std::vector<std::string>& arr,
                                    const std::string& arrayType) {
        std::vector<BenchResult> results;

        results.push_back(measure("QuickSort (std)", arrayType, arr,
            [](std::vector<std::string>& a) {
                if (a.size() > 1) {
                    quickSortStd(a, 0, (int)a.size() - 1);
                }
            }));

        results.push_back(measure("MergeSort (std)", arrayType, arr,
            [](std::vector<std::string>& a) {
                if (a.size() > 1) {
                    mergeSortStd(a, 0, (int)a.size() - 1);
                }
            }));

        results.push_back(measure("Ternary String QSort", arrayType, arr,
            [](std::vector<std::string>& a) {
                if (a.size() > 1) {
                    ternaryStringQuickSort(a, 0, (int)a.size() - 1, 0);
                }
            }));

        results.push_back(measure("String MergeSort (LCP)", arrayType, arr,
            [](std::vector<std::string>& a) {
                if (a.size() > 1) {
                    std::vector<int> lcp(a.size(), 0);
                    stringMergeSort(a, lcp, 0, (int)a.size() - 1);
                }
            }));

        results.push_back(measure("MSD Radix Sort", arrayType, arr,
            [](std::vector<std::string>& a) {
                if (a.size() > 1) {
                    msdRadixSort(a, 0, (int)a.size() - 1, 0);
                }
            }));

        results.push_back(measure("MSD Radix + TStrQSort", arrayType, arr,
            [](std::vector<std::string>& a) {
                if (a.size() > 1) {
                    msdRadixSortHybrid(a, 0, (int)a.size() - 1, 0);
                }
            }));

        return results;
    }

    static bool verify(std::vector<std::string> arr,
                       std::function<void(std::vector<std::string>&)> sortFn) {
        long long saved = charCount;
        sortFn(arr);
        charCount = saved;

        for (int i = 0; i + 1 < (int)arr.size(); ++i) {
            if (arr[i] > arr[i + 1]) {
                return false;
            }
        }
        return true;
    }
};

int main() {
    StringGenerator gen(12345);
    StringSortTester tester;

    {
        std::vector<std::string> sample = gen.randomArray(200);
        auto check = [&](auto fn, const std::string& name) {
            long long s = charCount;
            std::vector<std::string> copy = sample;
            fn(copy);
            charCount = s;
            bool ok = std::is_sorted(copy.begin(), copy.end());
            std::cerr << name << ": " << (ok ? "OK" : "FAIL") << "\n";
        };

        check([](std::vector<std::string>& a){ quickSortStd(a, 0, (int)a.size()-1); }, "QuickSort std");
        check([](std::vector<std::string>& a){ mergeSortStd(a, 0, (int)a.size()-1); }, "MergeSort std");
        check([](std::vector<std::string>& a){ ternaryStringQuickSort(a, 0, (int)a.size()-1, 0); }, "Ternary QSort");
        check([](std::vector<std::string>& a){
            std::vector<int> lcp(a.size(), 0);
            stringMergeSort(a, lcp, 0, (int)a.size()-1);
        }, "String MergeSort LCP");
        check([](std::vector<std::string>& a){ msdRadixSort(a, 0, (int)a.size()-1, 0); }, "MSD Radix");
        check([](std::vector<std::string>& a){ msdRadixSortHybrid(a, 0, (int)a.size()-1, 0); }, "MSD Radix Hybrid");
    }

    std::vector<std::string> baseRandom = gen.randomArray(3000);
    std::vector<std::string> baseReverse = gen.reverseSortedArray(3000);
    std::vector<std::string> baseNearSorted = gen.nearlySortedArray(3000);
    std::vector<std::string> baseCommonPrefix = gen.commonPrefixArray(3000, 50);

    std::ofstream csv("results.csv");
    csv << "algorithm,array_type,n,time_ms,char_cmps\n";

    auto runForSizes = [&](const std::vector<std::string>& base, const std::string& typeName) {
        for (int n = 100; n <= 3000; n += 100) {
            std::vector<std::string> arr(base.begin(), base.begin() + n);
            std::vector<BenchResult> results = tester.runAll(arr, typeName);
            for (BenchResult &r: results) {
                csv << r.name << ","
                    << r.arrayType << ","
                    << r.arraySize << ","
                    << r.timeMs << ","
                    << r.charCmps << "\n";
            }
            std::cerr << typeName << " n=" << n << " done\n";
        }
    };

    runForSizes(baseRandom,"random");
    runForSizes(baseReverse,"reverse_sorted");
    runForSizes(baseNearSorted,"nearly_sorted");
    runForSizes(baseCommonPrefix,"common_prefix");

    csv.close();
    return 0;
}