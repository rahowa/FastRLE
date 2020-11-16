//
// Created by Валентин Евсеенко on 5/9/20.
//

#ifndef FAST_RLE_TO_PARTS_H
#define FAST_RLE_TO_PARTS_H

#include <utility>
#include <iterator>
#include <algorithm>
#include <math.h>


template <typename Iterator>
class ToParts{
public:
    ToParts(size_t numParts, Iterator begin, Iterator end):
            numParts(numParts),
            partBegin(begin),
            partEnd(begin),
            end(end),
            batchSize(std::ceil(std::distance(begin, end)/float(numParts)))
    {
    };

    auto get(){
        auto tmpL = partEnd;
        std::advance(tmpL, batchSize);
        if(std::distance(partEnd, end) < batchSize)
            partEnd = end;

        else std::advance(partEnd, batchSize);
        auto res = std::make_pair(partBegin, partEnd);
        partBegin = partEnd;
        return res;
    }

    auto done() const{
        return std::distance(partEnd, end) == 0;
    }

private:
    size_t numParts;
    size_t batchSize;
    Iterator partBegin;
    Iterator partEnd;
    Iterator end;
};

#endif //FAST_RLE_TO_PARTS_H
