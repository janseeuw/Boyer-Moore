#include <iostream>
#include <vector>
#include <algorithm>

typedef unsigned int uint;
typedef unsigned char uchar;

using namespace std;

class BoyerMoore {
public:
    BoyerMoore(const uchar* pattern, uint m);
    void search(const uchar* text, uint n);
    
    //private:
    uint d; // ASCII
    uint m;
    const uchar* pattern;
    
    int heuristiek_1[256];
    vector<uint> suffix_tabel;
    vector<uint> verschuivings_tabel;
    
    int heuristiek_verkeerde_karakter(const uchar* text, uint k, uint i);
    int heuristiek_juiste_suffix(uint j);
    
    uint bereken_suffix(const uchar* str, uint length, uint j);
};

BoyerMoore::BoyerMoore(const uchar* pattern, uint m)
: d(256), pattern(pattern), m(m) {
    
    // Heuristiek 1 : Verkeerde karakter.
    // We bepalen voor ieder mogelijk karakter zijn meest rechtse voorkomen in het patroon.
    // Als het niet voorkomt in het patroon dan: -1
    for (uint i = 0; i < d; i++)
        heuristiek_1[i] = -1;
    
    for (uint i = 0; i < m; i++)
        heuristiek_1[pattern[i]] = i;
    
    // suffix table
    suffix_tabel.resize(m,0);
    for(int i = m-2; i >= 0; i--) {
        uint current = suffix_tabel[i+1];
        while(current>0 && pattern[m-1-current] != pattern[i])
            current = suffix_tabel[m-1-current];
        if(pattern[m-1-current] == pattern[i])
            current++;
        suffix_tabel[i] = current;
    }
    
    // shift table, base on suffix table: the same suffix with the
    // minimal shift
    int i;
    verschuivings_tabel.resize(suffix_tabel.size(),m - suffix_tabel[0] - 1);
    for(int j = suffix_tabel.size()-1; j>=0; j--){
        i =  m - suffix_tabel[j] - 1;
        verschuivings_tabel[i] = verschuivings_tabel[i]<(i+1-j) ? verschuivings_tabel[i]: i+1-j;
    }
    
}

int BoyerMoore::heuristiek_verkeerde_karakter(const uchar* text, uint k, uint i) {
    // k is de positie in de text, i is de positie in het patroon.
    
    // Dit is de eerste (simpele) heuristiek.
    // Dit zoekt de meest rechtse positie van het karakter op text[pos].
    // We bepalen de verschuiving zodat er een gelijkaardig karakter ligt op text[pos]
    int j = heuristiek_1[text[k]];
    int verschuiving = i - j;
    
    // Aangezien j (de meest rechtse index waar dit karakter voorkomt) _rechts_ van pos
    // kan liggen, zou dit een negatieve (of 0) verschuiving inhouden.
    // Dan verschuiven we gewoon met 1.
    if (verschuiving < 1)
        verschuiving = 1;
    
    return verschuiving;
}

int BoyerMoore::heuristiek_juiste_suffix(uint j) {
    // We hebben suffix van lengte m - i -1 gevonden. (-1 omdat c++ zero based is, de cursus is 1-based)
    // Situaties
    return verschuivings_tabel[j];
}

void BoyerMoore::search(const uchar* text, uint n) {
    
    std::vector<int> locaties;
    
    int i=0;
    while(i < n-m) {
        // Van achter naar voor over het pattern lopen en vergelijken met overeenkomstig karakter in de text.
        int j;
        for(j=m-1; j>=0 && pattern[j] == text[i+j]; j--);
        if(j<0){
            // Gevonden
            cout<<"Pattern found at index "<<i<<endl;
            i++;
        }else{
            int h1 = heuristiek_verkeerde_karakter(text, i+j, j);
            int h2= heuristiek_juiste_suffix(j);
            i+=max(h1,h2);
        }
        
    }
}

int main() {
    const uchar text[] = "alfa beta alfa charly";
    const uchar pattern[] = "alfa";
    
    uint n = 21;
    uint m = 4;
    
    BoyerMoore bm(pattern, m);
    
    bm.search(text, n);
    
    return 0;
}