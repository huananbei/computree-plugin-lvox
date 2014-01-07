/****************************************************************************

 Copyright (C) 2012-2012 Université de Sherbrooke, Québec, CANADA
                     All rights reserved.

 Contact :  richard.fournier@usherbrooke.ca
            jean-francois.cote@nrcan-rncan.gc.ca
            joris.ravaglia@gmail.com

 Developers : Joris RAVAGLIA

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

#include "visitorraytracingadddistancebutfirst.h"

VisitorRaytracingAddDistanceButFirst::VisitorRaytracingAddDistanceButFirst() : VisitorRaytracingAddDistance()
{
	_first = true;
}

void VisitorRaytracingAddDistanceButFirst::visit(CT_AbstractRegularGridDataInterface *grid, int voxelID)
{
	if ( _first )
	{
		_first = false;
	}
	
	if ( !_first )
	{
		VisitorRaytracingAddDistance::visit( grid, voxelID );
	}
}

void VisitorRaytracingAddDistanceButFirst::reset()
{
	_first = true;
}
