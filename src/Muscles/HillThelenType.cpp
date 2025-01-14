#define BIORBD_API_EXPORTS
#include "Muscles/HillThelenType.h"

#include <math.h>
#include "Utils/String.h"
#include "Muscles/Geometry.h"
#include "Muscles/Characteristics.h"

biorbd::muscles::HillThelenType::HillThelenType() :
    biorbd::muscles::HillType()
{
    setType();
}
biorbd::muscles::HillThelenType::HillThelenType(
        const biorbd::utils::String& name,
        const biorbd::muscles::Geometry& geometry,
        const biorbd::muscles::Characteristics& characteristics) :
    biorbd::muscles::HillType (name, geometry, characteristics)
{
    setType();
}

biorbd::muscles::HillThelenType::HillThelenType(
        const biorbd::utils::String &name,
        const biorbd::muscles::Geometry &geometry,
        const biorbd::muscles::Characteristics &characteristics,
        const biorbd::muscles::StateDynamics &dynamicState) :
    biorbd::muscles::HillType (name, geometry, characteristics, dynamicState)
{
    setType();
}

biorbd::muscles::HillThelenType::HillThelenType(
        const biorbd::utils::String &name,
        const biorbd::muscles::Geometry &geometry,
        const biorbd::muscles::Characteristics &characteristics,
        const biorbd::muscles::PathModifiers &pathModifiers) :
    biorbd::muscles::HillType (name, geometry, characteristics, pathModifiers)
{
    setType();
}

biorbd::muscles::HillThelenType::HillThelenType(
        const biorbd::utils::String& name,
        const biorbd::muscles::Geometry& geometry,
        const biorbd::muscles::Characteristics& characteristics,
        const biorbd::muscles::PathModifiers &pathModifiers,
        const biorbd::muscles::StateDynamics & state) :
    biorbd::muscles::HillType (name, geometry, characteristics, pathModifiers, state)
{
    setType();
}

biorbd::muscles::HillThelenType::HillThelenType(const biorbd::muscles::Muscle &other) :
    biorbd::muscles::HillType (other)
{

}

biorbd::muscles::HillThelenType::HillThelenType(const std::shared_ptr<biorbd::muscles::Muscle> other) :
    biorbd::muscles::HillType(other)
{

}

biorbd::muscles::HillThelenType biorbd::muscles::HillThelenType::DeepCopy() const
{
    biorbd::muscles::HillThelenType copy;
    copy.DeepCopy(*this);
    return copy;
}

void biorbd::muscles::HillThelenType::DeepCopy(const biorbd::muscles::HillThelenType &other)
{
    biorbd::muscles::HillType::DeepCopy(other);
}

void biorbd::muscles::HillThelenType::computeFlPE(){
    if (position().length() > characteristics().tendonSlackLength())
        *m_FlPE = (exp( *m_cste_FlPE_1 * (position().length()/characteristics().optimalLength()-1)) -1)
            /
            (exp( *m_cste_FlPE_2 )-1);
    else
        *m_FlPE = 0;
}

void biorbd::muscles::HillThelenType::computeFlCE(const biorbd::muscles::StateDynamics&){
    *m_FlCE = exp( -pow(((position().length() / characteristics().optimalLength())-1), 2 ) /  *m_cste_FlCE_2 );
}

void biorbd::muscles::HillThelenType::setType()
{
    *m_type = biorbd::muscles::MUSCLE_TYPE::HILL_THELEN;
}
