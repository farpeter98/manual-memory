#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "ObjectBase.h"

#include <cstdint>
#include <memory>
#include <new>
#include <utility>

#include "MemoryManager.h"


namespace ManualMemory {

template<typename T>
class Object : public ObjectBase {
public:
    Object ()
        : ObjectBase (MemoryManager::GetInstance ().Allocate (sizeof (T)), (std::size_t*) malloc (sizeof (std::size_t)))
    {
        new (*(this->m_ptr)) T ();
        *(this->m_refCount) = 1;
    }


    template<typename... Params>
    Object (Params&& ...args)
        : ObjectBase (MemoryManager::GetInstance ().Allocate (sizeof (T)), (std::size_t*) malloc (sizeof (std::size_t)))
    {
        new (*(this->m_ptr)) T (std::forward<Params>(args)...);
        *(this->m_refCount) = 1;
    }


    Object (const Object& other) : ObjectBase (other.m_ptr, other.m_refCount)
    {
        ++(*m_refCount);
    }


    Object (Object&& other) : ObjectBase (other.m_ptr, other.m_refCount)
    {
        other.m_ptr = nullptr;
        other.m_refCount = nullptr;
    }


    Object& operator= (const Object& other)
    {
        DecreaseRefCount ();
        this->m_ptr = other.m_ptr;
        this->m_refCount = other.m_refCount;
        ++(*m_refCount);
        return *this;
    }


    Object& operator= (Object&& other)
    {
        DecreaseRefCount ();
        this->m_ptr = other.m_ptr;
        this->m_refCount = other.m_refCount;
        other.m_ptr = nullptr;
        other.m_refCount = nullptr;
        return *this;
    }


    ~Object ()
    {
        DecreaseRefCount ();
    }


    T* operator-> () {
        return *GetTypedPtr ();
    }


    const T* operator-> () const {
        return *GetTypedPtr ();
    }


    T& operator* () {
        return **GetTypedPtr ();
    }


    const T& operator* () const {
        return **GetTypedPtr ();
    }


private:
    inline T** GetTypedPtr ()
    {
        return reinterpret_cast<T**> (this->m_ptr);
    }


    inline const T** GetTypedPtr () const
    {
        return const_cast<const T**> (reinterpret_cast<T**> (this->m_ptr));
    }


    inline void DecreaseRefCount ()
    {
        if (this->m_ptr == nullptr || this->m_refCount == nullptr)
            return;

        --(*(this->m_refCount));
        if (*(this->m_refCount) == 0) {
            (*GetTypedPtr ())->~T ();
            MemoryManager::GetInstance ().Deallocate (this->m_ptr);
            free (this->m_refCount);
        }
    }
};

} // namespace ManualMemory


#endif // OBJECT_HPP