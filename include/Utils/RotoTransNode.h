#ifndef BIORBD_UTILS_ROTO_TRANS_NODE_H
#define BIORBD_UTILS_ROTO_TRANS_NODE_H

#include <memory>
#include "biorbdConfig.h"
#include "Utils/RotoTrans.h"
#include "Utils/Node.h"

namespace biorbd {
namespace utils {
class String;
///
/// \brief A RotoTrans which is attached to a segment
///
class BIORBD_API RotoTransNode : public biorbd::utils::RotoTrans, public biorbd::utils::Node
{
public:
    ///
    /// \brief Construct a RotoTransNode
    ///
    RotoTransNode();
    
    ///
    /// \brief Construct RotoTransNode from an Eigen matrix
    /// \param other The eigen matrix
    ///
    template<typename OtherDerived> RotoTransNode(
            const Eigen::MatrixBase<OtherDerived>& other) :
        biorbd::utils::RotoTrans(other){}

    ///
    /// \brief Construct a RotoTransNode
    /// \param rt The RotoTrans matrix
    /// \param name The name of the rt
    /// \param parentName The name of the parent segment
    ///
    RotoTransNode(
            const RotoTrans& rt,
            const biorbd::utils::String &name,
            const biorbd::utils::String &parentName);

    ///
    /// \brief Deep copy of a RotoTransNode
    /// \return A deep copy of a RotoTrans node
    ///
    biorbd::utils::RotoTransNode DeepCopy() const;

    ///
    /// \brief Deep copy of a RotoTransNode into another RotoTransNode
    /// \param other The RotoTransNode to copy
    ///
    void DeepCopy(const biorbd::utils::RotoTransNode& other);

    ///
    /// \brief Allow to use the operator=
    /// \param other The Eigen matrix 
    ///
    template<typename OtherDerived>
        biorbd::utils::RotoTransNode& operator=(const Eigen::MatrixBase <OtherDerived>& other){
            this->biorbd::utils::RotoTrans::operator=(other);
            return *this;
        }

protected:
    ///
    /// \brief Set the type to ROTOTRANS
    ///
    void setType();

};

}}

#endif // BIORBD_UTILS_ROTO_TRANS_NODE_H
