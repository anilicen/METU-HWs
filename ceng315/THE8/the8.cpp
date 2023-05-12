#include "the8.h"
#define numberOfCharacters 300
// DO NOT ADD OTHER LIBRARIES

int nextState(std::string ptrn, int M, int st, int anotherWeirdVariable, std::string first, std::string second)
{
    // If the character c is same as next character
    // in pattern,then simply increment state
    if (st < M && (anotherWeirdVariable == ptrn[st]))
        return st + 1;
    else if (st < M && (ptrn[st] == first[0] && anotherWeirdVariable == second[0]))
        return st + 1;
    else if (st < M && (ptrn[st] == second[0] && anotherWeirdVariable == first[0]))
        return st + 1;

    int i;

    for (int ns = st; ns > 0; ns--)
    {
        if (ptrn[ns - 1] == anotherWeirdVariable)
        {
            for (i = 0; i < ns - 1; i++)
                if (ptrn[i] != ptrn[st - ns + 1 + i])
                    break;
            if (i == ns - 1)
                return ns;
        }
    }

    return 0;
}

void findTheTableForTransition(std::string ptrnnn, int sizeOfPtrnn, int TF[][numberOfCharacters], std::string first, std::string second)
{
    int state, x;
    for (state = 0; state <= sizeOfPtrnn; ++state)
        for (x = 0; x < numberOfCharacters; ++x)
            TF[state][x] = nextState(ptrnnn, sizeOfPtrnn, state, x, first, second);
}

void matchPattern(const std::string &sequence, const std::pair<std::string, std::string> mapping, const std::string &pattern)
{
    int sizeOfSequence = sequence.size();
    int sizeOfpattern = pattern.size();
    std::vector<int> ans;

    int tableOfAutomata[sizeOfpattern + 1][numberOfCharacters];

    findTheTableForTransition(pattern, sizeOfpattern, tableOfAutomata, mapping.first, mapping.second);

    int i, state = 0;
    for (i = 0; i < sizeOfSequence; i++)
    {
        state = tableOfAutomata[state][sequence[i]];
        if (state == sizeOfpattern)
            ans.push_back(i - sizeOfpattern + 1);
    }
    if (ans.size() == 0)
        std::cout << "NONE";
    for (int i = 0; i < ans.size(); i++)
    {
        if (ans.size() - 1 == i)
            std::cout << ans[i];
        else
            std::cout << ans[i] << " ";
    }
}
