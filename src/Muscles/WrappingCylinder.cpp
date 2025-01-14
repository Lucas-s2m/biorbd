#define BIORBD_API_EXPORTS
#include "Muscles/WrappingCylinder.h"

#include "Utils/String.h"
#include "Utils/RotoTrans.h"
#include "RigidBody/Joints.h"

biorbd::muscles::WrappingCylinder::WrappingCylinder() :
    biorbd::muscles::WrappingObject (),
    m_dia(std::make_shared<double>(0)),
    m_length(std::make_shared<double>(0)),
    m_isCylinderPositiveSign(std::make_shared<bool>(true)),
    m_RTtoParent(std::make_shared<biorbd::utils::RotoTrans>()),
    m_p1Wrap(std::make_shared<biorbd::utils::Vector3d>()),
    m_p2Wrap(std::make_shared<biorbd::utils::Vector3d>()),
    m_lengthAroundWrap(std::make_shared<double>(0))
{
    *m_typeOfNode = biorbd::utils::NODE_TYPE::WRAPPING_CYLINDER;
}

biorbd::muscles::WrappingCylinder::WrappingCylinder(
        const biorbd::utils::RotoTrans &rt,
        double diameter,
        double length,
        bool isCylinderPositiveSign) :
    biorbd::muscles::WrappingObject (rt.trans()),
    m_dia(std::make_shared<double>(diameter)),
    m_length(std::make_shared<double>(length)),
    m_isCylinderPositiveSign(std::make_shared<bool>(isCylinderPositiveSign)),
    m_RTtoParent(std::make_shared<biorbd::utils::RotoTrans>(rt)),
    m_p1Wrap(std::make_shared<biorbd::utils::Vector3d>()),
    m_p2Wrap(std::make_shared<biorbd::utils::Vector3d>()),
    m_lengthAroundWrap(std::make_shared<double>(0))
{
    *m_typeOfNode = biorbd::utils::NODE_TYPE::WRAPPING_CYLINDER;
}

biorbd::muscles::WrappingCylinder::WrappingCylinder(
        const biorbd::utils::RotoTrans &rt,
        double diameter,
        double length,
        bool isCylinderPositiveSign,
        const biorbd::utils::String &name,
        const biorbd::utils::String &parentName) :
    biorbd::muscles::WrappingObject (rt.trans(), name, parentName),
    m_dia(std::make_shared<double>(diameter)),
    m_length(std::make_shared<double>(length)),
    m_isCylinderPositiveSign(std::make_shared<bool>(isCylinderPositiveSign)),
    m_RTtoParent(std::make_shared<biorbd::utils::RotoTrans>(rt)),
    m_p1Wrap(std::make_shared<biorbd::utils::Vector3d>()),
    m_p2Wrap(std::make_shared<biorbd::utils::Vector3d>()),
    m_lengthAroundWrap(std::make_shared<double>(0))
{
    *m_typeOfNode = biorbd::utils::NODE_TYPE::WRAPPING_CYLINDER;
}

biorbd::muscles::WrappingCylinder biorbd::muscles::WrappingCylinder::DeepCopy() const
{
    biorbd::muscles::WrappingCylinder copy;
    copy.DeepCopy(*this);
    return copy;
}

void biorbd::muscles::WrappingCylinder::DeepCopy(const biorbd::muscles::WrappingCylinder &other)
{
    biorbd::muscles::WrappingObject::DeepCopy(other);
    *m_dia = *other.m_dia;
    *m_length = *other.m_length;
    *m_isCylinderPositiveSign = *other.m_isCylinderPositiveSign;
    *m_RTtoParent = *other.m_RTtoParent;
    *m_p1Wrap = other.m_p1Wrap->DeepCopy();
    *m_p2Wrap = other.m_p2Wrap->DeepCopy();
    *m_lengthAroundWrap = *other.m_lengthAroundWrap;
}

void biorbd::muscles::WrappingCylinder::wrapPoints(
        const biorbd::utils::RotoTrans& rt,
        const biorbd::utils::Vector3d& p1_bone,
        const biorbd::utils::Vector3d& p2_bone,
        biorbd::utils::Vector3d& p1,
        biorbd::utils::Vector3d& p2,
        double *length)
{
    // This function takes the position of the wrapping and finds the location where muscle 1 and 2 leave the wrapping object

    // Find the nodes in the RT reference (of the cylinder)
    NodeMusclePair p_glob(p1_bone, p2_bone);
    p_glob.m_p1->applyRT(rt.transpose());
    p_glob.m_p2->applyRT(rt.transpose());

    // Find the tangents of these points to the circle (cylinder seen from above)
    biorbd::utils::Vector3d p1_tan(0, 0, 0);
    biorbd::utils::Vector3d p2_tan(0, 0, 0);
    findTangentToCircle(*p_glob.m_p1, p1_tan);
    findTangentToCircle(*p_glob.m_p2, p2_tan);

    // Find the vertical component
    NodeMusclePair tanPoints(p1_tan, p2_tan);
    findVerticalNode(p_glob, tanPoints);

    // If asked, compute the distance distance traveled on the periphery of the cylinder
    // Apply pythagorus to the cercle arc
    if (length != nullptr) // If it is not nullptr 
        *length = computeLength(tanPoints);

    // Reset the points in global (space)
    tanPoints.m_p1->applyRT(rt);
    tanPoints.m_p2->applyRT(rt);

    // Reset the desired values
    p1 = *tanPoints.m_p1;
    p2 = *tanPoints.m_p2;

    // Store the values for a futur call
    m_p1Wrap = tanPoints.m_p1;
    m_p2Wrap = tanPoints.m_p2;
    if (length != nullptr) // If it is not nullptr
        *m_lengthAroundWrap = *length;
}

