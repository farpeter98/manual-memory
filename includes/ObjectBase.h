#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#include <cstdint>


namespace ManualMemory {

class ObjectBase {
    friend class MemoryManager;
protected:
    ObjectBase (std::uint8_t** ptr, std::size_t* refCount) : m_ptr (ptr), m_refCount (refCount) {}

    std::uint8_t**  m_ptr;
    std::size_t*    m_refCount;

};

} // namespace ManualMemory


#endif // OBJECTBASE_H
