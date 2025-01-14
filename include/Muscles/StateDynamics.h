#ifndef BIORBD_MUSCLES_STATE_DYNAMICS_H
#define BIORBD_MUSCLES_STATE_DYNAMICS_H

#include "biorbdConfig.h"
#include "Muscles/State.h"

namespace biorbd {
namespace muscles {
class Characteristics;
///
/// \brief EMG with the capability to compute the time derivative
///
class BIORBD_API StateDynamics : public biorbd::muscles::State
{
public:
    ///
    /// \brief Construct the state dynamics
    /// \param excitation The muscle excitation
    /// \param activation The muscle activation
    ///
    StateDynamics(
            double excitation = 0,
            double activation = 0);

    ///
    /// \brief Construct a state dynamics from another state dynamics
    /// \param other The other state dynamics
    ///
    StateDynamics(
            const biorbd::muscles::StateDynamics& other);

    ///
    /// \brief Destroy class properly
    ///
    virtual ~StateDynamics();

    ///
    /// \brief Deep copy of state dynamics
    /// \return A deep copy of state dynamics
    ///
    biorbd::muscles::StateDynamics DeepCopy() const;

    ///
    /// \brief Deep copy of state dynamics into another state dynamics
    /// \param other The state dynamics to copy
    ///
    void DeepCopy(
            const biorbd::muscles::StateDynamics& other);

    ///
    /// \brief Set the muscle excitation
    /// \param val The value of the muscle excitation
    ///
    virtual void setExcitation(double val);

    ///
    /// \brief Return the previous activation
    /// \return The previous activation
    ///
    double previousExcitation() const;

    ///
    /// \brief Set the muscle activation
    /// \param val The value of the muscle activation
    ///
    virtual void setActivation(double val);

    ///
    /// \brief Return the previous activation
    /// \return The previous activation
    ///
    double previousActivation() const;

    ///
    /// \brief Compute and return the activation time derivative from the excitation and activation
    /// \param excitation The muscle excitation
    /// \param activation The muscle activation
    /// \param characteristics The muscle characteristics
    /// \param alreadyNormalized If already normalized
    /// \return The activation time derivative
    ///
    virtual double timeDerivativeActivation(
            double excitation,
            double activation,
            const Characteristics& characteristics,
            bool alreadyNormalized = false); 

    ///
    /// \brief Compute and return the activation time derivative
    /// \param emg The emg
    /// \param characteristics The muscle characteristics
    /// \param alreadyNormalized If already normalized
    /// \return The activation time derivative
    ///
    virtual double timeDerivativeActivation(
            const StateDynamics& emg,
            const Characteristics& characteristics,
            bool alreadyNormalized = false); 

    ///
    /// \brief Compute and return the activation time derivative
    /// \param characteristics The muscle characteristics
    /// \param alreadyNormalized If already normalized
    /// \return The activation time derivative
    ///
    virtual double timeDerivativeActivation(
            const Characteristics& characteristics,
            bool alreadyNormalized = false);

    ///
    /// \brief Return the previously computed activation time derivative
    /// \return The activation time derivative
    ///   
    virtual double timeDerivativeActivation();

protected:
    virtual void setType();
    std::shared_ptr<double> m_previousExcitation; ///< The previous excitation
    std::shared_ptr<double> m_previousActivation; ///<The previous activation
    std::shared_ptr<double> m_activationDot;///< The activation velocity

};

}}

#endif // BIORBD_MUSCLES_STATE_DYNAMICS_H