void biorbd::muscles::WrappingCylinder::wrapPoints(
        biorbd::rigidbody::Joints& model,
        const biorbd::rigidbody::GeneralizedCoordinates& Q,
        const biorbd::utils::Vector3d& p1_bone,
        const biorbd::utils::Vector3d& p2_bone,
        biorbd::utils::Vector3d& p1,
        biorbd::utils::Vector3d& p2,
        double *length) {
    // This function takes a model and a position of the model and returns the location where muscle 1 et 2 leave the wrapping object

    wrapPoints(RT(model,Q), p1_bone, p2_bone, p1, p2, length);
}

void biorbd::muscles::WrappingCylinder::wrapPoints(
        biorbd::utils::Vector3d& p1,
        biorbd::utils::Vector3d& p2,
        double *length){
    p1 = *m_p1Wrap;
    p2 = *m_p2Wrap;
    if (length != nullptr)
        *length = *m_lengthAroundWrap;
}

const biorbd::utils::RotoTrans& biorbd::muscles::WrappingCylinder::RT(
        biorbd::rigidbody::Joints &model,
        const biorbd::rigidbody::GeneralizedCoordinates& Q,
        bool updateKin)
{
    if (updateKin)
        model.UpdateKinematicsCustom(&Q);

    // Get the RotoTrans matrix of the cylinder in space
    *m_RT = model.globalJCS(*m_parentName) * *m_RTtoParent;
    return *m_RT;
}

void biorbd::muscles::WrappingCylinder::setDiameter(double val)
{
    *m_dia = val;
}

double biorbd::muscles::WrappingCylinder::diameter() const
{
    return *m_dia;
}

double biorbd::muscles::WrappingCylinder::radius() const
{
    return *m_dia/2;
}

void biorbd::muscles::WrappingCylinder::setLength(double val)
{
    *m_length = val;
}

double biorbd::muscles::WrappingCylinder::length() const
{
    return *m_length;
}

void biorbd::muscles::WrappingCylinder::findTangentToCircle(
        const biorbd::utils::Vector3d& p,
        biorbd::utils::Vector3d& p_tan) const {
    double p_dot = p.block(0,0,2,1).dot(p.block(0,0,2,1));
    const Eigen::Vector2d& Q0(radius()*radius()/p_dot*p.block(0,0,2,1));
    Eigen::Matrix2d tp;
    tp << 0, -1,
          1, 0;

    const Eigen::Vector2d& T(radius()/p_dot*std::sqrt(p_dot-radius()*radius()) * tp * p.block(0,0,2,1));

    // GEt the tangent on both sides
    NodeMusclePair m(p,p);
    m.m_p1->block(0,0,2,1) = Q0 + T;
    m.m_p2->block(0,0,2,1) = Q0 - T;

    // Select on of the two tangents
    selectTangents(m, p_tan);
}

