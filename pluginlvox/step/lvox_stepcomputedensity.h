/****************************************************************************

 Copyright (C) 2012-2012 Universite de Sherbrooke, Quebec, CANADA
                     All rights reserved.

 Contact :  richard.fournier@usherbrooke.ca
            jean-francois.cote@nrcan-rncan.gc.ca
            joris.ravaglia@gmail.com

 Developers : Joris RAVAGLIA
 Adapted by Alexandre Piboule for Computree 2.0

 This file is part of Computree version 2.0.

 Computree is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Computree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Computree.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/


#ifndef LVOX_STEPCOMPUTEDENSITY_H
#define LVOX_STEPCOMPUTEDENSITY_H

/** \file LVOX_StepComputeDensity.h
    \author Ravaglia J. - mailto : joris.ravaglia@gmail.com
    \version 0.1
*/

#include "ct_step/abstract/ct_abstractstep.h"                    // The step inherits from ct_abstractstep

/** \class  LVOX_StepComputeDensity
    \brief
*/
class LVOX_StepComputeDensity : public CT_AbstractStep
{
    /** \def    Q_OBJECT :
        \brief  Macro from the Qt environment. Useful to get the name of the step (among others)*/
    Q_OBJECT

public:
    /*!
    *  \brief Constructor
    *
    *  Constructor of the class
    *  \param dataInit : informations about the step : parent step, plugin manager managing this step and some settings from Qt (for the Q_OBJECT?).
    */
    LVOX_StepComputeDensity(CT_StepInitializeData &dataInit);

    /*!
    *  \brief Gives a description of the step
    *  This method will be used in the GUI management : this description will be shown to the user when the mouse is over the step in the step menu.
    *
    *  \warning Inherited from CT_AbstractStep, this method is not pure virtual (could be not defined here).
    */
    virtual QString getStepDescription() const;

    /*!
    *  \brief Creates a new instance of this step
    *
    *  \param dataInit : informations about the step : parent step, plugin manager managing this step and some settings from Qt (for the Q_OBJECT?).
    *
    *  \warning Inherited from CT_AbstractStep, this method is pure virtual, each step MUST define its own method.
    */
    virtual CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);

protected:
    /*!
    *  \brief Creates the input of this step
    *
    *  This method defines what kind of input the step can accept and get them
    *
    *  \warning Inherited from CT_AbstractStep, this method is pure virtual, each step MUST define its own method.
    */
    virtual void createInResultModelListProtected();

    /*!
    *  \brief Creates the output of this step
    *
    *  This method defines what kind of results the step produces
    *
    *  \warning Inherited from CT_AbstractStep, this method is pure virtual, each step MUST define its own method.
    */
    virtual void createOutResultModelListProtected();

    /*!
    *  \brief Create a specific configuration dialog window for the step
    *
    *  This method creates a window for the user to set the different parameters of the step.
    *  The different parameters that can be set by the users and their properties (min, max, level of detail) are defined here :
    *       To add a parameter to this dialog window, just do a "_configDialog->addTYPEOFVALUE(...)"
    *
    *  \warning Inherited from CT_AbstractStep, this method is not pure virtual (could be not defined here).
    */
    virtual void createPostConfigurationDialog();

    /*!
    *  \brief Runs the step process
    *
    *  This method contains the effective calculus of the step (running the step is equivalent to launch this method).
    *  You might consider it as a "main" function for a step.
    *
    *  \warning Inherited from CT_AbstractStep, this method is pure virtual, each step MUST define its own method.
    */
    virtual void compute();

private:
//********************************************//
//              Attributes of LVox            //
//********************************************//

    double          _res;                   /*!< size of a voxel*/
    double          _intensityThresh;       /*!< minimum or maximum (depending on the _greaterthanThresh attribute) intensity for a point to contribute to the results*/
    bool            _greaterThanThresh;     /*!< if false, then the intensity threshold is considered as the maximum intensity to take into account*/

    double          _effectiveRayThresh;    /*!< minimum number of effective ray (Nt-Nb) in the voxel to calculate a density*/

//********************************************//
//           Attributes of the scanner        //
//********************************************//
    double      _scanPosX;                  /*!< Position of the scanner in world coordinate system*/
    double      _scanPosY;                  /*!< Position of the scanner in world coordinate system*/
    double      _scanPosZ;                  /*!< Position of the scanner in world coordinate system*/
    double      _scanHFov;                  /*!< horizontal field of view*/
    double      _scanVFov;                  /*!< vertical field of view*/
    double      _scanInitTheta;             /*!< initial horizontal angle of the measurement in the world coordinate system*/
    double      _scanInitPhi;               /*!< initial vertical angle of the measurement in the world coordinate system*/
    double      _scanHRes;                  /*!< horizontal angle resolution of the scan*/
    double      _scanVRes;                  /*!< vertical angle resolution of the scan*/
    bool        _scanClockWise;             /*!< Whether the scan has been done in clockwise or not*/
};

#endif // LVOX_STEPCOMPUTEDENSITY_H