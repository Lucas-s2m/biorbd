#ifndef BIORBD_MUSCLES_IDEALIZED_ACTUATOR_H
#define BIORBD_MUSCLES_IDEALIZED_ACTUATOR_H

#include "biorbdConfig.h"
#include "Muscles/Muscle.h"

namespace biorbd {
namespace muscles {

///
/// \brief Muscle that has a constant maximal force
///
class BIORBD_API IdealizedActuator : public biorbd::muscles::Muscle
{
public:
    ///
    /// \brief Contruct an idealized actuator
    ///
    IdealizedActuator();

    ///
    /// \brief Construct an idealized actuator
    /// \param name The name of the muscle
    /// \param geometry The muscle geometry
    /// \param characteristics The muscle characteristics
    ///
    IdealizedActuator(
            const biorbd::utils::String& name,
            const biorbd::muscles::Geometry& geometry,
            const biorbd::muscles::Characteristics& characteristics);

    ///
    /// \brief Construct an idealized actuator
    /// \param name The name of the muscle
    /// \param geometry The muscle geometry
    /// \param characteristics The muscle characteristics
    /// \param dynamicState The muscle dynamic state
    ///
    IdealizedActuator(
            const biorbd::utils::String& name,
            const biorbd::muscles::Geometry& geometry,
            const biorbd::muscles::Characteristics& characteristics,
            const biorbd::muscles::StateDynamics& dynamicState);

    ///
    /// \brief Construct an idealized actuator
    /// \param name The name of the muscle
    /// \param geometry The muscle geometry
    /// \param characteristics The muscle characteristics
    /// \param pathModifiers The set of path modifiers
    ///
    IdealizedActuator(
            const biorbd::utils::String& name,
            const biorbd::muscles::Geometry& geometry,
            const biorbd::muscles::Characteristics& characteristics,
            const biorbd::muscles::PathModifiers& pathModifiers);

    ///
    /// \brief Construct an idealized actuator
    /// \param name The name of the muscle
    /// \param geometry The muscle geometry
    /// \param characteristics The muscle characteristics
    /// \param pathModifiers The set of path modifiers
    /// \param dynamicState The dynamic state
    ///
    IdealizedActuator(
            const biorbd::utils::String& name,
            const biorbd::muscles::Geometry& geometry,
            const biorbd::muscles::Characteristics& characteristics,
            const biorbd::muscles::PathModifiers& pathModifiers,
            const biorbd::muscles::StateDynamics& dynamicState);

    ///
    /// \brief Construct an idealized actuator from another muscle
    /// \param other The other muscle
    ///
    IdealizedActuator(
            const biorbd::muscles::Muscle& other);

    ///
    /// \brief Construct an idealized actuator from another muscle
    /// \param other The other muscle
    ///
    IdealizedActuator(
            const std::shared_ptr<biorbd::muscles::Muscle> other);

    ///
    /// \brief Deep copy of an idealized actuator
    /// \return A deep copy of an idealized actuator
    ///
    biorbd::muscles::IdealizedActuator DeepCopy() const;

    ///
    /// \brief Deep copy of an idealized actuator from another idealized actuator
    /// \param other The idealized actuator to copy
    ///
    void DeepCopy(const biorbd::muscles::IdealizedActuator& other);

    ///
    /// \brief Return the force 
    /// \param emg EMG data
    /// \return The force
    ///
    virtual const std::vector<std::shared_ptr<biorbd::muscles::Force>>& force(
            const biorbd::muscles::StateDynamics& emg);

    ///
    /// \brief Return the muscle force vector at origin and insertion
    /// \param model The joint model
    /// \param Q The generalized coordinates
    /// \param Qdot The generalized velocities
    /// \param emg The EMG data
    /// \param updateKin Update kinematics (0: don't update, 1:only muscles, [2: both kinematics and muscles])
    /// \return The force vector at origin and insertion
    ///
    virtual const std::vector<std::shared_ptr<biorbd::muscles::Force>>& force(
            biorbd::rigidbody::Joints& model,
            const biorbd::rigidbody::GeneralizedCoordinates& Q,
            const biorbd::rigidbody::GeneralizedCoordinates& Qdot,
            const biorbd::muscles::StateDynamics& emg,
            int updateKin = 2);

    ///
    /// \brief Return the muscle force vector at origin and insertion
    /// \param model The joint model
    /// \param Q The generalized coordinates
    /// \param emg The EMG data
    /// \param updateKin Update kinematics (0: don't update, 1:only muscles, [2: both kinematics and muscles])
    /// \return The force vector at origin and insertion
    ///
    virtual const std::vector<std::shared_ptr<biorbd::muscles::Force>>& force(
            biorbd::rigidbody::Joints& model,
            const biorbd::rigidbody::GeneralizedCoordinates& Q,
            const biorbd::muscles::StateDynamics& emg,
            int updateKin = 2);
protected:
    ///
    /// \brief Function allowing modification of the way the multiplication is done in computeForce(EMG)
    /// \param emg The EMG data
    /// \return The force from activation
    virtual double getForceFromActivation(
            const biorbd::muscles::State &emg);

    ///
    /// \brief Set the type to Idealized_actuator
    ///
    virtual void setType();

};

}}

#endif // BIORBD_MUSCLES_IDEALIZED_ACTUATOR_H
