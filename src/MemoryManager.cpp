#include "../includes/MemoryManager.h"

#include <cstring>
#include <new>
#include <iostream>

using namespace ManualMemory;


MemoryManager::MemoryManager (std::size_t memSize)
{
    m_memory = (std::uint8_t*) malloc (sizeof(std::uint8_t) * memSize);
    if (m_memory == nullptr)
        throw std::bad_alloc ();
}


MemoryManager::~MemoryManager ()
{
    free (m_memory);
}


MemoryManager& MemoryManager::GetInstance ()
{
    static MemoryManager manager { MEMORY_SIZE };
    return manager;
}


std::uint8_t** MemoryManager::Allocate (std::size_t size)
{
    // the object has greater size than the total memory
    if (size > MEMORY_SIZE)
        throw std::bad_alloc ();

    // check if any dead memory could store the data
    std::uint8_t* startAddress = m_memory;
    for (ObjectDescriptor& descriptor : m_descriptors) {
        if (descriptor.size >= size && !descriptor.isLive) {
            descriptor.size = size;
            descriptor.isLive = true;
            return &descriptor.startAddress;
        }
        startAddress = descriptor.startAddress + descriptor.size;
    }

    // check if there's enough storage at the end
    if (startAddress + size <= m_memory + MEMORY_SIZE) {
        m_descriptors.push_back ({ startAddress, size, true });
        return &(m_descriptors.back ().startAddress);

    }

    // check if the data can be stored at the end after freeing and compacting over the dead storage
    startAddress = FreeAndCompact ();
	if (startAddress + size <= m_memory + MEMORY_SIZE) {
        m_descriptors.push_back ({ startAddress, size, true });
        return &(m_descriptors.back ().startAddress);
    }

    // there isn't enough storage at the end even after compacting
    throw std::bad_alloc ();
}


void MemoryManager::Deallocate (std::uint8_t** startAddressPtr)
{
    for (ObjectDescriptor& descriptor : m_descriptors) {
        if (&descriptor.startAddress == startAddressPtr) {
            descriptor.isLive = false;
            return;
        }
    }
    // the given address is not present
    throw std::exception ();
}


std::uint8_t* MemoryManager::FreeAndCompact ()
{
    std::uint8_t* currDest = m_memory;
    bool hasDead = false;
    decltype (m_descriptors)::iterator it = m_descriptors.begin ();
    while (it != m_descriptors.end ()) {
        if (it->isLive && hasDead) {
            memcpy (currDest, it->startAddress, it->size);
            it->startAddress = currDest;
            currDest = currDest + it->size;
            ++it;
        }
        else {
            hasDead = true;
            it = m_descriptors.erase (it);
        }
    }
    return currDest;

}


void MemoryManager::PrintDescriptors () const
{
    for (const ObjectDescriptor& descriptor : m_descriptors) {
        std::cout << "StartAddress:        "    << (void*) descriptor.startAddress      << "\n";
        std::cout << "StartAddress offset: "    << descriptor.startAddress - m_memory   << "\n";
        std::cout << "Size:                "    << descriptor.size                      << "\n";
        std::cout << "IsLive:              "    << std::boolalpha << descriptor.isLive  << "\n";
        std::cout << "----------------------------------------------------------------" << std::endl;
    }
}


void MemoryManager::PrintMemory () const
{
    int whole = MEMORY_SIZE / 16;
    int leftover = MEMORY_SIZE % 16;
    for (int i = 0; i < whole; ++i) {
        for (int j = 0; j < 16; ++j)
            std::cout << (int) m_memory[i * 16 + j] << " ";
        std::cout << "\n";
    }
    for (int j = 0; j < leftover; ++j) {
        std::cout << (int) m_memory[whole * 16 + j] << " ";
    }
    std::cout << std::endl;
} 
