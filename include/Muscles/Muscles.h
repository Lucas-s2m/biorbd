#ifndef BIORBD_MUSCLES_MUSCLES_H
#define BIORBD_MUSCLES_MUSCLES_H

#include <vector>
#include <memory>

#include "biorbdConfig.h"

namespace biorbd {
namespace utils {
class String;
class Matrix;
class Vector;
class Vector3d;
}

namespace rigidbody {
class GeneralizedCoordinates;
class GeneralizedTorque;
}

namespace muscles {
class MuscleGroup;
class StateDynamics;
class Force;

///
/// \brief Muscle group holder
///
class BIORBD_API Muscles
{
public:
    ///
    /// \brief Construct muscles
    ///
    Muscles();
    
    ///
    /// \brief Construct muscles from other muscles
    /// \param other The other muscles
    ///
    Muscles(const biorbd::muscles::Muscles& other);
    
    /// 
    /// \brief Destroy class properly
    ///
    virtual ~Muscles();

    ///
    /// \brief Deep copy of muscles
    /// \return A deep copy of muscles
    ///
    biorbd::muscles::Muscles DeepCopy() const;

    ///
    /// \brief Deep copy of muscles into another mucles
    /// \param other The muscles to copy
    ///
    void DeepCopy(
            const biorbd::muscles::Muscles& other);

    ///
    /// \brief Add a muscle group to the set
    /// \param name The name of the muscle group
    /// \param originName The origin segment name where the origin lies
    /// \param insertionName The insertion segment name where the origin lies
    ///
    void addMuscleGroup(
            const biorbd::utils::String &name,
            const biorbd::utils::String &originName,
            const biorbd::utils::String &insertionName);

    ///
    /// \brief Return the group ID 
    /// \param name The name of the muscle group
    /// \return The group ID (returns -1 if not found)
    ///
    int getGroupId(
            const biorbd::utils::String &name) const;

    ///
    /// \brief Return the muscle group of specific index
    /// \param idx The index of the muscle group to return
    /// \return A muscle group
    ///
    biorbd::muscles::MuscleGroup& muscleGroup(
            unsigned int idx);

    ///
    /// \brief Return the muscle group of specific index
    /// \param idx The index of the muscle group to return
    /// \return A muscle group
    ///
    const biorbd::muscles::MuscleGroup& muscleGroup(
            unsigned int idx) const;

    ///
    /// \brief Return the muscle group of specific name
    /// \param name The name of the muscle group to return
    /// \return A muscle group
    ///
    const biorbd::muscles::MuscleGroup& muscleGroup(
            const biorbd::utils::String& name) const;

    ///
    /// \brief Update all the muscles (positions, jacobian, etc.)
    /// \param Q The generalized coordinates
    /// \param updateKin Update kinematics (0: don't update, 1:only muscles, [2: both kinematics and muscles])
    ///
    void updateMuscles(
            const biorbd::rigidbody::GeneralizedCoordinates& Q,
            bool updateKin); 

    ///
    /// \brief Update all the muscles (positions, jacobian, etc.)
    /// \param Q The generalized coordinates
    /// \param QDot The generalized velocities
    /// \param updateKin Update kinematics (0: don't update, 1:only muscles, [2: both kinematics and muscles])
    ///
    void updateMuscles(
            const biorbd::rigidbody::GeneralizedCoordinates& Q,
            const biorbd::rigidbody::GeneralizedCoordinates& QDot,
            bool updateKin); 

    ///
    /// \brief Update by hand all the muscles (positions, jacobian, velocity, etc.)
    /// \param musclePointsInGlobal The muscle points in global reference frame
    /// \param jacoPointsInGlobal The jacobian points in global reference frame
    ///
    void updateMuscles(
            std::vector<std::vector<biorbd::utils::Vector3d>>& musclePointsInGlobal,
            std::vector<biorbd::utils::Matrix>& jacoPointsInGlobal); 

    ///
    /// \brief Update by hand all the muscles (positions, jacobian, velocity, etc.)
    /// \param musclePointsInGlobal The muscle points in global reference frame
    /// \param jacoPointsInGlobal The jacobian points in global reference frame
    /// \param QDot The generalized velocities
    ///
    void updateMuscles(
            std::vector<std::vector<biorbd::utils::Vector3d>>& musclePointsInGlobal,
            std::vector<biorbd::utils::Matrix>& jacoPointsInGlobal,
            const biorbd::rigidbody::GeneralizedCoordinates& QDot);

