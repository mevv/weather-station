#ifndef _RAM_CACHE_H_
#define _RAM_CACHE_H_

#include "settings.h"

// simple queue class to cache fixed amount of data (pop the oldest value on overflow)

template <typename T>
struct RamCacheNode
{
    explicit RamCacheNode(const T& d):
        data(d),
        next(nullptr)
    {}

    T data;
    RamCacheNode* next;
};

template <typename T>
class RamCache
{
public:
    explicit RamCache() :
        m_head(nullptr)
    {}

    ~RamCache()
    {
        while (!empty())
            pop();
    }

    void push(const T& item)
    {
        if (size() == RAM_CACHE_SIZE)
            pop();

        if (empty())
            m_head = new RamCacheNode<T>(item);
        else
            tail()->next = new RamCacheNode<T>(item);
    }

    T pop()
    {
        if (empty())
            return T();

        auto oldHead = m_head;
        T data = m_head->data;
        m_head = m_head->next;
        delete oldHead;
        return data;
    }

    bool empty() const
    {
        return !m_head;
    }

    byte size() const
    {
        if (empty())
            return 0;

        byte size = 1;
        auto cur = m_head;
        while(cur->next)
        {
            cur = cur->next;
            size++;
        }

        return size;
    }

private:
    RamCacheNode<T>* m_head;

    RamCacheNode<T>* tail()
    {
        if (empty())
            return nullptr;

        auto cur = m_head;
        while(cur->next)
            cur = cur->next;
        return cur;
    }
};

#endif
