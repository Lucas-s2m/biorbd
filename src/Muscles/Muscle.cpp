#define BIORBD_API_EXPORTS
#include "Muscles/Muscle.h"

#include "Utils/Error.h"
#include "RigidBody/Joints.h"
#include "RigidBody/GeneralizedCoordinates.h"
#include "Muscles/PathModifiers.h"
#include "Muscles/StateDynamics.h"
#include "Muscles/StateDynamicsBuchanan.h"
#include "Muscles/Characteristics.h"
#include "Muscles/Geometry.h"
#include "Muscles/Force.h"

biorbd::muscles::Muscle::Muscle() :
    biorbd::muscles::Compound(),
    m_position(std::make_shared<biorbd::muscles::Geometry>()),
    m_characteristics(std::make_shared<biorbd::muscles::Characteristics>()),
    m_state(std::make_shared<biorbd::muscles::StateDynamics>())
{

}

biorbd::muscles::Muscle::Muscle(
        const biorbd::utils::String & name,
        const biorbd::muscles::Geometry & position,
        const biorbd::muscles::Characteristics &characteristics) :
    biorbd::muscles::Compound (name),
    m_position(std::make_shared<biorbd::muscles::Geometry>(position)),
    m_characteristics(std::make_shared<biorbd::muscles::Characteristics>(characteristics)),
    m_state(std::make_shared<biorbd::muscles::StateDynamics>())
{

}

biorbd::muscles::Muscle::Muscle(
        const biorbd::utils::String &name,
        const biorbd::muscles::Geometry &position,
        const biorbd::muscles::Characteristics &characteristics,
        const biorbd::muscles::StateDynamics &dynamicState) :
    biorbd::muscles::Compound (name),
    m_position(std::make_shared<biorbd::muscles::Geometry>(position)),
    m_characteristics(std::make_shared<biorbd::muscles::Characteristics>(characteristics)),
    m_state(std::make_shared<biorbd::muscles::StateDynamics>(dynamicState))
{

}

biorbd::muscles::Muscle::Muscle(
        const biorbd::utils::String &name,
        const biorbd::muscles::Geometry &position,
        const biorbd::muscles::Characteristics &characteristics,
        const biorbd::muscles::PathModifiers &pathModifiers) :
    biorbd::muscles::Compound (name, pathModifiers),
    m_position(std::make_shared<biorbd::muscles::Geometry>(position)),
    m_characteristics(std::make_shared<biorbd::muscles::Characteristics>(characteristics)),
    m_state(std::make_shared<biorbd::muscles::StateDynamics>())
{

}

biorbd::muscles::Muscle::Muscle(const biorbd::muscles::Muscle &other) :
    biorbd::muscles::Compound (other),
    m_position(other.m_position),
    m_characteristics(other.m_characteristics),
    m_state(other.m_state)
{

}

biorbd::muscles::Muscle::Muscle(const std::shared_ptr<biorbd::muscles::Muscle> other) :
    biorbd::muscles::Compound (other),
    m_position(other->m_position),
    m_characteristics(other->m_characteristics),
    m_state(other->m_state)
{

}

biorbd::muscles::Muscle::Muscle(const biorbd::utils::String& name,
        const biorbd::muscles::Geometry& g,
        const biorbd::muscles::Characteristics& c,
        const biorbd::muscles::PathModifiers &pathModifiers,
        const biorbd::muscles::StateDynamics& s) :
    biorbd::muscles::Compound(name,pathModifiers),
    m_position(std::make_shared<biorbd::muscles::Geometry>(g)),
    m_characteristics(std::make_shared<biorbd::muscles::Characteristics>(c)),
    m_state(std::make_shared<biorbd::muscles::StateDynamics>())
{
    setState(s);

    biorbd::utils::Error::check(pathModifiers.nbWraps() <= 1, "Multiple wrapping objects is not implemented yet");
}

biorbd::muscles::Muscle::~Muscle()
{
    //dtor
}

void biorbd::muscles::Muscle::DeepCopy(const biorbd::muscles::Muscle &other)
{
    *m_position = other.m_position->DeepCopy();
    *m_characteristics = other.m_characteristics->DeepCopy();
    *m_state = other.m_state->DeepCopy();
}

