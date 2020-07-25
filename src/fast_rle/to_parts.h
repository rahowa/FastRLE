//
// Created by Валентин Евсеенко on 5/9/20.
//

#ifndef FAST_RLE_TO_PARTS_H
#define FAST_RLE_TO_PARTS_H

#include <utility>
#include <iterator>
#include <algorithm>

template <typename Iterator>
class ToParts{
public:
    ToParts(uint8_t numParts, Iterator begin, Iterator end):
            numParts(numParts),
            begin(begin),
            end(end),
            finish(begin),
            batchSize(std::distance(begin, end)/numParts)
    {
    };

    auto get(){
        if(producedIterations == numParts)
            return std::make_pair(end, end);

        begin = finish;
        finish += std::min(batchSize, std::distance(finish, end));
        ++producedIterations;
        return std::make_pair(begin, finish);
    }

    auto done() const{
        return producedIterations == numParts;
    }

private:
    uint8_t producedIterations{0};
    uint8_t numParts;
    long batchSize;
    Iterator begin;
    Iterator end;
    Iterator finish;
};

#endif //FAST_RLE_TO_PARTS_H
