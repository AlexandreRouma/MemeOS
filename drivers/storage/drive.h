#pragma once

class Drive_t {
public:
    virtual bool read(uint64_t addr, uint64_t size, uint8_t* buf) = 0 ;
    virtual bool write(uint64_t addr, uint64_t size, uint8_t* buf) = 0 ;
    //virtual uint64_t getCapacity() = 0 ;
};