void biorbd::muscles::Muscle::updateOrientations(
        biorbd::rigidbody::Joints& model,
        const biorbd::rigidbody::GeneralizedCoordinates &Q,
        int updateKin)
{
    // Update de la position des insertions et origines
    m_position->updateKinematics(model,*m_characteristics,*m_pathChanger,&Q,nullptr,updateKin);
}
void biorbd::muscles::Muscle::updateOrientations(
        biorbd::rigidbody::Joints& model,
        const biorbd::rigidbody::GeneralizedCoordinates &Q,
        const biorbd::rigidbody::GeneralizedCoordinates &Qdot,
        int updateKin)
{
    // Update de la position des insertions et origines
    m_position->updateKinematics(model,*m_characteristics,*m_pathChanger,&Q,&Qdot,updateKin);
}
void biorbd::muscles::Muscle::updateOrientations(
        std::vector<biorbd::utils::Vector3d>& musclePointsInGlobal,
        biorbd::utils::Matrix &jacoPointsInGlobal){
    // Update de la position des insertions et origines
    m_position->updateKinematics(musclePointsInGlobal,jacoPointsInGlobal,*m_characteristics,nullptr);
}
void biorbd::muscles::Muscle::updateOrientations(
        std::vector<biorbd::utils::Vector3d>& musclePointsInGlobal,
        biorbd::utils::Matrix &jacoPointsInGlobal,
        const biorbd::rigidbody::GeneralizedCoordinates &Qdot)
{
    // Update de la position des insertions et origines
    m_position->updateKinematics(musclePointsInGlobal,jacoPointsInGlobal,*m_characteristics,&Qdot);
}

void biorbd::muscles::Muscle::setPosition(
        const biorbd::muscles::Geometry &positions)
{
    *m_position = positions;
}
const biorbd::muscles::Geometry &biorbd::muscles::Muscle::position() const {
    return *m_position;
}

double biorbd::muscles::Muscle::length(
        biorbd::rigidbody::Joints& model,
        const biorbd::rigidbody::GeneralizedCoordinates &Q,
        int updateKin)
{
    if (updateKin != 0)
        m_position->updateKinematics(model,*m_characteristics,*m_pathChanger,&Q,nullptr,updateKin);

    return position().length();
}

double biorbd::muscles::Muscle::musculoTendonLength(
        biorbd::rigidbody::Joints &m,
        const biorbd::rigidbody::GeneralizedCoordinates &Q,
        int updateKin)
{
    if (updateKin != 0)
        m_position->updateKinematics(m,*m_characteristics,*m_pathChanger,&Q,nullptr,updateKin);

    return m_position->musculoTendonLength();
}

double biorbd::muscles::Muscle::velocity(
        biorbd::rigidbody::Joints &model,
        const biorbd::rigidbody::GeneralizedCoordinates &Q,
        const biorbd::rigidbody::GeneralizedCoordinates &Qdot,
        bool updateKin)
{
    if (updateKin)
        m_position->updateKinematics(model,*m_characteristics,*m_pathChanger,&Q,&Qdot);

    return m_position->velocity();
}

double biorbd::muscles::Muscle::activationDot(
        const biorbd::muscles::StateDynamics &state, bool alreadyNormalized)
{
    return m_state->timeDerivativeActivation(state, characteristics(), alreadyNormalized);
}

void biorbd::muscles::Muscle::computeForce(const biorbd::muscles::State &emg)
{
    double force = getForceFromActivation(emg);
    (*m_force)[0]->setForceFromMuscleGeometry(*m_position, force); // origine vers le deuxieme point
    (*m_force)[1]->setForceFromMuscleGeometry(*m_position, force); // insertion vers l'avant-dernier point
}

const std::vector<biorbd::utils::Vector3d>& biorbd::muscles::Muscle::musclesPointsInGlobal(
        biorbd::rigidbody::Joints &model,
        const biorbd::rigidbody::GeneralizedCoordinates &Q)
{
    m_position->updateKinematics(model,*m_characteristics,*m_pathChanger,&Q,nullptr);

    return musclesPointsInGlobal();
}

const std::vector<biorbd::utils::Vector3d> &biorbd::muscles::Muscle::musclesPointsInGlobal() const
{
    return m_position->musclesPointsInGlobal();
}

void biorbd::muscles::Muscle::setForceIsoMax(double forceMax)
{
    m_characteristics->setForceIsoMax(forceMax);
}

void biorbd::muscles::Muscle::setCharacteristics(
        const biorbd::muscles::Characteristics &characteristics)
{
    *m_characteristics = characteristics;
}
const biorbd::muscles::Characteristics& biorbd::muscles::Muscle::characteristics() const
{
    return *m_characteristics;
}

// Get and set
void biorbd::muscles::Muscle::setState(
        const biorbd::muscles::StateDynamics &emg)
{
    if (emg.type() == biorbd::muscles::STATE_TYPE::BUCHANAN){
        m_state = std::make_shared<biorbd::muscles::StateDynamicsBuchanan>(biorbd::muscles::StateDynamicsBuchanan());
    }
    else if (emg.type() == biorbd::muscles::STATE_TYPE::DYNAMIC){
        m_state = std::make_shared<biorbd::muscles::StateDynamics>(biorbd::muscles::StateDynamics());
    }
    else
        biorbd::utils::Error::raise(biorbd::utils::String(biorbd::muscles::STATE_TYPE_toStr(emg.type())) + " is not a valid type for setState");
    *m_state = emg;
}
const biorbd::muscles::StateDynamics& biorbd::muscles::Muscle::state() const
{
    return *m_state;
}
biorbd::muscles::StateDynamics& biorbd::muscles::Muscle::state()
{
    return *m_state;
}
