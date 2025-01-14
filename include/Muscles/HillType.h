#ifndef BIORBD_MUSCLES_HILL_TYPE_H
#define BIORBD_MUSCLES_HILL_TYPE_H

#include "biorbdConfig.h"
#include "Muscles/Muscle.h"

namespace biorbd {
namespace muscles {

///
/// \brief Base class for all HillType muscles
/// Values for the constants are as follow:
/// FlCE_1 = 0.15
/// FlCE_2 = 0.45
/// FvCE_1 = 1
/// FvCE_2 = -.33/2 * FvCE_1/(1+FvCE_1)
/// FlPE_1 = 10.0
/// FlPE_2 = 5.0
/// eccentricForceMultiplier = 1.8
/// damping = 0.1
/// maxShorteningSpeed = 10.0
///
class BIORBD_API HillType : public biorbd::muscles::Muscle
{
public:
    ///
    /// \brief Contruct a Hill-type muscle
    ///
    HillType();

    ///
    /// \brief Construct a Hill-type muscle
    /// \param name The muscle name
    /// \param geometry The muscle geometry
    /// \param characteristics The muscle characteristics
    ///
    HillType(
            const biorbd::utils::String& name,
            const biorbd::muscles::Geometry& geometry,
            const biorbd::muscles::Characteristics& characteristics);

    ///
    /// \brief Construct a Hill-type muscle
    /// \param name The muscle name
    /// \param geometry The muscle geometry
    /// \param characteristics The muscle characteristics
    /// \param dynamicState The muscle dynamic state
    ///
    HillType(
            const biorbd::utils::String& name,
            const biorbd::muscles::Geometry& geometry,
            const biorbd::muscles::Characteristics& characteristics,
            const biorbd::muscles::StateDynamics& dynamicState);

    ///
    /// \brief Construct a Hill-type muscle
    /// \param name The muscle name
    /// \param geometry The muscle geometry
    /// \param characteristics The muscle characteristics
    /// \param pathModifiers The set of path modifiers
    ///
    HillType(
            const biorbd::utils::String& name,
            const biorbd::muscles::Geometry& geometry,
            const biorbd::muscles::Characteristics& characteristics,
            const biorbd::muscles::PathModifiers& pathModifiers);

    ///
    /// \brief Construct a Hill-type muscle
    /// \param name The muscle name
    /// \param geometry The muscle geometry
    /// \param characteristics The muscle characteristics
    /// \param pathModifiers The set of path modifiers
    /// \param dynamicState The dynamic state
    ///
    HillType(
            const biorbd::utils::String& name,
            const biorbd::muscles::Geometry& geometry,
            const biorbd::muscles::Characteristics& characteristics,
            const biorbd::muscles::PathModifiers& pathModifiers,
            const biorbd::muscles::StateDynamics& dynamicState);

    ///
    /// \brief Construct a Hill-type muscle from another muscle
    /// \param other The other muscle
    ///
    HillType(
            const biorbd::muscles::Muscle& other);

    ///
    /// \brief Construct a Hill-type muscle from another muscle
    /// \param other THe other muscle
    ///
    HillType(
            const std::shared_ptr<biorbd::muscles::Muscle> other);

    ///
    /// \brief Deep copy of a Hill-type muscle
    /// \return A deep copy of a Hill-type muscle
    ///
    biorbd::muscles::HillType DeepCopy() const;

    ///
    /// \brief Deep copy of a Hill-type muscle in a new Hill-type muscle
    /// \param other The Hill-type to copy
    ///
    void DeepCopy(const biorbd::muscles::HillType& other);

    ///
    /// \brief Return the muscle force vector at origin and insertion
    /// \param emg The EMG data
    /// \return The force vector at origin and insertion
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

    ///
    /// \brief Return the Force-Length of the contractile element
    /// \return The Force-Length of the contractile element
    ///
    double FlCE(const biorbd::muscles::StateDynamics& EMG);

    ///
    /// \brief Return the Force-Length of the passive element
    /// \return The Force-Length of the passive element
    ///
    double FlPE();

    ///
    /// \brief Return the Force-Velocity of the contractile element
    /// \return The Force-Velocity of the contractile element
    ///
    double FvCE();

    ///
    /// \brief Return the muscle damping (spring force)
    /// \return The muscle damping
    ///
    double damping();

protected:
    ///
    /// \brief Set type to Hill
    ///
    virtual void setType();

    ///
    /// \brief Compute the muscle damping
    ///
    virtual void computeDamping(); 

    ///
    /// \brief Compute the Force-length of the contractile element
    ///
    virtual void computeFlCE(const biorbd::muscles::StateDynamics &EMG); 

    ///
    /// \brief Compute the Force-Velocity of the contractile element
    ///
    virtual void computeFvCE(); 

    ///
    /// \brief Compute the Force-Length of the passive element
    ///
    virtual void computeFlPE(); 

    ///
    /// \brief Function allowing modification of the way the multiplication is done in computeForce(EMG)
    /// \param emg The EMG data
    /// \return The force from activation
    virtual double getForceFromActivation(const biorbd::muscles::State &emg); 

    ///
    /// \brief Normalize the EMG data
    /// \param emg EMG data
    ///
    virtual biorbd::muscles::StateDynamics normalizeEMG(const biorbd::muscles::StateDynamics& emg);

    // Attributs intermédiaires lors du calcul de la force
    std::shared_ptr<double> m_damping; ///< Muscle damping (spring force)
    std::shared_ptr<double> m_FlCE; ///<Force-Length of the contractile element
    std::shared_ptr<double> m_FlPE; ///< Force-Length of the passive element
    std::shared_ptr<double> m_FvCE; ///<Force-Velocity of the contractile element

    // Declaration of multiple constants
    std::shared_ptr<double> m_cste_FlCE_1; ///< constant 1 used in the FlCE
    std::shared_ptr<double> m_cste_FlCE_2; ///< constant 2 used in the FlCE
    std::shared_ptr<double> m_cste_FvCE_1; ///< constant 1 used in the FvCE
    std::shared_ptr<double> m_cste_FvCE_2; ///< constant 2 used in the FvCE
    std::shared_ptr<double> m_cste_FlPE_1; ///< constant 1 used in the FlPE
    std::shared_ptr<double> m_cste_FlPE_2; ///< constant 2 used in the FlPE
    std::shared_ptr<double> m_cste_eccentricForceMultiplier; ///< Constant used for ForceVelocity
    std::shared_ptr<double> m_cste_damping; ///< parameters used in damping
    std::shared_ptr<double> m_cste_maxShorteningSpeed; ///< Maximal velocity of shortening

};

}}

#endif // BIORBD_MUSCLES_HILL_TYPE_H
