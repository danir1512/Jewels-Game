#include "jewels.hpp"

Jewels::Jewels(const JewelsType& type) 
    : m_type{type} 
{
}

auto Jewels::getJewelType() -> JewelsType 
{
    return m_type;
}

void Jewels::setJewelType(const JewelsType& type) {
    m_type = type;
}

// make this inline
bool Jewels::getValid() const {
    return isValid;
}

//make this inline
void Jewels::destroy() {
    m_type = JewelsType::UNKOWN;
    isValid = false;
}
