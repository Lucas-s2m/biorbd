#ifndef MATLAB_BIORBD_MUSCLES_JOINT_TORQUE_FROM_ACTIVATION_H
#define MATLAB_BIORBD_MUSCLES_JOINT_TORQUE_FROM_ACTIVATION_H

#include <mex.h>
#include "BiorbdModel.h"
#include "class_handle.h"
#include "processArguments.h"

void Matlab_muscleJointTorqueFromActivation( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray*prhs[] ){

    // Verifier les arguments d'entrée
    checkNombreInputParametres(nrhs, 3, 5, "3 arguments are required (+2 optional) where the 2nd is the handler on the model, "
                                           "3rd is the muscles states and optional 4th and 5th are the Q and QDot, respectively."
                                           "WARNING: if the function is called without Q and Qdot, the user MUST update by himself before calling this function (using updateMuscle).");
    // Recevoir le model
    biorbd::Model * model = convertMat2Ptr<biorbd::Model>(prhs[1]);
    unsigned int nQ = model->nbQ(); // Get the number of DoF
    unsigned int nQdot = model->nbQdot(); // Get the number of DoF
    unsigned int nTau = model->nbGeneralizedTorque(); // Get the number of DoF
    unsigned int nRoot = model->nbRoot(); // Get the number of DoF
    unsigned int nMuscleTotal = model->nbMuscleTotal();

    // Recevoir muscleStates
    std::vector<std::vector<std::shared_ptr<biorbd::muscles::StateDynamics>>> s = getParameterMuscleStateActivation(prhs,2,nMuscleTotal);
    unsigned int nFrame(static_cast<unsigned int>(s.size()));

    bool updateKin(false); // Par défaut c'est false, mais ce comportement est celui le moins attendu
    if (nrhs >= 4){ // Si on doit récupérer la cinématique
        if (nrhs != 5)
            mexErrMsgIdAndTxt("MATLAB:dim:WrongArguments", "Q and Qdot must be sent to the function");
        updateKin = true;
    } else { // Si on n'update pas, s'assurer qu'un seul frame est envoyé
        if (nFrame > 1)
            mexErrMsgIdAndTxt("MATLAB:dim:WrongDimension", "Update == false is incompatible with more than one frame.");
    }

    // Recueillir la cinématique
    std::vector<biorbd::rigidbody::GeneralizedCoordinates> Q, QDot;
    if (updateKin){ // Si on update pas la cinématique Q et Qdot ne sont pas nécessaire
        // Recevoir Q
        Q = getParameterQ(prhs, 3, nQ);
        // Recevoir Qdot
        QDot = getParameterQdot(prhs, 4, nQdot);

        // S'assurer que Q, Qdot et activations sont de la bonne dimension
        if (Q.size() != nFrame)
            mexErrMsgIdAndTxt( "MATLAB:dim:WrongDimension", "Q must have the same number of frames than muscles states");
        if (QDot.size() != nFrame)
            mexErrMsgIdAndTxt( "MATLAB:dim:WrongDimension", "QDot must have the same number of frames than muscles states");
    }

    // Create a matrix for the return argument
    mwSize dims[2];
    dims[0] = nTau;
    dims[1] = nFrame;
    plhs[0] = mxCreateNumericArray(2, dims, mxDOUBLE_CLASS, mxREAL);
    double *tau = mxGetPr(plhs[0]);

    // Si la personne a demandé de sortir les forces par muscle également
    double *Mus = nullptr;
    if (nlhs >= 2){
        mwSize dims2[2];
        dims2[0] = nMuscleTotal;
        dims2[1] = nFrame;
        plhs[1] = mxCreateNumericArray(2, dims2, mxDOUBLE_CLASS, mxREAL);
        Mus = mxGetPr(plhs[1]);
    }

    // Remplir le output
    biorbd::rigidbody::GeneralizedTorque muscleTorque;
    biorbd::utils::Vector force;
    for (unsigned int i=0; i<nFrame; ++i){
        force.setZero();
        if (nlhs >= 2) // Si on doit récupérer les forces
            if (updateKin)
                muscleTorque = model->muscularJointTorque(s[i], force, updateKin, &Q[i], &QDot[i]);
            else
                muscleTorque = model->muscularJointTorque(s[i], force, updateKin);

        else // Si non
            if (updateKin)
                muscleTorque = model->muscularJointTorque(s[i], updateKin, &Q[i], &QDot[i]);
            else
                muscleTorque = model->muscularJointTorque(s[i], updateKin);


        // distribuer les Tau
        for (unsigned int j=0; j<nTau; ++j){
            tau[i*nTau+j] = muscleTorque(j+nRoot);
        }

        // Distribuer les forces
        if (nlhs >= 2) // Si on doit récupérer les forces
            for (unsigned int j=0; j<nMuscleTotal; ++j)
                Mus[i*nMuscleTotal+j] = force(j);
    }

    return;
}

#endif // MATLAB_BIORBD_MUSCLES_JOINT_TORQUE_FROM_ACTIVATION_H
