#pragma once
#include <vector>
#include <functional>
#include <optional>

template <typename T>
class JSLikeVectorAdapter
{
private:
    std::vector<T> collection;

    void setCollection(const std::vector<T> & newCollection)
    {
        collection.clear();
        collection = newCollection;
    }

public:
    JSLikeVectorAdapter(const std::vector<T> & collection)
        : collection(collection) {}

    JSLikeVectorAdapter & reduce(const std::function<bool(const T&)> & predicate)
    {
        std::vector<T> newCollection;
        for (auto item : collection)
        {
            if (predicate(item))
            {
                newCollection.push_back(item);
            }
        }
        setCollection(newCollection);
        return *this;
    }

    JSLikeVectorAdapter & reduceIf(bool execute, const std::function<bool(const T&)> & predicate)
    {
        return execute ? reduce(predicate) : *this;
    }

    JSLikeVectorAdapter & map(const std::function<T(const T&)> & predicate)    
    {
        std::vector<T> newCollection;
        for (auto item : collection)
        {
            newCollection.push_back(predicate(item));
        }
        setCollection(newCollection);
        return *this;
    }

    JSLikeVectorAdapter & flat(const std::function<std::optional<std::vector<T>>(const T&)> & predicate)
    {
        std::vector<T> flatVector(collection);
        for (size_t i = 0; i < flatVector.size(); i++)
        {
            auto newCollectionOpt = predicate(flatVector[i]);
            if (newCollectionOpt.has_value())
            {
                auto newCollection = newCollectionOpt.value();
                auto newFlatVector = std::vector<T>();
                std::copy(flatVector.begin(), flatVector.begin() + i, std::back_inserter(newFlatVector));
                std::copy(newCollection.begin(), newCollection.end(), std::back_inserter(newFlatVector));
                std::copy(flatVector.begin() + i + 1, flatVector.end(), std::back_inserter(newFlatVector));
                flatVector.clear();
                flatVector = newFlatVector;
                i--;
            }
        }
        setCollection(flatVector);
        return *this;
    }

    JSLikeVectorAdapter & flatIf(bool execute, const std::function<std::optional<std::vector<T>>(const T&)> & predicate)
    {
        return execute ? flat(predicate) : *this;
    }

    std::vector<T> value()
    {
        return collection;
    }
};