    ///
    /// \brief Compute the muscular joint torque
    /// \param F The force vector of all the muscles
    /// \param updateKin If the kinematics should be update or not
    /// \param Q The generalized coordinates (not needed if updateKin is false)
    /// \param QDot The generalized velocities (not needed if updateKin is false)
    ///
    /// The computation for the muscular joint torque is done from virtual power:
    ///
    /// i.e. $\text{-J} \times \text{F}$
    ///
    /// where $J$ is the muscle lengths jacobian and $F$ is the force vector of all the muscles
    /// 
    biorbd::rigidbody::GeneralizedTorque muscularJointTorque(
            const biorbd::utils::Vector& F,
            bool updateKin = true,
            const biorbd::rigidbody::GeneralizedCoordinates* Q = nullptr,
            const biorbd::rigidbody::GeneralizedCoordinates* QDot = nullptr);

    ///
    /// \brief Compute the muscular joint torque
    /// \param emg The dynamic state to compute the force vector
    /// \param F The force vector of all the muscles (output)
    /// \param updateKin If the kinematics should be update or not
    /// \param Q The generalized coordinates (not needed if updateKin is false)
    /// \param QDot The generalized velocities (not needed if updateKin is false)
    ///
    /// The computation for the muscular joint torque is done from virtual power:
    ///
    /// i.e. $\text{-J} \times \text{F}$
    ///
    /// where $J$ is the muscle lengths jacobian and $F$ is the force vector of all the muscles
    ///
    biorbd::rigidbody::GeneralizedTorque muscularJointTorque(
            const std::vector<std::shared_ptr<StateDynamics>> &emg,
            biorbd::utils::Vector& F,
            bool updateKin = true,
            const biorbd::rigidbody::GeneralizedCoordinates* Q = nullptr,
            const biorbd::rigidbody::GeneralizedCoordinates* QDot = nullptr);

    ///
    /// \brief Compute the muscular joint torque
    /// \param emg The dynamic state to compute the force vector
    /// \param updateKin If the kinematics should be update or not
    /// \param Q The generalized coordinates (not needed if updateKin is false)
    /// \param QDot The generalized velocities (not needed if updateKin is false)
    ///
    /// The computation for the muscular joint torque is done from virtual power:
    ///
    /// i.e. $\text{-J} \times \text{F}$
    ///
    /// where $J$ is the muscle lengths jacobian and $F$ is the force vector of all the muscles
    ///
    biorbd::rigidbody::GeneralizedTorque muscularJointTorque(
            const std::vector<std::shared_ptr<StateDynamics>> &emg,
            bool updateKin = true,
            const biorbd::rigidbody::GeneralizedCoordinates* Q = nullptr,
            const biorbd::rigidbody::GeneralizedCoordinates* QDot = nullptr);

    ///
    /// \brief Return the previously computed muscle length jacobian
    /// \return The muscle length jacobian
    ///
    biorbd::utils::Matrix musclesLengthJacobian();

    ///
    /// \brief Compute and return the muscle length Jacobian
    /// \param Q The generalized coordinates
    /// \return The muscle length Jacobian
    ///
    biorbd::utils::Matrix musclesLengthJacobian(
            const biorbd::rigidbody::GeneralizedCoordinates& Q);

    ///
    /// \brief Compute and return the muscle forces
    /// \param emg The dynamic state
    /// \param updateKin If the kinematics should be update or not
    /// \param Q The generalized coordinates
    /// \param QDot The generalized velocities
    /// \return The muscle forces
    ///
    std::vector<std::vector<std::shared_ptr<Force>>> musclesForces(
            const std::vector<std::shared_ptr<StateDynamics>> &emg,
            bool updateKin = true,
            const biorbd::rigidbody::GeneralizedCoordinates* Q = nullptr,
            const biorbd::rigidbody::GeneralizedCoordinates* QDot = nullptr);

    ///
    /// \brief Return the total number of muscle groups
    /// \return The total number of muscle groups
    ///
    unsigned int nbMuscleGroups() const; 

    ///
    /// \brief Return the total number of muscle
    /// \return The total number of muscles
    ///
    unsigned int nbMuscleTotal() const; 
protected:
    std::shared_ptr<std::vector<biorbd::muscles::MuscleGroup>> m_mus; ///< Holder for muscle groups

};

}}

#endif // BIORBD_MUSCLES_MUSCLES_H