void biorbd::muscles::WrappingCylinder::selectTangents(
        const NodeMusclePair &p1,
        biorbd::utils::Vector3d &p_tan) const {
    if (m_isCylinderPositiveSign){
        if ((*p1.m_p2)(0) >= (*p1.m_p1)(0))
            p_tan = *p1.m_p2;
        else
            p_tan = *p1.m_p1;
    } else {
        if ( (*p1.m_p2)(0) < (*p1.m_p1)(0))
            p_tan = *p1.m_p2;
        else
            p_tan = *p1.m_p1;
    }

}
bool biorbd::muscles::WrappingCylinder::findVerticalNode(const NodeMusclePair &pointsInGlobal,
        NodeMusclePair &pointsToWrap) const {
    // Before eerything, make sure the point wrap
    if (!checkIfWraps(pointsInGlobal, pointsToWrap)){ // If it doesn't pass by the wrap, put NaN and stop
        for (unsigned int i=0; i<3; ++i){
            (*pointsToWrap.m_p1)(i) = static_cast<double>(NAN);
            (*pointsToWrap.m_p2)(i) = static_cast<double>(NAN);
        }
        return false;
    }
    else{
        // Make sure the z component won't cause any problem in the rotation computation
        (*pointsToWrap.m_p1)(2) = 0;
        (*pointsToWrap.m_p2)(2) = 0;
    }


    // Strategy : Find the matrix of the passage between the aligned points in x and the cylinder. Find the location where the points cross the cylinder

    // X is the straight line between the two points
    biorbd::utils::Vector3d X(*pointsInGlobal.m_p2 - *pointsInGlobal.m_p1);
    // Z is the empty axis of the cylinder
    biorbd::utils::Vector3d Z(0,0,1);

    biorbd::utils::Vector3d Y(Z.cross(X));
    // Re-compute X for it to be aligned with the cylinder
    X = Y.cross(Z);
    // Normalise everything
    X = X/X.norm();
    Y = Y/Y.norm();
    Z = Z/Z.norm();
    // Concatenate to get the rotation matrix
    biorbd::utils::RotoTrans R;
    R << X(0), X(1), X(2), 0,
         Y(0), Y(1), Y(2), 0,
         Z(0), Z(1), Z(2), 0,
         0,    0,    0,    1;

    // Turn the points in the R reference
    biorbd::utils::Vector3d globA(*pointsInGlobal.m_p1);
    biorbd::utils::Vector3d globB(*pointsInGlobal.m_p2);
    biorbd::utils::Vector3d wrapA(*pointsToWrap.m_p1);
    biorbd::utils::Vector3d wrapB(*pointsToWrap.m_p2);
    globA.applyRT(R);
    globB.applyRT(R);
    wrapA.applyRT(R);
    wrapB.applyRT(R);

    // The height depends on the relative distance
    (*pointsToWrap.m_p1)(2) = (wrapA(0)-globB(0)) / (globA(0)-globB(0)) * ((*pointsInGlobal.m_p1)(2)-(*pointsInGlobal.m_p2)(2)) + (*pointsInGlobal.m_p2)(2);
    (*pointsToWrap.m_p2)(2) = (wrapB(0)-globB(0)) / (globA(0)-globB(0)) * ((*pointsInGlobal.m_p1)(2)-(*pointsInGlobal.m_p2)(2)) + (*pointsInGlobal.m_p2)(2);

    return true;
}
bool biorbd::muscles::WrappingCylinder::checkIfWraps(
        const NodeMusclePair &pointsInGlobal,
        NodeMusclePair &pointsToWrap) const {
    bool isWrap = true;

    // First quick tests
    // if both points are on the left and we have to go left
    if (m_isCylinderPositiveSign){
        if ((*pointsInGlobal.m_p1)(0) > radius() && (*pointsInGlobal.m_p2)(0) > radius())
            isWrap = false;
    }
    // if both points are on the right and we have to go right
    else{
        if ((*pointsInGlobal.m_p1)(0) < -radius() && (*pointsInGlobal.m_p2)(0) < -radius())
            isWrap = false;
    }

    // If we are on top of the wrap, it is impossible to determine because the wrap Si on est en haut du wrap*, 
    // is not a cylinder but a half-cylinder
    // * en haut lorsque vue de dessus avec l'axe y pointant vers le haut...
    if ( ( (*pointsInGlobal.m_p1)(1) > 0 && (*pointsInGlobal.m_p2)(1) > 0) || ( (*pointsInGlobal.m_p1)(1) < 0 && (*pointsInGlobal.m_p2)(1) < 0) )
        isWrap = false;

    // If we have a height* smaller than the radius, there is a numerical aberation
    // * en haut lorsque vue de dessus avec l'axe y pointant vers le haut...
    if ( abs( (*pointsInGlobal.m_p1)(1)) < radius() || abs( (*pointsInGlobal.m_p2)(1)) < radius() )
        isWrap = false;

    // If we have reached this stage, one test is left
    // If the straight line between the two points go through the cylinder,there is a wrap
    if (    ( (*pointsToWrap.m_p1)(0) < (*pointsToWrap.m_p2)(0) && (*pointsInGlobal.m_p1)(0) > (*pointsInGlobal.m_p2)(0)) ||
            ( (*pointsToWrap.m_p1)(0) > (*pointsToWrap.m_p2)(0) && (*pointsInGlobal.m_p1)(0) < (*pointsInGlobal.m_p2)(0))   )
        isWrap = false;
    else
        isWrap = true;

    // Return the answer
    return isWrap;
}
double biorbd::muscles::WrappingCylinder::computeLength(const NodeMusclePair &p) const {
    double arc = std::acos(    ( (*p.m_p1)(0) * (*p.m_p2)(0) + (*p.m_p1)(1) * (*p.m_p2)(1))
                                            /
             std::sqrt( ((*p.m_p1)(0) * (*p.m_p1)(0) + (*p.m_p1)(1) * (*p.m_p1)(1)) *
                        ( (*p.m_p2)(0) * (*p.m_p2)(0) + (*p.m_p2)(1) * (*p.m_p2)(1))   )
            ) * radius();

    return std::sqrt(arc*arc + ( (*p.m_p1)(2) - (*p.m_p2)(2)) * ( (*p.m_p1)(2) - (*p.m_p2)(2))  );
}
