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
            begin(begin),
            end(end),
            finish(begin),
            batchSize(std::ceil(std::distance(begin, end)/float(numParts)))
    {
    };

    auto get(){
        if(producedIterations == numParts)
            return std::make_pair(end, end);

        begin = finish;
        auto diff = std::min(batchSize, static_cast<size_t>(std::distance(finish, end)));
        std::advance(finish, diff);
        ++producedIterations;
        return std::make_pair(begin, finish);
    }

    auto done() const{
        return producedIterations == numParts;
    }

private:
    size_t producedIterations{0};
    size_t numParts;
    size_t batchSize;
    Iterator begin;
    Iterator end;
    Iterator finish;
};

#endif //FAST_RLE_TO_PARTS_H
